#include "CTPTraderApi.h"
#include <boost/locale.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/format.hpp>
#include <sstream>

namespace MTP {

bool parse_logininfo(CTPUserLoginInfo& logininfo_, boost::property_tree::ptree& cfg)
{
	try
	{
		///经纪公司代码
		snprintf(logininfo_.BrokerID, _countof(logininfo_.BrokerID), "%s", cfg.get<std::string>("BrokerID", "").c_str());
		///用户代码
		snprintf(logininfo_.UserID, _countof(logininfo_.UserID), "%s", cfg.get<std::string>("UserID", "").c_str());
		///密码
		snprintf(logininfo_.Password, _countof(logininfo_.Password), "%s", cfg.get<std::string>("Password", "").c_str());
		///用户端产品信息
		snprintf(logininfo_.UserProductInfo, _countof(logininfo_.UserProductInfo), "%s", cfg.get<std::string>("UserProductInfo", "").c_str());
		///接口端产品信息
		snprintf(logininfo_.InterfaceProductInfo, _countof(logininfo_.InterfaceProductInfo), "%s", cfg.get<std::string>("InterfaceProductInfo", "").c_str());
		///协议信息
		snprintf(logininfo_.ProtocolInfo, _countof(logininfo_.ProtocolInfo), "%s", cfg.get<std::string>("ProtocolInfo", "").c_str());
		///Mac地址
		snprintf(logininfo_.MacAddress, _countof(logininfo_.MacAddress), "%s", cfg.get<std::string>("MacAddress", "").c_str());
		///动态密码
		snprintf(logininfo_.OneTimePassword, _countof(logininfo_.OneTimePassword), "%s", cfg.get<std::string>("OneTimePassword", "").c_str());
		///终端IP地址
		snprintf(logininfo_.ClientIPAddress, _countof(logininfo_.ClientIPAddress), "%s", cfg.get<std::string>("ClientIPAddress", "").c_str());
		///登录备注
		snprintf(logininfo_.LoginRemark, _countof(logininfo_.LoginRemark), "%s", cfg.get<std::string>("LoginRemark", "").c_str());
		///前置机服务器网络地址
		auto opt_front = cfg.get_child_optional("FrontAddress");
		if (opt_front) {
			BOOST_FOREACH (const boost::property_tree::ptree::value_type &cfg_front, opt_front.get())
			{
				logininfo_.FrontAddress.push_back(cfg_front.second.get_value<std::string>());
			}
		}
		///名字服务器网络地址
		auto opt_nsaddr = cfg.get_child_optional("NsAddress");
		if (opt_nsaddr) {
			BOOST_FOREACH (const boost::property_tree::ptree::value_type &cfg_addr, opt_nsaddr.get())
			{
				logininfo_.NsAddress.push_back(cfg_addr.second.get_value<std::string>());
			}
		}
		///登录模式
		logininfo_.LoginMode = cfg.get<TThostFtdcLoginModeType>("LoginMode", (char)0);
		///认证码
		snprintf(logininfo_.AuthCode, _countof(logininfo_.AuthCode), "%s", cfg.get<std::string>("AuthCode", "").c_str());
		///行情前置机服务器网络地址
		auto opt_md_front = cfg.get_child_optional("MdFrontAddress");
		if (opt_md_front)
		{
			BOOST_FOREACH (const boost::property_tree::ptree::value_type &cfg_md_front, opt_md_front.get())
			{
				logininfo_.MdFrontAddress.push_back(cfg_md_front.second.get_value<std::string>());
			}
		}
		///行情名字服务器网络地址
		auto opt_md_nsaddr = cfg.get_child_optional("MdNsAddress");
		if (opt_nsaddr)
		{
			BOOST_FOREACH (const boost::property_tree::ptree::value_type &cfg_md_addr, opt_md_nsaddr.get())
			{
				logininfo_.MdNsAddress.push_back(cfg_md_addr.second.get_value<std::string>());
			}
		}
		if (logininfo_.BrokerID[0] && logininfo_.UserID[0] && logininfo_.Password[0] && !logininfo_.FrontAddress.empty()) {
			return true;
		}	
	}
	catch (...)
	{
	}
	return false;
}

CTPTraderApi::CTPTraderApi(const CTPUserLoginInfo& userinfo):api_(nullptr)
	, userinfo_(userinfo)
{
	InitParam();
}

CTPTraderApi::~CTPTraderApi()
{
	stop();
}

bool CTPTraderApi::start(char *str)
{
	boost::filesystem::path flowpath(str);
	flowpath.append(userinfo_.BrokerID).append(userinfo_.UserID);
	if(!boost::filesystem::exists(flowpath))
	{
		boost::filesystem::create_directories(flowpath);
	}
	boost::filesystem::path currentpath = boost::filesystem::current_path();
	boost::filesystem::current_path(flowpath);
	api_ = CThostFtdcTraderApi::CreateFtdcTraderApi();
	boost::filesystem::current_path(currentpath);

	for(size_t i=0; i<userinfo_.FrontAddress.size(); i++)
	{
		api_->RegisterFront((char*)userinfo_.FrontAddress[i].c_str());
	}
	for(size_t i=0; i<userinfo_.NsAddress.size(); i++)
	{
		api_->RegisterNameServer((char*)userinfo_.NsAddress[i].c_str());
	}

	api_->SubscribePublicTopic(THOST_TERT_RESTART); //这样可以更新到最新的合约状态
	api_->SubscribePrivateTopic(THOST_TERT_QUICK); //交易委托、成交、持仓通过请求+推送更新到最新

	api_->RegisterSpi(this);

	// 使客户端开始与行情发布服务器建立连接
	api_->Init();

	return true;
}

void CTPTraderApi::stop()
{
	if (api_) {
		api_->Release();
		api_ = NULL;
	}
}

bool CTPTraderApi::IsNormal()
{
	if (IsStateful(CTP_STAT_MAX)) {
		return true;
	}
	return false;
}

bool CTPTraderApi::IsNetNormal()
{
	return IsStateful(CTP_STAT_CONNECT);
}

void CTPTraderApi::InitParam()
{
	memset(&m_InvestorInfo, 0, sizeof(m_InvestorInfo)); //投资者信息
	memset(&m_TradingAccount, 0, sizeof(m_TradingAccount)); //交易账户
	memset(&m_TradingCode, 0, sizeof(m_TradingCode)); //交易编码

	//m_SettlementConfirm; //结算单确认

	//m_NoticeInfo;	//通知

	//memset(&m_CFMMCKeyInfo,0,sizeof(m_CFMMCKeyInfo)); //保证金监管系统经纪公司资金账户密钥

	/*m_ExchangeInfos.clear();
	m_ProductInfos.clear();
	m_InstrumentInfos.clear();*/
}

int CTPTraderApi::NextRequestID()
{
	static int _nRequestID = 0;
	return ++_nRequestID;
}

int CTPTraderApi::NextRequest()
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
		//CTP_STAT_EXCHANGE, //市场列表
		//CTP_STAT_PRODUCT,	//产品列表
		//CTP_STAT_INSTRUMENT,	//合约列表
		//CTP_STAT_DEPTHMARKETDATA,	//深度行情
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

int CTPTraderApi::Request(int nStatus, int nRequestID)
{
	int rlt = -1;
	ASSERT(!IsInStateful(nStatus));
	SetInState(nStatus);
	switch (nStatus)
	{
	case CTP_STAT_AUTHENTICATE:
	{
		if (userinfo_.ProductInfo[0] || userinfo_.AuthCode[0])
		{
			CThostFtdcReqAuthenticateField req = { 0 };
			memcpy(req.BrokerID, userinfo_.BrokerID, sizeof(userinfo_.BrokerID));
			memcpy(req.UserID, userinfo_.UserID, sizeof(userinfo_.UserID));
			memcpy(req.UserProductInfo, userinfo_.ProductInfo, sizeof(userinfo_.ProductInfo));
			memcpy(req.AuthCode, userinfo_.AuthCode, sizeof(userinfo_.AuthCode));
			DoRequest(api_->ReqAuthenticate, req, nRequestID);
		}
		else
		{
			rlt = 0;
			CThostFtdcRspAuthenticateField rsp = { 0 };
			CThostFtdcRspInfoField rspInfo = { 0 };
			OnRspAuthenticate(&rsp, &rspInfo, nRequestID, true);
		}
	}
	break;
	case CTP_STAT_LOGIN:
	{
		LOG4I("CTPTraderApi ReqUserLogin %s", userinfo_.UserID);
		CThostFtdcReqUserLoginField req = { 0 };
		//CThostFtdcReqUserLoginField req = {0};
		//TradingDay;
		memcpy(req.BrokerID, userinfo_.BrokerID, sizeof(userinfo_.BrokerID));
		memcpy(req.UserID, userinfo_.UserID, sizeof(userinfo_.UserID));
		memcpy(req.Password, userinfo_.Password, sizeof(userinfo_.Password));
		memcpy(req.UserProductInfo, m_AuthInfo.UserProductInfo, sizeof(m_AuthInfo.UserProductInfo));
		//InterfaceProductInfo;
		//ProtocolInfo;
		//MacAddress;
		//OneTimePassword;
		//ClientIPAddress;
		DoRequest(api_->ReqUserLogin, req, nRequestID);
	}
	break;
	case CTP_STAT_INVESTORINFO:
	{
		LOG4I("CTPTraderApi ReqQryInvestor %s", userinfo_.UserID);
		CThostFtdcQryInvestorField req = { 0 };
		strcpy(req.BrokerID, userinfo_.BrokerID);
		strcpy(req.InvestorID, userinfo_.UserID);
		DoRequest(api_->ReqQryInvestor, req, nRequestID);
	}
	break;
	case CTP_STAT_TRADINGACCOUNT:
	{
		LOG4I("CTPTraderApi ReqQryTradingAccount %s", m_InvestorInfo.InvestorID);
		//RMB(CNY)-人民币 USD-美圆 HKD-港元
		CThostFtdcQryTradingAccountField req = { 0 };
		strcpy(req.BrokerID, m_InvestorInfo.BrokerID);
		strcpy(req.InvestorID, m_InvestorInfo.InvestorID);
		//strcpy(req.CurrencyID, "CNY");
		DoRequest(api_->ReqQryTradingAccount, req, nRequestID);
	}
	break;
	case CTP_STAT_TRADINGCODE: 
	{
		LOG4I("CTPTraderApi ReqQryTradingCode %s", m_InvestorInfo.InvestorID);
		CThostFtdcQryTradingCodeField req = { 0 };
		strcpy(req.BrokerID, m_InvestorInfo.BrokerID);
		strcpy(req.InvestorID, m_InvestorInfo.InvestorID);
		/*if(CLIENT_ID != NULL)
		strcpy_s(req.ClientID,CLIENT_ID);
		if(EXCHANGE_ID != NULL)
		strcpy_s(req.ExchangeID,EXCHANGE_ID);*/
		DoRequest(api_->ReqQryTradingCode, req, nRequestID);
	}
	break;
	case CTP_STAT_NOTICE:
	{
		LOG4I("CTPTraderApi ReqQryNotice");
		CThostFtdcQryNoticeField  req = { 0 };
		strcpy(req.BrokerID, userinfo_.BrokerID);
		DoRequest(api_->ReqQryNotice, req, nRequestID);
	}
	break;
	case CTP_STAT_CFMMC:
	{
		LOG4I("CTPTraderApi ReqQryCFMMCTradingAccountKey %s", m_InvestorInfo.InvestorID);
		CThostFtdcQryCFMMCTradingAccountKeyField req = { 0 };
		strcpy(req.BrokerID, m_InvestorInfo.BrokerID);
		strcpy(req.InvestorID, m_InvestorInfo.InvestorID);
		DoRequest(api_->ReqQryCFMMCTradingAccountKey, req, nRequestID);
	}
	break;
	case CTP_STAT_QRYSETTLEMENTINFOCONFIRM:
	{
		LOG4I("CTPTraderApi ReqQrySettlementInfoConfirm %s", m_InvestorInfo.InvestorID);
		CThostFtdcQrySettlementInfoConfirmField req = { 0 };
		strcpy(req.BrokerID, m_InvestorInfo.BrokerID);
		strcpy(req.InvestorID, m_InvestorInfo.InvestorID);
		DoRequest(api_->ReqQrySettlementInfoConfirm, req, nRequestID);
	}
	break;
	case CTP_STAT_QRYSETTLEMENTINFO:
	{
		LOG4I("CTPTraderApi ReqQrySettlementInfo %s", m_InvestorInfo.InvestorID);
		CThostFtdcQrySettlementInfoField req = { 0 };
		strcpy(req.BrokerID, m_InvestorInfo.BrokerID);
		strcpy(req.InvestorID, m_InvestorInfo.InvestorID);
		//req.TradingDay;
		DoRequest(api_->ReqQrySettlementInfo, req, nRequestID);
	}
	break;
	case CTP_STAT_SETTLEMENTINFOCONFIRM:
	{
		LOG4I("CTPTraderApi ReqSettlementInfoConfirm %s", m_InvestorInfo.InvestorID);
		CThostFtdcSettlementInfoConfirmField req = { 0 };
		strcpy(req.BrokerID, m_InvestorInfo.BrokerID);
		strcpy(req.InvestorID, m_InvestorInfo.InvestorID);
		//ConfirmDate	//rsp
		//ConfirmTime	//rsp
		DoRequest(api_->ReqSettlementInfoConfirm, req, nRequestID);
	}
	break;
	case CTP_STAT_EXCHANGE:
	{
		LOG4I("CTPTraderApi ReqQryExchange");
		CThostFtdcQryExchangeField req = { 0 };
		DoRequest(api_->ReqQryExchange, req, nRequestID);
	}
	break;
	case CTP_STAT_PRODUCT:
	{
		LOG4I("CTPTraderApi ReqQryProduct");
		CThostFtdcQryProductField req = { 0 };
		//req.ProductID
		//req.ProductClass;
		DoRequest(api_->ReqQryProduct, req, nRequestID);
	}
	break;
	case CTP_STAT_INSTRUMENT:
	{
		LOG4I("CTPTraderApi ReqQryInstrument");
		CThostFtdcQryInstrumentField req = { 0 };
		//req.InstrumentID;
#if 0
		strcpy(req.ExchangeID, "DCE");
#endif//
		//req.ExchangeInstID;
		//req.ProductID;
		DoRequest(api_->ReqQryInstrument, req, nRequestID);
	}
	break;
	case CTP_STAT_DEPTHMARKETDATA:
	{
		LOG4I("CTPTraderApi ReqQryDepthMarketData");
		CThostFtdcQryDepthMarketDataField req = { 0 };
		DoRequest(api_->ReqQryDepthMarketData, req, nRequestID);
	}
	break;
	case CTP_STAT_ORDER:
	{
		LOG4I("CTPTraderApi ReqQryOrder");
		CThostFtdcQryOrderField req = { 0 };
		strcpy(req.BrokerID, m_InvestorInfo.BrokerID);
		strcpy(req.InvestorID, m_InvestorInfo.InvestorID);
		DoRequest(api_->ReqQryOrder, req, nRequestID);
	}
	break;
	case CTP_STAT_TRADE:
	{
		LOG4I("CTPTraderApi ReqQryTrade");
		CThostFtdcQryTradeField req = { 0 };
		strcpy(req.BrokerID, m_InvestorInfo.BrokerID);
		strcpy(req.InvestorID, m_InvestorInfo.InvestorID);
		DoRequest(api_->ReqQryTrade, req, nRequestID);
	}
	break;
	case CTP_STAT_POSITION:
	{
		LOG4I("CTPTraderApi ReqQryInvestorPosition");
		CThostFtdcQryInvestorPositionField req = { 0 };
		strcpy(req.BrokerID, m_InvestorInfo.BrokerID);
		strcpy(req.InvestorID, m_InvestorInfo.InvestorID);
		DoRequest(api_->ReqQryInvestorPosition, req, nRequestID);
	}
	break;
	default:
	{
		ASSERT(0);
	}
	break;
	}
	return rlt;
}

void CTPTraderApi::OnLogin()
{
	
}

void CTPTraderApi::OnResponse(int nStatus, void* pData, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	ASSERT(nStatus);
	if (!nStatus) {
		return;
	}
	if (IsStateful(nStatus)) {
		return;
	}

	if (!bIsLast) {
		return;
	}

	//这里对初始化过程编排过了，都在NextRequest和Request实现的，通过bNext来判断是否继续初始化
	bool bNext = true;
	if (pRspInfo && IsErrorRspInfo(pRspInfo)) {
		//出错
		SetInState(nStatus, false);
	}
	else {
		SetState(nStatus);
		switch (nStatus)
		{
		case CTP_STAT_LOGIN:
		{
			OnLogin();
		}
		break;
		case CTP_STAT_QRYSETTLEMENTINFOCONFIRM:
		{
			CThostFtdcSettlementInfoConfirmField* pSettlementInfoConfirm = (CThostFtdcSettlementInfoConfirmField*)pData;
			if (pSettlementInfoConfirm) {
				if (stricmp(m_UserInfo.TradingDay, pSettlementInfoConfirm->ConfirmDate) == 0) {
					//说明已经查询结算过了
					SetState(CTP_STAT_QRYSETTLEMENTINFO);
					SetState(CTP_STAT_SETTLEMENTINFOCONFIRM);
				}
				else {
					//后面会先查询结算单，再通知上层确认结算单
				}
			}
		}
		break;
		case CTP_STAT_QRYSETTLEMENTINFO:
		{
			//确认结算单查询
			CThostFtdcSettlementInfoField *pSettlementInfo = (CThostFtdcSettlementInfoField *)pData;
			//直接确认结算单
		}
		break;
		default:
			break;
		}
	}
	if (bNext) {
		int nNext = NextRequest();
		if (nNext) {
			Request(nNext, nRequestID);
		}
		else {
			OnNormal();
		}
	}
}

void CTPTraderApi::OnNormal()
{
	SetState(CTP_STAT_MAX);
}

void CTPTraderApi::OnFrontConnected()
{
	//boost::unique_lock<boost::recursive_mutex> lock(mutex_);
	LOG4I("CTPTraderApi OnFrontConnected");
	if (IsStateful(CTP_STAT_CONNECT)) {
		LOG4I("CTPTraderApi Already OnFrontConnected");
		return;
	}
	InitParam(); //初始化数据
	ResetAllState();
	SetState(CTP_STAT_CONNECT);
	Request(NextRequest(), NextRequestID());
}

void CTPTraderApi::OnFrontDisconnected(int nReason)
{
	///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
	///@param nReason 错误原因
	///        0x1001 网络读失败
	///        0x1002 网络写失败
	///        0x2001 接收心跳超时
	///        0x2002 发送心跳失败
	///        0x2003 收到错误报文
	switch (nReason)
	{
	case 0x1001:
		LOG4E("CTPTraderApi OnFrontDisconnected 0x1001 network read failed.");
		break;
	case 0x1002:
		LOG4E("CTPTraderApi OnFrontDisconnected 0x1002 network write failed.");
		break;
	case 0x2001:
		LOG4E("CTPTraderApi OnFrontDisconnected 0x2001 receive heartbeat failed.");
		break;
	case 0x2002:
		LOG4E("CTPTraderApi OnFrontDisconnected 0x2002 send heartbeat failed.");
		break;
	case 0x2003:
		LOG4E("CTPTraderApi OnFrontDisconnected 0x2003 receive error package.");
		break;
	default:
		LOG4E("CTPTraderApi OnFrontDisconnected %d", nReason);
		break;
	}
	//boost::unique_lock<boost::recursive_mutex> lock(mutex_);
	if (IsInStateful(CTP_STAT_CONNECT)) {
		LOG4I("CTPTraderApi Already OnFrontDisconnected");
		return;
	}
	//CTP会自动重连
	ResetAllState();
	SetInState(CTP_STAT_CONNECT);
}

void CTPTraderApi::OnRspAuthenticate(CThostFtdcRspAuthenticateField *pRspAuthenticateField, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(bIsLast)
		LOG4I("CTPTraderApi OnRspAuthenticate %d %s", pRspInfo?pRspInfo->ErrorID:0, pRspInfo?pRspInfo->ErrorMsg:"0");
	if (pRspAuthenticateField) {
		m_AuthInfo = *pRspAuthenticateField;
	}
	OnResponse(CTP_STAT_AUTHENTICATE, pRspAuthenticateField, pRspInfo, nRequestID, bIsLast);
}

void CTPTraderApi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(bIsLast)
		LOG4I("CTPTraderApi OnRspUserLogin %d %s", pRspInfo?pRspInfo->ErrorID:0, pRspInfo?pRspInfo->ErrorMsg:"0");
	if (pRspUserLogin) {
		m_UserInfo = *pRspUserLogin;
	}
	OnResponse(CTP_STAT_LOGIN, pRspUserLogin, pRspInfo, nRequestID, bIsLast);
}

void CTPTraderApi::OnRspQryInvestor(CThostFtdcInvestorField *pInvestor, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(bIsLast)
		LOG4I("CTPTraderApi OnRspQryInvestor %d %s", pRspInfo?pRspInfo->ErrorID:0, pRspInfo?pRspInfo->ErrorMsg:"0");
	if (pInvestor) {
		m_InvestorInfo = *pInvestor;
	}
	OnResponse(CTP_STAT_INVESTORINFO, pInvestor, pRspInfo, nRequestID, bIsLast);
}

void CTPTraderApi::OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(bIsLast)
		LOG4I("CTPTraderApi OnRspQryTradingAccount %d %s", pRspInfo?pRspInfo->ErrorID:0, pRspInfo?pRspInfo->ErrorMsg:"0");
	if (pTradingAccount) {
		m_TradingAccount = *pTradingAccount;
	}
	OnResponse(CTP_STAT_TRADINGACCOUNT, pTradingAccount, pRspInfo, nRequestID, bIsLast);
}

void CTPTraderApi::OnRspQryTradingCode(CThostFtdcTradingCodeField *pTradingCode, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(bIsLast)
		LOG4I("CTPTraderApi OnRspQryTradingCode %d %s", pRspInfo?pRspInfo->ErrorID:0, pRspInfo?pRspInfo->ErrorMsg:"0");
	if (pTradingCode) {
		m_TradingCode = *pTradingCode;
	}
	OnResponse(CTP_STAT_TRADINGCODE, pTradingCode, pRspInfo, nRequestID, bIsLast);
}

void CTPTraderApi::OnRspQrySettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(bIsLast)
		LOG4I("CTPTraderApi OnRspQrySettlementInfoConfirm %d %s", pRspInfo?pRspInfo->ErrorID:0, pRspInfo?pRspInfo->ErrorMsg:"0");
	OnResponse(CTP_STAT_QRYSETTLEMENTINFOCONFIRM, pSettlementInfoConfirm, pRspInfo, nRequestID, bIsLast);
}

void CTPTraderApi::OnRspQrySettlementInfo(CThostFtdcSettlementInfoField *pSettlementInfo, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(bIsLast)
		LOG4I("CTPTraderApi OnRspQrySettlementInfo %d %s", pRspInfo?pRspInfo->ErrorID:0, pRspInfo?pRspInfo->ErrorMsg:"0");
	OnResponse(CTP_STAT_QRYSETTLEMENTINFO, pSettlementInfo, pRspInfo, nRequestID, bIsLast);
}

void CTPTraderApi::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(bIsLast)
		LOG4I("CTPTraderApi OnRspSettlementInfoConfirm %d %s", pRspInfo?pRspInfo->ErrorID:0, pRspInfo?pRspInfo->ErrorMsg:"0");
	OnResponse(CTP_STAT_SETTLEMENTINFOCONFIRM, pSettlementInfoConfirm, pRspInfo, nRequestID, bIsLast);
}

void CTPTraderApi::OnRspQryAccountregister(CThostFtdcAccountregisterField *pAccountregister, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(bIsLast)
		LOG4I("CTPTraderApi OnRspQryAccountregister %d %s", pRspInfo?pRspInfo->ErrorID:0, pRspInfo?pRspInfo->ErrorMsg:"0");
	
}

void CTPTraderApi::OnRtnOrder(CThostFtdcOrderField *pOrder)
{
	
}

void CTPTraderApi::OnRtnTrade(CThostFtdcTradeField *pTrade)
{
	
}

void CTPTraderApi::OnRtnInstrumentStatus(CThostFtdcInstrumentStatusField *pInstrumentStatus)
{
	
}

void CTPTraderApi::OnRspQryTransferBank(CThostFtdcTransferBankField *pTransferBank, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(bIsLast)
		LOG4I("CTPTraderApi OnRspQryTransferBank %d %s", pRspInfo?pRspInfo->ErrorID:0, pRspInfo?pRspInfo->ErrorMsg:"0");
}

void CTPTraderApi::OnRspQryExchange(CThostFtdcExchangeField *pExchange, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(bIsLast)
		LOG4I("CTPTraderApi OnRspQryExchange %d %s", pRspInfo?pRspInfo->ErrorID:0, pRspInfo?pRspInfo->ErrorMsg:"0");
	OnResponse(CTP_STAT_EXCHANGE, pExchange, pRspInfo, nRequestID, bIsLast);
}

void CTPTraderApi::OnRspQryProduct(CThostFtdcProductField *pProduct, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(bIsLast)
		LOG4I("CTPTraderApi OnRspQryProduct %d %s", pRspInfo?pRspInfo->ErrorID:0, pRspInfo?pRspInfo->ErrorMsg:"0");
	OnResponse(CTP_STAT_PRODUCT, pProduct, pRspInfo, nRequestID, bIsLast);
}

void CTPTraderApi::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(bIsLast)
		LOG4I("CTPTraderApi OnRspQryInstrument %d %s", pRspInfo?pRspInfo->ErrorID:0, pRspInfo?pRspInfo->ErrorMsg:"0");
	OnResponse(CTP_STAT_INSTRUMENT, pInstrument, pRspInfo, nRequestID, bIsLast);
}

void CTPTraderApi::OnRspQryDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(bIsLast)
		LOG4I("CTPTraderApi OnRspQryDepthMarketData %d %s", pRspInfo?pRspInfo->ErrorID:0, pRspInfo?pRspInfo->ErrorMsg:"0");
	OnResponse(CTP_STAT_DEPTHMARKETDATA, pDepthMarketData, pRspInfo, nRequestID, bIsLast);
}

void CTPTraderApi::OnRspQryParkedOrder(CThostFtdcParkedOrderField *pParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	
}

void CTPTraderApi::OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	
}

void CTPTraderApi::OnRspQryTrade(CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	
}

void CTPTraderApi::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	
}

void CTPTraderApi::OnRspQryInvestorPositionDetail(CThostFtdcInvestorPositionDetailField *pInvestorPositionDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	
}

void CTPTraderApi::OnRspQryNotice(CThostFtdcNoticeField *pNotice, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(bIsLast)
		LOG4I("CTPTraderApi OnRspQryNotice %d %s", pRspInfo?pRspInfo->ErrorID:0, pRspInfo?pRspInfo->ErrorMsg:"0");
	OnResponse(CTP_STAT_NOTICE, pNotice, pRspInfo, nRequestID, bIsLast);
}

void CTPTraderApi::OnRspQryCFMMCTradingAccountKey(CThostFtdcCFMMCTradingAccountKeyField *pCFMMCTradingAccountKey, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(bIsLast)
		LOG4I("CTPTraderApi OnRspQryCFMMCTradingAccountKey %d %s", pRspInfo?pRspInfo->ErrorID:0, pRspInfo?pRspInfo->ErrorMsg:"0");
	OnResponse(CTP_STAT_CFMMC, pCFMMCTradingAccountKey, pRspInfo, nRequestID, bIsLast);
}

}
