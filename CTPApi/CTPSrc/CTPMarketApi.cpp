#include "CTPMarketApi.h"
#include <boost/locale.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/format.hpp>
#include <sstream>

namespace MTP {

CTPOwner::CTPOwner()
	: requestid_(0)
{
	
}

CTPOwner::~CTPOwner()
{
	
}

int CTPOwner::gen_requestid()
{
	return ++requestid_;
}

int CTPOwner::get_requestid()
{
	return requestid_;
}

//////////////////////////////////////////////////////////////////////////

CTPMarketApi::CTPMarketApi(CTPOwner* owner)
: owner_(owner), api_(nullptr)
, userinfo_(owner->logininfo())
{
	memset(&rsp_userinfo_, 0, sizeof(rsp_userinfo_));
}

CTPMarketApi::~CTPMarketApi()
{
	stop();
}

void CTPMarketApi::start(char* str)
{
	boost::filesystem::path flowpath(str);
	flowpath.append(userinfo_.BrokerID).append(userinfo_.UserID);
	if(!boost::filesystem::exists(flowpath))
	{
		boost::filesystem::create_directories(flowpath);
	}
	boost::filesystem::path currentpath = boost::filesystem::current_path();
	boost::filesystem::current_path(flowpath);
	api_ = CThostFtdcMdApi::CreateFtdcMdApi();
	boost::filesystem::current_path(currentpath);
	for(size_t i=0; i<userinfo_.MdFrontAddress.size(); i++)
	{
		api_->RegisterFront((char*)userinfo_.MdFrontAddress[i].c_str());
	}
	for(size_t i=0; i<userinfo_.MdNsAddress.size(); i++)
	{
		api_->RegisterNameServer((char*)userinfo_.MdNsAddress[i].c_str());
	}

	api_->RegisterSpi(this);

	// 使客户端开始与行情发布服务器建立连接
	api_->Init();
}

void CTPMarketApi::stop()
{
	if (api_) {
		api_->Release();
		api_ = NULL;
	}
}

bool CTPMarketApi::IsOk()
{
	if(rsp_userinfo_.TradingDay[0]) {
		return true;
	}
	return false;
}

void CTPMarketApi::OnFrontConnected()
{
	LOG4I("CTPMarketApi OnFrontConnected");
	if (IsStateful(CTP_STAT_CONNECT)) {
		LOG4I("CTPMarketApi Already OnFrontConnected");
		return;
	}
	ResetAllState();
	SetState(CTP_STAT_CONNECT);
	SetInState(CTP_STAT_LOGIN);
	LOG4I("CTPMarketApi ReqUserLogin %s", userinfo_.UserID);
	api_->ReqUserLogin((CThostFtdcReqUserLoginField*)&userinfo_, 0);
}

void CTPMarketApi::OnFrontDisconnected(int nReason)
{
	LOG4I("CTPMarketApi OnFrontDisconnected %d", nReason);
	if (IsInStateful(CTP_STAT_CONNECT)) {
		LOG4I("CTPMarketApi Already OnFrontDisconnected");
		return;
	}
	ResetAllState();
	SetInState(CTP_STAT_CONNECT);
	memset(&rsp_userinfo_, 0, sizeof(CThostFtdcRspUserLoginField));
}

void CTPMarketApi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (bIsLast)
		LOG4I("CTPMarketApi OnRspUserLogin %d %s", pRspInfo ? pRspInfo->ErrorID : 0, pRspInfo ? pRspInfo->ErrorMsg : "0");
	
	if (bIsLast) {
		if (IsErrorRspInfo(pRspInfo)) {
			SetInState(CTP_STAT_LOGIN, false);
			return;
		}
		SetState(CTP_STAT_LOGIN);
	}
	if(pRspUserLogin) {
		rsp_userinfo_ = *pRspUserLogin;
		/*char* InstrumentID[] = { "zn1906" };
		std::vector<char*> pInstrumentIDs;
		pInstrumentIDs.push_back(InstrumentID[0]);
		api_->SubscribeMarketData(&pInstrumentIDs[0], pInstrumentIDs.size());*/
		LOG4I("CTPMarketApi SubscribeMarketData %d", subscribes_.size());
		if (!subscribes_.empty()) {
			api_->SubscribeMarketData((char**)&subscribes_[0], subscribes_.size());
		}
	}
}

void CTPMarketApi::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
	owner_->OnRtnDepthMarketData(pDepthMarketData);
}

CTPTradeApi::CTPTradeApi(CTPOwner* owner):owner_(owner), Base(owner->logininfo())
{
	
}

CTPTradeApi::~CTPTradeApi()
{
	
}

bool CTPTradeApi::start(char *str)
{
	return Base::start(str);
}

void CTPTradeApi::stop()
{
	Base::stop();
}

int CTPTradeApi::NextRequestID()
{
	return owner_->gen_requestid();
}

int CTPTradeApi::NextRequest()
{
	const int nStatus[] =
	{
		CTP_STAT_AUTHENTICATE,	//认证
		CTP_STAT_LOGIN,	//登录
		CTP_STAT_INVESTORINFO,	//投资者信息
		CTP_STAT_TRADINGACCOUNT,	//交易账户
		CTP_STAT_TRADINGCODE,	//交易编码
		//CTP_STAT_NOTICE,	//公告通知
		CTP_STAT_CFMMC,	//保证金监管系统经纪公司资金账户密钥
		CTP_STAT_QRYSETTLEMENTINFOCONFIRM,	//查询结算单确认
		CTP_STAT_QRYSETTLEMENTINFO,	//查询结算单
		CTP_STAT_SETTLEMENTINFOCONFIRM,	//结算单确认
		CTP_STAT_EXCHANGE, //市场列表
		CTP_STAT_PRODUCT,	//产品列表
		CTP_STAT_INSTRUMENT,	//合约列表
		CTP_STAT_DEPTHMARKETDATA,	//深度行情
	};
	for (int i = 0, j = sizeof(nStatus) / sizeof(long); i<j;i++)
	{
		if (!IsStateful(nStatus[i]))
		{
			return nStatus[i];
		}
	}
	return 0;
}

int CTPTradeApi::Request(int nStatus, int nRequestID)
{
	int rlt = -1;
	switch (nStatus)
	{
	case CTP_STAT_EXCHANGE:
	{
		ASSERT(!IsInStateful(nStatus));
		SetInState(nStatus);
		if (!owner_->IsNeedRequestAndSet(CTP_STAT_EXCHANGE)) {
			rlt = 0;
			OnResponse(CTP_STAT_EXCHANGE, NULL, NULL, nRequestID, true);
		}
		else {
			LOG4I("CTPTradeApi ReqQryExchange");
			CThostFtdcQryExchangeField req = { 0 };
			DoRequest(api_->ReqQryExchange, req, nRequestID);
		}
	}
	break;
	case CTP_STAT_PRODUCT:
	{
		ASSERT(!IsInStateful(nStatus));
		SetInState(nStatus);
		if (!owner_->IsNeedRequestAndSet(CTP_STAT_PRODUCT)) {
			rlt = 0;
			OnResponse(CTP_STAT_PRODUCT, NULL, NULL, nRequestID, true);
		}
		else {
			LOG4I("CTPTradeApi ReqQryProduct");
			CThostFtdcQryProductField req = { 0 };
			//req.ProductID
			//req.ProductClass;
			DoRequest(api_->ReqQryProduct, req, nRequestID);
		}
	}
	break;
	case CTP_STAT_INSTRUMENT:
	{
		ASSERT(!IsInStateful(nStatus));
		SetInState(nStatus);
		if (!owner_->IsNeedRequestAndSet(CTP_STAT_INSTRUMENT)) {
			rlt = 0;
			OnResponse(CTP_STAT_INSTRUMENT, NULL, NULL, nRequestID, true);
		}
		else {
			LOG4I("CTPTradeApi ReqQryInstrument");
			CThostFtdcQryInstrumentField req = { 0 };
			//req.InstrumentID;
#if 0
			strcpy(req.ExchangeID, "DCE");
#endif//
			//req.ExchangeInstID;
			//req.ProductID;
			DoRequest(api_->ReqQryInstrument, req, nRequestID);
		}
	}
	break;
	case CTP_STAT_DEPTHMARKETDATA:
	{
		ASSERT(!IsInStateful(nStatus));
		SetInState(nStatus);
		if (!owner_->IsNeedRequestAndSet(CTP_STAT_DEPTHMARKETDATA)) {
			rlt = 0;
			OnResponse(CTP_STAT_DEPTHMARKETDATA, NULL, NULL, nRequestID, true);
		}
		else {
			LOG4I("CTPTradeApi ReqQryDepthMarketData");
			CThostFtdcQryDepthMarketDataField req = { 0 };
			DoRequest(api_->ReqQryDepthMarketData, req, nRequestID);
		}
	}
	break;
	default:
	{
		rlt = Base::Request(nStatus, nRequestID);
	}
	break;
	}
	return rlt;
}

void CTPTradeApi::OnLogin()
{
	Base::OnLogin();
	owner_->on_market_date(m_UserInfo.TradingDay);
}

void CTPTradeApi::OnNormal()
{
	Base::OnNormal();
	owner_->on_normal();
	owner_->on_market_init();
}

void CTPTradeApi::OnFrontConnected()
{
	if (IsStateful(CTP_STAT_CONNECT)) {
		LOG4I("CTPTradeApi Already OnFrontConnected");
		return;
	}
	owner_->on_connect();
	Base::OnFrontConnected();
}

void CTPTradeApi::OnFrontDisconnected(int nReason)
{
	if (IsInStateful(CTP_STAT_CONNECT)) {
		LOG4I("CTPTradeApi Already OnFrontDisconnected %d", nReason);
		return;
	}
	Base::OnFrontDisconnected(nReason);
	owner_->on_disconnect();
}

void CTPTradeApi::OnRtnInstrumentStatus(CThostFtdcInstrumentStatusField *pInstrumentStatus)
{
	owner_->OnRtnInstrumentStatus(pInstrumentStatus);
}

void CTPTradeApi::OnRspQryExchange(CThostFtdcExchangeField *pExchange, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	owner_->OnRspQryExchange(pExchange, pRspInfo, nRequestID, bIsLast);
	Base::OnRspQryExchange(pExchange, pRspInfo, nRequestID, bIsLast);
}

void CTPTradeApi::OnRspQryProduct(CThostFtdcProductField *pProduct, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	owner_->OnRspQryProduct(pProduct, pRspInfo, nRequestID, bIsLast);
	Base::OnRspQryProduct(pProduct, pRspInfo, nRequestID, bIsLast);
}

void CTPTradeApi::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	owner_->OnRspQryInstrument(pInstrument, pRspInfo, nRequestID, bIsLast);
	Base::OnRspQryInstrument(pInstrument, pRspInfo, nRequestID, bIsLast);
}

void CTPTradeApi::OnRspQryDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	owner_->OnRspQryDepthMarketData(pDepthMarketData, pRspInfo, nRequestID, bIsLast);
	Base::OnRspQryDepthMarketData(pDepthMarketData, pRspInfo, nRequestID, bIsLast);
}

}
