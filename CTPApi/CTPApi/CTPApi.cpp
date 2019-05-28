//#include "stdafx.h"
#include <CTPApi.h>
#include <boost/property_tree/ptree.hpp>  
#include <boost/property_tree/json_parser.hpp>  
#include <boost/property_tree/xml_parser.hpp>  
#include <boost/typeof/typeof.hpp>  
#include <boost/foreach.hpp>  
#include <boost/locale.hpp>
#include <utility>
#include <XUtil/XXml.hpp>

//////////////////////////////////////////////////////////////////////////

//-1，表示网络连接失败；
//-2，表示未处理请求超过许可数；
//-3，表示每秒发送请求数超过许可数。

///CTPTrader内部使用的发送请求宏
#define DoRequest(func,req,reqId) \
	do \
	{ \
		rlt = func(&req, reqId); \
		if (rlt==-2 || rlt==-3) \
		{ \
			Sleep(50); \
		} \
		else \
		{ \
			break; \
		} \
	} while(true)

//////////////////////////////////////////////////////////////////////////

CTPSessionApi* CTPSessionApi::CreateCTPSessionApi()
{
	return new CTPApi();
}

void CTPSessionApi::DestroyCTPSessionApi(CTPSessionApi* p)
{
	if (p) {
		delete p;
	}
}

//////////////////////////////////////////////////////////////////////////

CTPApi::CTPApi() :m_pTradeApi(nullptr),m_pMdApi(nullptr),m_pSpi(nullptr)
, m_CloseFlag(true)
{

}

CTPApi::~CTPApi()
{
	if (m_pSpi) {
		m_pSpi->OnClose();
	}
}

void CTPApi::OpenApi(const CTPUserLoginInfo& userInfo)
{
	bool expected = true;
	if (!m_CloseFlag.compare_exchange_strong(expected, false))
	{
		return;
	}

	mUserInfo = userInfo;
	m_pTradeApi = new CTPTraderApi(userInfo, this);
	m_pMdApi = new CTPMdApi(userInfo, this);
}

void CTPApi::CloseApi()
{
	bool expected = false;
	if (!m_CloseFlag.compare_exchange_strong(expected, true))
	{
		return;
	}

	if (m_pMdApi) {
		delete m_pMdApi;
		m_pMdApi = nullptr;
	}
	if (m_pTradeApi) {
		delete m_pTradeApi;
		m_pTradeApi = nullptr;
	}
}

bool CTPApi::IsOpenApi()
{
	return !m_CloseFlag;
}

void CTPApi::RegisterSpi(CTPSessionSpi* pSpi)
{
	m_pSpi = pSpi;
}

void CTPApi::Send(const std::string& data)
{
	if (!IsOpenApi()) {
		return;
	}
#ifdef _DEBUG
	std::cout << data << std::endl;
#endif
	try
	{
		boost::property_tree::ptree jsonObject;
		if (XUtil::json_from_str(data, jsonObject)) {
			int nResult = -1;
			///RequestApi
			std::string RequestApi = jsonObject.get<std::string>("RequestApi");
			int nRequestID = jsonObject.get<int>("RequestID",0);
			if (RequestApi == "SubscribeMarketData") {
				nResult = SubscribeMarketData(jsonObject, nRequestID);
			}
			else if (RequestApi == "UnSubscribeMarketData") {
				nResult = UnSubscribeMarketData(jsonObject, nRequestID);
			}
			else if (RequestApi == "ReqAuthenticate") {
				nResult = ReqAuthenticate(jsonObject, nRequestID);
			}
			else if (RequestApi == "ReqUserLogin") {
				nResult = ReqUserLogin(jsonObject,nRequestID);
			} 
			else if (RequestApi == "ReqUserLogout") {
				nResult = ReqUserLogout(jsonObject, nRequestID);
			}
			else if (RequestApi == "ReqUserPasswordUpdate") {
				nResult = ReqUserPasswordUpdate(jsonObject, nRequestID);
			}
			else if (RequestApi == "ReqTradingAccountPasswordUpdate") {
				nResult = ReqTradingAccountPasswordUpdate(jsonObject, nRequestID);
			}
			else if (RequestApi == "ReqOrderInsert") {
				nResult = ReqOrderInsert(jsonObject, nRequestID);
			}
			else if (RequestApi == "ReqParkedOrderInsert") {
				nResult = ReqParkedOrderInsert(jsonObject, nRequestID);
			}
			else if (RequestApi == "ReqParkedOrderAction") {
				nResult = ReqParkedOrderAction(jsonObject, nRequestID);
			}
			else if (RequestApi == "ReqUserLogout") {
				nResult = ReqUserLogout(jsonObject, nRequestID);
			}
			else if (RequestApi == "ReqOrderAction") {
				nResult = ReqOrderAction(jsonObject, nRequestID);
			}
			else if (RequestApi == "ReqQueryMaxOrderVolume") {
				nResult = ReqQueryMaxOrderVolume(jsonObject, nRequestID);
			}
			else if (RequestApi == "ReqSettlementInfoConfirm") {
				nResult = ReqSettlementInfoConfirm(jsonObject, nRequestID);
			}
			else if (RequestApi == "ReqRemoveParkedOrder") {
				nResult = ReqRemoveParkedOrder(jsonObject, nRequestID);
			}
			else if (RequestApi == "ReqRemoveParkedOrderAction") {
				nResult = ReqRemoveParkedOrderAction(jsonObject, nRequestID);
			}
			else if (RequestApi == "ReqExecOrderInsert") {
				nResult = ReqExecOrderInsert(jsonObject, nRequestID);
			}
			else if (RequestApi == "ReqExecOrderAction") {
				nResult = ReqExecOrderAction(jsonObject, nRequestID);
			}
			else if (RequestApi == "ReqForQuoteInsert") {
				nResult = ReqForQuoteInsert(jsonObject, nRequestID);
			}
			else if (RequestApi == "ReqQuoteInsert") {
				nResult = ReqQuoteInsert(jsonObject, nRequestID);
			}
			else if (RequestApi == "ReqQuoteAction") {
				nResult = ReqQuoteAction(jsonObject, nRequestID);
			}
			else if (RequestApi == "ReqLockInsert") {
				nResult = ReqLockInsert(jsonObject, nRequestID);
			}
			else if (RequestApi == "ReqBatchOrderAction") {
				nResult = ReqBatchOrderAction(jsonObject, nRequestID);
			}
			else if (RequestApi == "ReqCombActionInsert") {
				nResult = ReqCombActionInsert(jsonObject, nRequestID);
			}
			else if (RequestApi == "ReqQryOrder") {
				nResult = ReqQryOrder(jsonObject, nRequestID);
			}
			else if (RequestApi == "ReqQryTrade") {
				nResult = ReqQryTrade(jsonObject, nRequestID);
			}
			else if (RequestApi == "ReqQryInvestorPosition") {
				nResult = ReqQryInvestorPosition(jsonObject, nRequestID);
			}
			else if (RequestApi == "ReqQryTradingAccount") {
				nResult = ReqQryTradingAccount(jsonObject, nRequestID);
			}
			else if (RequestApi == "ReqQryInvestor") {
				nResult = ReqQryInvestor(jsonObject, nRequestID);
			}
			else if (RequestApi == "ReqQryTradingCode") {
				nResult = ReqQryTradingCode(jsonObject, nRequestID);
			}
			else if (RequestApi == "ReqQryInstrumentMarginRate") {
				nResult = ReqQryInstrumentMarginRate(jsonObject, nRequestID);
			}
			else if (RequestApi == "ReqQryInstrumentCommissionRate") {
				nResult = ReqQryInstrumentCommissionRate(jsonObject, nRequestID);
			}
			else if (RequestApi == "ReqQryExchange") {
				nResult = ReqQryExchange(jsonObject, nRequestID);
			}
			else if (RequestApi == "ReqQryProduct") {
				nResult = ReqQryProduct(jsonObject, nRequestID);
			}
			else if (RequestApi == "ReqQryInstrument") {
				nResult = ReqQryInstrument(jsonObject, nRequestID);
			}
			else if (RequestApi == "ReqQryDepthMarketData") {
				nResult = ReqQryDepthMarketData(jsonObject, nRequestID);
			}
			else if (RequestApi == "ReqQrySettlementInfo") {
				nResult = ReqQrySettlementInfo(jsonObject, nRequestID);
			}
			else if (RequestApi == "ReqQryTransferBank") {
				nResult = ReqQryTransferBank(jsonObject, nRequestID);
			}
			else if (RequestApi == "ReqQryInvestorPositionDetail") {
				nResult = ReqQryInvestorPositionDetail(jsonObject, nRequestID);
			}
			else if (RequestApi == "ReqQryNotice") {
				nResult = ReqQryNotice(jsonObject, nRequestID);
			}
			else if (RequestApi == "ReqQrySettlementInfoConfirm") {
				nResult = ReqQrySettlementInfoConfirm(jsonObject, nRequestID);
			}
			else if (RequestApi == "ReqQryInvestorPositionCombineDetail") {
				nResult = ReqQryInvestorPositionCombineDetail(jsonObject, nRequestID);
			}
			else if (RequestApi == "ReqQryCFMMCTradingAccountKey") {
				nResult = ReqQryCFMMCTradingAccountKey(jsonObject, nRequestID);
			}
			else if (RequestApi == "ReqQryEWarrantOffset") {
				nResult = ReqQryEWarrantOffset(jsonObject, nRequestID);
			}
			else if (RequestApi == "ReqQryInvestorProductGroupMargin") {
				nResult = ReqQryInvestorProductGroupMargin(jsonObject, nRequestID);
			}
			else if (RequestApi == "ReqQryExchangeMarginRate") {
				nResult = ReqQryExchangeMarginRate(jsonObject, nRequestID);
			}
			else if (RequestApi == "ReqQryExchangeMarginRateAdjust") {
				nResult = ReqQryExchangeMarginRateAdjust(jsonObject, nRequestID);
			}
			else if (RequestApi == "ReqQryExchangeRate") {
				nResult = ReqQryExchangeRate(jsonObject, nRequestID);
			}
			else if (RequestApi == "ReqQrySecAgentACIDMap") {
				nResult = ReqQrySecAgentACIDMap(jsonObject, nRequestID);
			}
			else if (RequestApi == "ReqQryProductExchRate") {
				nResult = ReqQryProductExchRate(jsonObject, nRequestID);
			}
			else if (RequestApi == "ReqQryProductGroup") {
				nResult = ReqQryProductGroup(jsonObject, nRequestID);
			}
			else if (RequestApi == "ReqQryMMInstrumentCommissionRate") {
				nResult = ReqQryMMInstrumentCommissionRate(jsonObject, nRequestID);
			}
			else if (RequestApi == "ReqQryMMOptionInstrCommRate") {
				nResult = ReqQryMMOptionInstrCommRate(jsonObject, nRequestID);
			}
			else if (RequestApi == "ReqQryInstrumentOrderCommRate") {
				nResult = ReqQryInstrumentOrderCommRate(jsonObject, nRequestID);
			}
			else if (RequestApi == "ReqQryOptionInstrTradeCost") {
				nResult = ReqQryOptionInstrTradeCost(jsonObject, nRequestID);
			}
			else if (RequestApi == "ReqQryOptionInstrCommRate") {
				nResult = ReqQryOptionInstrCommRate(jsonObject, nRequestID);
			}
			else if (RequestApi == "ReqQryExecOrder") {
				nResult = ReqQryExecOrder(jsonObject, nRequestID);
			}
			else if (RequestApi == "ReqQryForQuote") {
				nResult = ReqQryForQuote(jsonObject, nRequestID);
			}
			else if (RequestApi == "ReqQryQuote") {
				nResult = ReqQryQuote(jsonObject, nRequestID);
			}
			else if (RequestApi == "ReqQryLock") {
				nResult = ReqQryLock(jsonObject, nRequestID);
			}
			else if (RequestApi == "ReqQryLockPosition") {
				nResult = ReqQryLockPosition(jsonObject, nRequestID);
			}
			else if (RequestApi == "ReqQryETFOptionInstrCommRate") {
				nResult = ReqQryETFOptionInstrCommRate(jsonObject, nRequestID);
			}
			else if (RequestApi == "ReqQryInvestorLevel") {
				nResult = ReqQryInvestorLevel(jsonObject, nRequestID);
			}
			else if (RequestApi == "ReqQryExecFreeze") {
				nResult = ReqQryExecFreeze(jsonObject, nRequestID);
			}
			else if (RequestApi == "ReqQryCombInstrumentGuard") {
				nResult = ReqQryCombInstrumentGuard(jsonObject, nRequestID);
			}
			else if (RequestApi == "ReqQryCombAction") {
				nResult = ReqQryCombAction(jsonObject, nRequestID);
			}
			else if (RequestApi == "ReqQryTransferSerial") {
				nResult = ReqQryTransferSerial(jsonObject, nRequestID);
			}
			else if (RequestApi == "ReqQryAccountregister") {
				nResult = ReqQryAccountregister(jsonObject, nRequestID);
			}
			else if (RequestApi == "ReqQryContractBank") {
				nResult = ReqQryContractBank(jsonObject, nRequestID);
			}
			else if (RequestApi == "ReqQryParkedOrder") {
				nResult = ReqQryParkedOrder(jsonObject, nRequestID);
			}
			else if (RequestApi == "ReqQryParkedOrderAction") {
				nResult = ReqQryParkedOrderAction(jsonObject, nRequestID);
			}
			else if (RequestApi == "ReqQryTradingNotice") {
				nResult = ReqQryTradingNotice(jsonObject, nRequestID);
			}
			else if (RequestApi == "ReqQryBrokerTradingParams") {
				nResult = ReqQryBrokerTradingParams(jsonObject, nRequestID);
			}
			else if (RequestApi == "ReqQryBrokerTradingAlgos") {
				nResult = ReqQryBrokerTradingAlgos(jsonObject, nRequestID);
			}
			else if (RequestApi == "ReqQueryCFMMCTradingAccountToken") {
				nResult = ReqQueryCFMMCTradingAccountToken(jsonObject, nRequestID);
			}
			else if (RequestApi == "ReqFromBankToFutureByFuture") {
				nResult = ReqFromBankToFutureByFuture(jsonObject, nRequestID);
			}
			else if (RequestApi == "ReqFromFutureToBankByFuture") {
				nResult = ReqFromFutureToBankByFuture(jsonObject, nRequestID);
			}
			else if (RequestApi == "ReqQueryBankAccountMoneyByFuture") {
				nResult = ReqQueryBankAccountMoneyByFuture(jsonObject, nRequestID);
			}
			else {
				HandleRequestResult(nResult, RequestApi.c_str(), nRequestID);
			}
			return;
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
}

int CTPApi::HandleRequestResult(int nResult, const char* pRequestApi, int nRequestID)
{
	try
	{
		if (nResult && pRequestApi) {
			boost::property_tree::ptree jsonObject;
			jsonObject.put("ReturnApi", pRequestApi);
			//jsonObject.put("ErrorMsg", pRspInfo->ErrorMsg);
			jsonObject.put("ErrorID", nResult);
			jsonObject.put("RequestID", nRequestID);
			jsonObject.put("IsLast", true);
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return nResult;
			}
		}
		else {
			return nResult;
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
	return nResult;
}

bool CTPApi::BuildDefaultJsonObject(boost::property_tree::ptree & jsonObject
	, const char* pReturnApi, const CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (!pReturnApi || !pReturnApi[0]) {
		return false;
	}
	try
	{
		jsonObject.put("ReturnApi", pReturnApi);
		if (pRspInfo) {
			jsonObject.put("ErrorMsg", pRspInfo->ErrorMsg);
			jsonObject.put("ErrorID", pRspInfo->ErrorID);
		}
		else {
			jsonObject.put("ErrorMsg", "");
			jsonObject.put("ErrorID", 0);
		}
		jsonObject.put("RequestID", nRequestID);
		jsonObject.put("IsLast", bIsLast);
		return true;
	}
	catch (...)
	{
	}
	return false;
}

///订阅行情
int CTPApi::SubscribeMarketData(const boost::property_tree::ptree & jsonObject, int nRequestID)
{
	int nResult = -1;
	try
	{
		auto optData = jsonObject.get_child_optional("Data");
		if (optData) {
			const boost::property_tree::ptree & jsonData = optData.get();
			std::vector<char*> pInstrumentIDs;
			std::string strInstrumentIDs = jsonData.get<std::string>("InstrumentID", "");
			if (!strInstrumentIDs.empty()) {
				if (strInstrumentIDs.back() != ';') {
					strInstrumentIDs += ';';
				}
				char* pInstrumentID = (char*)&strInstrumentIDs[0];
				for (int i = 0, j = strInstrumentIDs.size(); i<j; i++)
				{
					if (strInstrumentIDs[i] == ';') {
						strInstrumentIDs[i] = 0;
						pInstrumentIDs.push_back(pInstrumentID);
						if ((i + 1) < j) {
							pInstrumentID = (char*)&strInstrumentIDs[i + 1];
						}
					}
				}
			}
			if (!pInstrumentIDs.empty()) {
				nResult = m_pMdApi->getApi()->SubscribeMarketData((char**)&pInstrumentIDs[0], pInstrumentIDs.size());
			}
		}
	}
	catch (...)
	{

	}
	return HandleRequestResult(nResult, "SubscribeMarketData", nRequestID);
}

///退订行情
int CTPApi::UnSubscribeMarketData(const boost::property_tree::ptree & jsonObject, int nRequestID)
{
	int nResult = -1;
	try
	{
		auto optData = jsonObject.get_child_optional("Data");
		if (optData) {
			const boost::property_tree::ptree & jsonData = optData.get();
			std::vector<char*> pInstrumentIDs;
			std::string strInstrumentIDs = jsonData.get<std::string>("InstrumentID", "");
			if (!strInstrumentIDs.empty()) {
				if (strInstrumentIDs.back() != ';') {
					strInstrumentIDs += ';';
				}
				char* pInstrumentID = (char*)&strInstrumentIDs[0];
				for (int i = 0, j = strInstrumentIDs.size(); i < j; i++)
				{
					if (strInstrumentIDs[i] == ';') {
						strInstrumentIDs[i] = 0;
						pInstrumentIDs.push_back(pInstrumentID);
						if ((i + 1) < j) {
							pInstrumentID = (char*)&strInstrumentIDs[i + 1];
						}
					}
				}
			}
			if (!pInstrumentIDs.empty()) {
				nResult = m_pMdApi->getApi()->UnSubscribeMarketData((char**)&pInstrumentIDs[0], pInstrumentIDs.size());
			}
		}
	}
	catch (...)
	{

	}
	return HandleRequestResult(nResult, "UnSubscribeMarketData", nRequestID);
}

///订阅行情应答
void CTPApi::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRspSubMarketData", pRspInfo, nRequestID, bIsLast)) {
			//构造Data
			boost::property_tree::ptree jsonData;
			///经纪公司代码
			jsonData.put("InstrumentID", pSpecificInstrument->InstrumentID);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
}

///取消订阅行情应答
void CTPApi::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRspUnSubMarketData", pRspInfo, nRequestID, bIsLast)) {
			//构造Data
			boost::property_tree::ptree jsonData;
			///经纪公司代码
			jsonData.put("InstrumentID", pSpecificInstrument->InstrumentID);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
}

void CTPApi::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRtnDepthMarketData")) {
			//构造Data
			boost::property_tree::ptree jsonData;
			///交易日
			jsonData.put("TradingDay", pDepthMarketData->TradingDay);
			///合约代码
			jsonData.put("InstrumentID", pDepthMarketData->InstrumentID);
			///交易所代码
			jsonData.put("ExchangeID", pDepthMarketData->ExchangeID);
			///合约在交易所的代码
			jsonData.put("ExchangeInstID", pDepthMarketData->ExchangeInstID);
			///最新价
			jsonData.put("LastPrice", pDepthMarketData->LastPrice);
			///上次结算价
			jsonData.put("PreSettlementPrice", pDepthMarketData->PreSettlementPrice);
			///昨收盘
			jsonData.put("PreClosePrice", pDepthMarketData->PreClosePrice);
			///昨持仓量
			jsonData.put("PreOpenInterest", pDepthMarketData->PreOpenInterest);
			///今开盘
			jsonData.put("OpenPrice", pDepthMarketData->OpenPrice);
			///最高价
			jsonData.put("HighestPrice", pDepthMarketData->HighestPrice);
			///最低价
			jsonData.put("LowestPrice", pDepthMarketData->LowestPrice);
			///数量
			jsonData.put("Volume", pDepthMarketData->Volume);
			///成交金额
			jsonData.put("Turnover", pDepthMarketData->Turnover);
			///持仓量
			jsonData.put("OpenInterest", pDepthMarketData->OpenInterest);
			///今收盘
			jsonData.put("ClosePrice", pDepthMarketData->ClosePrice);
			///本次结算价
			jsonData.put("SettlementPrice", pDepthMarketData->SettlementPrice);
			///涨停板价
			jsonData.put("UpperLimitPrice", pDepthMarketData->UpperLimitPrice);
			///跌停板价
			jsonData.put("LowerLimitPrice", pDepthMarketData->LowerLimitPrice);
			///昨虚实度
			jsonData.put("PreDelta", pDepthMarketData->PreDelta);
			///今虚实度
			jsonData.put("CurrDelta", pDepthMarketData->CurrDelta);
			///最后修改时间
			jsonData.put("UpdateTime", pDepthMarketData->UpdateTime);
			///最后修改毫秒
			jsonData.put("UpdateMillisec", pDepthMarketData->UpdateMillisec);
			///申买价一
			jsonData.put("BidPrice1", pDepthMarketData->BidPrice1);
			///申买量一
			jsonData.put("BidVolume1", pDepthMarketData->BidVolume1);
			///申卖价一
			jsonData.put("AskPrice1", pDepthMarketData->AskPrice1);
			///申卖量一
			jsonData.put("AskVolume1", pDepthMarketData->AskVolume1);
			///申买价二
			jsonData.put("BidPrice2", pDepthMarketData->BidPrice2);
			///申买量二
			jsonData.put("BidVolume2", pDepthMarketData->BidVolume2);
			///申卖价二
			jsonData.put("AskPrice2", pDepthMarketData->AskPrice2);
			///申卖量二
			jsonData.put("AskVolume2", pDepthMarketData->AskVolume2);
			///申买价三
			jsonData.put("BidPrice3", pDepthMarketData->BidPrice3);
			///申买量三
			jsonData.put("BidVolume3", pDepthMarketData->BidVolume3);
			///申卖价三
			jsonData.put("AskPrice3", pDepthMarketData->AskPrice3);
			///申卖量三
			jsonData.put("AskVolume3", pDepthMarketData->AskVolume3);
			///申买价四
			jsonData.put("BidPrice4", pDepthMarketData->BidPrice4);
			///申买量四
			jsonData.put("BidVolume4", pDepthMarketData->BidVolume4);
			///申卖价四
			jsonData.put("AskPrice4", pDepthMarketData->AskPrice4);
			///申卖量四
			jsonData.put("AskVolume4", pDepthMarketData->AskVolume4);
			///申买价五
			jsonData.put("BidPrice5", pDepthMarketData->BidPrice5);
			///申买量五
			jsonData.put("BidVolume5", pDepthMarketData->BidVolume5);
			///申卖价五
			jsonData.put("AskPrice5", pDepthMarketData->AskPrice5);
			///申卖量五
			jsonData.put("AskVolume5", pDepthMarketData->AskVolume5);
			///当日均价
			jsonData.put("AveragePrice", pDepthMarketData->AveragePrice);
			///业务日期
			jsonData.put("ActionDay", pDepthMarketData->ActionDay);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
}

///客户端认证请求
int CTPApi::ReqAuthenticate(const boost::property_tree::ptree & jsonObject, int nRequestID)
{
	int nResult = -1;
	try
	{
		CThostFtdcReqAuthenticateField req = { 0 };
		auto optData = jsonObject.get_child_optional("Data");
		if (optData) {
			const boost::property_tree::ptree & jsonData = optData.get();
			BuildDefaultRequest(req);
			snprintf(req.UserProductInfo, _countof(req.UserProductInfo), "%s", jsonData.get<std::string>("UserProductInfo", "").c_str());
			snprintf(req.AuthCode, _countof(req.UserProductInfo), "%s", jsonData.get<std::string>("AuthCode", "").c_str());
		}
		nResult = m_pTradeApi->getApi()->ReqAuthenticate(&req, nRequestID);
	}
	catch (...)
	{

	}
	return HandleRequestResult(nResult, "ReqAuthenticate", nRequestID);
}

///用户登录请求
int CTPApi::ReqUserLogin(const boost::property_tree::ptree & jsonObject, int nRequestID)
{
	int nResult = -1;
	try
	{
		CThostFtdcReqUserLoginField req = { 0 };
		BuildDefaultRequest(req);
		auto optData = jsonObject.get_child_optional("Data");
		if (optData) {
			const boost::property_tree::ptree & jsonData = optData.get();
			///密码
			snprintf(req.Password, _countof(req.Password), "%s", jsonData.get<std::string>("Password", "").c_str());
			///用户端产品信息
			snprintf(req.UserProductInfo, _countof(req.UserProductInfo), "%s", jsonData.get<std::string>("UserProductInfo", "").c_str());
			///接口端产品信息
			snprintf(req.InterfaceProductInfo, _countof(req.InterfaceProductInfo), "%s", jsonData.get<std::string>("InterfaceProductInfo", "").c_str());
			///协议信息
			snprintf(req.ProtocolInfo, _countof(req.ProtocolInfo), "%s", jsonData.get<std::string>("ProtocolInfo", "").c_str());
			///Mac地址
			snprintf(req.MacAddress, _countof(req.MacAddress), "%s", jsonData.get<std::string>("MacAddress", "").c_str());
			///动态密码
			snprintf(req.OneTimePassword, _countof(req.OneTimePassword), "%s", jsonData.get<std::string>("OneTimePassword", "").c_str());
			///终端IP地址
			snprintf(req.ClientIPAddress, _countof(req.ClientIPAddress), "%s", jsonData.get<std::string>("ClientIPAddress", "").c_str());
			///登录备注
			snprintf(req.LoginRemark, _countof(req.LoginRemark), "%s", jsonData.get<std::string>("LoginRemark", "").c_str());
		}
		nResult = m_pTradeApi->getApi()->ReqUserLogin(&req, nRequestID);
	}
	catch (...)
	{

	}
	return HandleRequestResult(nResult, "ReqUserLogin", nRequestID);
}


///登出请求
int CTPApi::ReqUserLogout(const boost::property_tree::ptree & jsonObject, int nRequestID)
{
	int nResult = -1;
	try
	{
		CThostFtdcUserLogoutField req = { 0 };
		BuildDefaultRequest(req);
		nResult = m_pTradeApi->getApi()->ReqUserLogout(&req, nRequestID);
	}
	catch (...)
	{

	}
	return HandleRequestResult(nResult, "ReqUserLogout", nRequestID);
}

///用户口令更新请求
int CTPApi::ReqUserPasswordUpdate(const boost::property_tree::ptree & jsonObject, int nRequestID)
{
	int nResult = -1;
	try
	{
		CThostFtdcUserPasswordUpdateField req = { 0 };
		BuildDefaultRequest(req);
		auto optData = jsonObject.get_child_optional("Data");
		if (optData) {
			const boost::property_tree::ptree & jsonData = optData.get();
			///原来的口令
			snprintf(req.OldPassword, _countof(req.OldPassword), "%s", jsonData.get<std::string>("OldPassword", "").c_str());
			///新的口令
			snprintf(req.NewPassword, _countof(req.NewPassword), "%s", jsonData.get<std::string>("NewPassword", "").c_str());
		}
		nResult = m_pTradeApi->getApi()->ReqUserPasswordUpdate(&req, nRequestID);
	}
	catch (...)
	{

	}
	return HandleRequestResult(nResult, "ReqUserPasswordUpdate", nRequestID);
}

///资金账户口令更新请求
int CTPApi::ReqTradingAccountPasswordUpdate(const boost::property_tree::ptree & jsonObject, int nRequestID)
{
	int nResult = -1;
	try
	{
		CThostFtdcTradingAccountPasswordUpdateField req = { 0 };
		auto optData = jsonObject.get_child_optional("Data");
		if (optData) {
			const boost::property_tree::ptree & jsonData = optData.get();
			BuildAccountRequest(req, jsonData);
			///原来的口令
			snprintf(req.OldPassword, _countof(req.OldPassword), "%s", jsonData.get<std::string>("OldPassword", "").c_str());
			///新的口令
			snprintf(req.NewPassword, _countof(req.NewPassword), "%s", jsonData.get<std::string>("NewPassword", "").c_str());
			///币种代码
			snprintf(req.CurrencyID, _countof(req.CurrencyID), "%s", jsonData.get<std::string>("CurrencyID", "").c_str());
		}
		nResult = m_pTradeApi->getApi()->ReqTradingAccountPasswordUpdate(&req, nRequestID);
	}
	catch (...)
	{

	}
	return HandleRequestResult(nResult, "ReqTradingAccountPasswordUpdate", nRequestID);
}

///报单录入请求
int CTPApi::ReqOrderInsert(const boost::property_tree::ptree & jsonObject, int nRequestID)
{
	int nResult = -1;
	try
	{
		CThostFtdcInputOrderField req = { 0 };
		auto optData = jsonObject.get_child_optional("Data");
		if (optData) {
			const boost::property_tree::ptree & jsonData = optData.get();
			BuildTradeRequest(req, jsonData);
			///合约代码
			snprintf(req.InstrumentID, _countof(req.InstrumentID), "%s", jsonData.get<std::string>("InstrumentID", "").c_str());
			///报单引用
			snprintf(req.OrderRef, _countof(req.OrderRef), "%s", jsonData.get<std::string>("OrderRef", "").c_str());
			///报单价格条件
			req.OrderPriceType = jsonData.get<TThostFtdcOrderPriceTypeType>("OrderPriceType", (char)0);
			///买卖方向
			req.Direction = jsonData.get<TThostFtdcDirectionType>("Direction", (char)0);
			///组合开平标志
			snprintf(req.CombOffsetFlag, _countof(req.CombOffsetFlag), "%s", jsonData.get<std::string>("CombOffsetFlag", "").c_str());
			///组合投机套保标志
			snprintf(req.CombHedgeFlag, _countof(req.CombHedgeFlag), "%s", jsonData.get<std::string>("CombHedgeFlag", "").c_str());
			///价格
			req.LimitPrice = jsonData.get<TThostFtdcPriceType>("LimitPrice", (char)0);
			///数量
			req.VolumeTotalOriginal = jsonData.get<TThostFtdcVolumeType>("VolumeTotalOriginal", (char)0);
			///有效期类型
			req.TimeCondition = jsonData.get<TThostFtdcTimeConditionType>("TimeCondition", (char)0);
			///GTD日期
			snprintf(req.GTDDate, _countof(req.GTDDate), "%s", jsonData.get<std::string>("GTDDate", "").c_str());
			///成交量类型
			req.VolumeCondition = jsonData.get<TThostFtdcVolumeConditionType>("VolumeCondition", (char)0);
			///最小成交量
			req.MinVolume = jsonData.get<TThostFtdcVolumeType>("MinVolume", (char)0);
			///触发条件
			req.ContingentCondition = jsonData.get<TThostFtdcContingentConditionType>("ContingentCondition", (char)0);
			///止损价
			req.StopPrice = jsonData.get<TThostFtdcPriceType>("StopPrice", (char)0);
			///强平原因
			req.ForceCloseReason = jsonData.get<TThostFtdcForceCloseReasonType>("ForceCloseReason", (char)0);
			///自动挂起标志
			req.IsAutoSuspend = jsonData.get<TThostFtdcBoolType>("IsAutoSuspend", (char)0);
			///业务单元
			snprintf(req.BusinessUnit, _countof(req.BusinessUnit), "%s", jsonData.get<std::string>("BusinessUnit", "").c_str());
			///请求编号
			req.RequestID = jsonData.get<TThostFtdcRequestIDType>("RequestID", (char)0);
			///用户强评标志
			req.UserForceClose = jsonData.get<TThostFtdcBoolType>("UserForceClose", (char)0);
			///互换单标志
			req.IsSwapOrder = jsonData.get<TThostFtdcBoolType>("IsSwapOrder", (char)0);
			///交易所代码
			snprintf(req.ExchangeID, _countof(req.ExchangeID), "%s", jsonData.get<std::string>("ExchangeID", "").c_str());
			///投资单元代码
			snprintf(req.InvestUnitID, _countof(req.InvestUnitID), "%s", jsonData.get<std::string>("InvestUnitID", "").c_str());
			///币种代码
			snprintf(req.CurrencyID, _countof(req.CurrencyID), "%s", jsonData.get<std::string>("CurrencyID", "").c_str());
			///交易编码
			snprintf(req.ClientID, _countof(req.ClientID), "%s", jsonData.get<std::string>("ClientID", "").c_str());
			///IP地址
			snprintf(req.IPAddress, _countof(req.IPAddress), "%s", jsonData.get<std::string>("IPAddress", "").c_str());
			///Mac地址
			snprintf(req.MacAddress, _countof(req.MacAddress), "%s", jsonData.get<std::string>("MacAddress", "").c_str());
		}
		nResult = m_pTradeApi->getApi()->ReqOrderInsert(&req, nRequestID);
	}
	catch (...)
	{

	}
	return HandleRequestResult(nResult, "ReqOrderInsert", nRequestID);
}

///预埋单录入请求
int CTPApi::ReqParkedOrderInsert(const boost::property_tree::ptree & jsonObject, int nRequestID)
{
	int nResult = -1;
	try
	{
		CThostFtdcParkedOrderField req = { 0 };
		auto optData = jsonObject.get_child_optional("Data");
		if (optData) {
			const boost::property_tree::ptree & jsonData = optData.get();
			BuildTradeRequest(req, jsonData);
			///合约代码
			snprintf(req.InstrumentID, _countof(req.InstrumentID), "%s", jsonData.get<std::string>("InstrumentID", "").c_str());
			///报单引用
			snprintf(req.OrderRef, _countof(req.OrderRef), "%s", jsonData.get<std::string>("OrderRef", "").c_str());
			///报单价格条件
			req.OrderPriceType = jsonData.get<TThostFtdcOrderPriceTypeType>("OrderPriceType", (char)0);
			///买卖方向
			req.Direction = jsonData.get<TThostFtdcDirectionType>("Direction", (char)0);
			///组合开平标志
			snprintf(req.CombOffsetFlag, _countof(req.CombOffsetFlag), "%s", jsonData.get<std::string>("CombOffsetFlag", "").c_str());
			///组合投机套保标志
			snprintf(req.CombHedgeFlag, _countof(req.CombHedgeFlag), "%s", jsonData.get<std::string>("CombHedgeFlag", "").c_str());
			///价格
			req.LimitPrice = jsonData.get<TThostFtdcPriceType>("LimitPrice", (char)0);
			///数量
			req.VolumeTotalOriginal = jsonData.get<TThostFtdcVolumeType>("LimitPrice", (char)0);
			///有效期类型
			req.TimeCondition = jsonData.get<TThostFtdcTimeConditionType>("TimeCondition", (char)0);
			///GTD日期
			snprintf(req.GTDDate, _countof(req.GTDDate), "%s", jsonData.get<std::string>("GTDDate", "").c_str());
			///成交量类型
			req.VolumeCondition = jsonData.get<TThostFtdcVolumeConditionType>("VolumeCondition", (char)0);
			///最小成交量
			req.MinVolume = jsonData.get<TThostFtdcVolumeType>("MinVolume", (char)0);
			///触发条件
			req.ContingentCondition = jsonData.get<TThostFtdcContingentConditionType>("ContingentCondition", (char)0);
			///止损价
			req.StopPrice = jsonData.get<TThostFtdcPriceType>("StopPrice", (char)0);
			///强平原因
			req.ForceCloseReason = jsonData.get<TThostFtdcForceCloseReasonType>("ForceCloseReason", (char)0);
			///自动挂起标志
			req.IsAutoSuspend = jsonData.get<TThostFtdcBoolType>("IsAutoSuspend", (char)0);
			///业务单元
			snprintf(req.BusinessUnit, _countof(req.BusinessUnit), "%s", jsonData.get<std::string>("BusinessUnit", "").c_str());
			///请求编号
			req.RequestID = jsonData.get<TThostFtdcRequestIDType>("RequestID", (char)0);
			///用户强评标志
			req.UserForceClose = jsonData.get<TThostFtdcBoolType>("UserForceClose", (char)0);
			///交易所代码
			snprintf(req.ExchangeID, _countof(req.ExchangeID), "%s", jsonData.get<std::string>("ExchangeID", "").c_str());
			///预埋报单编号
			snprintf(req.ParkedOrderID, _countof(req.ParkedOrderID), "%s", jsonData.get<std::string>("ParkedOrderID", "").c_str());
			///用户类型
			req.UserType = jsonData.get<TThostFtdcUserTypeType>("UserType", (char)0);
			///预埋单状态
			req.Status = jsonData.get<TThostFtdcParkedOrderStatusType>("Status", (char)0);
			///错误代码
			req.ErrorID = jsonData.get<TThostFtdcErrorIDType>("ErrorID", (char)0);
			///错误信息
			snprintf(req.ErrorMsg, _countof(req.ErrorMsg), "%s", jsonData.get<std::string>("ErrorMsg", "").c_str());
			///互换单标志
			req.IsSwapOrder = jsonData.get<TThostFtdcBoolType>("IsSwapOrder", (char)0);
			///币种代码
			snprintf(req.CurrencyID, _countof(req.CurrencyID), "%s", jsonData.get<std::string>("CurrencyID", "").c_str());
			///交易编码
			snprintf(req.ClientID, _countof(req.ClientID), "%s", jsonData.get<std::string>("CurrencyID", "").c_str());
			///投资单元代码
			snprintf(req.InvestUnitID, _countof(req.InvestUnitID), "%s", jsonData.get<std::string>("InvestUnitID", "").c_str());
			///IP地址
			snprintf(req.IPAddress, _countof(req.IPAddress), "%s", jsonData.get<std::string>("IPAddress", "").c_str());
			///Mac地址
			snprintf(req.MacAddress, _countof(req.MacAddress), "%s", jsonData.get<std::string>("MacAddress", "").c_str());
		}
		nResult = m_pTradeApi->getApi()->ReqParkedOrderInsert(&req, nRequestID);
	}
	catch (...)
	{

	}
	return HandleRequestResult(nResult, "ReqParkedOrderInsert", nRequestID);
}

///预埋撤单录入请求
int CTPApi::ReqParkedOrderAction(const boost::property_tree::ptree & jsonObject, int nRequestID)
{
	int nResult = -1;
	try
	{
		CThostFtdcParkedOrderActionField req = { 0 };
		auto optData = jsonObject.get_child_optional("Data");
		if (optData) {
			const boost::property_tree::ptree & jsonData = optData.get();
			BuildTradeRequest(req, jsonData);
			///报单操作引用
			req.OrderActionRef = jsonData.get<TThostFtdcOrderActionRefType>("OrderActionRef", (char)0);
			///报单引用
			snprintf(req.OrderRef, _countof(req.OrderRef), jsonData.get<std::string>("OrderRef", "").c_str());
			///请求编号
			req.RequestID = jsonData.get<TThostFtdcRequestIDType>("RequestID", (char)0);
			///前置编号
			req.FrontID = jsonData.get<TThostFtdcFrontIDType>("FrontID", (char)0);
			///会话编号
			req.SessionID = jsonData.get<TThostFtdcSessionIDType>("SessionID", (char)0);
			///交易所代码
			snprintf(req.ExchangeID, _countof(req.ExchangeID), "%s", jsonData.get<std::string>("ExchangeID", "").c_str());
			///报单编号
			snprintf(req.OrderSysID, _countof(req.OrderSysID), "%s", jsonData.get<std::string>("OrderSysID", "").c_str());
			///操作标志
			req.ActionFlag = jsonData.get<TThostFtdcActionFlagType>("ActionFlag", (char)0);
			///价格
			req.LimitPrice = jsonData.get<TThostFtdcPriceType>("LimitPrice", (char)0);
			///数量变化
			req.VolumeChange = jsonData.get<TThostFtdcVolumeType>("VolumeChange", (char)0);
			///合约代码
			snprintf(req.InstrumentID, _countof(req.InstrumentID), "%s", jsonData.get<std::string>("InstrumentID", "").c_str());
			///预埋撤单单编号
			snprintf(req.ParkedOrderActionID, _countof(req.ParkedOrderActionID), "%s", jsonData.get<std::string>("ParkedOrderActionID", "").c_str());
			///用户类型
			req.UserType = jsonData.get<TThostFtdcUserTypeType>("UserType", (char)0);
			///预埋单状态
			req.Status = jsonData.get<TThostFtdcParkedOrderStatusType>("Status", (char)0);
			///错误代码
			req.ErrorID = jsonData.get<TThostFtdcErrorIDType>("ErrorID", (char)0);
			///错误信息
			snprintf(req.ErrorMsg, _countof(req.ErrorMsg), "%s", jsonData.get<std::string>("ErrorMsg", "").c_str());
			///投资单元代码
			snprintf(req.InvestUnitID, _countof(req.InvestUnitID), "%s", jsonData.get<std::string>("InvestUnitID", "").c_str());
			///IP地址
			snprintf(req.IPAddress, _countof(req.IPAddress), "%s", jsonData.get<std::string>("IPAddress", "").c_str());
			///Mac地址
			snprintf(req.MacAddress, _countof(req.MacAddress), "%s", jsonData.get<std::string>("MacAddress", "").c_str());
		}
		nResult = m_pTradeApi->getApi()->ReqParkedOrderAction(&req, nRequestID);
	}
	catch (...)
	{

	}
	return HandleRequestResult(nResult, "ReqParkedOrderAction", nRequestID);
}

///报单操作请求
int CTPApi::ReqOrderAction(const boost::property_tree::ptree & jsonObject, int nRequestID)
{
	int nResult = -1;
	try
	{
		CThostFtdcInputOrderActionField req = { 0 };
		auto optData = jsonObject.get_child_optional("Data");
		if (optData) {
			const boost::property_tree::ptree & jsonData = optData.get();
			BuildTradeRequest(req, jsonData);
			///合约代码
			snprintf(req.InstrumentID, _countof(req.InstrumentID), "%s", jsonData.get<std::string>("InstrumentID", "").c_str());
			///报单操作引用
			req.OrderActionRef = jsonData.get<TThostFtdcOrderActionRefType>("OrderActionRef", (char)0);
			///报单引用
			snprintf(req.OrderRef, _countof(req.OrderRef), "%s", jsonData.get<std::string>("OrderRef", "").c_str());
			///价格
			req.LimitPrice = jsonData.get<TThostFtdcPriceType>("LimitPrice", (char)0);
			///请求编号
			req.RequestID = jsonData.get<TThostFtdcRequestIDType>("RequestID", (char)0);
			///前置编号
			req.FrontID = jsonData.get<TThostFtdcFrontIDType>("FrontID", (char)0);
			///会话编号
			req.SessionID = jsonData.get<TThostFtdcSessionIDType>("SessionID", (char)0);
			///交易所代码
			snprintf(req.ExchangeID, _countof(req.ExchangeID), "%s", jsonData.get<std::string>("ExchangeID", "").c_str());
			///报单编号
			snprintf(req.OrderSysID, _countof(req.OrderSysID), "%s", jsonData.get<std::string>("OrderSysID", "").c_str());
			///操作标志
			req.ActionFlag = jsonData.get<TThostFtdcActionFlagType>("ActionFlag", (char)0);
			///价格
			req.LimitPrice = jsonData.get<TThostFtdcPriceType>("LimitPrice", (char)0);
			///数量变化
			req.VolumeChange = jsonData.get<TThostFtdcVolumeType>("VolumeChange", (char)0);
			///投资单元代码
			snprintf(req.InvestUnitID, _countof(req.InvestUnitID), "%s", jsonData.get<std::string>("InvestUnitID", "").c_str());
			///IP地址
			snprintf(req.IPAddress, _countof(req.IPAddress), "%s", jsonData.get<std::string>("IPAddress", "").c_str());
			///Mac地址
			snprintf(req.MacAddress, _countof(req.MacAddress), "%s", jsonData.get<std::string>("MacAddress", "").c_str());
		}
		nResult = m_pTradeApi->getApi()->ReqOrderAction(&req, nRequestID);
	}
	catch (...)
	{

	}
	return HandleRequestResult(nResult, "ReqOrderAction", nRequestID);
}

///查询最大报单数量请求
int CTPApi::ReqQueryMaxOrderVolume(const boost::property_tree::ptree & jsonObject, int nRequestID)
{
	int nResult = -1;
	try
	{
		const boost::property_tree::ptree & jsonData = jsonObject.get_child("Data");
		CThostFtdcQueryMaxOrderVolumeField req = { 0 };
		auto optData = jsonObject.get_child_optional("Data");
		if (optData) {
			const boost::property_tree::ptree & jsonData = optData.get();
			BuildInvestorRequest(req, jsonData);
			///合约代码
			snprintf(req.InstrumentID, _countof(req.InstrumentID), "%s", jsonData.get<std::string>("InstrumentID", "").c_str());
			///买卖方向
			req.Direction = jsonData.get<TThostFtdcDirectionType>("Direction", (char)0);
			///开平标志
			req.OffsetFlag = jsonData.get<TThostFtdcOffsetFlagType>("OffsetFlag", (char)0);
			///投机套保标志
			req.HedgeFlag = jsonData.get<TThostFtdcHedgeFlagType>("HedgeFlag", (char)0);
			///最大允许报单数量
			req.MaxVolume = jsonData.get<TThostFtdcVolumeType>("MaxVolume", (char)0);
			///交易所代码
			snprintf(req.ExchangeID, _countof(req.ExchangeID), "%s", jsonData.get<std::string>("ExchangeID", "").c_str());
		}
		nResult = m_pTradeApi->getApi()->ReqQueryMaxOrderVolume(&req, nRequestID);
	}
	catch (...)
	{

	}
	return HandleRequestResult(nResult, "ReqQueryMaxOrderVolume", nRequestID);
}

///投资者结算结果确认
int CTPApi::ReqSettlementInfoConfirm(const boost::property_tree::ptree & jsonObject, int nRequestID)
{
	int nResult = -1;
	try
	{
		const boost::property_tree::ptree & jsonData = jsonObject.get_child("Data");
		CThostFtdcSettlementInfoConfirmField req = { 0 };
		auto optData = jsonObject.get_child_optional("Data");
		if (optData) {
			const boost::property_tree::ptree & jsonData = optData.get();
			BuildInvestorRequest(req, jsonData);
			///确认日期
			snprintf(req.ConfirmDate, _countof(req.ConfirmDate), "%s", jsonData.get<std::string>("ConfirmDate", "").c_str());
			///确认时间
			snprintf(req.ConfirmTime, _countof(req.ConfirmTime), "%s", jsonData.get<std::string>("ConfirmTime", "").c_str());
		}
		nResult = m_pTradeApi->getApi()->ReqSettlementInfoConfirm(&req, nRequestID);
	}
	catch (...)
	{

	}
	return HandleRequestResult(nResult, "ReqSettlementInfoConfirm", nRequestID);
}

///请求删除预埋单
int CTPApi::ReqRemoveParkedOrder(const boost::property_tree::ptree & jsonObject, int nRequestID)
{
	int nResult = -1;
	try
	{
		const boost::property_tree::ptree & jsonData = jsonObject.get_child("Data");
		CThostFtdcRemoveParkedOrderField req = { 0 };
		auto optData = jsonObject.get_child_optional("Data");
		if (optData) {
			const boost::property_tree::ptree & jsonData = optData.get();
			BuildInvestorRequest(req, jsonData);
			///预埋报单编号
			snprintf(req.ParkedOrderID, _countof(req.ParkedOrderID), "%s", jsonData.get<std::string>("ParkedOrderID", "").c_str());
		}
		nResult = m_pTradeApi->getApi()->ReqRemoveParkedOrder(&req, nRequestID);
	}
	catch (...)
	{

	}
	return HandleRequestResult(nResult, "ReqRemoveParkedOrder", nRequestID);
}

///请求删除预埋撤单
int CTPApi::ReqRemoveParkedOrderAction(const boost::property_tree::ptree & jsonObject, int nRequestID)
{
	int nResult = -1;
	try
	{
		const boost::property_tree::ptree & jsonData = jsonObject.get_child("Data");
		CThostFtdcRemoveParkedOrderActionField req = { 0 };
		auto optData = jsonObject.get_child_optional("Data");
		if (optData) {
			const boost::property_tree::ptree & jsonData = optData.get();
			BuildInvestorRequest(req, jsonData);
			///预埋报单编号
			snprintf(req.ParkedOrderActionID, _countof(req.ParkedOrderActionID), "%s", jsonData.get<std::string>("ParkedOrderActionID", "").c_str());
		}
		nResult = m_pTradeApi->getApi()->ReqRemoveParkedOrderAction(&req, nRequestID);
	}
	catch (...)
	{

	}
	return HandleRequestResult(nResult, "ReqRemoveParkedOrderAction", nRequestID);
}

///执行宣告录入请求
int CTPApi::ReqExecOrderInsert(const boost::property_tree::ptree & jsonObject, int nRequestID)
{
	int nResult = -1;
	try
	{
		const boost::property_tree::ptree & jsonData = jsonObject.get_child("Data");
		CThostFtdcInputExecOrderField req = { 0 };
		auto optData = jsonObject.get_child_optional("Data");
		if (optData) {
			const boost::property_tree::ptree & jsonData = optData.get();
			BuildTradeRequest(req, jsonData);
		}
		nResult = m_pTradeApi->getApi()->ReqExecOrderInsert(&req, nRequestID);
	}
	catch (...)
	{

	}
	return HandleRequestResult(nResult, "ReqExecOrderInsert", nRequestID);
}

///执行宣告操作请求
int CTPApi::ReqExecOrderAction(const boost::property_tree::ptree & jsonObject, int nRequestID)
{
	int nResult = -1;
	try
	{
		CThostFtdcInputExecOrderActionField req = { 0 };
		auto optData = jsonObject.get_child_optional("Data");
		if (optData) {
			const boost::property_tree::ptree & jsonData = optData.get();
			BuildTradeRequest(req, jsonData);
		}
		nResult = m_pTradeApi->getApi()->ReqExecOrderAction(&req, nRequestID);
	}
	catch (...)
	{

	}
	return HandleRequestResult(nResult, "ReqExecOrderAction", nRequestID);
}

///询价录入请求
int CTPApi::ReqForQuoteInsert(const boost::property_tree::ptree & jsonObject, int nRequestID)
{
	int nResult = -1;
	try
	{
		CThostFtdcInputForQuoteField req = { 0 };
		auto optData = jsonObject.get_child_optional("Data");
		if (optData) {
			const boost::property_tree::ptree & jsonData = optData.get();
			BuildTradeRequest(req, jsonData);
		}
		nResult = m_pTradeApi->getApi()->ReqForQuoteInsert(&req, nRequestID);
	}
	catch (...)
	{

	}
	return HandleRequestResult(nResult, "ReqForQuoteInsert", nRequestID);
}

///报价录入请求
int CTPApi::ReqQuoteInsert(const boost::property_tree::ptree & jsonObject, int nRequestID)
{
	int nResult = -1;
	try
	{
		CThostFtdcInputQuoteField req = { 0 };
		auto optData = jsonObject.get_child_optional("Data");
		if (optData) {
			const boost::property_tree::ptree & jsonData = optData.get();
			BuildTradeRequest(req, jsonData);
		}
		nResult = m_pTradeApi->getApi()->ReqQuoteInsert(&req, nRequestID);
	}
	catch (...)
	{

	}
	return HandleRequestResult(nResult, "ReqQuoteInsert", nRequestID);
}

///报价操作请求
int CTPApi::ReqQuoteAction(const boost::property_tree::ptree & jsonObject, int nRequestID)
{
	int nResult = -1;
	try
	{
		CThostFtdcInputQuoteActionField req = { 0 };
		auto optData = jsonObject.get_child_optional("Data");
		if (optData) {
			const boost::property_tree::ptree & jsonData = optData.get();
			BuildTradeRequest(req, jsonData);
		}
		nResult = m_pTradeApi->getApi()->ReqQuoteAction(&req, nRequestID);
	}
	catch (...)
	{

	}
	return HandleRequestResult(nResult, "ReqQuoteAction", nRequestID);
}

///锁定请求
int CTPApi::ReqLockInsert(const boost::property_tree::ptree & jsonObject, int nRequestID)
{
	int nResult = -1;
	try
	{
		CThostFtdcInputLockField req = { 0 };
		auto optData = jsonObject.get_child_optional("Data");
		if (optData) {
			const boost::property_tree::ptree & jsonData = optData.get();
			BuildTradeRequest(req, jsonData);
		}
		nResult = m_pTradeApi->getApi()->ReqLockInsert(&req, nRequestID);
	}
	catch (...)
	{

	}
	return HandleRequestResult(nResult, "ReqLockInsert", nRequestID);
}

///批量报单操作请求
int CTPApi::ReqBatchOrderAction(const boost::property_tree::ptree & jsonObject, int nRequestID)
{
	int nResult = -1;
	try
	{
		CThostFtdcInputBatchOrderActionField req = { 0 };
		auto optData = jsonObject.get_child_optional("Data");
		if (optData) {
			const boost::property_tree::ptree & jsonData = optData.get();
			BuildTradeRequest(req, jsonData);
		}
		nResult = m_pTradeApi->getApi()->ReqBatchOrderAction(&req, nRequestID);
	}
	catch (...)
	{

	}
	return HandleRequestResult(nResult, "ReqBatchOrderAction", nRequestID);
}

///申请组合录入请求
int CTPApi::ReqCombActionInsert(const boost::property_tree::ptree & jsonObject, int nRequestID)
{
	int nResult = -1;
	try
	{
		CThostFtdcInputCombActionField req = { 0 };
		auto optData = jsonObject.get_child_optional("Data");
		if (optData) {
			const boost::property_tree::ptree & jsonData = optData.get();
			BuildTradeRequest(req, jsonData);
		}
		nResult = m_pTradeApi->getApi()->ReqCombActionInsert(&req, nRequestID);
	}
	catch (...)
	{

	}
	return HandleRequestResult(nResult, "ReqCombActionInsert", nRequestID);
}

///请求查询报单
int CTPApi::ReqQryOrder(const boost::property_tree::ptree & jsonObject, int nRequestID)
{
	int nResult = -1;
	try
	{
		CThostFtdcQryOrderField req = { 0 };
		auto optData = jsonObject.get_child_optional("Data");
		if (optData) {
			const boost::property_tree::ptree & jsonData = optData.get();
			BuildInvestorRequest(req, jsonData);
			///合约代码
			snprintf(req.InstrumentID, _countof(req.InstrumentID), "%s", jsonData.get<std::string>("InstrumentID", "").c_str());
			///交易所代码
			snprintf(req.ExchangeID, _countof(req.ExchangeID), "%s", jsonData.get<std::string>("ExchangeID", "").c_str());
			///报单编号
			snprintf(req.OrderSysID, _countof(req.OrderSysID), "%s", jsonData.get<std::string>("OrderSysID", "").c_str());
			///开始时间
			snprintf(req.InsertTimeStart, _countof(req.InsertTimeStart), "%s", jsonData.get<std::string>("InsertTimeStart", "").c_str());
			///结束时间
			snprintf(req.InsertTimeEnd, _countof(req.InsertTimeEnd), "%s", jsonData.get<std::string>("InsertTimeEnd", "").c_str());
		}
		nResult = m_pTradeApi->getApi()->ReqQryOrder(&req, nRequestID);
	}
	catch (...)
	{

	}
	return HandleRequestResult(nResult, "ReqQryOrder", nRequestID);
}

///请求查询成交
int CTPApi::ReqQryTrade(const boost::property_tree::ptree & jsonObject, int nRequestID)
{
	int nResult = -1;
	try
	{
		CThostFtdcQryTradeField req = { 0 };
		auto optData = jsonObject.get_child_optional("Data");
		if (optData) {
			const boost::property_tree::ptree & jsonData = optData.get();
			BuildInvestorRequest(req, jsonData);
			///合约代码
			snprintf(req.InstrumentID, _countof(req.InstrumentID), "%s", jsonData.get<std::string>("InstrumentID", "").c_str());
			///交易所代码
			snprintf(req.ExchangeID, _countof(req.ExchangeID), "%s", jsonData.get<std::string>("ExchangeID", "").c_str());
			///成交编号
			snprintf(req.TradeID, _countof(req.TradeID), "%s", jsonData.get<std::string>("TradeID", "").c_str());
			///开始时间
			snprintf(req.TradeTimeStart, _countof(req.TradeTimeStart), "%s", jsonData.get<std::string>("TradeTimeStart", "").c_str());
			///结束时间
			snprintf(req.TradeTimeEnd, _countof(req.TradeTimeEnd), "%s", jsonData.get<std::string>("TradeTimeEnd", "").c_str());
		}
		nResult = m_pTradeApi->getApi()->ReqQryTrade(&req, nRequestID);
	}
	catch (...)
	{

	}
	return HandleRequestResult(nResult, "ReqQryTrade", nRequestID);
}

///请求查询投资者持仓
int CTPApi::ReqQryInvestorPosition(const boost::property_tree::ptree & jsonObject, int nRequestID)
{
	int nResult = -1;
	try
	{

		CThostFtdcQryInvestorPositionField req = { 0 };
		auto optData = jsonObject.get_child_optional("Data");
		if (optData) {
			const boost::property_tree::ptree & jsonData = optData.get();
			BuildInvestorRequest(req, jsonData);
			///合约代码
			snprintf(req.InstrumentID, _countof(req.InstrumentID), "%s", jsonData.get<std::string>("InstrumentID", "").c_str());
			///交易所代码
			snprintf(req.ExchangeID, _countof(req.ExchangeID), "%s", jsonData.get<std::string>("ExchangeID", "").c_str());
		}
		nResult = m_pTradeApi->getApi()->ReqQryInvestorPosition(&req, nRequestID);
	}
	catch (...)
	{

	}
	return HandleRequestResult(nResult, "ReqQryInvestorPosition", nRequestID);
}

///请求查询资金账户
int CTPApi::ReqQryTradingAccount(const boost::property_tree::ptree & jsonObject, int nRequestID)
{
	int nResult = -1;
	try
	{
		CThostFtdcQryTradingAccountField req = { 0 };
		auto optData = jsonObject.get_child_optional("Data");
		if (optData) {
			const boost::property_tree::ptree & jsonData = optData.get();
			BuildInvestorRequest(req, jsonData);
			///币种代码
			snprintf(req.CurrencyID, _countof(req.CurrencyID), "%s", jsonData.get<std::string>("CurrencyID", "").c_str());
			///业务类型
			req.BizType = jsonData.get<TThostFtdcBizTypeType>("BizType", (char)0);
		}
		nResult = m_pTradeApi->getApi()->ReqQryTradingAccount(&req, nRequestID);
	}
	catch (...)
	{

	}
	return HandleRequestResult(nResult, "ReqQryTradingAccount", nRequestID);
}

///请求查询投资者
int CTPApi::ReqQryInvestor(const boost::property_tree::ptree & jsonObject, int nRequestID)
{
	int nResult = -1;
	try
	{
		CThostFtdcQryInvestorField req = { 0 };
		auto optData = jsonObject.get_child_optional("Data");
		if (optData) {
			const boost::property_tree::ptree & jsonData = optData.get();
			BuildInvestorRequest(req, jsonData);
		}
		nResult = m_pTradeApi->getApi()->ReqQryInvestor(&req, nRequestID);
	}
	catch (...)
	{

	}
	return HandleRequestResult(nResult, "ReqQryInvestor", nRequestID);
}

///请求查询交易编码
int CTPApi::ReqQryTradingCode(const boost::property_tree::ptree & jsonObject, int nRequestID)
{
	int nResult = -1;
	try
	{
		CThostFtdcQryTradingCodeField req = { 0 };
		auto optData = jsonObject.get_child_optional("Data");
		if (optData) {
			const boost::property_tree::ptree & jsonData = optData.get();
			BuildInvestorRequest(req, jsonData);
			///交易所代码
			snprintf(req.ExchangeID, _countof(req.ExchangeID), "%s", jsonData.get<std::string>("ExchangeID", "").c_str());
			///客户代码
			snprintf(req.ClientID, _countof(req.ClientID), "%s", jsonData.get<std::string>("ClientID", "").c_str());
			///交易编码类型
			req.ClientIDType = jsonData.get<TThostFtdcClientIDTypeType>("ClientIDType", (char)0);
		}
		nResult = m_pTradeApi->getApi()->ReqQryTradingCode(&req, nRequestID);
	}
	catch (...)
	{

	}
	return HandleRequestResult(nResult, "ReqQryTradingCode", nRequestID);
}

///请求查询合约保证金率
int CTPApi::ReqQryInstrumentMarginRate(const boost::property_tree::ptree & jsonObject, int nRequestID)
{
	int nResult = -1;
	try
	{
		CThostFtdcQryInstrumentMarginRateField req = { 0 };
		auto optData = jsonObject.get_child_optional("Data");
		if (optData) {
			const boost::property_tree::ptree & jsonData = optData.get();
			BuildInvestorRequest(req, jsonData);
			///合约代码
			snprintf(req.InstrumentID, _countof(req.InstrumentID), "%s", jsonData.get<std::string>("InstrumentID", "").c_str());
			///投机套保标志
			req.HedgeFlag = jsonData.get<TThostFtdcHedgeFlagType>("HedgeFlag", (char)0);
		}
		nResult = m_pTradeApi->getApi()->ReqQryInstrumentMarginRate(&req, nRequestID);
	}
	catch (...)
	{

	}
	return HandleRequestResult(nResult, "ReqQryInstrumentMarginRate", nRequestID);
}

///请求查询合约手续费率
int CTPApi::ReqQryInstrumentCommissionRate(const boost::property_tree::ptree & jsonObject, int nRequestID)
{
	int nResult = -1;
	try
	{
		CThostFtdcQryInstrumentCommissionRateField req = { 0 };
		auto optData = jsonObject.get_child_optional("Data");
		if (optData) {
			const boost::property_tree::ptree & jsonData = optData.get();
			BuildInvestorRequest(req, jsonData);
			///合约代码
			snprintf(req.InstrumentID, _countof(req.InstrumentID), "%s", jsonData.get<std::string>("InstrumentID", "").c_str());
			///交易所代码
			snprintf(req.ExchangeID, _countof(req.ExchangeID), "%s", jsonData.get<std::string>("ExchangeID", "").c_str());
		}
		nResult = m_pTradeApi->getApi()->ReqQryInstrumentCommissionRate(&req, nRequestID);
	}
	catch (...)
	{

	}
	return HandleRequestResult(nResult, "ReqQryInstrumentCommissionRate", nRequestID);
}

///请求查询交易所
int CTPApi::ReqQryExchange(const boost::property_tree::ptree & jsonObject, int nRequestID)
{
	int nResult = -1;
	try
	{
		CThostFtdcQryExchangeField req = { 0 };
		auto optData = jsonObject.get_child_optional("Data");
		if (optData) {
			const boost::property_tree::ptree & jsonData = optData.get();
			///交易所代码
			snprintf(req.ExchangeID, _countof(req.ExchangeID), "%s", jsonData.get<std::string>("ExchangeID", "").c_str());
		}
		nResult = m_pTradeApi->getApi()->ReqQryExchange(&req, nRequestID);
	}
	catch (...)
	{

	}
	return HandleRequestResult(nResult, "ReqQryExchange", nRequestID);
}

///请求查询产品
int CTPApi::ReqQryProduct(const boost::property_tree::ptree & jsonObject, int nRequestID)
{
	int nResult = -1;
	try
	{
		CThostFtdcQryProductField req = { 0 };
		auto optData = jsonObject.get_child_optional("Data");
		if (optData) {
			const boost::property_tree::ptree & jsonData = optData.get();
			///产品代码
			snprintf(req.ProductID, _countof(req.ProductID), "%s", jsonData.get<std::string>("ProductID", "").c_str());
			///产品类型
			req.ProductClass = jsonData.get<TThostFtdcProductClassType>("ProductClass", (char)0);
			///交易所代码
			snprintf(req.ExchangeID, _countof(req.ExchangeID), "%s", jsonData.get<std::string>("ExchangeID", "").c_str());
		}
		nResult = m_pTradeApi->getApi()->ReqQryProduct(&req, nRequestID);
	}
	catch (...)
	{

	}
	return HandleRequestResult(nResult, "ReqQryProduct", nRequestID);
}

///请求查询合约
int CTPApi::ReqQryInstrument(const boost::property_tree::ptree & jsonObject, int nRequestID)
{
	int nResult = -1;
	try
	{
		CThostFtdcQryInstrumentField req = { 0 };
		auto optData = jsonObject.get_child_optional("Data");
		if (optData) {
			const boost::property_tree::ptree & jsonData = optData.get();

			///合约代码
			snprintf(req.InstrumentID, _countof(req.InstrumentID), "%s", jsonData.get<std::string>("InstrumentID","").c_str());
			///交易所代码
			snprintf(req.ExchangeID, _countof(req.ExchangeID), "%s", jsonData.get<std::string>("ExchangeID","").c_str());
			///合约在交易所的代码
			snprintf(req.ExchangeInstID, _countof(req.ExchangeInstID), "%s", jsonData.get<std::string>("ExchangeInstID","").c_str());
			///产品代码
			snprintf(req.ProductID, _countof(req.ProductID), "%s", jsonData.get<std::string>("ProductID","").c_str());
		}
		nResult = m_pTradeApi->getApi()->ReqQryInstrument(&req, nRequestID);
	}
	catch (...)
	{

	}
	return HandleRequestResult(nResult, "ReqQryInstrument", nRequestID);
}

///请求查询行情
int CTPApi::ReqQryDepthMarketData(const boost::property_tree::ptree & jsonObject, int nRequestID)
{
	int nResult = -1;
	try
	{
		CThostFtdcQryDepthMarketDataField req = { 0 };
		auto optData = jsonObject.get_child_optional("Data");
		if (optData) {
			const boost::property_tree::ptree & jsonData = optData.get();
			///合约代码
			snprintf(req.InstrumentID, _countof(req.InstrumentID), "%s", jsonData.get<std::string>("InstrumentID", "").c_str());
			///交易所代码
			snprintf(req.ExchangeID, _countof(req.ExchangeID), "%s", jsonData.get<std::string>("ExchangeID", "").c_str());
		}
		nResult = m_pTradeApi->getApi()->ReqQryDepthMarketData(&req, nRequestID);
	}
	catch (...)
	{

	}
	return HandleRequestResult(nResult, "ReqQryDepthMarketData", nRequestID);
}

///请求查询投资者结算结果
int CTPApi::ReqQrySettlementInfo(const boost::property_tree::ptree & jsonObject, int nRequestID)
{
	int nResult = -1;
	try
	{
		CThostFtdcQrySettlementInfoField req = { 0 };
		auto optData = jsonObject.get_child_optional("Data");
		if (optData) {
			const boost::property_tree::ptree & jsonData = optData.get();
			BuildInvestorRequest(req, jsonData);
			///交易日
			snprintf(req.TradingDay, _countof(req.TradingDay), "%s", jsonData.get<std::string>("TradingDay", "").c_str());
		}
		nResult = m_pTradeApi->getApi()->ReqQrySettlementInfo(&req, nRequestID);
	}
	catch (...)
	{

	}
	return HandleRequestResult(nResult, "ReqQrySettlementInfo", nRequestID);
}

///请求查询转帐银行
int CTPApi::ReqQryTransferBank(const boost::property_tree::ptree & jsonObject, int nRequestID)
{
	int nResult = -1;
	try
	{
		CThostFtdcQryTransferBankField req = { 0 };
		auto optData = jsonObject.get_child_optional("Data");
		if (optData) {
			const boost::property_tree::ptree & jsonData = optData.get();
			///银行代码
			snprintf(req.BankID, _countof(req.BankID), "%s", jsonData.get<std::string>("BankID", "").c_str());
			///银行分中心代码
			snprintf(req.BankBrchID, _countof(req.BankBrchID), "%s", jsonData.get<std::string>("BankBrchID", "").c_str());
		}
		nResult = m_pTradeApi->getApi()->ReqQryTransferBank(&req, nRequestID);
	}
	catch (...)
	{

	}
	return HandleRequestResult(nResult, "ReqQryTransferBank", nRequestID);
}

///请求查询投资者持仓明细
int CTPApi::ReqQryInvestorPositionDetail(const boost::property_tree::ptree & jsonObject, int nRequestID)
{
	int nResult = -1;
	try
	{

		CThostFtdcQryInvestorPositionDetailField req = { 0 };
		auto optData = jsonObject.get_child_optional("Data");
		if (optData) {
			const boost::property_tree::ptree & jsonData = optData.get();
			BuildInvestorRequest(req, jsonData);
			///合约代码
			snprintf(req.InstrumentID, _countof(req.InstrumentID), "%s", jsonData.get<std::string>("InstrumentID", "").c_str());
			///交易所代码
			snprintf(req.ExchangeID, _countof(req.ExchangeID), "%s", jsonData.get<std::string>("ExchangeID", "").c_str());
		}
		nResult = m_pTradeApi->getApi()->ReqQryInvestorPositionDetail(&req, nRequestID);
	}
	catch (...)
	{

	}
	return HandleRequestResult(nResult, "ReqQryInvestorPositionDetail", nRequestID);
}

///请求查询客户通知
int CTPApi::ReqQryNotice(const boost::property_tree::ptree & jsonObject, int nRequestID)
{
	int nResult = -1;
	try
	{
		CThostFtdcQryNoticeField req = { 0 };
		auto optData = jsonObject.get_child_optional("Data");
		if (optData) {
			const boost::property_tree::ptree & jsonData = optData.get();
			///经纪公司代码
			snprintf(req.BrokerID, _countof(req.BrokerID), "%s", jsonData.get<std::string>("BrokerID", "").c_str());
		}
		nResult = m_pTradeApi->getApi()->ReqQryNotice(&req, nRequestID);
	}
	catch (...)
	{

	}
	return HandleRequestResult(nResult, "ReqQryNotice", nRequestID);
}

///请求查询结算信息确认
int CTPApi::ReqQrySettlementInfoConfirm(const boost::property_tree::ptree & jsonObject, int nRequestID)
{
	int nResult = -1;
	try
	{
		CThostFtdcQrySettlementInfoConfirmField req = { 0 };
		auto optData = jsonObject.get_child_optional("Data");
		if (optData) {
			const boost::property_tree::ptree & jsonData = optData.get();
			BuildInvestorRequest(req, jsonData);
		}
		nResult = m_pTradeApi->getApi()->ReqQrySettlementInfoConfirm(&req, nRequestID);
	}
	catch (...)
	{

	}
	return HandleRequestResult(nResult, "ReqQrySettlementInfoConfirm", nRequestID);
}

///请求查询投资者持仓明细
int CTPApi::ReqQryInvestorPositionCombineDetail(const boost::property_tree::ptree & jsonObject, int nRequestID)
{
	int nResult = -1;
	try
	{
		CThostFtdcQryInvestorPositionCombineDetailField req = { 0 };
		auto optData = jsonObject.get_child_optional("Data");
		if (optData) {
			const boost::property_tree::ptree & jsonData = optData.get();
			BuildInvestorRequest(req, jsonData);
			///组合持仓合约编码
			snprintf(req.CombInstrumentID, _countof(req.CombInstrumentID), "%s", jsonData.get<std::string>("CombInstrumentID", "").c_str());
		}
		nResult = m_pTradeApi->getApi()->ReqQryInvestorPositionCombineDetail(&req, nRequestID);
	}
	catch (...)
	{

	}
	return HandleRequestResult(nResult, "ReqQryInvestorPositionCombineDetail", nRequestID);
}

///请求查询保证金监管系统经纪公司资金账户密钥
int CTPApi::ReqQryCFMMCTradingAccountKey(const boost::property_tree::ptree & jsonObject, int nRequestID)
{
	int nResult = -1;
	try
	{
		CThostFtdcQryCFMMCTradingAccountKeyField req = { 0 };
		auto optData = jsonObject.get_child_optional("Data");
		if (optData) {
			const boost::property_tree::ptree & jsonData = optData.get();
			BuildInvestorRequest(req, jsonData);
		}
		nResult = m_pTradeApi->getApi()->ReqQryCFMMCTradingAccountKey(&req, nRequestID);
	}
	catch (...)
	{

	}
	return HandleRequestResult(nResult, "ReqQryCFMMCTradingAccountKey", nRequestID);
}

///请求查询仓单折抵信息
int CTPApi::ReqQryEWarrantOffset(const boost::property_tree::ptree & jsonObject, int nRequestID)
{
	int nResult = -1;
	try
	{
		CThostFtdcQryEWarrantOffsetField req = { 0 };
		auto optData = jsonObject.get_child_optional("Data");
		if (optData) {
			const boost::property_tree::ptree & jsonData = optData.get();
			BuildInvestorRequest(req, jsonData);
			///交易所代码
			snprintf(req.ExchangeID, _countof(req.ExchangeID), "%s", jsonData.get<std::string>("ExchangeID", "").c_str());
			///合约代码
			snprintf(req.InstrumentID, _countof(req.InstrumentID), "%s", jsonData.get<std::string>("InstrumentID", "").c_str());
		}
		nResult = m_pTradeApi->getApi()->ReqQryEWarrantOffset(&req, nRequestID);
	}
	catch (...)
	{

	}
	return HandleRequestResult(nResult, "ReqQryEWarrantOffset", nRequestID);
}

///请求查询投资者品种/跨品种保证金
int CTPApi::ReqQryInvestorProductGroupMargin(const boost::property_tree::ptree & jsonObject, int nRequestID)
{
	int nResult = -1;
	try
	{
		CThostFtdcQryInvestorProductGroupMarginField req = { 0 };
		auto optData = jsonObject.get_child_optional("Data");
		if (optData) {
			const boost::property_tree::ptree & jsonData = optData.get();
			BuildInvestorRequest(req, jsonData);
			///品种/跨品种标示
			snprintf(req.ProductGroupID, _countof(req.ProductGroupID), "%s", jsonData.get<std::string>("ProductGroupID", "").c_str());
			///投机套保标志
			req.HedgeFlag = jsonData.get<TThostFtdcHedgeFlagType>("HedgeFlag", (char)0);
		}
		nResult = m_pTradeApi->getApi()->ReqQryInvestorProductGroupMargin(&req, nRequestID);
	}
	catch (...)
	{

	}
	return HandleRequestResult(nResult, "ReqQryInvestorProductGroupMargin", nRequestID);
}

///请求查询交易所保证金率
int CTPApi::ReqQryExchangeMarginRate(const boost::property_tree::ptree & jsonObject, int nRequestID)
{
	int nResult = -1;
	try
	{
		CThostFtdcQryExchangeMarginRateField req = { 0 };

		auto optData = jsonObject.get_child_optional("Data");
		if (optData) {
			const boost::property_tree::ptree & jsonData = optData.get();
			///经纪公司代码
			snprintf(req.BrokerID, _countof(req.BrokerID), "%s", jsonData.get<std::string>("BrokerID", "").c_str());
			///合约代码
			snprintf(req.InstrumentID, _countof(req.InstrumentID), "%s", jsonData.get<std::string>("InstrumentID", "").c_str());
			///投机套保标志
			req.HedgeFlag = jsonData.get<TThostFtdcHedgeFlagType>("HedgeFlag", (char)0);
		}
		nResult = m_pTradeApi->getApi()->ReqQryExchangeMarginRate(&req, nRequestID);
	}
	catch (...)
	{

	}
	return HandleRequestResult(nResult, "ReqQryExchangeMarginRate", nRequestID);
}

///请求查询交易所调整保证金率
int CTPApi::ReqQryExchangeMarginRateAdjust(const boost::property_tree::ptree & jsonObject, int nRequestID)
{
	int nResult = -1;
	try
	{
		CThostFtdcQryExchangeMarginRateAdjustField req = { 0 };

		auto optData = jsonObject.get_child_optional("Data");
		if (optData) {
			const boost::property_tree::ptree & jsonData = optData.get();
			///经纪公司代码
			snprintf(req.BrokerID, _countof(req.BrokerID), "%s", jsonData.get<std::string>("BrokerID", "").c_str());
			///合约代码
			snprintf(req.InstrumentID, _countof(req.InstrumentID), "%s", jsonData.get<std::string>("InstrumentID", "").c_str());
			///投机套保标志
			req.HedgeFlag = jsonData.get<TThostFtdcHedgeFlagType>("HedgeFlag", (char)0);
		}
		nResult = m_pTradeApi->getApi()->ReqQryExchangeMarginRateAdjust(&req, nRequestID);
	}
	catch (...)
	{

	}
	return HandleRequestResult(nResult, "ReqQryExchangeMarginRateAdjust", nRequestID);
}

///请求查询汇率
int CTPApi::ReqQryExchangeRate(const boost::property_tree::ptree & jsonObject, int nRequestID)
{
	int nResult = -1;
	try
	{
		CThostFtdcQryExchangeRateField req = { 0 };

		auto optData = jsonObject.get_child_optional("Data");
		if (optData) {
			const boost::property_tree::ptree & jsonData = optData.get();
			///经纪公司代码
			snprintf(req.BrokerID, _countof(req.BrokerID), "%s", jsonData.get<std::string>("BrokerID", "").c_str());
			///源币种
			snprintf(req.FromCurrencyID, _countof(req.FromCurrencyID), "%s", jsonData.get<std::string>("FromCurrencyID", "").c_str());
			///目标币种
			snprintf(req.ToCurrencyID, _countof(req.ToCurrencyID), "%s", jsonData.get<std::string>("ToCurrencyID", "").c_str());
		}
		nResult = m_pTradeApi->getApi()->ReqQryExchangeRate(&req, nRequestID);
	}
	catch (...)
	{

	}
	return HandleRequestResult(nResult, "ReqQryExchangeRate", nRequestID);
}

///请求查询二级代理操作员银期权限
int CTPApi::ReqQrySecAgentACIDMap(const boost::property_tree::ptree & jsonObject, int nRequestID)
{
	int nResult = -1;
	try
	{
		CThostFtdcQrySecAgentACIDMapField req = { 0 };
		auto optData = jsonObject.get_child_optional("Data");
		if (optData) {
			const boost::property_tree::ptree & jsonData = optData.get();
			///经纪公司代码
			snprintf(req.BrokerID, _countof(req.BrokerID), "%s", jsonData.get<std::string>("BrokerID", "").c_str());
			///用户代码
			snprintf(req.UserID, _countof(req.UserID), "%s", jsonData.get<std::string>("UserID", "").c_str());
			///资金账户
			snprintf(req.AccountID, _countof(req.AccountID), "%s", jsonData.get<std::string>("AccountID", "").c_str());
			///币种
			snprintf(req.CurrencyID, _countof(req.CurrencyID), "%s", jsonData.get<std::string>("CurrencyID", "").c_str());
		}
		nResult = m_pTradeApi->getApi()->ReqQrySecAgentACIDMap(&req, nRequestID);
	}
	catch (...)
	{

	}
	return HandleRequestResult(nResult, "ReqQrySecAgentACIDMap", nRequestID);
}

///请求查询产品报价汇率
int CTPApi::ReqQryProductExchRate(const boost::property_tree::ptree & jsonObject, int nRequestID)
{
	int nResult = -1;
	try
	{
		CThostFtdcQryProductExchRateField req = { 0 };
		auto optData = jsonObject.get_child_optional("Data");
		if (optData) {
			const boost::property_tree::ptree & jsonData = optData.get();
			///产品代码
			snprintf(req.ProductID, _countof(req.ProductID), "%s", jsonData.get<std::string>("ProductID", "").c_str());
		}
		nResult = m_pTradeApi->getApi()->ReqQryProductExchRate(&req, nRequestID);
	}
	catch (...)
	{

	}
	return HandleRequestResult(nResult, "ReqQryProductExchRate", nRequestID);
}

///请求查询产品组
int CTPApi::ReqQryProductGroup(const boost::property_tree::ptree & jsonObject, int nRequestID)
{
	int nResult = -1;
	try
	{
		CThostFtdcQryProductGroupField req = { 0 };
		auto optData = jsonObject.get_child_optional("Data");
		if (optData) {
			const boost::property_tree::ptree & jsonData = optData.get();
			///产品代码
			snprintf(req.ProductID, _countof(req.ProductID), "%s", jsonData.get<std::string>("ProductID", "").c_str());
			///交易所代码
			snprintf(req.ExchangeID, _countof(req.ExchangeID), "%s", jsonData.get<std::string>("ExchangeID", "").c_str());
		}
		nResult = m_pTradeApi->getApi()->ReqQryProductGroup(&req, nRequestID);
	}
	catch (...)
	{

	}
	return HandleRequestResult(nResult, "ReqQryProductGroup", nRequestID);
}

///请求查询做市商合约手续费率
int CTPApi::ReqQryMMInstrumentCommissionRate(const boost::property_tree::ptree & jsonObject, int nRequestID)
{
	int nResult = -1;
	try
	{
		CThostFtdcQryMMInstrumentCommissionRateField req = { 0 };
		auto optData = jsonObject.get_child_optional("Data");
		if (optData) {
			const boost::property_tree::ptree & jsonData = optData.get();
		}
		nResult = m_pTradeApi->getApi()->ReqQryMMInstrumentCommissionRate(&req, nRequestID);
	}
	catch (...)
	{

	}
	return HandleRequestResult(nResult, "ReqQryMMInstrumentCommissionRate", nRequestID);
}

///请求查询做市商期权合约手续费
int CTPApi::ReqQryMMOptionInstrCommRate(const boost::property_tree::ptree & jsonObject, int nRequestID)
{
	int nResult = -1;
	try
	{
		CThostFtdcQryMMOptionInstrCommRateField req = { 0 };
		auto optData = jsonObject.get_child_optional("Data");
		if (optData) {
			const boost::property_tree::ptree & jsonData = optData.get();
		}
		nResult = m_pTradeApi->getApi()->ReqQryMMOptionInstrCommRate(&req, nRequestID);
	}
	catch (...)
	{

	}
	return HandleRequestResult(nResult, "ReqQryMMOptionInstrCommRate", nRequestID);
}

///请求查询报单手续费
int CTPApi::ReqQryInstrumentOrderCommRate(const boost::property_tree::ptree & jsonObject, int nRequestID)
{
	int nResult = -1;
	try
	{
		CThostFtdcQryInstrumentOrderCommRateField req = { 0 };
		auto optData = jsonObject.get_child_optional("Data");
		if (optData) {
			const boost::property_tree::ptree & jsonData = optData.get();
			BuildInvestorRequest(req, jsonData);
			///合约代码
			snprintf(req.InstrumentID, _countof(req.InstrumentID), "%s", jsonData.get<std::string>("InstrumentID", "").c_str());
			///交易所代码
			snprintf(req.ExchangeID, _countof(req.ExchangeID), "%s", jsonData.get<std::string>("ExchangeID", "").c_str());
		}
		nResult = m_pTradeApi->getApi()->ReqQryInstrumentOrderCommRate(&req, nRequestID);
	}
	catch (...)
	{

	}
	return HandleRequestResult(nResult, "ReqQryInstrumentOrderCommRate", nRequestID);
}

///请求查询期权交易成本
int CTPApi::ReqQryOptionInstrTradeCost(const boost::property_tree::ptree & jsonObject, int nRequestID)
{
	int nResult = -1;
	try
	{
		CThostFtdcQryOptionInstrTradeCostField req = { 0 };
		auto optData = jsonObject.get_child_optional("Data");
		if (optData) {
			const boost::property_tree::ptree & jsonData = optData.get();
		}
		nResult = m_pTradeApi->getApi()->ReqQryOptionInstrTradeCost(&req, nRequestID);
	}
	catch (...)
	{

	}
	return HandleRequestResult(nResult, "ReqQryOptionInstrTradeCost", nRequestID);
}

///请求查询期权合约手续费
int CTPApi::ReqQryOptionInstrCommRate(const boost::property_tree::ptree & jsonObject, int nRequestID)
{
	int nResult = -1;
	try
	{
		CThostFtdcQryOptionInstrCommRateField req = { 0 };
		auto optData = jsonObject.get_child_optional("Data");
		if (optData) {
			const boost::property_tree::ptree & jsonData = optData.get();
		}
		nResult = m_pTradeApi->getApi()->ReqQryOptionInstrCommRate(&req, nRequestID);
	}
	catch (...)
	{

	}
	return HandleRequestResult(nResult, "ReqQryOptionInstrCommRate", nRequestID);
}

///请求查询执行宣告
int CTPApi::ReqQryExecOrder(const boost::property_tree::ptree & jsonObject, int nRequestID)
{
	int nResult = -1;
	try
	{
		CThostFtdcQryExecOrderField req = { 0 };
		auto optData = jsonObject.get_child_optional("Data");
		if (optData) {
			const boost::property_tree::ptree & jsonData = optData.get();
		}
		nResult = m_pTradeApi->getApi()->ReqQryExecOrder(&req, nRequestID);
	}
	catch (...)
	{

	}
	return HandleRequestResult(nResult, "ReqQryExecOrder", nRequestID);
}

///请求查询询价
int CTPApi::ReqQryForQuote(const boost::property_tree::ptree & jsonObject, int nRequestID)
{
	int nResult = -1;
	try
	{
		CThostFtdcQryForQuoteField req = { 0 };
		auto optData = jsonObject.get_child_optional("Data");
		if (optData) {
			const boost::property_tree::ptree & jsonData = optData.get();
		}
		nResult = m_pTradeApi->getApi()->ReqQryForQuote(&req, nRequestID);
	}
	catch (...)
	{

	}
	return HandleRequestResult(nResult, "ReqQryForQuote", nRequestID);
}

///请求查询报价
int CTPApi::ReqQryQuote(const boost::property_tree::ptree & jsonObject, int nRequestID)
{
	int nResult = -1;
	try
	{
		CThostFtdcQryQuoteField req = { 0 };
		auto optData = jsonObject.get_child_optional("Data");
		if (optData) {
			const boost::property_tree::ptree & jsonData = optData.get();
		}
		nResult = m_pTradeApi->getApi()->ReqQryQuote(&req, nRequestID);
	}
	catch (...)
	{

	}
	return HandleRequestResult(nResult, "ReqQryQuote", nRequestID);
}

///请求查询锁定
int CTPApi::ReqQryLock(const boost::property_tree::ptree & jsonObject, int nRequestID)
{
	int nResult = -1;
	try
	{
		CThostFtdcQryLockField req = { 0 };
		auto optData = jsonObject.get_child_optional("Data");
		if (optData) {
			const boost::property_tree::ptree & jsonData = optData.get();
		}
		nResult = m_pTradeApi->getApi()->ReqQryLock(&req, nRequestID);
	}
	catch (...)
	{

	}
	return HandleRequestResult(nResult, "ReqQryLock", nRequestID);
}

///请求查询锁定证券仓位
int CTPApi::ReqQryLockPosition(const boost::property_tree::ptree & jsonObject, int nRequestID)
{
	int nResult = -1;
	try
	{
		CThostFtdcQryLockPositionField req = { 0 };
		auto optData = jsonObject.get_child_optional("Data");
		if (optData) {
			const boost::property_tree::ptree & jsonData = optData.get();
		}
		nResult = m_pTradeApi->getApi()->ReqQryLockPosition(&req, nRequestID);
	}
	catch (...)
	{

	}
	return HandleRequestResult(nResult, "ReqQryLockPosition", nRequestID);
}

///请求查询ETF期权合约手续费
int CTPApi::ReqQryETFOptionInstrCommRate(const boost::property_tree::ptree & jsonObject, int nRequestID)
{
	int nResult = -1;
	try
	{
		CThostFtdcQryETFOptionInstrCommRateField req = { 0 };
		auto optData = jsonObject.get_child_optional("Data");
		if (optData) {
			const boost::property_tree::ptree & jsonData = optData.get();
		}
		nResult = m_pTradeApi->getApi()->ReqQryETFOptionInstrCommRate(&req, nRequestID);
	}
	catch (...)
	{

	}
	return HandleRequestResult(nResult, "ReqQryETFOptionInstrCommRate", nRequestID);
}

///请求查询投资者分级
int CTPApi::ReqQryInvestorLevel(const boost::property_tree::ptree & jsonObject, int nRequestID)
{
	int nResult = -1;
	try
	{
		CThostFtdcQryInvestorLevelField req = { 0 };
		auto optData = jsonObject.get_child_optional("Data");
		if (optData) {
			const boost::property_tree::ptree & jsonData = optData.get();
			BuildInvestorRequest(req, jsonData);
			///交易所代码
			snprintf(req.ExchangeID, _countof(req.ExchangeID), "%s", jsonData.get<std::string>("ExchangeID", "").c_str());
		}
		nResult = m_pTradeApi->getApi()->ReqQryInvestorLevel(&req, nRequestID);
	}
	catch (...)
	{

	}
	return HandleRequestResult(nResult, "ReqQryInvestorLevel", nRequestID);
}

///请求查询E+1日行权冻结
int CTPApi::ReqQryExecFreeze(const boost::property_tree::ptree & jsonObject, int nRequestID)
{
	int nResult = -1;
	try
	{
		CThostFtdcQryExecFreezeField req = { 0 };
		auto optData = jsonObject.get_child_optional("Data");
		if (optData) {
			const boost::property_tree::ptree & jsonData = optData.get();
		}
		nResult = m_pTradeApi->getApi()->ReqQryExecFreeze(&req, nRequestID);
	}
	catch (...)
	{

	}
	return HandleRequestResult(nResult, "ReqQryExecFreeze", nRequestID);
}

///请求查询组合合约安全系数
int CTPApi::ReqQryCombInstrumentGuard(const boost::property_tree::ptree & jsonObject, int nRequestID)
{
	int nResult = -1;
	try
	{
		CThostFtdcQryCombInstrumentGuardField req = { 0 };
		auto optData = jsonObject.get_child_optional("Data");
		if (optData) {
			const boost::property_tree::ptree & jsonData = optData.get();
			///经纪公司代码
			snprintf(req.BrokerID, _countof(req.BrokerID), "%s", jsonData.get<std::string>("BrokerID", "").c_str());
			///合约代码
			snprintf(req.InstrumentID, _countof(req.InstrumentID), "%s", jsonData.get<std::string>("InstrumentID", "").c_str());
		}
		nResult = m_pTradeApi->getApi()->ReqQryCombInstrumentGuard(&req, nRequestID);
	}
	catch (...)
	{

	}
	return HandleRequestResult(nResult, "ReqQryCombInstrumentGuard", nRequestID);
}

///请求查询申请组合
int CTPApi::ReqQryCombAction(const boost::property_tree::ptree & jsonObject, int nRequestID)
{
	int nResult = -1;
	try
	{
		CThostFtdcQryCombActionField req = { 0 };
		auto optData = jsonObject.get_child_optional("Data");
		if (optData) {
			const boost::property_tree::ptree & jsonData = optData.get();
		}
		nResult = m_pTradeApi->getApi()->ReqQryCombAction(&req, nRequestID);
	}
	catch (...)
	{

	}
	return HandleRequestResult(nResult, "ReqQryCombAction", nRequestID);
}

///请求查询转帐流水
int CTPApi::ReqQryTransferSerial(const boost::property_tree::ptree & jsonObject, int nRequestID)
{
	int nResult = -1;
	try
	{
		CThostFtdcQryTransferSerialField req = { 0 };
		auto optData = jsonObject.get_child_optional("Data");
		if (optData) {
			const boost::property_tree::ptree & jsonData = optData.get();
			BuildAccountRequest(req, jsonData);
			///银行编码
			snprintf(req.BankID, _countof(req.BankID), "%s", jsonData.get<std::string>("BankID", "").c_str());
			///币种代码
			snprintf(req.CurrencyID, _countof(req.CurrencyID), "%s", jsonData.get<std::string>("CurrencyID", "").c_str());
		}
		nResult = m_pTradeApi->getApi()->ReqQryTransferSerial(&req, nRequestID);
	}
	catch (...)
	{

	}
	return HandleRequestResult(nResult, "ReqQryTransferSerial", nRequestID);
}

///请求查询银期签约关系
int CTPApi::ReqQryAccountregister(const boost::property_tree::ptree & jsonObject, int nRequestID)
{
	int nResult = -1;
	try
	{
		CThostFtdcQryAccountregisterField req = { 0 };
		auto optData = jsonObject.get_child_optional("Data");
		if (optData) {
			const boost::property_tree::ptree & jsonData = optData.get();
			BuildAccountRequest(req, jsonData);
			///银行编码
			snprintf(req.BankID, _countof(req.BankID), "%s", jsonData.get<std::string>("BankID", "").c_str());
			///银行分支机构编码
			snprintf(req.BankBranchID, _countof(req.BankBranchID), "%s", jsonData.get<std::string>("BankBranchID", "").c_str());
			///币种代码
			snprintf(req.CurrencyID, _countof(req.CurrencyID), "%s", jsonData.get<std::string>("CurrencyID", "").c_str());
		}
		nResult = m_pTradeApi->getApi()->ReqQryAccountregister(&req, nRequestID);
	}
	catch (...)
	{

	}
	return HandleRequestResult(nResult, "ReqQryAccountregister", nRequestID);
}

///请求查询签约银行
int CTPApi::ReqQryContractBank(const boost::property_tree::ptree & jsonObject, int nRequestID)
{
	int nResult = -1;
	try
	{
		CThostFtdcQryContractBankField req = { 0 };
		auto optData = jsonObject.get_child_optional("Data");
		if (optData) {
			const boost::property_tree::ptree & jsonData = optData.get();
			///经纪公司代码
			snprintf(req.BrokerID, _countof(req.BrokerID), "%s", jsonData.get<std::string>("BrokerID", "").c_str());
			///银行编码
			snprintf(req.BankID, _countof(req.BankID), "%s", jsonData.get<std::string>("BankID", "").c_str());
			///银行分支机构编码
			snprintf(req.BankBrchID, _countof(req.BankBrchID), "%s", jsonData.get<std::string>("BankBrchID", "").c_str());
		}
		nResult = m_pTradeApi->getApi()->ReqQryContractBank(&req, nRequestID);
	}
	catch (...)
	{

	}
	return HandleRequestResult(nResult, "ReqQryContractBank", nRequestID);
}

///请求查询预埋单
int CTPApi::ReqQryParkedOrder(const boost::property_tree::ptree & jsonObject, int nRequestID)
{
	int nResult = -1;
	try
	{
		CThostFtdcQryParkedOrderField req = { 0 };
		auto optData = jsonObject.get_child_optional("Data");
		if (optData) {
			const boost::property_tree::ptree & jsonData = optData.get();
			BuildInvestorRequest(req, jsonData);
			///合约代码
			snprintf(req.InstrumentID, _countof(req.InstrumentID), "%s", jsonData.get<std::string>("InstrumentID", "").c_str());
			///交易所代码
			snprintf(req.ExchangeID, _countof(req.ExchangeID), "%s", jsonData.get<std::string>("ExchangeID", "").c_str());
		}
		nResult = m_pTradeApi->getApi()->ReqQryParkedOrder(&req, nRequestID);
	}
	catch (...)
	{

	}
	return HandleRequestResult(nResult, "ReqQryParkedOrder", nRequestID);
}

///请求查询预埋撤单
int CTPApi::ReqQryParkedOrderAction(const boost::property_tree::ptree & jsonObject, int nRequestID)
{
	int nResult = -1;
	try
	{
		CThostFtdcQryParkedOrderActionField req = { 0 };
		auto optData = jsonObject.get_child_optional("Data");
		if (optData) {
			const boost::property_tree::ptree & jsonData = optData.get();
			BuildInvestorRequest(req, jsonData);
			///合约代码
			snprintf(req.InstrumentID, _countof(req.InstrumentID), "%s", jsonData.get<std::string>("InstrumentID", "").c_str());
			///交易所代码
			snprintf(req.ExchangeID, _countof(req.ExchangeID), "%s", jsonData.get<std::string>("ExchangeID", "").c_str());
		}
		nResult = m_pTradeApi->getApi()->ReqQryParkedOrderAction(&req, nRequestID);
	}
	catch (...)
	{

	}
	return HandleRequestResult(nResult, "ReqQryParkedOrderAction", nRequestID);
}

///请求查询交易通知
int CTPApi::ReqQryTradingNotice(const boost::property_tree::ptree & jsonObject, int nRequestID)
{
	int nResult = -1;
	try
	{
		CThostFtdcQryTradingNoticeField req = { 0 };
		auto optData = jsonObject.get_child_optional("Data");
		if (optData) {
			const boost::property_tree::ptree & jsonData = optData.get();
			BuildInvestorRequest(req, jsonData);
		}
		nResult = m_pTradeApi->getApi()->ReqQryTradingNotice(&req, nRequestID);
	}
	catch (...)
	{

	}
	return HandleRequestResult(nResult, "ReqQryTradingNotice", nRequestID);
}

///请求查询经纪公司交易参数
int CTPApi::ReqQryBrokerTradingParams(const boost::property_tree::ptree & jsonObject, int nRequestID)
{
	int nResult = -1;
	try
	{
		CThostFtdcQryBrokerTradingParamsField req = { 0 };
		auto optData = jsonObject.get_child_optional("Data");
		if (optData) {
			const boost::property_tree::ptree & jsonData = optData.get();
			BuildInvestorRequest(req, jsonData);
			///币种代码
			snprintf(req.CurrencyID, _countof(req.CurrencyID), "%s", jsonData.get<std::string>("CurrencyID", "").c_str());
		}
		nResult = m_pTradeApi->getApi()->ReqQryBrokerTradingParams(&req, nRequestID);
	}
	catch (...)
	{

	}
	return HandleRequestResult(nResult, "ReqQryBrokerTradingParams", nRequestID);
}

///请求查询经纪公司交易算法
int CTPApi::ReqQryBrokerTradingAlgos(const boost::property_tree::ptree & jsonObject, int nRequestID)
{
	int nResult = -1;
	try
	{
		CThostFtdcQryBrokerTradingAlgosField req = { 0 };
		auto optData = jsonObject.get_child_optional("Data");
		if (optData) {
			const boost::property_tree::ptree & jsonData = optData.get();
			//BuildInvestorRequest(req,jsonData);
			///经纪公司代码
			snprintf(req.BrokerID, _countof(req.BrokerID), "%s", jsonData.get<std::string>("BrokerID", "").c_str());
			///交易所代码
			snprintf(req.ExchangeID, _countof(req.ExchangeID), "%s", jsonData.get<std::string>("ExchangeID", "").c_str());
			///合约代码
			snprintf(req.InstrumentID, _countof(req.InstrumentID), "%s", jsonData.get<std::string>("InstrumentID", "").c_str());
		}
		nResult = m_pTradeApi->getApi()->ReqQryBrokerTradingAlgos(&req, nRequestID);
	}
	catch (...)
	{

	}
	return HandleRequestResult(nResult, "ReqQryBrokerTradingAlgos", nRequestID);
}

///请求查询监控中心用户令牌
int CTPApi::ReqQueryCFMMCTradingAccountToken(const boost::property_tree::ptree & jsonObject, int nRequestID)
{
	int nResult = -1;
	try
	{
		CThostFtdcQueryCFMMCTradingAccountTokenField req = { 0 };
		auto optData = jsonObject.get_child_optional("Data");
		if (optData) {
			const boost::property_tree::ptree & jsonData = optData.get();
			BuildInvestorRequest(req, jsonData);
		}
		nResult = m_pTradeApi->getApi()->ReqQueryCFMMCTradingAccountToken(&req, nRequestID);
	}
	catch (...)
	{

	}
	return HandleRequestResult(nResult, "ReqQueryCFMMCTradingAccountToken", nRequestID);
}

///期货发起银行资金转期货请求
int CTPApi::ReqFromBankToFutureByFuture(const boost::property_tree::ptree & jsonObject, int nRequestID)
{
	int nResult = -1;
	try
	{
		CThostFtdcReqTransferField req = { 0 };
		auto optData = jsonObject.get_child_optional("Data");
		if (optData) {
			const boost::property_tree::ptree & jsonData = optData.get();
			//BuildTradeRequest(req,jsonData);
			///业务功能码
			snprintf(req.TradeCode, _countof(req.TradeCode), "%s", jsonData.get<std::string>("TradeCode", "").c_str());
			///银行代码
			snprintf(req.BankID, _countof(req.BankID), "%s", jsonData.get<std::string>("BankID", "").c_str());
			///银行分支机构代码
			snprintf(req.BankBranchID, _countof(req.BankBranchID), "%s", jsonData.get<std::string>("BankBranchID", "").c_str());
			///期商代码
			snprintf(req.BrokerID, _countof(req.BrokerID), "%s", jsonData.get<std::string>("BrokerID", "").c_str());
			///期商分支机构代码
			snprintf(req.BrokerBranchID, _countof(req.BrokerBranchID), "%s", jsonData.get<std::string>("BrokerBranchID", "").c_str());
			///交易日期
			snprintf(req.TradeDate, _countof(req.TradeDate), "%s", jsonData.get<std::string>("TradeDate", "").c_str());
			///交易时间
			snprintf(req.TradeTime, _countof(req.TradeTime), "%s", jsonData.get<std::string>("TradeTime", "").c_str());
			///银行流水号
			snprintf(req.BankSerial, _countof(req.BankSerial), "%s", jsonData.get<std::string>("BankSerial", "").c_str());
			///交易系统日期 
			snprintf(req.TradingDay, _countof(req.TradingDay), "%s", jsonData.get<std::string>("TradingDay", "").c_str());
			///银期平台消息流水号
			req.PlateSerial = jsonData.get<TThostFtdcSerialType>("PlateSerial", (char)0);
			///最后分片标志
			req.LastFragment = jsonData.get<TThostFtdcLastFragmentType>("LastFragment", (char)0);
			///会话号
			req.SessionID = jsonData.get<TThostFtdcSessionIDType>("SessionID", (char)0);
			///客户姓名
			snprintf(req.CustomerName, _countof(req.CustomerName), "%s", jsonData.get<std::string>("CustomerName", "").c_str());
			///证件类型
			req.IdCardType = jsonData.get<TThostFtdcIdCardTypeType>("IdCardType", (char)0);
			///证件号码
			snprintf(req.IdentifiedCardNo, _countof(req.IdentifiedCardNo), "%s", jsonData.get<std::string>("IdentifiedCardNo", "").c_str());
			///客户类型
			req.CustType = jsonData.get<TThostFtdcCustTypeType>("CustType", (char)0);
			///银行帐号
			snprintf(req.BankAccount, _countof(req.BankAccount), "%s", jsonData.get<std::string>("BankAccount", "").c_str());
			///银行密码
			snprintf(req.BankPassWord, _countof(req.BankPassWord), "%s", jsonData.get<std::string>("BankPassWord", "").c_str());
			///投资者帐号
			snprintf(req.AccountID, _countof(req.AccountID), "%s", jsonData.get<std::string>("AccountID", "").c_str());
			///期货密码
			snprintf(req.Password, _countof(req.Password), "%s", jsonData.get<std::string>("Password", "").c_str());
			///安装编号
			req.InstallID = jsonData.get<TThostFtdcInstallIDType>("InstallID", (char)0);
			///期货公司流水号
			req.FutureSerial = jsonData.get<TThostFtdcFutureSerialType>("FutureSerial", (char)0);
			///用户标识
			snprintf(req.UserID, _countof(req.UserID), "%s", jsonData.get<std::string>("UserID", "").c_str());
			///验证客户证件号码标志
			req.VerifyCertNoFlag = jsonData.get<TThostFtdcYesNoIndicatorType>("VerifyCertNoFlag", (char)0);
			///币种代码
			snprintf(req.CurrencyID, _countof(req.CurrencyID), "%s", jsonData.get<std::string>("CurrencyID", "").c_str());
			///转帐金额
			req.TradeAmount = jsonData.get<TThostFtdcTradeAmountType>("TradeAmount", (char)0);
			///期货可取金额
			req.FutureFetchAmount = jsonData.get<TThostFtdcTradeAmountType>("FutureFetchAmount", (char)0);
			///费用支付标志
			req.FeePayFlag = jsonData.get<TThostFtdcFeePayFlagType>("FeePayFlag", (char)0);
			///应收客户费用
			req.CustFee = jsonData.get<TThostFtdcCustFeeType>("CustFee", (char)0);
			///应收期货公司费用
			req.BrokerFee = jsonData.get<TThostFtdcFutureFeeType>("CustFee", (char)0);
			///发送方给接收方的消息
			snprintf(req.Message, _countof(req.Message), "%s", jsonData.get<std::string>("Message", "").c_str());
			///摘要
			snprintf(req.Digest, _countof(req.Digest), "%s", jsonData.get<std::string>("Digest", "").c_str());
			///银行帐号类型
			req.BankAccType = jsonData.get<TThostFtdcBankAccTypeType>("BankAccType", (char)0);
			///渠道标志
			snprintf(req.DeviceID, _countof(req.DeviceID), "%s", jsonData.get<std::string>("DeviceID", "").c_str());
			///期货单位帐号类型
			req.BankSecuAccType = jsonData.get<TThostFtdcBankAccTypeType>("BankSecuAccType", (char)0);
			///期货公司银行编码
			snprintf(req.BrokerIDByBank, _countof(req.BrokerIDByBank), "%s", jsonData.get<std::string>("BrokerIDByBank", "").c_str());
			///期货单位帐号
			snprintf(req.BankSecuAcc, _countof(req.BankSecuAcc), "%s", jsonData.get<std::string>("BankSecuAcc", "").c_str());
			///银行密码标志
			req.BankPwdFlag = jsonData.get<TThostFtdcPwdFlagType>("BankPwdFlag", (char)0);
			///期货资金密码核对标志
			req.SecuPwdFlag = jsonData.get<TThostFtdcPwdFlagType>("SecuPwdFlag", (char)0);
			///交易柜员
			snprintf(req.OperNo, _countof(req.OperNo), "%s", jsonData.get<std::string>("OperNo", "").c_str());
			///请求编号
			req.RequestID = jsonData.get<TThostFtdcRequestIDType>("RequestID", (char)0);
			///交易ID
			req.TID = jsonData.get<TThostFtdcTIDType>("TID", (char)0);
			///转账交易状态
			req.TransferStatus = jsonData.get<TThostFtdcTransferStatusType>("TransferStatus", (char)0);
		}
		nResult = m_pTradeApi->getApi()->ReqFromBankToFutureByFuture(&req, nRequestID);
	}
	catch (...)
	{

	}
	return HandleRequestResult(nResult, "ReqFromBankToFutureByFuture", nRequestID);
}

///期货发起期货资金转银行请求
int CTPApi::ReqFromFutureToBankByFuture(const boost::property_tree::ptree & jsonObject, int nRequestID)
{
	int nResult = -1;
	try
	{
		CThostFtdcReqTransferField req = { 0 };
		auto optData = jsonObject.get_child_optional("Data");
		if (optData) {
			const boost::property_tree::ptree & jsonData = optData.get();
			///业务功能码
			snprintf(req.TradeCode, _countof(req.TradeCode), "%s", jsonData.get<std::string>("TradeCode", "").c_str());
			///银行代码
			snprintf(req.BankID, _countof(req.BankID), "%s", jsonData.get<std::string>("BankID", "").c_str());
			///银行分支机构代码
			snprintf(req.BankBranchID, _countof(req.BankBranchID), "%s", jsonData.get<std::string>("BankBranchID", "").c_str());
			///期商代码
			snprintf(req.BrokerID, _countof(req.BrokerID), "%s", jsonData.get<std::string>("BrokerID", "").c_str());
			///期商分支机构代码
			snprintf(req.BrokerBranchID, _countof(req.BrokerBranchID), "%s", jsonData.get<std::string>("BrokerBranchID", "").c_str());
			///交易日期
			snprintf(req.TradeDate, _countof(req.TradeDate), "%s", jsonData.get<std::string>("TradeDate", "").c_str());
			///交易时间
			snprintf(req.TradeTime, _countof(req.TradeTime), "%s", jsonData.get<std::string>("TradeTime", "").c_str());
			///银行流水号
			snprintf(req.BankSerial, _countof(req.BankSerial), "%s", jsonData.get<std::string>("BankSerial", "").c_str());
			///交易系统日期 
			snprintf(req.TradingDay, _countof(req.TradingDay), "%s", jsonData.get<std::string>("TradingDay", "").c_str());
			///银期平台消息流水号
			req.PlateSerial = jsonData.get<TThostFtdcSerialType>("PlateSerial", (char)0);
			///最后分片标志
			req.LastFragment = jsonData.get<TThostFtdcLastFragmentType>("LastFragment", (char)0);
			///会话号
			req.SessionID = jsonData.get<TThostFtdcSessionIDType>("SessionID", (char)0);
			///客户姓名
			snprintf(req.CustomerName, _countof(req.CustomerName), "%s", jsonData.get<std::string>("CustomerName", "").c_str());
			///证件类型
			req.IdCardType = jsonData.get<TThostFtdcIdCardTypeType>("IdCardType", (char)0);
			///证件号码
			snprintf(req.IdentifiedCardNo, _countof(req.IdentifiedCardNo), jsonData.get<std::string>("IdentifiedCardNo", "").c_str());
			///客户类型
			req.CustType = jsonData.get<TThostFtdcCustTypeType>("CustType", (char)0);
			///银行帐号
			snprintf(req.BankAccount, _countof(req.BankAccount), "%s", jsonData.get<std::string>("BankAccount", "").c_str());
			///银行密码
			snprintf(req.BankPassWord, _countof(req.BankPassWord), "%s", jsonData.get<std::string>("BankPassWord", "").c_str());
			///投资者帐号
			snprintf(req.AccountID, _countof(req.AccountID), "%s", jsonData.get<std::string>("AccountID", "").c_str());
			///期货密码
			snprintf(req.Password, _countof(req.Password), "%s", jsonData.get<std::string>("Password", "").c_str());
			///安装编号
			req.InstallID = jsonData.get<TThostFtdcInstallIDType>("InstallID", (char)0);
			///期货公司流水号
			req.FutureSerial = jsonData.get<TThostFtdcFutureSerialType>("FutureSerial", (char)0);
			///用户标识
			snprintf(req.UserID, _countof(req.UserID), "%s", jsonData.get<std::string>("UserID", "").c_str());
			///验证客户证件号码标志
			req.VerifyCertNoFlag = jsonData.get<TThostFtdcYesNoIndicatorType>("VerifyCertNoFlag", (char)0);
			///币种代码
			snprintf(req.CurrencyID, _countof(req.CurrencyID), "%s", jsonData.get<std::string>("CurrencyID", "").c_str());
			///转帐金额
			req.TradeAmount = jsonData.get<TThostFtdcTradeAmountType>("TradeAmount", (char)0);
			///期货可取金额
			req.FutureFetchAmount = jsonData.get<TThostFtdcTradeAmountType>("FutureFetchAmount", (char)0);
			///费用支付标志
			req.FeePayFlag = jsonData.get<TThostFtdcFeePayFlagType>("FeePayFlag", (char)0);
			///应收客户费用
			req.CustFee = jsonData.get<TThostFtdcCustFeeType>("CustFee", (char)0);
			///应收期货公司费用
			req.BrokerFee = jsonData.get<TThostFtdcFutureFeeType>("CustFee", (char)0);
			///发送方给接收方的消息
			snprintf(req.Message, _countof(req.Message), "%s", jsonData.get<std::string>("Message", "").c_str());
			///摘要
			snprintf(req.Digest, _countof(req.Digest), "%s", jsonData.get<std::string>("Digest", "").c_str());
			///银行帐号类型
			req.BankAccType = jsonData.get<TThostFtdcBankAccTypeType>("BankAccType", (char)0);
			///渠道标志
			snprintf(req.DeviceID, _countof(req.DeviceID), "%s", jsonData.get<std::string>("DeviceID", "").c_str());
			///期货单位帐号类型
			req.BankSecuAccType = jsonData.get<TThostFtdcBankAccTypeType>("BankSecuAccType", (char)0);
			///期货公司银行编码
			snprintf(req.BrokerIDByBank, _countof(req.BrokerIDByBank), "%s", jsonData.get<std::string>("BrokerIDByBank", "").c_str());
			///期货单位帐号
			snprintf(req.BankSecuAcc, _countof(req.BankSecuAcc), "%s", jsonData.get<std::string>("BankSecuAcc", "").c_str());
			///银行密码标志
			req.BankPwdFlag = jsonData.get<TThostFtdcPwdFlagType>("BankPwdFlag", (char)0);
			///期货资金密码核对标志
			req.SecuPwdFlag = jsonData.get<TThostFtdcPwdFlagType>("SecuPwdFlag", (char)0);
			///交易柜员
			snprintf(req.OperNo, _countof(req.OperNo), "%s", jsonData.get<std::string>("OperNo", "").c_str());
			///请求编号
			req.RequestID = jsonData.get<TThostFtdcRequestIDType>("RequestID", (char)0);
			///交易ID
			req.TID = jsonData.get<TThostFtdcTIDType>("TID", (char)0);
			///转账交易状态
			req.TransferStatus = jsonData.get<TThostFtdcTransferStatusType>("TransferStatus", (char)0);
		}
		nResult = m_pTradeApi->getApi()->ReqFromFutureToBankByFuture(&req, nRequestID);
	}
	catch (...)
	{

	}
	return HandleRequestResult(nResult, "ReqFromFutureToBankByFuture", nRequestID);
}

///期货发起查询银行余额请求
int CTPApi::ReqQueryBankAccountMoneyByFuture(const boost::property_tree::ptree & jsonObject, int nRequestID)
{
	int nResult = -1;
	try
	{
		CThostFtdcReqQueryAccountField req = { 0 };
		auto optData = jsonObject.get_child_optional("Data");
		if (optData) {
			const boost::property_tree::ptree & jsonData = optData.get();
			///业务功能码
			snprintf(req.TradeCode, _countof(req.TradeCode), "%s", jsonData.get<std::string>("TradeCode", "").c_str());
			///银行代码
			snprintf(req.BankID, _countof(req.BankID), "%s", jsonData.get<std::string>("BankID", "").c_str());
			///银行分支机构代码
			snprintf(req.BankBranchID, _countof(req.BankBranchID), "%s", jsonData.get<std::string>("BankBranchID", "").c_str());
			///期商代码
			snprintf(req.BrokerID, _countof(req.BrokerID), "%s", jsonData.get<std::string>("BrokerID", "").c_str());
			///期商分支机构代码
			snprintf(req.BrokerBranchID, _countof(req.BrokerBranchID), "%s", jsonData.get<std::string>("BrokerBranchID", "").c_str());
			///交易日期
			snprintf(req.TradeDate, _countof(req.TradeDate), "%s", jsonData.get<std::string>("TradeDate", "").c_str());
			///交易时间
			snprintf(req.TradeTime, _countof(req.TradeTime), "%s", jsonData.get<std::string>("TradeTime", "").c_str());
			///银行流水号
			snprintf(req.BankSerial, _countof(req.BankSerial), "%s", jsonData.get<std::string>("BankSerial", "").c_str());
			///交易系统日期 
			snprintf(req.TradingDay, _countof(req.TradingDay), "%s", jsonData.get<std::string>("TradingDay", "").c_str());
			///银期平台消息流水号
			req.PlateSerial = jsonData.get<TThostFtdcSerialType>("PlateSerial", (char)0);
			///最后分片标志
			req.LastFragment = jsonData.get<TThostFtdcLastFragmentType>("LastFragment", (char)0);
			///会话号
			req.SessionID = jsonData.get<TThostFtdcSessionIDType>("SessionID", (char)0);
			///客户姓名
			snprintf(req.CustomerName, _countof(req.CustomerName), "%s", jsonData.get<std::string>("CustomerName", "").c_str());
			///证件类型
			req.IdCardType = jsonData.get<TThostFtdcIdCardTypeType>("IdCardType", (char)0);
			///证件号码
			snprintf(req.IdentifiedCardNo, _countof(req.IdentifiedCardNo), "%s", jsonData.get<std::string>("IdentifiedCardNo", "").c_str());
			///客户类型
			req.CustType = jsonData.get<TThostFtdcCustTypeType>("CustType", (char)0);
			///银行帐号
			snprintf(req.BankAccount, _countof(req.BankAccount), "%s", jsonData.get<std::string>("BankAccount", "").c_str());
			///银行密码
			snprintf(req.BankPassWord, _countof(req.BankPassWord), "%s", jsonData.get<std::string>("BankPassWord", "").c_str());
			///投资者帐号
			snprintf(req.AccountID, _countof(req.AccountID), "%s", jsonData.get<std::string>("AccountID", "").c_str());
			///期货密码
			snprintf(req.Password, _countof(req.Password), "%s", jsonData.get<std::string>("Password", "").c_str());
			///安装编号
			req.InstallID = jsonData.get<TThostFtdcInstallIDType>("InstallID", (char)0);
			///期货公司流水号
			req.FutureSerial = jsonData.get<TThostFtdcFutureSerialType>("FutureSerial", (char)0);
			///用户标识
			snprintf(req.UserID, _countof(req.UserID), "%s", jsonData.get<std::string>("UserID", "").c_str());
			///验证客户证件号码标志
			req.VerifyCertNoFlag = jsonData.get<TThostFtdcYesNoIndicatorType>("VerifyCertNoFlag", (char)0);
			///币种代码
			snprintf(req.CurrencyID, _countof(req.CurrencyID), "%s", jsonData.get<std::string>("CurrencyID", "").c_str());
			///摘要
			snprintf(req.Digest, _countof(req.Digest), jsonData.get<std::string>("Digest", "").c_str());
			///银行帐号类型
			req.BankAccType = jsonData.get<TThostFtdcBankAccTypeType>("BankAccType", (char)0);
			///渠道标志
			snprintf(req.DeviceID, _countof(req.DeviceID), "%s", jsonData.get<std::string>("DeviceID", "").c_str());
			///期货单位帐号类型
			req.BankSecuAccType = jsonData.get<TThostFtdcBankAccTypeType>("BankSecuAccType", (char)0);
			///期货公司银行编码
			snprintf(req.BrokerIDByBank, _countof(req.BrokerIDByBank), "%s", jsonData.get<std::string>("BrokerIDByBank", "").c_str());
			///期货单位帐号
			snprintf(req.BankSecuAcc, _countof(req.BankSecuAcc), "%s", jsonData.get<std::string>("BankSecuAcc", "").c_str());
			///银行密码标志
			req.BankPwdFlag = jsonData.get<TThostFtdcPwdFlagType>("BankPwdFlag", (char)0);
			///期货资金密码核对标志
			req.SecuPwdFlag = jsonData.get<TThostFtdcPwdFlagType>("SecuPwdFlag", (char)0);
			///交易柜员
			snprintf(req.OperNo, _countof(req.OperNo), "%s", jsonData.get<std::string>("OperNo", "").c_str());
			///请求编号
			req.RequestID = jsonData.get<TThostFtdcRequestIDType>("RequestID", (char)0);
			///交易ID
			req.TID = jsonData.get<TThostFtdcTIDType>("TID", (char)0);
		}
		nResult = m_pTradeApi->getApi()->ReqQueryBankAccountMoneyByFuture(&req, nRequestID);
	}
	catch (...)
	{

	}
	return HandleRequestResult(nResult, "ReqQueryBankAccountMoneyByFuture", nRequestID);
}

void CTPApi::OnFrontConnected()
{
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnFrontConnected")) {
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();

}

void CTPApi::OnFrontDisconnected(int nReason)
{
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnFrontDisconnected")) {
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
}

void CTPApi::OnHeartBeatWarning(int nTimeLapse) {

}

///客户端认证响应
void CTPApi::OnRspAuthenticate(CThostFtdcRspAuthenticateField *pRspAuthenticateField, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRspAuthenticate", pRspInfo, nRequestID, bIsLast)) {
			//构造Data
			boost::property_tree::ptree jsonData;
			///经纪公司代码
			jsonData.put("BrokerID", pRspAuthenticateField->BrokerID);
			///用户代码
			jsonData.put("UserID", pRspAuthenticateField->UserID);
			///交易系统名称
			jsonData.put("UserProductInfo", pRspAuthenticateField->UserProductInfo);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
}

///登录请求响应
void CTPApi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRspUserLogin", pRspInfo, nRequestID, bIsLast)) {
			//构造Data
			boost::property_tree::ptree jsonData;
			///交易日
			jsonData.put("TradingDay", pRspUserLogin->TradingDay);
			///登录成功时间
			jsonData.put("LoginTime", pRspUserLogin->LoginTime);
			///经纪公司代码
			jsonData.put("BrokerID", pRspUserLogin->BrokerID);
			///用户代码
			jsonData.put("UserID", pRspUserLogin->UserID);
			///交易系统名称
			jsonData.put("SystemName", pRspUserLogin->SystemName);
			///前置编号
			jsonData.put("FrontID", pRspUserLogin->FrontID);
			///会话编号
			jsonData.put("SessionID", pRspUserLogin->SessionID);
			///最大报单引用
			jsonData.put("MaxOrderRef", pRspUserLogin->MaxOrderRef);
			///上期所时间
			jsonData.put("SHFETime", pRspUserLogin->SHFETime);
			///大商所时间
			jsonData.put("DCETime", pRspUserLogin->DCETime);
			///郑商所时间
			jsonData.put("CZCETime", pRspUserLogin->CZCETime);
			///中金所时间
			jsonData.put("FFEXTime", pRspUserLogin->FFEXTime);
			///能源中心时间
			jsonData.put("INETime", pRspUserLogin->INETime);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
#ifdef _DEBUG
				//Send("{ \"RequestApi\":\"ReqQryInstrument\", \"RequestID\":\"1\", \"Data\":{ } }");
				//Send("{ \"RequestApi\":\"ReqQryDepthMarketData\", \"Data\":{ \"InstrumentID\":\"AP805\"} }");
				//Send("{ \"RequestApi\":\"SubscribeMarketData\", \"Data\":{ \"InstrumentID\":\"IH1812;AP810;CF809;CY805;FG809;JR807;LR809;MA809;OI809;PM805;RI811;RM809;RS809;SF805;SM805;SR809;TA809;WH809;ZC809;a1809\"} }");
				//Send(R"({"RequestApi":"ReqOrderInsert","RequestID":0,"Data":{"BrokerID":"9999","ExchangeID":"CZCE","InvestorID":"115569","InstrumentID":"AP810","OrderRef":"1800","UserID":"115569","OrderPriceType":"3","Direction":"0","CombOffsetFlag":"0","CombHedgeFlag":"1","LimitPrice":9837,"VolumeTotalOriginal":2,"TimeCondition":"1","GTDDate":"","VolumeCondition":"3","MinVolume":0,"ContingentCondition":"1","StopPrice":0,"ForceCloseReason":"0","IsAutoSuspend":0}})");
				Send(R"({"RequestApi":"ReqQrySettlementInfoConfirm", "Data":{"BrokerID":"9999", "InvestorID":"115569"}})");
#endif//
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
}

///登出请求响应
void CTPApi::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRspUserLogout", pRspInfo, nRequestID, bIsLast)) {
			//构造Data
			boost::property_tree::ptree jsonData;
			///经纪公司代码
			jsonData.put("BrokerID", pUserLogout->BrokerID);
			///用户代码
			jsonData.put("UserID", pUserLogout->UserID);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
}

///用户口令更新请求响应
void CTPApi::OnRspUserPasswordUpdate(CThostFtdcUserPasswordUpdateField *pUserPasswordUpdate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRspUserPasswordUpdate", pRspInfo, nRequestID, bIsLast)) {
			//构造Data
			boost::property_tree::ptree jsonData;
			///经纪公司代码
			jsonData.put("BrokerID", pUserPasswordUpdate->BrokerID);
			///用户代码
			jsonData.put("UserID", pUserPasswordUpdate->UserID);
			///经纪公司代码
			jsonData.put("OldPassword", pUserPasswordUpdate->OldPassword);
			///经纪公司代码
			jsonData.put("NewPassword", pUserPasswordUpdate->NewPassword);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
}

///资金账户口令更新请求响应
void CTPApi::OnRspTradingAccountPasswordUpdate(CThostFtdcTradingAccountPasswordUpdateField *pTradingAccountPasswordUpdate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRspTradingAccountPasswordUpdate", pRspInfo, nRequestID, bIsLast)) {
			//构造Data
			boost::property_tree::ptree jsonData;
			///经纪公司代码
			jsonData.put("BrokerID", pTradingAccountPasswordUpdate->BrokerID);
			///用户代码
			jsonData.put("AccountID", pTradingAccountPasswordUpdate->AccountID);
			///经纪公司代码
			jsonData.put("OldPassword", pTradingAccountPasswordUpdate->OldPassword);
			///经纪公司代码
			jsonData.put("NewPassword", pTradingAccountPasswordUpdate->NewPassword);
			///经纪公司代码
			jsonData.put("CurrencyID", pTradingAccountPasswordUpdate->CurrencyID);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
}

///报单录入请求响应
void CTPApi::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRspOrderInsert", pRspInfo, nRequestID, bIsLast)) {
			//构造Data
			boost::property_tree::ptree jsonData;
			BuildTradeAccountReturn(pInputOrder, jsonData);
			BuildInstrumentReturn(pInputOrder, jsonData);
			///经纪公司代码
			jsonData.put("BrokerID", pInputOrder->BrokerID);
			///投资者代码
			jsonData.put("InvestorID", pInputOrder->InvestorID);
			///合约代码
			jsonData.put("InstrumentID", pInputOrder->InstrumentID);
			///报单引用
			jsonData.put("OrderRef", pInputOrder->OrderRef);
			///用户代码
			jsonData.put("UserID", pInputOrder->UserID);
			///报单价格条件
			jsonData.put("OrderPriceType", pInputOrder->OrderPriceType);
			///买卖方向
			jsonData.put("Direction", pInputOrder->Direction);
			///组合开平标志
			jsonData.put("CombOffsetFlag", pInputOrder->CombOffsetFlag);
			///组合投机套保标志
			jsonData.put("CombHedgeFlag", pInputOrder->CombHedgeFlag);
			///价格
			jsonData.put("LimitPrice", pInputOrder->LimitPrice);
			///数量
			jsonData.put("VolumeTotalOriginal", pInputOrder->VolumeTotalOriginal);
			///有效期类型
			jsonData.put("TimeCondition", pInputOrder->TimeCondition);
			///GTD日期
			jsonData.put("GTDDate", pInputOrder->GTDDate);
			///成交量类型
			jsonData.put("VolumeCondition", pInputOrder->VolumeCondition);
			///最小成交量
			jsonData.put("MinVolume", pInputOrder->MinVolume);
			///触发条件
			jsonData.put("ContingentCondition", pInputOrder->ContingentCondition);
			///止损价
			jsonData.put("StopPrice", pInputOrder->StopPrice);
			///强平原因
			jsonData.put("ForceCloseReason", pInputOrder->ForceCloseReason);
			///自动挂起标志
			jsonData.put("IsAutoSuspend", pInputOrder->IsAutoSuspend);
			///业务单元
			jsonData.put("BusinessUnit", pInputOrder->BusinessUnit);
			///请求编号
			jsonData.put("RequestID", pInputOrder->RequestID);
			///用户强评标志
			jsonData.put("UserForceClose", pInputOrder->UserForceClose);
			///互换单标志
			jsonData.put("IsSwapOrder", pInputOrder->IsSwapOrder);
			///交易所代码
			jsonData.put("ExchangeID", pInputOrder->ExchangeID);
			///投资单元代码
			jsonData.put("InvestUnitID", pInputOrder->InvestUnitID);
			///资金账号
			jsonData.put("AccountID", pInputOrder->AccountID);
			///币种代码
			jsonData.put("CurrencyID", pInputOrder->CurrencyID);
			///交易编码
			jsonData.put("ClientID", pInputOrder->ClientID);
			///IP地址
			jsonData.put("IPAddress", pInputOrder->IPAddress);
			///Mac地址
			jsonData.put("MacAddress", pInputOrder->MacAddress);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
}

///预埋单录入请求响应
void CTPApi::OnRspParkedOrderInsert(CThostFtdcParkedOrderField *pParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRspParkedOrderInsert", pRspInfo, nRequestID, bIsLast)) {
			//构造Data
			boost::property_tree::ptree jsonData;
			///经纪公司代码
			jsonData.put("BrokerID", pParkedOrder->BrokerID);
			///投资者代码
			jsonData.put("InvestorID", pParkedOrder->InvestorID);
			///合约代码
			jsonData.put("InstrumentID", pParkedOrder->InstrumentID);
			///报单引用
			jsonData.put("OrderRef", pParkedOrder->OrderRef);
			///用户代码
			jsonData.put("UserID", pParkedOrder->UserID);
			///报单价格条件
			jsonData.put("OrderPriceType", pParkedOrder->OrderPriceType);
			///买卖方向
			jsonData.put("Direction", pParkedOrder->Direction);
			///组合开平标志
			jsonData.put("CombOffsetFlag", pParkedOrder->CombOffsetFlag);
			///组合投机套保标志
			jsonData.put("CombHedgeFlag", pParkedOrder->CombHedgeFlag);
			///价格
			jsonData.put("LimitPrice", pParkedOrder->LimitPrice);
			///数量
			jsonData.put("VolumeTotalOriginal", pParkedOrder->VolumeTotalOriginal);
			///有效期类型
			jsonData.put("TimeCondition", pParkedOrder->TimeCondition);
			///GTD日期
			jsonData.put("GTDDate", pParkedOrder->GTDDate);
			///成交量类型
			jsonData.put("VolumeCondition", pParkedOrder->VolumeCondition);
			///最小成交量
			jsonData.put("MinVolume", pParkedOrder->MinVolume);
			///触发条件
			jsonData.put("ContingentCondition", pParkedOrder->ContingentCondition);
			///止损价
			jsonData.put("StopPrice", pParkedOrder->StopPrice);
			///强平原因
			jsonData.put("ForceCloseReason", pParkedOrder->ForceCloseReason);
			///自动挂起标志
			jsonData.put("IsAutoSuspend", pParkedOrder->IsAutoSuspend);
			///业务单元
			jsonData.put("BusinessUnit", pParkedOrder->BusinessUnit);
			///请求编号
			jsonData.put("RequestID", pParkedOrder->RequestID);
			///用户强评标志
			jsonData.put("UserForceClose", pParkedOrder->UserForceClose);
			///交易所代码
			jsonData.put("ExchangeID", pParkedOrder->ExchangeID);
			///预埋报单编号
			jsonData.put("ParkedOrderID", pParkedOrder->ParkedOrderID);
			///用户类型
			jsonData.put("UserType", pParkedOrder->UserType);
			///预埋单状态
			jsonData.put("Status", pParkedOrder->Status);
			///错误代码
			jsonData.put("ErrorID", pParkedOrder->ErrorID);
			///错误信息
			jsonData.put("ErrorMsg", pParkedOrder->ErrorMsg);
			///互换单标志
			jsonData.put("IsSwapOrder", pParkedOrder->IsSwapOrder);
			///资金账号
			jsonData.put("AccountID", pParkedOrder->AccountID);
			///币种代码
			jsonData.put("CurrencyID", pParkedOrder->CurrencyID);
			///交易编码
			jsonData.put("ClientID", pParkedOrder->ClientID);
			///投资单元代码
			jsonData.put("InvestUnitID", pParkedOrder->InvestUnitID);
			///IP地址
			jsonData.put("IPAddress", pParkedOrder->IPAddress);
			///Mac地址
			jsonData.put("MacAddress", pParkedOrder->MacAddress);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
}

///预埋撤单录入请求响应
void CTPApi::OnRspParkedOrderAction(CThostFtdcParkedOrderActionField *pParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRspParkedOrderAction", pRspInfo, nRequestID, bIsLast)) {
			//构造Data
			boost::property_tree::ptree jsonData;
			///经纪公司代码
			jsonData.put("BrokerID", pParkedOrderAction->BrokerID);
			///投资者代码
			jsonData.put("InvestorID", pParkedOrderAction->InvestorID);
			///报单操作引用
			jsonData.put("OrderActionRef", pParkedOrderAction->OrderActionRef);
			///报单引用
			jsonData.put("OrderRef", pParkedOrderAction->OrderRef);
			///请求编号
			jsonData.put("RequestID", pParkedOrderAction->RequestID);
			///前置编号
			jsonData.put("FrontID", pParkedOrderAction->FrontID);
			///会话编号
			jsonData.put("SessionID", pParkedOrderAction->SessionID);
			///交易所代码
			jsonData.put("ExchangeID", pParkedOrderAction->ExchangeID);
			///报单编号
			jsonData.put("OrderSysID", pParkedOrderAction->OrderSysID);
			///操作标志
			jsonData.put("ActionFlag", pParkedOrderAction->ActionFlag);
			///价格
			jsonData.put("LimitPrice", pParkedOrderAction->LimitPrice);
			///数量变化
			jsonData.put("VolumeChange", pParkedOrderAction->VolumeChange);
			///用户代码
			jsonData.put("UserID", pParkedOrderAction->UserID);
			///合约代码
			jsonData.put("InstrumentID", pParkedOrderAction->InstrumentID);
			///预埋撤单单编号
			jsonData.put("ParkedOrderActionID", pParkedOrderAction->ParkedOrderActionID);
			///用户类型
			jsonData.put("UserType", pParkedOrderAction->UserType);
			///预埋撤单状态
			jsonData.put("Status", pParkedOrderAction->Status);
			///错误代码
			jsonData.put("ErrorID", pParkedOrderAction->ErrorID);
			///错误信息
			jsonData.put("ErrorMsg", pParkedOrderAction->ErrorMsg);
			///投资单元代码
			jsonData.put("InvestUnitID", pParkedOrderAction->InvestUnitID);
			///IP地址
			jsonData.put("IPAddress", pParkedOrderAction->IPAddress);
			///Mac地址
			jsonData.put("MacAddress", pParkedOrderAction->MacAddress);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
}

///报单操作请求响应
void CTPApi::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRspOrderAction", pRspInfo, nRequestID, bIsLast)) {
			//构造Data
			boost::property_tree::ptree jsonData;
			///经纪公司代码
			jsonData.put("BrokerID", pInputOrderAction->BrokerID);
			///投资者代码
			jsonData.put("InvestorID", pInputOrderAction->InvestorID);
			///报单操作引用
			jsonData.put("OrderActionRef", pInputOrderAction->OrderActionRef);
			///报单引用
			jsonData.put("OrderRef", pInputOrderAction->OrderRef);
			///请求编号
			jsonData.put("RequestID", pInputOrderAction->RequestID);
			///前置编号
			jsonData.put("FrontID", pInputOrderAction->FrontID);
			///会话编号
			jsonData.put("SessionID", pInputOrderAction->SessionID);
			///交易所代码
			jsonData.put("ExchangeID", pInputOrderAction->ExchangeID);
			///报单编号
			jsonData.put("OrderSysID", pInputOrderAction->OrderSysID);
			///操作标志
			jsonData.put("ActionFlag", pInputOrderAction->ActionFlag);	
			///价格
			jsonData.put("LimitPrice", pInputOrderAction->LimitPrice);
			///数量变化
			jsonData.put("VolumeChange", pInputOrderAction->VolumeChange);
			///用户代码
			jsonData.put("UserID", pInputOrderAction->UserID);
			///合约代码
			jsonData.put("InstrumentID", pInputOrderAction->InstrumentID);
			///投资单元代码
			jsonData.put("InvestUnitID", pInputOrderAction->InvestUnitID);
			///IP地址
			jsonData.put("IPAddress", pInputOrderAction->IPAddress);
			///Mac地址
			jsonData.put("MacAddress", pInputOrderAction->MacAddress);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
}

///查询最大报单数量响应
void CTPApi::OnRspQueryMaxOrderVolume(CThostFtdcQueryMaxOrderVolumeField *pQueryMaxOrderVolume, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRspQueryMaxOrderVolume", pRspInfo, nRequestID, bIsLast)) {
			//构造Data
			boost::property_tree::ptree jsonData;
			///经纪公司代码
			jsonData.put("BrokerID", pQueryMaxOrderVolume->BrokerID);
			///投资者代码
			jsonData.put("InvestorID", pQueryMaxOrderVolume->InvestorID);
			///合约代码
			jsonData.put("InstrumentID", pQueryMaxOrderVolume->InstrumentID);
			///买卖方向
			jsonData.put("Direction", pQueryMaxOrderVolume->Direction);
			///开平标志
			jsonData.put("OffsetFlag", pQueryMaxOrderVolume->OffsetFlag);
			///投机套保标志
			jsonData.put("HedgeFlag", pQueryMaxOrderVolume->HedgeFlag);
			///最大允许报单数量
			jsonData.put("MaxVolume", pQueryMaxOrderVolume->MaxVolume);
			///交易所代码
			jsonData.put("ExchangeID", pQueryMaxOrderVolume->ExchangeID);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///投资者结算结果确认响应
void CTPApi::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRspSettlementInfoConfirm", pRspInfo, nRequestID, bIsLast)) {
			//构造Data
			boost::property_tree::ptree jsonData;
			///经纪公司代码
			jsonData.put("BrokerID", pSettlementInfoConfirm->BrokerID);
			///投资者代码
			jsonData.put("InvestorID", pSettlementInfoConfirm->InvestorID);
			///确认日期
			jsonData.put("ConfirmDate", pSettlementInfoConfirm->ConfirmDate);
			///确认时间
			jsonData.put("ConfirmTime", pSettlementInfoConfirm->ConfirmTime);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///删除预埋单响应
void CTPApi::OnRspRemoveParkedOrder(CThostFtdcRemoveParkedOrderField *pRemoveParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRspRemoveParkedOrder", pRspInfo, nRequestID, bIsLast)) {
			//构造Data
			boost::property_tree::ptree jsonData;
			///经纪公司代码
			jsonData.put("BrokerID", pRemoveParkedOrder->BrokerID);
			///投资者代码
			jsonData.put("InvestorID", pRemoveParkedOrder->InvestorID);
			///预埋报单编号
			jsonData.put("ParkedOrderID", pRemoveParkedOrder->ParkedOrderID);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///删除预埋撤单响应
void CTPApi::OnRspRemoveParkedOrderAction(CThostFtdcRemoveParkedOrderActionField *pRemoveParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRspRemoveParkedOrderAction", pRspInfo, nRequestID, bIsLast)) {
			//构造Data
			boost::property_tree::ptree jsonData;
			//经纪公司代码
			jsonData.put("BrokerID", pRemoveParkedOrderAction->BrokerID);
			///投资者代码
			jsonData.put("InvestorID", pRemoveParkedOrderAction->InvestorID);
			///预埋撤单编号
			jsonData.put("ParkedOrderActionID", pRemoveParkedOrderAction->ParkedOrderActionID);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///执行宣告录入请求响应
void CTPApi::OnRspExecOrderInsert(CThostFtdcInputExecOrderField *pInputExecOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRspExecOrderInsert", pRspInfo, nRequestID, bIsLast)) {
			//构造Data
			boost::property_tree::ptree jsonData;
			///经纪公司代码
			jsonData.put("BrokerID", pInputExecOrder->BrokerID);
			///投资者代码
			jsonData.put("InvestorID", pInputExecOrder->InvestorID);
			///合约代码
			jsonData.put("InstrumentID", pInputExecOrder->InstrumentID);
			///执行宣告引用
			jsonData.put("ExecOrderRef", pInputExecOrder->ExecOrderRef);
			///用户代码
			jsonData.put("UserID", pInputExecOrder->UserID);
			///数量
			jsonData.put("Volume", pInputExecOrder->Volume);
			///请求编号
			jsonData.put("RequestID", pInputExecOrder->RequestID);
			///业务单元
			jsonData.put("BusinessUnit", pInputExecOrder->BusinessUnit);
			///开平标志
			jsonData.put("OffsetFlag", pInputExecOrder->OffsetFlag);
			///投机套保标志
			jsonData.put("HedgeFlag", pInputExecOrder->HedgeFlag);
			///执行类型
			jsonData.put("ActionType", pInputExecOrder->ActionType);
			///保留头寸申请的持仓方向
			jsonData.put("PosiDirection", pInputExecOrder->PosiDirection);
			///期权行权后是否保留期货头寸的标记
			jsonData.put("ReservePositionFlag", pInputExecOrder->ReservePositionFlag);
			///期权行权后生成的头寸是否自动平仓
			jsonData.put("CloseFlag", pInputExecOrder->CloseFlag);
			///交易所代码
			jsonData.put("ExchangeID", pInputExecOrder->ExchangeID);
			///投资单元代码
			jsonData.put("InvestUnitID", pInputExecOrder->InvestUnitID);
			///资金账号
			jsonData.put("AccountID", pInputExecOrder->AccountID);
			///币种代码
			jsonData.put("CurrencyID", pInputExecOrder->CurrencyID);
			///交易编码
			jsonData.put("ClientID", pInputExecOrder->ClientID);
			///IP地址
			jsonData.put("IPAddress", pInputExecOrder->IPAddress);
			///Mac地址
			jsonData.put("MacAddress", pInputExecOrder->MacAddress);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///执行宣告操作请求响应
void CTPApi::OnRspExecOrderAction(CThostFtdcInputExecOrderActionField *pInputExecOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRspExecOrderAction", pRspInfo, nRequestID, bIsLast)) {
			//构造Data
			boost::property_tree::ptree jsonData;
			///经纪公司代码
			jsonData.put("BrokerID", pInputExecOrderAction->BrokerID);
			///投资者代码
			jsonData.put("InvestorID", pInputExecOrderAction->InvestorID);
			///执行宣告操作引用
			jsonData.put("ExecOrderActionRef", pInputExecOrderAction->ExecOrderActionRef);
			///执行宣告引用
			jsonData.put("ExecOrderRef", pInputExecOrderAction->ExecOrderRef);
			///请求编号
			jsonData.put("RequestID", pInputExecOrderAction->RequestID);
			///前置编号
			jsonData.put("FrontID", pInputExecOrderAction->FrontID);
			///会话编号
			jsonData.put("SessionID", pInputExecOrderAction->SessionID);
			///交易所代码
			jsonData.put("ExchangeID", pInputExecOrderAction->ExchangeID);
			///执行宣告操作编号
			jsonData.put("ExecOrderSysID", pInputExecOrderAction->ExecOrderSysID);
			///操作标志
			jsonData.put("ActionFlag", pInputExecOrderAction->ActionFlag);
			///用户代码
			jsonData.put("UserID", pInputExecOrderAction->UserID);
			///合约代码
			jsonData.put("InstrumentID", pInputExecOrderAction->InstrumentID);
			///投资单元代码
			jsonData.put("InvestUnitID", pInputExecOrderAction->InvestUnitID);
			///IP地址
			jsonData.put("IPAddress", pInputExecOrderAction->IPAddress);
			///Mac地址
			jsonData.put("MacAddress", pInputExecOrderAction->MacAddress);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///询价录入请求响应
void CTPApi::OnRspForQuoteInsert(CThostFtdcInputForQuoteField *pInputForQuote, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRspForQuoteInsert", pRspInfo, nRequestID, bIsLast)) {
			//构造Data
			boost::property_tree::ptree jsonData;
			///经纪公司代码
			jsonData.put("BrokerID", pInputForQuote->BrokerID);
			///投资者代码
			jsonData.put("InvestorID", pInputForQuote->InvestorID);
			///合约代码
			jsonData.put("InstrumentID", pInputForQuote->InstrumentID);
			///询价引用
			jsonData.put("ForQuoteRef", pInputForQuote->ForQuoteRef);
			///用户代码
			jsonData.put("UserID", pInputForQuote->UserID);
			///交易所代码
			jsonData.put("ExchangeID", pInputForQuote->ExchangeID);
			///投资单元代码
			jsonData.put("InvestUnitID", pInputForQuote->InvestUnitID);
			///IP地址
			jsonData.put("IPAddress", pInputForQuote->IPAddress);
			///Mac地址
			jsonData.put("MacAddress", pInputForQuote->MacAddress);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///报价录入请求响应
void CTPApi::OnRspQuoteInsert(CThostFtdcInputQuoteField *pInputQuote, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRspQuoteInsert", pRspInfo, nRequestID, bIsLast)) {
			//构造Data
			boost::property_tree::ptree jsonData;
			///经纪公司代码
			jsonData.put("BrokerID", pInputQuote->BrokerID);
			///投资者代码
			jsonData.put("InvestorID", pInputQuote->InvestorID);
			///合约代码
			///投资者代码
			jsonData.put("InstrumentID", pInputQuote->InstrumentID);
			///报价引用
			jsonData.put("QuoteRef", pInputQuote->QuoteRef);
			///用户代码
			jsonData.put("UserID", pInputQuote->UserID);
			///卖价格
			jsonData.put("AskPrice", pInputQuote->AskPrice);
			///买价格
			jsonData.put("BidPrice", pInputQuote->BidPrice);
			///卖数量
			jsonData.put("AskVolume", pInputQuote->AskVolume);
			///买数量
			jsonData.put("BidVolume", pInputQuote->BidVolume);
			///请求编号
			jsonData.put("RequestID", pInputQuote->RequestID);
			///业务单元
			jsonData.put("BusinessUnit", pInputQuote->BusinessUnit);
			///卖开平标志
			jsonData.put("AskOffsetFlag", pInputQuote->AskOffsetFlag);
			///买开平标志
			jsonData.put("BidOffsetFlag", pInputQuote->BidOffsetFlag);
			///卖投机套保标志
			jsonData.put("AskHedgeFlag", pInputQuote->AskHedgeFlag);
			///买投机套保标志
			jsonData.put("BidHedgeFlag", pInputQuote->BidHedgeFlag);
			///衍生卖报单引用
			jsonData.put("AskOrderRef", pInputQuote->AskOrderRef);
			///衍生买报单引用
			jsonData.put("BidOrderRef", pInputQuote->BidOrderRef);
			///应价编号
			jsonData.put("ForQuoteSysID", pInputQuote->ForQuoteSysID);
			///交易所代码
			jsonData.put("ExchangeID", pInputQuote->ExchangeID);
			///投资单元代码
			jsonData.put("InvestUnitID", pInputQuote->InvestUnitID);
			///交易编码
			jsonData.put("ClientID", pInputQuote->ClientID);
			///IP地址
			jsonData.put("IPAddress", pInputQuote->IPAddress);
			///Mac地址
			jsonData.put("MacAddress", pInputQuote->MacAddress);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///报价操作请求响应
void CTPApi::OnRspQuoteAction(CThostFtdcInputQuoteActionField *pInputQuoteAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRspQuoteAction", pRspInfo, nRequestID, bIsLast)) {
			//构造Data
			boost::property_tree::ptree jsonData;
			///经纪公司代码
			jsonData.put("BrokerID", pInputQuoteAction->BrokerID);
			///投资者代码
			jsonData.put("InvestorID", pInputQuoteAction->InvestorID);
			///报价操作引用
			jsonData.put("QuoteActionRef", pInputQuoteAction->QuoteActionRef);
			///报价引用
			jsonData.put("QuoteRef", pInputQuoteAction->QuoteRef);
			///请求编号
			jsonData.put("RequestID", pInputQuoteAction->RequestID);
			///前置编号
			jsonData.put("FrontID", pInputQuoteAction->FrontID);
			///会话编号
			jsonData.put("SessionID", pInputQuoteAction->SessionID);
			///交易所代码
			jsonData.put("ExchangeID", pInputQuoteAction->ExchangeID);
			///报价操作编号
			jsonData.put("QuoteSysID", pInputQuoteAction->QuoteSysID);
			///操作标志
			jsonData.put("QuoteSActionFlagysID", pInputQuoteAction->ActionFlag);
			///用户代码
			jsonData.put("UserID", pInputQuoteAction->UserID);
			///合约代码
			jsonData.put("InstrumentID", pInputQuoteAction->InstrumentID);
			///投资单元代码
			jsonData.put("InvestUnitID", pInputQuoteAction->InvestUnitID);
			///交易编码
			jsonData.put("ClientID", pInputQuoteAction->ClientID);
			///IP地址
			jsonData.put("IPAddress", pInputQuoteAction->IPAddress);
			///Mac地址
			jsonData.put("MacAddress", pInputQuoteAction->MacAddress);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///锁定应答
void CTPApi::OnRspLockInsert(CThostFtdcInputLockField *pInputLock, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRspLockInsert", pRspInfo, nRequestID, bIsLast)) {
			//构造Data
			boost::property_tree::ptree jsonData;
			///经纪公司代码
			jsonData.put("BrokerID", pInputLock->BrokerID);
			///投资者代码
			jsonData.put("InvestorID", pInputLock->InvestorID);
			///合约代码
			jsonData.put("InstrumentID", pInputLock->InstrumentID);
			///锁定引用
			jsonData.put("LockRef", pInputLock->LockRef);
			///用户代码
			jsonData.put("UserID", pInputLock->UserID);
			///数量
			jsonData.put("Volume", pInputLock->Volume);
			///请求编号
			jsonData.put("RequestID", pInputLock->RequestID);
			///业务单元
			jsonData.put("BusinessUnit", pInputLock->BusinessUnit);
			///锁定类型
			jsonData.put("LockType", pInputLock->LockType);
			///交易所代码
			jsonData.put("ExchangeID", pInputLock->ExchangeID);
			///IP地址
			jsonData.put("IPAddress", pInputLock->IPAddress);
			///Mac地址
			jsonData.put("MacAddress", pInputLock->MacAddress);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///批量报单操作请求响应
void CTPApi::OnRspBatchOrderAction(CThostFtdcInputBatchOrderActionField *pInputBatchOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRspBatchOrderAction", pRspInfo, nRequestID, bIsLast)) {
			//构造Data
			boost::property_tree::ptree jsonData;
			///经纪公司代码
			jsonData.put("BrokerID", pInputBatchOrderAction->BrokerID);
			///投资者代码
			jsonData.put("InvestorID", pInputBatchOrderAction->InvestorID);
			///报单操作引用
			jsonData.put("OrderActionRef", pInputBatchOrderAction->OrderActionRef);
			///请求编号
			jsonData.put("RequestID", pInputBatchOrderAction->RequestID);
			///前置编号
			jsonData.put("FrontID", pInputBatchOrderAction->FrontID);
			///会话编号
			jsonData.put("SessionID", pInputBatchOrderAction->SessionID);
			///交易所代码
			jsonData.put("ExchangeID", pInputBatchOrderAction->ExchangeID);
			///用户代码
			jsonData.put("UserID", pInputBatchOrderAction->UserID);
			///投资单元代码
			jsonData.put("InvestUnitID", pInputBatchOrderAction->InvestUnitID);
			///IP地址
			jsonData.put("IPAddress", pInputBatchOrderAction->IPAddress);
			///Mac地址
			jsonData.put("MacAddress", pInputBatchOrderAction->MacAddress);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///申请组合录入请求响应
void CTPApi::OnRspCombActionInsert(CThostFtdcInputCombActionField *pInputCombAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRspCombActionInsert", pRspInfo, nRequestID, bIsLast)) {
			//构造Data
			boost::property_tree::ptree jsonData;
			///经纪公司代码
			jsonData.put("BrokerID", pInputCombAction->BrokerID);
			///投资者代码
			jsonData.put("InvestorID", pInputCombAction->InvestorID);
			///合约代码
			jsonData.put("InstrumentID", pInputCombAction->InstrumentID);
			///组合引用
			jsonData.put("CombActionRef", pInputCombAction->CombActionRef);
			///用户代码
			jsonData.put("UserID", pInputCombAction->UserID);
			///买卖方向
			jsonData.put("Direction", pInputCombAction->Direction);
			///数量
			jsonData.put("Volume", pInputCombAction->Volume);
			///组合指令方向
			jsonData.put("CombDirection", pInputCombAction->CombDirection);
			///投机套保标志
			jsonData.put("HedgeFlag", pInputCombAction->HedgeFlag);
			///交易所代码
			jsonData.put("ExchangeID", pInputCombAction->ExchangeID);
			///IP地址
			jsonData.put("IPAddress", pInputCombAction->IPAddress);
			///Mac地址
			jsonData.put("MacAddress", pInputCombAction->MacAddress);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///请求查询报单响应
void CTPApi::OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRspQryOrder", pRspInfo, nRequestID, bIsLast)) {
			//构造Data
			boost::property_tree::ptree jsonData;
			///经纪公司代码
			jsonData.put("BrokerID", pOrder->BrokerID);
			///投资者代码
			jsonData.put("InvestorID", pOrder->InvestorID);
			///合约代码
			jsonData.put("InstrumentID", pOrder->InstrumentID);
			///报单引用
			jsonData.put("OrderRef", pOrder->OrderRef);
			///用户代码
			jsonData.put("UserID", pOrder->UserID);
			///报单价格条件
			jsonData.put("OrderPriceType", pOrder->OrderPriceType);
			///买卖方向
			jsonData.put("Direction", pOrder->Direction);
			///组合开平标志
			jsonData.put("CombOffsetFlag", pOrder->CombOffsetFlag);
			///组合投机套保标志
			jsonData.put("CombHedgeFlag", pOrder->CombHedgeFlag);
			///价格
			jsonData.put("LimitPrice", pOrder->LimitPrice);
			///数量
			jsonData.put("VolumeTotalOriginal", pOrder->VolumeTotalOriginal);
			///有效期类型
			jsonData.put("TimeCondition", pOrder->TimeCondition);
			///GTD日期
			jsonData.put("GTDDate", pOrder->GTDDate);
			///成交量类型
			jsonData.put("VolumeCondition", pOrder->VolumeCondition);
			///最小成交量
			jsonData.put("MinVolume", pOrder->MinVolume);
			///触发条件
			jsonData.put("ContingentCondition", pOrder->ContingentCondition);
			///止损价
			jsonData.put("StopPrice", pOrder->StopPrice);
			///强平原因
			jsonData.put("ForceCloseReason", pOrder->ForceCloseReason);
			///自动挂起标志
			jsonData.put("IsAutoSuspend", pOrder->IsAutoSuspend);
			///业务单元
			jsonData.put("BusinessUnit", pOrder->BusinessUnit);
			///请求编号
			jsonData.put("RequestID", pOrder->RequestID);
			///本地报单编号
			jsonData.put("OrderLocalID", pOrder->OrderLocalID);
			///交易所代码
			jsonData.put("ExchangeID", pOrder->ExchangeID);
			///会员代码
			jsonData.put("ParticipantID", pOrder->ParticipantID);
			///客户代码
			jsonData.put("ClientID", pOrder->ClientID);
			///合约在交易所的代码
			jsonData.put("ExchangeInstID", pOrder->ExchangeInstID);
			///交易所交易员代码
			jsonData.put("TraderID", pOrder->TraderID);
			///安装编号
			jsonData.put("InstallID", pOrder->InstallID);
			///报单提交状态
			jsonData.put("OrderSubmitStatus", pOrder->OrderSubmitStatus);
			///报单提示序号
			jsonData.put("NotifySequence", pOrder->NotifySequence);
			///交易日
			jsonData.put("TradingDay", pOrder->TradingDay);
			///结算编号
			jsonData.put("SettlementID", pOrder->SettlementID);
			///报单编号
			jsonData.put("OrderSysID", pOrder->OrderSysID);
			///报单来源
			jsonData.put("OrderSource", pOrder->OrderSource);
			///报单状态
			jsonData.put("OrderStatus", pOrder->OrderStatus);
			///报单类型
			jsonData.put("OrderType", pOrder->OrderType);
			///今成交数量
			jsonData.put("VolumeTraded", pOrder->VolumeTraded);
			///剩余数量
			jsonData.put("VolumeTotal", pOrder->VolumeTotal);
			///报单日期
			jsonData.put("InsertDate", pOrder->InsertDate);
			///委托时间
			jsonData.put("InsertTime", pOrder->InsertTime);
			///激活时间
			jsonData.put("ActiveTime", pOrder->ActiveTime);
			///挂起时间
			jsonData.put("SuspendTime", pOrder->SuspendTime);
			///最后修改时间
			jsonData.put("UpdateTime", pOrder->UpdateTime);
			///撤销时间
			jsonData.put("CancelTime", pOrder->CancelTime);
			///最后修改交易所交易员代码
			jsonData.put("ActiveTraderID", pOrder->ActiveTraderID);
			///结算会员编号
			jsonData.put("ClearingPartID", pOrder->ClearingPartID);
			///序号
			jsonData.put("SequenceNo", pOrder->SequenceNo);
			///前置编号
			jsonData.put("FrontID", pOrder->FrontID);
			///会话编号
			jsonData.put("SessionID", pOrder->SessionID);
			///用户端产品信息
			jsonData.put("UserProductInfo", pOrder->UserProductInfo);
			///状态信息
			jsonData.put("StatusMsg", pOrder->StatusMsg);
			///用户强评标志
			jsonData.put("UserForceClose", pOrder->UserForceClose);
			///操作用户代码
			jsonData.put("ActiveUserID", pOrder->ActiveUserID);
			///经纪公司报单编号
			jsonData.put("BrokerOrderSeq", pOrder->BrokerOrderSeq);
			///相关报单
			jsonData.put("RelativeOrderSysID", pOrder->RelativeOrderSysID);
			///郑商所成交数量
			jsonData.put("ZCETotalTradedVolume", pOrder->ZCETotalTradedVolume);
			///互换单标志
			jsonData.put("IsSwapOrder", pOrder->IsSwapOrder);
			///营业部编号
			jsonData.put("BranchID", pOrder->BranchID);
			///投资单元代码
			jsonData.put("InvestUnitID", pOrder->InvestUnitID);
			///资金账号
			jsonData.put("AccountID", pOrder->AccountID);
			///币种代码
			jsonData.put("CurrencyID", pOrder->CurrencyID);
			///IP地址
			jsonData.put("IPAddress", pOrder->IPAddress);
			///Mac地址
			jsonData.put("MacAddress", pOrder->MacAddress);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///请求查询成交响应
void CTPApi::OnRspQryTrade(CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRspQryTrade", pRspInfo, nRequestID, bIsLast)) {
			//构造Data
			boost::property_tree::ptree jsonData;
			//经纪公司代码
			jsonData.put("BrokerID", pTrade->BrokerID);
			///投资者代码
			jsonData.put("InvestorID", pTrade->InvestorID);
			///合约代码
			jsonData.put("InstrumentID", pTrade->InstrumentID);
			///报单引用
			jsonData.put("OrderRef", pTrade->OrderRef);
			///用户代码
			jsonData.put("UserID", pTrade->UserID);
			///交易所代码
			jsonData.put("ExchangeID", pTrade->ExchangeID);
			///成交编号
			jsonData.put("TradeID", pTrade->TradeID);
			///买卖方向
			jsonData.put("Direction", pTrade->Direction);
			///报单编号
			jsonData.put("OrderSysID", pTrade->OrderSysID);
			///会员代码
			jsonData.put("ParticipantID", pTrade->ParticipantID);
			///客户代码
			jsonData.put("ClientID", pTrade->ClientID);
			///交易角色
			jsonData.put("TradingRole", pTrade->TradingRole);
			///合约在交易所的代码
			jsonData.put("ExchangeInstID", pTrade->ExchangeInstID);
			///开平标志
			jsonData.put("OffsetFlag", pTrade->OffsetFlag);
			///投机套保标志
			jsonData.put("HedgeFlag", pTrade->HedgeFlag);
			///价格
			jsonData.put("Price", pTrade->Price);
			///数量
			jsonData.put("Volume", pTrade->Volume);
			///成交时期
			jsonData.put("TradeDate", pTrade->TradeDate);
			///成交时间
			jsonData.put("TradeTime", pTrade->TradeTime);
			///成交类型
			jsonData.put("TradeType", pTrade->TradeType);
			///成交价来源
			jsonData.put("PriceSource", pTrade->PriceSource);
			///交易所交易员代码
			jsonData.put("TraderID", pTrade->TraderID);
			///本地报单编号
			jsonData.put("OrderLocalID", pTrade->OrderLocalID);
			///结算会员编号
			jsonData.put("ClearingPartID", pTrade->ClearingPartID);
			///业务单元
			jsonData.put("BusinessUnit", pTrade->BusinessUnit);
			///序号
			jsonData.put("SequenceNo", pTrade->SequenceNo);
			///交易日
			jsonData.put("TradingDay", pTrade->TradingDay);
			///结算编号
			jsonData.put("SettlementID", pTrade->SettlementID);
			///经纪公司报单编号
			jsonData.put("BrokerOrderSeq", pTrade->BrokerOrderSeq);
			///成交来源
			jsonData.put("TradeSource", pTrade->TradeSource);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///请求查询投资者持仓响应
void CTPApi::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRspQryInvestorPosition", pRspInfo, nRequestID, bIsLast)) {
			//构造Data
			boost::property_tree::ptree jsonData;
			///合约代码
			jsonData.put("InstrumentID", pInvestorPosition->InstrumentID);
			///经纪公司代码
			jsonData.put("BrokerID", pInvestorPosition->BrokerID);
			///投资者代码
			jsonData.put("InvestorID", pInvestorPosition->InvestorID);
			///持仓多空方向
			jsonData.put("PosiDirection", pInvestorPosition->PosiDirection);
			///投机套保标志
			jsonData.put("HedgeFlag", pInvestorPosition->HedgeFlag);
			///持仓日期
			jsonData.put("PositionDate", pInvestorPosition->PositionDate);
			///上日持仓
			jsonData.put("YdPosition", pInvestorPosition->YdPosition);
			///今日持仓
			jsonData.put("Position", pInvestorPosition->Position);
			///多头冻结
			jsonData.put("LongFrozen", pInvestorPosition->LongFrozen);
			///空头冻结
			jsonData.put("ShortFrozen", pInvestorPosition->ShortFrozen);
			///开仓冻结金额
			jsonData.put("LongFrozenAmount", pInvestorPosition->LongFrozenAmount);
			///开仓冻结金额
			jsonData.put("ShortFrozenAmount", pInvestorPosition->ShortFrozenAmount);
			///开仓量
			jsonData.put("OpenVolume", pInvestorPosition->OpenVolume);
			///平仓量
			jsonData.put("CloseVolume", pInvestorPosition->CloseVolume);
			///开仓金额
			jsonData.put("OpenAmount", pInvestorPosition->OpenAmount);
			///平仓金额
			jsonData.put("CloseAmount", pInvestorPosition->CloseAmount);
			///持仓成本
			jsonData.put("PositionCost", pInvestorPosition->PositionCost);
			///上次占用的保证金
			jsonData.put("PreMargin", pInvestorPosition->PreMargin);
			///占用的保证金
			jsonData.put("UseMargin", pInvestorPosition->UseMargin);
			///冻结的保证金
			jsonData.put("FrozenMargin", pInvestorPosition->FrozenMargin);
			///冻结的资金
			jsonData.put("FrozenCash", pInvestorPosition->FrozenCash);
			///冻结的手续费
			jsonData.put("FrozenCommission", pInvestorPosition->FrozenCommission);
			///资金差额
			jsonData.put("CashIn", pInvestorPosition->CashIn);
			///手续费
			jsonData.put("Commission", pInvestorPosition->Commission);
			///平仓盈亏
			jsonData.put("CloseProfit", pInvestorPosition->CloseProfit);
			///持仓盈亏
			jsonData.put("PositionProfit", pInvestorPosition->PositionProfit);
			///上次结算价
			jsonData.put("PreSettlementPrice", pInvestorPosition->PreSettlementPrice);
			///本次结算价
			jsonData.put("SettlementPrice", pInvestorPosition->SettlementPrice);
			///交易日
			jsonData.put("TradingDay", pInvestorPosition->TradingDay);
			///结算编号
			jsonData.put("SettlementID", pInvestorPosition->SettlementID);
			///开仓成本
			jsonData.put("OpenCost", pInvestorPosition->OpenCost);
			///交易所保证金
			jsonData.put("ExchangeMargin", pInvestorPosition->ExchangeMargin);
			///组合成交形成的持仓
			jsonData.put("CombPosition", pInvestorPosition->CombPosition);
			///组合多头冻结
			jsonData.put("CombLongFrozen", pInvestorPosition->CombLongFrozen);
			///组合空头冻结
			jsonData.put("CombShortFrozen", pInvestorPosition->CombShortFrozen);
			///逐日盯市平仓盈亏
			jsonData.put("CloseProfitByDate", pInvestorPosition->CloseProfitByDate);
			///逐笔对冲平仓盈亏
			jsonData.put("CloseProfitByTrade", pInvestorPosition->CloseProfitByTrade);
			///今日持仓
			jsonData.put("TodayPosition", pInvestorPosition->TodayPosition);
			///保证金率
			jsonData.put("MarginRateByMoney", pInvestorPosition->MarginRateByMoney);
			///保证金率(按手数)
			jsonData.put("MarginRateByVolume", pInvestorPosition->MarginRateByVolume);
			///执行冻结
			jsonData.put("StrikeFrozen", pInvestorPosition->StrikeFrozen);
			///执行冻结金额
			jsonData.put("StrikeFrozenAmount", pInvestorPosition->StrikeFrozenAmount);
			///放弃执行冻结
			jsonData.put("AbandonFrozen", pInvestorPosition->AbandonFrozen);
			///交易所代码
			jsonData.put("ExchangeID", pInvestorPosition->ExchangeID);
			///执行冻结的昨仓
			jsonData.put("YdStrikeFrozen", pInvestorPosition->YdStrikeFrozen);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///请求查询资金账户响应
void CTPApi::OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRspQryTradingAccount", pRspInfo, nRequestID, bIsLast)) {
			//构造Data
			boost::property_tree::ptree jsonData;
			//经纪公司代码
			jsonData.put("BrokerID", pTradingAccount->BrokerID);
			///投资者帐号
			jsonData.put("AccountID", pTradingAccount->AccountID);
			///上次质押金额
			jsonData.put("PreMortgage", pTradingAccount->PreMortgage);
			///上次信用额度
			jsonData.put("PreCredit", pTradingAccount->PreCredit);
			///上次存款额
			jsonData.put("PreDeposit", pTradingAccount->PreDeposit);
			///上次结算准备金
			jsonData.put("PreBalance", pTradingAccount->PreBalance);
			///上次占用的保证金
			jsonData.put("PreMargin", pTradingAccount->PreMargin);
			///利息基数
			jsonData.put("InterestBase", pTradingAccount->InterestBase);
			///利息收入
			jsonData.put("Interest", pTradingAccount->Interest);
			///入金金额
			jsonData.put("Deposit", pTradingAccount->Deposit);
			///出金金额
			jsonData.put("Withdraw", pTradingAccount->Withdraw);
			///冻结的保证金
			jsonData.put("FrozenMargin", pTradingAccount->FrozenMargin);
			///冻结的资金
			jsonData.put("FrozenCash", pTradingAccount->FrozenCash);
			///冻结的手续费
			jsonData.put("FrozenCommission", pTradingAccount->FrozenCommission);
			///当前保证金总额
			jsonData.put("CurrMargin", pTradingAccount->CurrMargin);
			///资金差额
			jsonData.put("CashIn", pTradingAccount->CashIn);
			///手续费
			jsonData.put("Commission", pTradingAccount->Commission);
			///平仓盈亏
			jsonData.put("CloseProfit", pTradingAccount->CloseProfit);
			///持仓盈亏
			jsonData.put("PositionProfit", pTradingAccount->PositionProfit);
			///期货结算准备金
			jsonData.put("Balance", pTradingAccount->Balance);
			///可用资金
			jsonData.put("Available", pTradingAccount->Available);
			///可取资金
			jsonData.put("WithdrawQuota", pTradingAccount->WithdrawQuota);
			///基本准备金
			jsonData.put("Reserve", pTradingAccount->Reserve);
			///交易日
			jsonData.put("TradingDay", pTradingAccount->TradingDay);
			///结算编号
			jsonData.put("SettlementID", pTradingAccount->SettlementID);
			///信用额度
			jsonData.put("Credit", pTradingAccount->Credit);
			///质押金额
			jsonData.put("Mortgage", pTradingAccount->Mortgage);
			///交易所保证金
			jsonData.put("ExchangeMargin", pTradingAccount->ExchangeMargin);
			///投资者交割保证金
			jsonData.put("DeliveryMargin", pTradingAccount->DeliveryMargin);
			///交易所交割保证金
			jsonData.put("ExchangeDeliveryMargin", pTradingAccount->ExchangeDeliveryMargin);
			///保底期货结算准备金
			jsonData.put("ReserveBalance", pTradingAccount->ReserveBalance);
			///币种代码
			jsonData.put("CurrencyID", pTradingAccount->CurrencyID);
			///上次货币质入金额
			jsonData.put("PreFundMortgageIn", pTradingAccount->PreFundMortgageIn);
			///上次货币质出金额
			jsonData.put("PreFundMortgageOut", pTradingAccount->PreFundMortgageOut);
			///货币质入金额
			jsonData.put("FundMortgageIn", pTradingAccount->FundMortgageIn);
			///货币质出金额
			jsonData.put("FundMortgageOut", pTradingAccount->FundMortgageOut);
			///货币质押余额
			jsonData.put("FundMortgageAvailable", pTradingAccount->FundMortgageAvailable);
			///可质押货币金额
			jsonData.put("MortgageableFund", pTradingAccount->MortgageableFund);
			///特殊产品占用保证金
			jsonData.put("SpecProductMargin", pTradingAccount->SpecProductMargin);
			///特殊产品冻结保证金
			jsonData.put("SpecProductFrozenMargin", pTradingAccount->SpecProductFrozenMargin);
			///特殊产品手续费
			jsonData.put("SpecProductCommission", pTradingAccount->SpecProductCommission);
			///特殊产品冻结手续费
			jsonData.put("SpecProductFrozenCommission", pTradingAccount->SpecProductFrozenCommission);
			///特殊产品持仓盈亏
			jsonData.put("SpecProductPositionProfit", pTradingAccount->SpecProductPositionProfit);
			///特殊产品平仓盈亏
			jsonData.put("SpecProductCloseProfit", pTradingAccount->SpecProductCloseProfit);
			///根据持仓盈亏算法计算的特殊产品持仓盈亏
			jsonData.put("SpecProductPositionProfitByAlg", pTradingAccount->SpecProductPositionProfitByAlg);
			///特殊产品交易所保证金
			jsonData.put("SpecProductExchangeMargin", pTradingAccount->SpecProductExchangeMargin);
			///业务类型
			jsonData.put("BizType", pTradingAccount->BizType);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///请求查询投资者响应
void CTPApi::OnRspQryInvestor(CThostFtdcInvestorField *pInvestor, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRspQryInvestor", pRspInfo, nRequestID, bIsLast)) {
			//构造Data
			boost::property_tree::ptree jsonData;
			///投资者代码
			jsonData.put("InvestorID", pInvestor->InvestorID);
			//经纪公司代码
			jsonData.put("BrokerID", pInvestor->BrokerID);
			///投资者分组代码
			jsonData.put("InvestorGroupID", pInvestor->InvestorGroupID);
			///投资者名称
			jsonData.put("InvestorName", pInvestor->InvestorName);
			///证件类型
			jsonData.put("IdentifiedCardType", pInvestor->IdentifiedCardType);
			///证件号码
			jsonData.put("IdentifiedCardNo", pInvestor->IdentifiedCardNo);
			///是否活跃
			jsonData.put("IsActive", pInvestor->IsActive);
			///联系电话
			jsonData.put("Telephone", pInvestor->Telephone);
			///通讯地址
			jsonData.put("Address", pInvestor->Address);
			///开户日期
			jsonData.put("OpenDate", pInvestor->OpenDate);
			///手机
			jsonData.put("Mobile", pInvestor->Mobile);
			///手续费率模板代码
			jsonData.put("CommModelID", pInvestor->CommModelID);
			///保证金率模板代码
			jsonData.put("MarginModelID", pInvestor->MarginModelID);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///请求查询交易编码响应
void CTPApi::OnRspQryTradingCode(CThostFtdcTradingCodeField *pTradingCode, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRspQryTradingCode", pRspInfo, nRequestID, bIsLast)) {
			//构造Data
			boost::property_tree::ptree jsonData;
			///投资者代码
			jsonData.put("InvestorID", pTradingCode->InvestorID);
			//经纪公司代码
			jsonData.put("BrokerID", pTradingCode->BrokerID);
			///交易所代码
			jsonData.put("ExchangeID", pTradingCode->ExchangeID);
			///客户代码
			jsonData.put("ClientID", pTradingCode->ExchangeID);
			///是否活跃
			jsonData.put("IsActive", pTradingCode->IsActive);
			///交易编码类型
			jsonData.put("ClientIDType", pTradingCode->ClientIDType);
			///营业部编号
			jsonData.put("BranchID", pTradingCode->BranchID);
			///业务类型
			jsonData.put("BizType", pTradingCode->BizType);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///请求查询合约保证金率响应
void CTPApi::OnRspQryInstrumentMarginRate(CThostFtdcInstrumentMarginRateField *pInstrumentMarginRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRspQryInstrumentMarginRate", pRspInfo, nRequestID, bIsLast)) {
			//构造Data
			boost::property_tree::ptree jsonData;
			///合约代码
			jsonData.put("InstrumentID", pInstrumentMarginRate->InstrumentID);
			///投资者范围
			jsonData.put("InvestorRange", pInstrumentMarginRate->BrokerID);
			//经纪公司代码
			jsonData.put("BrokerID", pInstrumentMarginRate->BrokerID);
			///投资者代码
			jsonData.put("InvestorID", pInstrumentMarginRate->InvestorID);
			///投机套保标志
			jsonData.put("HedgeFlag", pInstrumentMarginRate->HedgeFlag);
			///多头保证金率
			jsonData.put("LongMarginRatioByMoney", pInstrumentMarginRate->LongMarginRatioByMoney);
			///多头保证金费
			jsonData.put("LongMarginRatioByVolume", pInstrumentMarginRate->LongMarginRatioByVolume);
			///空头保证金率
			jsonData.put("ShortMarginRatioByMoney", pInstrumentMarginRate->ShortMarginRatioByMoney);
			///空头保证金费
			jsonData.put("ShortMarginRatioByVolume", pInstrumentMarginRate->ShortMarginRatioByVolume);
			///是否相对交易所收取
			jsonData.put("IsRelative", pInstrumentMarginRate->IsRelative);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///请求查询合约手续费率响应
void CTPApi::OnRspQryInstrumentCommissionRate(CThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRspQryInstrumentCommissionRate", pRspInfo, nRequestID, bIsLast)) {
			//构造Data
			boost::property_tree::ptree jsonData;
			///合约代码
			jsonData.put("InstrumentID", pInstrumentCommissionRate->InstrumentID);
			///投资者范围
			jsonData.put("InvestorRange", pInstrumentCommissionRate->InvestorRange);
			//经纪公司代码
			jsonData.put("BrokerID", pInstrumentCommissionRate->BrokerID);
			///投资者代码
			jsonData.put("InvestorID", pInstrumentCommissionRate->InvestorID);
			///开仓手续费率
			jsonData.put("OpenRatioByMoney", pInstrumentCommissionRate->OpenRatioByMoney);
			///开仓手续费
			jsonData.put("OpenRatioByVolume", pInstrumentCommissionRate->OpenRatioByVolume);
			///平仓手续费率
			jsonData.put("CloseRatioByMoney", pInstrumentCommissionRate->CloseRatioByMoney);
			///平仓手续费
			jsonData.put("CloseRatioByVolume", pInstrumentCommissionRate->CloseRatioByVolume);
			///平今手续费率
			jsonData.put("CloseTodayRatioByMoney", pInstrumentCommissionRate->CloseTodayRatioByMoney);
			///平今手续费
			jsonData.put("CloseTodayRatioByVolume", pInstrumentCommissionRate->CloseTodayRatioByVolume);
			///交易所代码
			jsonData.put("ExchangeID", pInstrumentCommissionRate->ExchangeID);
			///业务类型
			jsonData.put("BizType", pInstrumentCommissionRate->BizType);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
}

///请求查询交易所响应
void CTPApi::OnRspQryExchange(CThostFtdcExchangeField *pExchange, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRspQryExchange", pRspInfo, nRequestID, bIsLast)) {
			//构造Data
			boost::property_tree::ptree jsonData;
			///交易所代码
			jsonData.put("ExchangeID", pExchange->ExchangeID);
			///交易所名称
			jsonData.put("ExchangeName", pExchange->ExchangeName);
			///交易所属性
			jsonData.put("ExchangeProperty", pExchange->ExchangeProperty);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///请求查询产品响应
void CTPApi::OnRspQryProduct(CThostFtdcProductField *pProduct, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRspQryProduct", pRspInfo, nRequestID, bIsLast)) {
			//构造Data
			boost::property_tree::ptree jsonData;
			///产品代码
			jsonData.put("ProductID", pProduct->ProductID);
			///产品名称
			jsonData.put("ProductName", pProduct->ProductName);
			///交易所代码
			jsonData.put("ExchangeID", pProduct->ExchangeID);
			///产品类型
			jsonData.put("ProductClass", pProduct->ProductClass);
			///合约数量乘数
			jsonData.put("VolumeMultiple", pProduct->VolumeMultiple);
			///最小变动价位
			jsonData.put("PriceTick", pProduct->PriceTick);
			///市价单最大下单量
			jsonData.put("MaxMarketOrderVolume", pProduct->MaxMarketOrderVolume);
			///市价单最小下单量
			jsonData.put("MinMarketOrderVolume", pProduct->MinMarketOrderVolume);
			///限价单最大下单量
			jsonData.put("MaxLimitOrderVolume", pProduct->MaxLimitOrderVolume);
			///限价单最小下单量
			jsonData.put("MinLimitOrderVolume", pProduct->MinLimitOrderVolume);
			///持仓类型
			jsonData.put("PositionType", pProduct->PositionType);
			///持仓日期类型
			jsonData.put("PositionDateType", pProduct->PositionDateType);
			///平仓处理类型
			jsonData.put("CloseDealType", pProduct->CloseDealType);
			///交易币种类型
			jsonData.put("TradeCurrencyID", pProduct->TradeCurrencyID);
			///质押资金可用范围
			jsonData.put("MortgageFundUseRange", pProduct->MortgageFundUseRange);
			///交易所产品代码
			jsonData.put("ExchangeProductID", pProduct->ExchangeProductID);
			///合约基础商品乘数
			jsonData.put("UnderlyingMultiple", pProduct->UnderlyingMultiple);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///请求查询合约响应
void CTPApi::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRspQryInstrument", pRspInfo, nRequestID, bIsLast)) {
			//构造Data
			boost::property_tree::ptree jsonData;
			///合约代码
			jsonData.put("InstrumentID", pInstrument->InstrumentID);
			///交易所代码
			jsonData.put("ExchangeID", pInstrument->ExchangeID);
			///合约名称
			jsonData.put("InstrumentName", pInstrument->InstrumentName);
			///合约在交易所的代码
			jsonData.put("ExchangeInstID", pInstrument->ExchangeInstID);
			///产品代码
			jsonData.put("ProductID", pInstrument->ProductID);
			///产品类型
			jsonData.put("ProductClass", pInstrument->ProductClass);
			///交割年份
			jsonData.put("DeliveryYear", pInstrument->DeliveryYear);
			///交割月
			jsonData.put("DeliveryMonth", pInstrument->DeliveryMonth);
			///市价单最大下单量
			jsonData.put("MaxMarketOrderVolume", pInstrument->MaxMarketOrderVolume);
			///市价单最小下单量
			jsonData.put("MinMarketOrderVolume", pInstrument->MinMarketOrderVolume);
			///限价单最大下单量
			jsonData.put("MaxLimitOrderVolume", pInstrument->MaxLimitOrderVolume);
			///限价单最小下单量
			jsonData.put("MinLimitOrderVolume", pInstrument->MinLimitOrderVolume);
			///合约数量乘数
			jsonData.put("VolumeMultiple", pInstrument->VolumeMultiple);
			///最小变动价位
			jsonData.put("PriceTick", pInstrument->PriceTick);
			///创建日
			jsonData.put("CreateDate", pInstrument->CreateDate);
			///上市日
			jsonData.put("OpenDate", pInstrument->OpenDate);
			///到期日
			jsonData.put("ExpireDate", pInstrument->ExpireDate);
			///开始交割日
			jsonData.put("StartDelivDate", pInstrument->StartDelivDate);
			///结束交割日
			jsonData.put("EndDelivDate", pInstrument->EndDelivDate);
			///合约生命周期状态
			jsonData.put("InstLifePhase", pInstrument->InstLifePhase);
			///当前是否交易
			jsonData.put("IsTrading", pInstrument->IsTrading);
			///持仓类型
			jsonData.put("PositionType", pInstrument->PositionType);
			///持仓日期类型
			jsonData.put("PositionDateType", pInstrument->PositionDateType);
			///多头保证金率
			jsonData.put("LongMarginRatio", pInstrument->LongMarginRatio);
			///空头保证金率
			jsonData.put("ShortMarginRatio", pInstrument->ShortMarginRatio);
			///是否使用大额单边保证金算法
			jsonData.put("MaxMarginSideAlgorithm", pInstrument->MaxMarginSideAlgorithm);
			///基础商品代码
			jsonData.put("UnderlyingInstrID", pInstrument->UnderlyingInstrID);
			///执行价
			jsonData.put("StrikePrice", pInstrument->StrikePrice);
			///期权类型
			jsonData.put("OptionsType", pInstrument->OptionsType);
			///合约基础商品乘数
			jsonData.put("UnderlyingMultiple", pInstrument->UnderlyingMultiple);
			///组合类型
			jsonData.put("CombinationType", pInstrument->CombinationType);
			///最小买下单单位
			jsonData.put("MinBuyVolume", pInstrument->MinBuyVolume);
			///最小卖下单单位
			jsonData.put("MinSellVolume", pInstrument->MinSellVolume);
			///合约标识码
			jsonData.put("InstrumentCode", pInstrument->InstrumentCode);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
}

///请求查询行情响应
void CTPApi::OnRspQryDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRspQryDepthMarketData", pRspInfo, nRequestID, bIsLast)) {
			//构造Data
			boost::property_tree::ptree jsonData;
			///交易日
			jsonData.put("TradingDay", pDepthMarketData->TradingDay);
			///合约代码
			jsonData.put("InstrumentID", pDepthMarketData->InstrumentID);
			///交易所代码
			jsonData.put("ExchangeID", pDepthMarketData->ExchangeID);
			///合约在交易所的代码
			jsonData.put("ExchangeInstID", pDepthMarketData->ExchangeInstID);
			///最新价
			jsonData.put("LastPrice", pDepthMarketData->LastPrice);
			///上次结算价
			jsonData.put("PreSettlementPrice", pDepthMarketData->PreSettlementPrice);
			///昨收盘
			jsonData.put("PreClosePrice", pDepthMarketData->PreClosePrice);
			///昨持仓量
			jsonData.put("PreOpenInterest", pDepthMarketData->PreOpenInterest);
			///今开盘
			jsonData.put("OpenPrice", pDepthMarketData->OpenPrice);
			///最高价
			jsonData.put("HighestPrice", pDepthMarketData->HighestPrice);
			///最低价
			jsonData.put("LowestPrice", pDepthMarketData->LowestPrice);
			///数量
			jsonData.put("Volume", pDepthMarketData->Volume);
			///成交金额
			jsonData.put("Turnover", pDepthMarketData->Turnover);
			///持仓量
			jsonData.put("OpenInterest", pDepthMarketData->OpenInterest);
			///今收盘
			jsonData.put("ClosePrice", pDepthMarketData->ClosePrice);
			///本次结算价
			jsonData.put("SettlementPrice", pDepthMarketData->SettlementPrice);
			///涨停板价
			jsonData.put("UpperLimitPrice", pDepthMarketData->UpperLimitPrice);
			///跌停板价
			jsonData.put("LowerLimitPrice", pDepthMarketData->LowerLimitPrice);
			///昨虚实度
			jsonData.put("PreDelta", pDepthMarketData->PreDelta);
			///今虚实度
			jsonData.put("CurrDelta", pDepthMarketData->CurrDelta);
			///最后修改时间
			jsonData.put("UpdateTime", pDepthMarketData->UpdateTime);
			///最后修改毫秒
			jsonData.put("UpdateMillisec", pDepthMarketData->UpdateMillisec);
			///申买价一
			jsonData.put("BidPrice1", pDepthMarketData->BidPrice1);
			///申买量一
			jsonData.put("BidVolume1", pDepthMarketData->BidVolume1);
			///申卖价一
			jsonData.put("AskPrice1", pDepthMarketData->AskPrice1);
			///申卖量一
			jsonData.put("AskVolume1", pDepthMarketData->AskVolume1);
			///申买价二
			jsonData.put("BidPrice2", pDepthMarketData->BidPrice2);
			///申买量二
			jsonData.put("BidVolume2", pDepthMarketData->BidVolume2);
			///申卖价二
			jsonData.put("AskPrice2", pDepthMarketData->AskPrice2);
			///申卖量二
			jsonData.put("AskVolume2", pDepthMarketData->AskVolume2);
			///申买价三
			jsonData.put("BidPrice3", pDepthMarketData->BidPrice3);
			///申买量三
			jsonData.put("BidVolume3", pDepthMarketData->BidVolume3);
			///申卖价三
			jsonData.put("AskPrice3", pDepthMarketData->AskPrice3);
			///申卖量三
			jsonData.put("AskVolume3", pDepthMarketData->AskVolume3);
			///申买价四
			jsonData.put("BidPrice4", pDepthMarketData->BidPrice4);
			///申买量四
			jsonData.put("BidVolume4", pDepthMarketData->BidVolume4);
			///申卖价四
			jsonData.put("AskPrice4", pDepthMarketData->AskPrice4);
			///申卖量四
			jsonData.put("AskVolume4", pDepthMarketData->AskVolume4);
			///申买价五
			jsonData.put("BidPrice5", pDepthMarketData->BidPrice5);
			///申买量五
			jsonData.put("BidVolume5", pDepthMarketData->BidVolume5);
			///申卖价五
			jsonData.put("AskPrice5", pDepthMarketData->AskPrice5);
			///申卖量五
			jsonData.put("AskVolume5", pDepthMarketData->AskVolume5);
			///当日均价
			jsonData.put("AveragePrice", pDepthMarketData->AveragePrice);
			///业务日期
			jsonData.put("ActionDay", pDepthMarketData->ActionDay);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///请求查询投资者结算结果响应
void CTPApi::OnRspQrySettlementInfo(CThostFtdcSettlementInfoField *pSettlementInfo, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRspQrySettlementInfo", pRspInfo, nRequestID, bIsLast)) {
			//构造Data
			boost::property_tree::ptree jsonData;
			///交易日
			jsonData.put("TradingDay", pSettlementInfo->TradingDay);
			///结算编号
			jsonData.put("SettlementID", pSettlementInfo->SettlementID);
			//经纪公司代码
			jsonData.put("BrokerID", pSettlementInfo->BrokerID);
			///投资者代码
			jsonData.put("InvestorID", pSettlementInfo->InvestorID);
			///序号
			jsonData.put("SequenceNo", pSettlementInfo->SequenceNo);
			///消息正文
			jsonData.put("Content", pSettlementInfo->Content);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///请求查询转帐银行响应
void CTPApi::OnRspQryTransferBank(CThostFtdcTransferBankField *pTransferBank, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRspQryTransferBank", pRspInfo, nRequestID, bIsLast)) {
			//构造Data
			boost::property_tree::ptree jsonData;
			///银行代码
			jsonData.put("BankID", pTransferBank->BankID);
			///银行分中心代码
			jsonData.put("BankBrchID", pTransferBank->BankBrchID);
			///银行名称
			jsonData.put("BankName", pTransferBank->BankName);
			///是否活跃
			jsonData.put("IsActive", pTransferBank->IsActive);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///请求查询投资者持仓明细响应
void CTPApi::OnRspQryInvestorPositionDetail(CThostFtdcInvestorPositionDetailField *pInvestorPositionDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRspQryInvestorPositionDetail", pRspInfo, nRequestID, bIsLast)) {
			//构造Data
			boost::property_tree::ptree jsonData;
			///合约代码
			jsonData.put("InstrumentID", pInvestorPositionDetail->InstrumentID);
			//经纪公司代码
			jsonData.put("BrokerID", pInvestorPositionDetail->BrokerID);
			///投资者代码
			jsonData.put("InvestorID", pInvestorPositionDetail->InvestorID);
			///投机套保标志
			jsonData.put("HedgeFlag", pInvestorPositionDetail->HedgeFlag);
			///买卖
			jsonData.put("Direction", pInvestorPositionDetail->Direction);
			///开仓日期
			jsonData.put("OpenDate", pInvestorPositionDetail->OpenDate);
			///成交编号
			jsonData.put("TradeID", pInvestorPositionDetail->TradeID);
			///数量
			jsonData.put("Volume", pInvestorPositionDetail->Volume);
			///开仓价
			jsonData.put("OpenPrice", pInvestorPositionDetail->OpenPrice);
			///交易日
			jsonData.put("TradingDay", pInvestorPositionDetail->TradingDay);
			///结算编号
			jsonData.put("SettlementID", pInvestorPositionDetail->SettlementID);
			///成交类型
			jsonData.put("TradeType", pInvestorPositionDetail->TradeType);
			///组合合约代码
			jsonData.put("CombInstrumentID", pInvestorPositionDetail->CombInstrumentID);
			///交易所代码
			jsonData.put("ExchangeID", pInvestorPositionDetail->ExchangeID);
			///逐日盯市平仓盈亏
			jsonData.put("CloseProfitByDate", pInvestorPositionDetail->CloseProfitByDate);
			///逐笔对冲平仓盈亏
			jsonData.put("CloseProfitByTrade", pInvestorPositionDetail->CloseProfitByTrade);
			///逐日盯市持仓盈亏
			jsonData.put("PositionProfitByDate", pInvestorPositionDetail->PositionProfitByDate);
			///逐笔对冲持仓盈亏
			jsonData.put("PositionProfitByTrade", pInvestorPositionDetail->PositionProfitByTrade);
			///投资者保证金
			jsonData.put("Margin", pInvestorPositionDetail->Margin);
			///交易所保证金
			jsonData.put("ExchMargin", pInvestorPositionDetail->ExchMargin);
			///保证金率
			jsonData.put("MarginRateByMoney", pInvestorPositionDetail->MarginRateByMoney);
			///保证金率(按手数)
			jsonData.put("MarginRateByVolume", pInvestorPositionDetail->MarginRateByVolume);
			///昨结算价
			jsonData.put("LastSettlementPrice", pInvestorPositionDetail->LastSettlementPrice);
			///结算价
			jsonData.put("SettlementPrice", pInvestorPositionDetail->SettlementPrice);
			///平仓量
			jsonData.put("CloseVolume", pInvestorPositionDetail->CloseVolume);
			///平仓金额
			jsonData.put("CloseAmount", pInvestorPositionDetail->CloseAmount);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///请求查询客户通知响应
void CTPApi::OnRspQryNotice(CThostFtdcNoticeField *pNotice, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRspQryNotice", pRspInfo, nRequestID, bIsLast)) {
			//构造Data
			boost::property_tree::ptree jsonData;
			//经纪公司代码
			jsonData.put("BrokerID", pNotice->BrokerID);
			///消息正文
			jsonData.put("Content", pNotice->Content);
			///经纪公司通知内容序列号
			jsonData.put("SequenceLabel", pNotice->SequenceLabel);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///请求查询结算信息确认响应
void CTPApi::OnRspQrySettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRspQrySettlementInfoConfirm", pRspInfo, nRequestID, bIsLast)) {
			//构造Data
			boost::property_tree::ptree jsonData;
			if (pSettlementInfoConfirm) {
				///经纪公司代码
				jsonData.put("BrokerID", pSettlementInfoConfirm->BrokerID);
				///投资者代码
				jsonData.put("InvestorID", pSettlementInfoConfirm->InvestorID);
				///确认日期
				jsonData.put("ConfirmDate", pSettlementInfoConfirm->ConfirmDate);
				///确认时间
				jsonData.put("ConfirmTime", pSettlementInfoConfirm->ConfirmTime);
			}
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///请求查询投资者持仓明细响应
void CTPApi::OnRspQryInvestorPositionCombineDetail(CThostFtdcInvestorPositionCombineDetailField *pInvestorPositionCombineDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRspQryInvestorPositionCombineDetail", pRspInfo, nRequestID, bIsLast)) {
			//构造Data
			boost::property_tree::ptree jsonData;
			///交易日
			jsonData.put("TradingDay", pInvestorPositionCombineDetail->TradingDay);
			///开仓日期
			jsonData.put("OpenDate", pInvestorPositionCombineDetail->OpenDate);
			///交易所代码
			jsonData.put("ExchangeID", pInvestorPositionCombineDetail->ExchangeID);
			///结算编号
			jsonData.put("SettlementID", pInvestorPositionCombineDetail->ExchangeID);
			///经纪公司代码
			jsonData.put("BrokerID", pInvestorPositionCombineDetail->BrokerID);
			///投资者代码
			jsonData.put("InvestorID", pInvestorPositionCombineDetail->InvestorID);
			///组合编号
			jsonData.put("ComTradeID", pInvestorPositionCombineDetail->ComTradeID);
			///撮合编号
			jsonData.put("TradeID", pInvestorPositionCombineDetail->TradeID);
			///合约代码
			jsonData.put("InstrumentID", pInvestorPositionCombineDetail->InstrumentID);
			///投机套保标志
			jsonData.put("HedgeFlag", pInvestorPositionCombineDetail->HedgeFlag);
			///买卖
			jsonData.put("Direction", pInvestorPositionCombineDetail->Direction);
			///持仓量
			jsonData.put("TotalAmt", pInvestorPositionCombineDetail->TotalAmt);
			///投资者保证金
			jsonData.put("Margin", pInvestorPositionCombineDetail->Margin);
			///交易所保证金
			jsonData.put("ExchMargin", pInvestorPositionCombineDetail->ExchMargin);
			///保证金率
			jsonData.put("MarginRateByMoney", pInvestorPositionCombineDetail->MarginRateByMoney);
			///保证金率(按手数)
			jsonData.put("MarginRateByVolume", pInvestorPositionCombineDetail->MarginRateByVolume);
			///单腿编号
			jsonData.put("LegID", pInvestorPositionCombineDetail->LegID);
			///单腿乘数
			jsonData.put("LegMultiple", pInvestorPositionCombineDetail->LegMultiple);
			///组合持仓合约编码
			jsonData.put("CombInstrumentID", pInvestorPositionCombineDetail->CombInstrumentID);
			///成交组号
			jsonData.put("TradeGroupID", pInvestorPositionCombineDetail->TradeGroupID);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///查询保证金监管系统经纪公司资金账户密钥响应
void CTPApi::OnRspQryCFMMCTradingAccountKey(CThostFtdcCFMMCTradingAccountKeyField *pCFMMCTradingAccountKey, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRspQryCFMMCTradingAccountKey", pRspInfo, nRequestID, bIsLast)) {
			//构造Data
			boost::property_tree::ptree jsonData;
			///经纪公司代码
			jsonData.put("BrokerID", pCFMMCTradingAccountKey->BrokerID);
			///经纪公司统一编码
			jsonData.put("ParticipantID", pCFMMCTradingAccountKey->ParticipantID);
			///投资者帐号
			jsonData.put("AccountID", pCFMMCTradingAccountKey->AccountID);
			///密钥编号
			jsonData.put("KeyID", pCFMMCTradingAccountKey->KeyID);
			///动态密钥
			jsonData.put("CurrentKey", pCFMMCTradingAccountKey->CurrentKey);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///请求查询仓单折抵信息响应
void CTPApi::OnRspQryEWarrantOffset(CThostFtdcEWarrantOffsetField *pEWarrantOffset, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRspQryEWarrantOffset", pRspInfo, nRequestID, bIsLast)) {
			//构造Data
			boost::property_tree::ptree jsonData;
			///交易日期
			jsonData.put("TradingDay", pEWarrantOffset->TradingDay);
			///经纪公司代码
			jsonData.put("BrokerID", pEWarrantOffset->BrokerID);
			///投资者代码
			jsonData.put("InvestorID", pEWarrantOffset->InvestorID);
			///交易所代码
			jsonData.put("ExchangeID", pEWarrantOffset->ExchangeID);
			///合约代码
			jsonData.put("InstrumentID", pEWarrantOffset->InstrumentID);
			///买卖方向
			jsonData.put("Direction", pEWarrantOffset->Direction);
			///投机套保标志
			jsonData.put("HedgeFlag", pEWarrantOffset->HedgeFlag);
			///数量
			jsonData.put("Volume", pEWarrantOffset->Volume);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///请求查询投资者品种/跨品种保证金响应
void CTPApi::OnRspQryInvestorProductGroupMargin(CThostFtdcInvestorProductGroupMarginField *pInvestorProductGroupMargin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRspQryInvestorProductGroupMargin", pRspInfo, nRequestID, bIsLast)) {
			///构造Data
			boost::property_tree::ptree jsonData;
			///品种/跨品种标示
			TThostFtdcInstrumentIDType	ProductGroupID;
			///经纪公司代码
			jsonData.put("BrokerID", pInvestorProductGroupMargin->BrokerID);
			///投资者代码
			jsonData.put("InvestorID", pInvestorProductGroupMargin->InvestorID);
			///交易日
			TThostFtdcDateType	TradingDay;
			///结算编号
			TThostFtdcSettlementIDType	SettlementID;
			///冻结的保证金
			TThostFtdcMoneyType	FrozenMargin;
			///多头冻结的保证金
			TThostFtdcMoneyType	LongFrozenMargin;
			///空头冻结的保证金
			TThostFtdcMoneyType	ShortFrozenMargin;
			///占用的保证金
			TThostFtdcMoneyType	UseMargin;
			///多头保证金
			TThostFtdcMoneyType	LongUseMargin;
			///空头保证金
			TThostFtdcMoneyType	ShortUseMargin;
			///交易所保证金
			TThostFtdcMoneyType	ExchMargin;
			///交易所多头保证金
			TThostFtdcMoneyType	LongExchMargin;
			///交易所空头保证金
			TThostFtdcMoneyType	ShortExchMargin;
			///平仓盈亏
			TThostFtdcMoneyType	CloseProfit;
			///冻结的手续费
			TThostFtdcMoneyType	FrozenCommission;
			///手续费
			TThostFtdcMoneyType	Commission;
			///冻结的资金
			TThostFtdcMoneyType	FrozenCash;
			///资金差额
			TThostFtdcMoneyType	CashIn;
			///持仓盈亏
			TThostFtdcMoneyType	PositionProfit;
			///折抵总金额
			TThostFtdcMoneyType	OffsetAmount;
			///多头折抵总金额
			TThostFtdcMoneyType	LongOffsetAmount;
			///空头折抵总金额
			TThostFtdcMoneyType	ShortOffsetAmount;
			///交易所折抵总金额
			TThostFtdcMoneyType	ExchOffsetAmount;
			///交易所多头折抵总金额
			TThostFtdcMoneyType	LongExchOffsetAmount;
			///交易所空头折抵总金额
			TThostFtdcMoneyType	ShortExchOffsetAmount;
			///投机套保标志
			TThostFtdcHedgeFlagType	HedgeFlag;
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///请求查询交易所保证金率响应
void CTPApi::OnRspQryExchangeMarginRate(CThostFtdcExchangeMarginRateField *pExchangeMarginRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRspQryExchangeMarginRate", pRspInfo, nRequestID, bIsLast)) {
			//构造Data
			boost::property_tree::ptree jsonData;
			///经纪公司代码
			jsonData.put("BrokerID", pExchangeMarginRate->BrokerID);
			///合约代码
			jsonData.put("InstrumentID", pExchangeMarginRate->InstrumentID);
			///投机套保标志
			jsonData.put("HedgeFlag", pExchangeMarginRate->HedgeFlag);
			///多头保证金率
			jsonData.put("LongMarginRatioByMoney", pExchangeMarginRate->LongMarginRatioByMoney);
			///多头保证金费
			jsonData.put("LongMarginRatioByVolume", pExchangeMarginRate->LongMarginRatioByVolume);
			///空头保证金率
			jsonData.put("ShortMarginRatioByMoney", pExchangeMarginRate->ShortMarginRatioByMoney);
			///空头保证金费
			jsonData.put("ShortMarginRatioByVolume", pExchangeMarginRate->ShortMarginRatioByVolume);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///请求查询交易所调整保证金率响应
void CTPApi::OnRspQryExchangeMarginRateAdjust(CThostFtdcExchangeMarginRateAdjustField *pExchangeMarginRateAdjust, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRspQryExchangeMarginRateAdjust", pRspInfo, nRequestID, bIsLast)) {
			//构造Data
			boost::property_tree::ptree jsonData;
			///经纪公司代码
			jsonData.put("BrokerID", pExchangeMarginRateAdjust->BrokerID);
			///合约代码
			TThostFtdcInstrumentIDType	InstrumentID;
			///投机套保标志
			TThostFtdcHedgeFlagType	HedgeFlag;
			///跟随交易所投资者多头保证金率
			TThostFtdcRatioType	LongMarginRatioByMoney;
			///跟随交易所投资者多头保证金费
			TThostFtdcMoneyType	LongMarginRatioByVolume;
			///跟随交易所投资者空头保证金率
			TThostFtdcRatioType	ShortMarginRatioByMoney;
			///跟随交易所投资者空头保证金费
			TThostFtdcMoneyType	ShortMarginRatioByVolume;
			///交易所多头保证金率
			TThostFtdcRatioType	ExchLongMarginRatioByMoney;
			///交易所多头保证金费
			TThostFtdcMoneyType	ExchLongMarginRatioByVolume;
			///交易所空头保证金率
			TThostFtdcRatioType	ExchShortMarginRatioByMoney;
			///交易所空头保证金费
			TThostFtdcMoneyType	ExchShortMarginRatioByVolume;
			///不跟随交易所投资者多头保证金率
			TThostFtdcRatioType	NoLongMarginRatioByMoney;
			///不跟随交易所投资者多头保证金费
			TThostFtdcMoneyType	NoLongMarginRatioByVolume;
			///不跟随交易所投资者空头保证金率
			TThostFtdcRatioType	NoShortMarginRatioByMoney;
			///不跟随交易所投资者空头保证金费
			TThostFtdcMoneyType	NoShortMarginRatioByVolume;
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///请求查询汇率响应
void CTPApi::OnRspQryExchangeRate(CThostFtdcExchangeRateField *pExchangeRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRspQryExchangeRate", pRspInfo, nRequestID, bIsLast)) {
			//构造Data
			boost::property_tree::ptree jsonData;
			///经纪公司代码
			jsonData.put("BrokerID", pExchangeRate->BrokerID);
			///源币种
			TThostFtdcCurrencyIDType	FromCurrencyID;
			///源币种单位数量
			TThostFtdcCurrencyUnitType	FromCurrencyUnit;
			///目标币种
			TThostFtdcCurrencyIDType	ToCurrencyID;
			///汇率
			TThostFtdcExchangeRateType	ExchangeRate;
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///请求查询二级代理操作员银期权限响应
void CTPApi::OnRspQrySecAgentACIDMap(CThostFtdcSecAgentACIDMapField *pSecAgentACIDMap, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRspQrySecAgentACIDMap", pRspInfo, nRequestID, bIsLast)) {
			//构造Data
			boost::property_tree::ptree jsonData;
			///经纪公司代码
			jsonData.put("BrokerID", pSecAgentACIDMap->BrokerID);
			///用户代码
			TThostFtdcUserIDType	UserID;
			///资金账户
			TThostFtdcAccountIDType	AccountID;
			///币种
			TThostFtdcCurrencyIDType	CurrencyID;
			///境外中介机构资金帐号
			TThostFtdcAccountIDType	BrokerSecAgentID;
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///请求查询产品报价汇率
void CTPApi::OnRspQryProductExchRate(CThostFtdcProductExchRateField *pProductExchRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRspQryProductExchRate", pRspInfo, nRequestID, bIsLast)) {
			//构造Data
			boost::property_tree::ptree jsonData;
			///产品代码
			TThostFtdcInstrumentIDType	ProductID;
			///报价币种类型
			TThostFtdcCurrencyIDType	QuoteCurrencyID;
			///汇率
			TThostFtdcExchangeRateType	ExchangeRate;
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///请求查询产品组
void CTPApi::OnRspQryProductGroup(CThostFtdcProductGroupField *pProductGroup, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRspQryProductGroup", pRspInfo, nRequestID, bIsLast)) {
			//构造Data
			boost::property_tree::ptree jsonData;
			///产品代码
			TThostFtdcInstrumentIDType	ProductID;
			///交易所代码
			TThostFtdcExchangeIDType	ExchangeID;
			///产品组代码
			TThostFtdcInstrumentIDType	ProductGroupID;
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///请求查询做市商合约手续费率响应
void CTPApi::OnRspQryMMInstrumentCommissionRate(CThostFtdcMMInstrumentCommissionRateField *pMMInstrumentCommissionRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRspQryMMInstrumentCommissionRate", pRspInfo, nRequestID, bIsLast)) {
			//构造Data
			boost::property_tree::ptree jsonData;
			///合约代码
			TThostFtdcInstrumentIDType	InstrumentID;
			///投资者范围
			TThostFtdcInvestorRangeType	InvestorRange;
			///经纪公司代码
			jsonData.put("BrokerID", pMMInstrumentCommissionRate->BrokerID);
			///投资者代码
			jsonData.put("InvestorID", pMMInstrumentCommissionRate->InvestorID);
			///开仓手续费率
			TThostFtdcRatioType	OpenRatioByMoney;
			///开仓手续费
			TThostFtdcRatioType	OpenRatioByVolume;
			///平仓手续费率
			TThostFtdcRatioType	CloseRatioByMoney;
			///平仓手续费
			TThostFtdcRatioType	CloseRatioByVolume;
			///平今手续费率
			TThostFtdcRatioType	CloseTodayRatioByMoney;
			///平今手续费
			TThostFtdcRatioType	CloseTodayRatioByVolume;
			///交易所代码
			TThostFtdcExchangeIDType	ExchangeID;
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///请求查询做市商期权合约手续费响应
void CTPApi::OnRspQryMMOptionInstrCommRate(CThostFtdcMMOptionInstrCommRateField *pMMOptionInstrCommRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRspQryMMOptionInstrCommRate", pRspInfo, nRequestID, bIsLast)) {
			//构造Data
			boost::property_tree::ptree jsonData;
			///合约代码
			TThostFtdcInstrumentIDType	InstrumentID;
			///投资者范围
			TThostFtdcInvestorRangeType	InvestorRange;
			///经纪公司代码
			jsonData.put("BrokerID", pMMOptionInstrCommRate->BrokerID);
			///投资者代码
			jsonData.put("InvestorID", pMMOptionInstrCommRate->InvestorID);
			///开仓手续费率
			TThostFtdcRatioType	OpenRatioByMoney;
			///开仓手续费
			TThostFtdcRatioType	OpenRatioByVolume;
			///平仓手续费率
			TThostFtdcRatioType	CloseRatioByMoney;
			///平仓手续费
			TThostFtdcRatioType	CloseRatioByVolume;
			///平今手续费率
			TThostFtdcRatioType	CloseTodayRatioByMoney;
			///平今手续费
			TThostFtdcRatioType	CloseTodayRatioByVolume;
			///执行手续费率
			TThostFtdcRatioType	StrikeRatioByMoney;
			///执行手续费
			TThostFtdcRatioType	StrikeRatioByVolume;
			///交易所代码
			TThostFtdcExchangeIDType	ExchangeID;
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///请求查询报单手续费响应
void CTPApi::OnRspQryInstrumentOrderCommRate(CThostFtdcInstrumentOrderCommRateField *pInstrumentOrderCommRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRspQryInstrumentOrderCommRate", pRspInfo, nRequestID, bIsLast)) {
			//构造Data
			boost::property_tree::ptree jsonData;
			///合约代码
			TThostFtdcInstrumentIDType	InstrumentID;
			///投资者范围
			TThostFtdcInvestorRangeType	InvestorRange;
			///经纪公司代码
			jsonData.put("BrokerID", pInstrumentOrderCommRate->BrokerID);
			///投资者代码
			jsonData.put("InvestorID", pInstrumentOrderCommRate->InvestorID);
			///交易所代码
			TThostFtdcExchangeIDType	ExchangeID;
			///投机套保标志
			TThostFtdcHedgeFlagType	HedgeFlag;
			///报单手续费
			TThostFtdcRatioType	OrderCommByVolume;
			///撤单手续费
			TThostFtdcRatioType	OrderActionCommByVolume;
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///请求查询期权交易成本响应
void CTPApi::OnRspQryOptionInstrTradeCost(CThostFtdcOptionInstrTradeCostField *pOptionInstrTradeCost, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRspQryOptionInstrTradeCost", pRspInfo, nRequestID, bIsLast)) {
			//构造Data
			boost::property_tree::ptree jsonData;
			///经纪公司代码
			jsonData.put("BrokerID", pOptionInstrTradeCost->BrokerID);
			///投资者代码
			jsonData.put("InvestorID", pOptionInstrTradeCost->InvestorID);
			///合约代码
			TThostFtdcInstrumentIDType	InstrumentID;
			///投机套保标志
			TThostFtdcHedgeFlagType	HedgeFlag;
			///期权合约保证金不变部分
			TThostFtdcMoneyType	FixedMargin;
			///期权合约最小保证金
			TThostFtdcMoneyType	MiniMargin;
			///期权合约权利金
			TThostFtdcMoneyType	Royalty;
			///交易所期权合约保证金不变部分
			TThostFtdcMoneyType	ExchFixedMargin;
			///交易所期权合约最小保证金
			TThostFtdcMoneyType	ExchMiniMargin;
			///交易所代码
			TThostFtdcExchangeIDType	ExchangeID;
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///请求查询期权合约手续费响应
void CTPApi::OnRspQryOptionInstrCommRate(CThostFtdcOptionInstrCommRateField *pOptionInstrCommRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRspQryOptionInstrCommRate", pRspInfo, nRequestID, bIsLast)) {
			//构造Data
			boost::property_tree::ptree jsonData;
			///合约代码
			TThostFtdcInstrumentIDType	InstrumentID;
			///投资者范围
			TThostFtdcInvestorRangeType	InvestorRange;
			///经纪公司代码
			jsonData.put("BrokerID", pOptionInstrCommRate->BrokerID);
			///投资者代码
			jsonData.put("InvestorID", pOptionInstrCommRate->InvestorID);
			///开仓手续费率
			TThostFtdcRatioType	OpenRatioByMoney;
			///开仓手续费
			TThostFtdcRatioType	OpenRatioByVolume;
			///平仓手续费率
			TThostFtdcRatioType	CloseRatioByMoney;
			///平仓手续费
			TThostFtdcRatioType	CloseRatioByVolume;
			///平今手续费率
			TThostFtdcRatioType	CloseTodayRatioByMoney;
			///平今手续费
			TThostFtdcRatioType	CloseTodayRatioByVolume;
			///执行手续费率
			TThostFtdcRatioType	StrikeRatioByMoney;
			///执行手续费
			TThostFtdcRatioType	StrikeRatioByVolume;
			///交易所代码
			TThostFtdcExchangeIDType	ExchangeID;
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///请求查询执行宣告响应
void CTPApi::OnRspQryExecOrder(CThostFtdcExecOrderField *pExecOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRspQryExecOrder", pRspInfo, nRequestID, bIsLast)) {
			//构造Data
			boost::property_tree::ptree jsonData;

			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///请求查询询价响应
void CTPApi::OnRspQryForQuote(CThostFtdcForQuoteField *pForQuote, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRspQryForQuote", pRspInfo, nRequestID, bIsLast)) {
			//构造Data
			boost::property_tree::ptree jsonData;

			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///请求查询报价响应
void CTPApi::OnRspQryQuote(CThostFtdcQuoteField *pQuote, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRspQryQuote", pRspInfo, nRequestID, bIsLast)) {
			//构造Data
			boost::property_tree::ptree jsonData;

			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///请求查询锁定应答
void CTPApi::OnRspQryLock(CThostFtdcLockField *pLock, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRspQryLock", pRspInfo, nRequestID, bIsLast)) {
			//构造Data
			boost::property_tree::ptree jsonData;

			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///请求查询锁定证券仓位应答
void CTPApi::OnRspQryLockPosition(CThostFtdcLockPositionField *pLockPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRspQryLockPosition", pRspInfo, nRequestID, bIsLast)) {
			//构造Data
			boost::property_tree::ptree jsonData;

			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///请求查询ETF期权合约手续费响应
void CTPApi::OnRspQryETFOptionInstrCommRate(CThostFtdcETFOptionInstrCommRateField *pETFOptionInstrCommRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRspQryETFOptionInstrCommRate", pRspInfo, nRequestID, bIsLast)) {
			//构造Data
			boost::property_tree::ptree jsonData;

			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///请求查询投资者分级
void CTPApi::OnRspQryInvestorLevel(CThostFtdcInvestorLevelField *pInvestorLevel, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRspQryInvestorLevel", pRspInfo, nRequestID, bIsLast)) {
			//构造Data
			boost::property_tree::ptree jsonData;
			///经纪公司代码
			jsonData.put("BrokerID", pInvestorLevel->BrokerID);
			///投资者代码
			jsonData.put("InvestorID", pInvestorLevel->InvestorID);
			///交易所代码
			jsonData.put("ExchangeID", pInvestorLevel->ExchangeID);
			///投资者分级类型
			jsonData.put("LevelType", pInvestorLevel->LevelType);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///请求查询E+1日行权冻结响应
void CTPApi::OnRspQryExecFreeze(CThostFtdcExecFreezeField *pExecFreeze, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRspQryExecFreeze", pRspInfo, nRequestID, bIsLast)) {
			//构造Data
			boost::property_tree::ptree jsonData;

			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///请求查询组合合约安全系数响应
void CTPApi::OnRspQryCombInstrumentGuard(CThostFtdcCombInstrumentGuardField *pCombInstrumentGuard, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRspQryCombInstrumentGuard", pRspInfo, nRequestID, bIsLast)) {
			//构造Data
			boost::property_tree::ptree jsonData;

			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///请求查询申请组合响应
void CTPApi::OnRspQryCombAction(CThostFtdcCombActionField *pCombAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {

	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRspQryCombAction", pRspInfo, nRequestID, bIsLast)) {
			//构造Data
			boost::property_tree::ptree jsonData;

			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///请求查询转帐流水响应
void CTPApi::OnRspQryTransferSerial(CThostFtdcTransferSerialField *pTransferSerial, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRspQryTransferSerial", pRspInfo, nRequestID, bIsLast)) {
			//构造Data
			boost::property_tree::ptree jsonData;

			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///请求查询银期签约关系响应
void CTPApi::OnRspQryAccountregister(CThostFtdcAccountregisterField *pAccountregister, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRspQryAccountregister", pRspInfo, nRequestID, bIsLast)) {
			//构造Data
			boost::property_tree::ptree jsonData;

			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///错误应答
void CTPApi::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRspError", pRspInfo, nRequestID, bIsLast)) {
			//构造Data
			boost::property_tree::ptree jsonData;

			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///报单通知
void CTPApi::OnRtnOrder(CThostFtdcOrderField *pOrder) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRtnOrder")) {
			//构造Data
			boost::property_tree::ptree jsonData;
			///经纪公司代码
			jsonData.put("BrokerID", pOrder->BrokerID);
			///投资者代码
			jsonData.put("InvestorID", pOrder->InvestorID);
			///合约代码
			jsonData.put("InstrumentID", pOrder->InstrumentID);
			///报单引用
			jsonData.put("OrderRef", pOrder->OrderRef);
			///用户代码
			jsonData.put("UserID", pOrder->UserID);
			///报单价格条件
			jsonData.put("OrderPriceType", pOrder->OrderPriceType);
			///买卖方向
			jsonData.put("Direction", pOrder->Direction);
			///组合开平标志
			jsonData.put("CombOffsetFlag", pOrder->CombOffsetFlag);
			///组合投机套保标志
			jsonData.put("CombHedgeFlag", pOrder->CombHedgeFlag);
			///价格
			jsonData.put("LimitPrice", pOrder->LimitPrice);
			///数量
			jsonData.put("VolumeTotalOriginal", pOrder->VolumeTotalOriginal);
			///有效期类型
			jsonData.put("TimeCondition", pOrder->TimeCondition);
			///GTD日期
			jsonData.put("GTDDate", pOrder->GTDDate);
			///成交量类型
			jsonData.put("VolumeCondition", pOrder->VolumeCondition);
			///最小成交量
			jsonData.put("MinVolume", pOrder->MinVolume);
			///触发条件
			jsonData.put("ContingentCondition", pOrder->ContingentCondition);
			///止损价
			jsonData.put("StopPrice", pOrder->StopPrice);
			///强平原因
			jsonData.put("ForceCloseReason", pOrder->ForceCloseReason);
			///自动挂起标志
			jsonData.put("IsAutoSuspend", pOrder->IsAutoSuspend);
			///业务单元
			jsonData.put("BusinessUnit", pOrder->BusinessUnit);
			///请求编号
			jsonData.put("RequestID", pOrder->RequestID);
			///本地报单编号
			jsonData.put("OrderLocalID", pOrder->OrderLocalID);
			///交易所代码
			jsonData.put("ExchangeID", pOrder->ExchangeID);
			///会员代码
			jsonData.put("ParticipantID", pOrder->ParticipantID);
			///客户代码
			jsonData.put("ClientID", pOrder->ClientID);
			///合约在交易所的代码
			jsonData.put("ExchangeInstID", pOrder->ExchangeInstID);
			///交易所交易员代码
			jsonData.put("TraderID", pOrder->TraderID);
			///安装编号
			jsonData.put("InstallID", pOrder->InstallID);
			///报单提交状态
			jsonData.put("OrderSubmitStatus", pOrder->OrderSubmitStatus);
			///报单提示序号
			jsonData.put("NotifySequence", pOrder->NotifySequence);
			///交易日
			jsonData.put("TradingDay", pOrder->TradingDay);
			///结算编号
			jsonData.put("SettlementID", pOrder->SettlementID);
			///报单编号
			jsonData.put("OrderSysID", pOrder->OrderSysID);
			///报单来源
			jsonData.put("OrderSource", pOrder->OrderSource);
			///报单状态
			jsonData.put("OrderStatus", pOrder->OrderStatus);
			///报单类型
			jsonData.put("OrderType", pOrder->OrderType);
			///今成交数量
			jsonData.put("VolumeTraded", pOrder->VolumeTraded);
			///剩余数量
			jsonData.put("VolumeTotal", pOrder->VolumeTotal);
			///报单日期
			jsonData.put("InsertDate", pOrder->InsertDate);
			///委托时间
			jsonData.put("InsertTime", pOrder->InsertTime);
			///激活时间
			jsonData.put("ActiveTime", pOrder->ActiveTime);
			///挂起时间
			jsonData.put("SuspendTime", pOrder->SuspendTime);
			///最后修改时间
			jsonData.put("UpdateTime", pOrder->UpdateTime);
			///撤销时间
			jsonData.put("CancelTime", pOrder->CancelTime);
			///最后修改交易所交易员代码
			jsonData.put("ActiveTraderID", pOrder->ActiveTraderID);
			///结算会员编号
			jsonData.put("ClearingPartID", pOrder->ClearingPartID);
			///序号
			jsonData.put("SequenceNo", pOrder->SequenceNo);
			///前置编号
			jsonData.put("FrontID", pOrder->FrontID);
			///会话编号
			jsonData.put("SessionID", pOrder->SessionID);
			///用户端产品信息
			jsonData.put("UserProductInfo", pOrder->UserProductInfo);
			///状态信息
			jsonData.put("StatusMsg", pOrder->StatusMsg);
			///用户强评标志
			jsonData.put("UserForceClose", pOrder->UserForceClose);
			///操作用户代码
			jsonData.put("ActiveUserID", pOrder->ActiveUserID);
			///经纪公司报单编号
			jsonData.put("BrokerOrderSeq", pOrder->BrokerOrderSeq);
			///相关报单
			jsonData.put("RelativeOrderSysID", pOrder->RelativeOrderSysID);
			///郑商所成交数量
			jsonData.put("ZCETotalTradedVolume", pOrder->ZCETotalTradedVolume);
			///互换单标志
			jsonData.put("IsSwapOrder", pOrder->IsSwapOrder);
			///营业部编号
			jsonData.put("BranchID", pOrder->BranchID);
			///投资单元代码
			jsonData.put("InvestUnitID", pOrder->InvestUnitID);
			///资金账号
			jsonData.put("AccountID", pOrder->AccountID);
			///币种代码
			jsonData.put("CurrencyID", pOrder->CurrencyID);
			///IP地址
			jsonData.put("IPAddress", pOrder->IPAddress);
			///Mac地址
			jsonData.put("MacAddress", pOrder->MacAddress);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///成交通知
void CTPApi::OnRtnTrade(CThostFtdcTradeField *pTrade) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRtnTrade")) {
			//构造Data
			boost::property_tree::ptree jsonData;
			///经纪公司代码
			jsonData.put("BrokerID", pTrade->BrokerID);
			///投资者代码
			jsonData.put("InvestorID", pTrade->InvestorID);
			///合约代码
			jsonData.put("InstrumentID", pTrade->InstrumentID);
			///报单引用
			jsonData.put("OrderRef", pTrade->OrderRef);
			///用户代码
			jsonData.put("UserID", pTrade->UserID);
			///交易所代码
			jsonData.put("ExchangeID", pTrade->ExchangeID);
			///成交编号
			jsonData.put("TradeID", pTrade->TradeID);
			///买卖方向
			jsonData.put("Direction", pTrade->Direction);
			///报单编号
			jsonData.put("OrderSysID", pTrade->OrderSysID);
			///会员代码
			jsonData.put("ParticipantID", pTrade->ParticipantID);
			///客户代码
			jsonData.put("ClientID", pTrade->ClientID);
			///交易角色
			jsonData.put("TradingRole", pTrade->TradingRole);
			///合约在交易所的代码
			jsonData.put("ExchangeInstID", pTrade->ExchangeInstID);
			///开平标志
			jsonData.put("OffsetFlag", pTrade->OffsetFlag);
			///投机套保标志
			jsonData.put("HedgeFlag", pTrade->HedgeFlag);
			///价格
			jsonData.put("Price", pTrade->Price);
			///数量
			jsonData.put("Volume", pTrade->Volume);
			///成交时期
			jsonData.put("TradeDate", pTrade->TradeDate);
			///成交时间
			jsonData.put("TradeTime", pTrade->TradeTime);
			///成交类型
			jsonData.put("TradeType", pTrade->TradeType);
			///成交价来源
			jsonData.put("PriceSource", pTrade->PriceSource);
			///交易所交易员代码
			jsonData.put("TraderID", pTrade->TraderID);
			///本地报单编号
			jsonData.put("OrderLocalID", pTrade->OrderLocalID);
			///结算会员编号
			jsonData.put("ClearingPartID", pTrade->ClearingPartID);
			///业务单元
			jsonData.put("BusinessUnit", pTrade->BusinessUnit);
			///序号
			jsonData.put("SequenceNo", pTrade->SequenceNo);
			///交易日
			jsonData.put("TradingDay", pTrade->TradingDay);
			///结算编号
			jsonData.put("SettlementID", pTrade->SettlementID);
			///经纪公司报单编号
			jsonData.put("BrokerOrderSeq", pTrade->BrokerOrderSeq);
			///成交来源
			jsonData.put("TradeSource", pTrade->TradeSource);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///报单录入错误回报
void CTPApi::OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnErrRtnOrderInsert", pRspInfo)) {
			//构造Data
			boost::property_tree::ptree jsonData;
			///经纪公司代码
			jsonData.put("BrokerID", pInputOrder->BrokerID);
			///投资者代码
			jsonData.put("InvestorID", pInputOrder->InvestorID);
			///合约代码
			jsonData.put("InstrumentID", pInputOrder->InstrumentID);
			///报单引用
			jsonData.put("OrderRef", pInputOrder->OrderRef);
			///用户代码
			jsonData.put("UserID", pInputOrder->UserID);
			///报单价格条件
			jsonData.put("OrderPriceType", pInputOrder->OrderPriceType);
			///买卖方向
			jsonData.put("Direction", pInputOrder->Direction);
			///组合开平标志
			jsonData.put("CombOffsetFlag", pInputOrder->CombOffsetFlag);
			///组合投机套保标志
			jsonData.put("CombHedgeFlag", pInputOrder->CombHedgeFlag);
			///价格
			jsonData.put("LimitPrice", pInputOrder->LimitPrice);
			///数量
			jsonData.put("VolumeTotalOriginal", pInputOrder->VolumeTotalOriginal);
			///有效期类型
			jsonData.put("TimeCondition", pInputOrder->TimeCondition);
			///GTD日期
			jsonData.put("GTDDate", pInputOrder->GTDDate);
			///成交量类型
			jsonData.put("VolumeCondition", pInputOrder->VolumeCondition);
			///最小成交量
			jsonData.put("MinVolume", pInputOrder->MinVolume);
			///触发条件
			jsonData.put("ContingentCondition", pInputOrder->ContingentCondition);
			///止损价
			jsonData.put("StopPrice", pInputOrder->StopPrice);
			///强平原因
			jsonData.put("ForceCloseReason", pInputOrder->ForceCloseReason);
			///自动挂起标志
			jsonData.put("IsAutoSuspend", pInputOrder->IsAutoSuspend);
			///业务单元
			jsonData.put("BusinessUnit", pInputOrder->BusinessUnit);
			///请求编号
			jsonData.put("RequestID", pInputOrder->RequestID);
			///用户强评标志
			jsonData.put("UserForceClose", pInputOrder->UserForceClose);
			///互换单标志
			jsonData.put("IsSwapOrder", pInputOrder->IsSwapOrder);
			///交易所代码
			jsonData.put("ExchangeID", pInputOrder->ExchangeID);
			///投资单元代码
			jsonData.put("InvestUnitID", pInputOrder->InvestUnitID);
			///资金账号
			jsonData.put("AccountID", pInputOrder->AccountID);
			///币种代码
			jsonData.put("CurrencyID", pInputOrder->CurrencyID);
			///交易编码
			jsonData.put("ClientID", pInputOrder->ClientID);
			///IP地址
			jsonData.put("IPAddress", pInputOrder->IPAddress);
			///Mac地址
			jsonData.put("MacAddress", pInputOrder->MacAddress);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///报单操作错误回报
void CTPApi::OnErrRtnOrderAction(CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnErrRtnOrderAction", pRspInfo)) {
			//构造Data
			boost::property_tree::ptree jsonData;
			///经纪公司代码
			jsonData.put("BrokerID", pOrderAction->BrokerID);
			///投资者代码
			jsonData.put("InvestorID", pOrderAction->InvestorID);
			///报单操作引用
			jsonData.put("OrderActionRef", pOrderAction->OrderActionRef);
			///报单引用
			jsonData.put("OrderRef", pOrderAction->OrderRef);
			///请求编号
			jsonData.put("RequestID", pOrderAction->RequestID);
			///前置编号
			jsonData.put("FrontID", pOrderAction->FrontID);
			///会话编号
			jsonData.put("SessionID", pOrderAction->SessionID);
			///交易所代码
			jsonData.put("ExchangeID", pOrderAction->ExchangeID);
			///报单编号
			jsonData.put("OrderSysID", pOrderAction->OrderSysID);
			///操作标志
			jsonData.put("ActionFlag", pOrderAction->ActionFlag);
			///价格
			jsonData.put("LimitPrice", pOrderAction->LimitPrice);
			///数量变化
			jsonData.put("VolumeChange", pOrderAction->VolumeChange);
			///操作日期
			jsonData.put("ActionDate", pOrderAction->ActionDate);
			///操作时间
			jsonData.put("ActionTime", pOrderAction->ActionTime);
			///交易所交易员代码
			jsonData.put("TraderID", pOrderAction->TraderID);
			///安装编号
			jsonData.put("InstallID", pOrderAction->InstallID);
			///本地报单编号
			jsonData.put("OrderLocalID", pOrderAction->OrderLocalID);
			///操作本地编号
			jsonData.put("ActionLocalID", pOrderAction->ActionLocalID);
			///会员代码
			jsonData.put("ParticipantID", pOrderAction->ParticipantID);
			///客户代码
			jsonData.put("ClientID", pOrderAction->ClientID);
			///业务单元
			jsonData.put("BusinessUnit", pOrderAction->BusinessUnit);
			///报单操作状态
			jsonData.put("OrderActionStatus", pOrderAction->OrderActionStatus);
			///用户代码
			jsonData.put("UserID", pOrderAction->UserID);
			///状态信息
			jsonData.put("StatusMsg", pOrderAction->StatusMsg);
			///合约代码
			jsonData.put("InstrumentID", pOrderAction->InstrumentID);
			///营业部编号
			jsonData.put("BranchID", pOrderAction->BranchID);
			///投资单元代码
			jsonData.put("InvestUnitID", pOrderAction->InvestUnitID);
			///IP地址
			jsonData.put("IPAddress", pOrderAction->IPAddress);
			///Mac地址
			jsonData.put("MacAddress", pOrderAction->MacAddress);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///合约交易状态通知
void CTPApi::OnRtnInstrumentStatus(CThostFtdcInstrumentStatusField *pInstrumentStatus) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRtnInstrumentStatus")) {
			//构造Data
			boost::property_tree::ptree jsonData;
			///交易所代码
			jsonData.put("ExchangeID", pInstrumentStatus->ExchangeID);
			///合约在交易所的代码
			jsonData.put("ExchangeInstID", pInstrumentStatus->ExchangeInstID);
			///结算组代码
			jsonData.put("SettlementGroupID", pInstrumentStatus->SettlementGroupID);
			///合约代码
			jsonData.put("InstrumentID", pInstrumentStatus->InstrumentID);
			///合约交易状态
			jsonData.put("InstrumentStatus", pInstrumentStatus->InstrumentStatus);
			///交易阶段编号
			jsonData.put("TradingSegmentSN", pInstrumentStatus->TradingSegmentSN);
			///进入本状态时间
			jsonData.put("EnterTime", pInstrumentStatus->EnterTime);
			///进入本状态原因
			jsonData.put("EnterReason", pInstrumentStatus->EnterReason);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///交易所公告通知
void CTPApi::OnRtnBulletin(CThostFtdcBulletinField *pBulletin) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRtnBulletin")) {
			//构造Data
			boost::property_tree::ptree jsonData;
			///交易所代码
			jsonData.put("ExchangeID", pBulletin->ExchangeID);
			///交易日
			jsonData.put("TradingDay", pBulletin->TradingDay);
			///公告编号
			jsonData.put("BulletinID", pBulletin->BulletinID);
			///序列号
			jsonData.put("SequenceNo", pBulletin->SequenceNo);
			///公告类型
			jsonData.put("NewsType", pBulletin->NewsType);
			///紧急程度
			jsonData.put("NewsUrgency", pBulletin->NewsUrgency);
			///发送时间
			jsonData.put("SendTime", pBulletin->SendTime);
			///消息摘要
			jsonData.put("Abstract", pBulletin->Abstract);
			///消息来源
			jsonData.put("ComeFrom", pBulletin->ComeFrom);
			///消息正文
			jsonData.put("Content", pBulletin->Content);
			///WEB地址
			jsonData.put("URLLink", pBulletin->URLLink);
			///市场代码
			jsonData.put("MarketID", pBulletin->MarketID);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///交易通知
void CTPApi::OnRtnTradingNotice(CThostFtdcTradingNoticeInfoField *pTradingNoticeInfo) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRtnTradingNotice")) {
			//构造Data
			boost::property_tree::ptree jsonData;
			//经纪公司代码
			jsonData.put("BrokerID", pTradingNoticeInfo->BrokerID);
			///投资者代码
			jsonData.put("InvestorID", pTradingNoticeInfo->InvestorID);
			///发送时间
			jsonData.put("SendTime", pTradingNoticeInfo->SendTime);
			///消息正文
			jsonData.put("FieldContent", pTradingNoticeInfo->FieldContent);
			///序列系列号
			jsonData.put("SequenceSeries", pTradingNoticeInfo->SequenceSeries);
			///序列号
			jsonData.put("SequenceNo", pTradingNoticeInfo->SequenceNo);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///提示条件单校验错误
void CTPApi::OnRtnErrorConditionalOrder(CThostFtdcErrorConditionalOrderField *pErrorConditionalOrder) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRtnErrorConditionalOrder")) {
			//构造Data
			boost::property_tree::ptree jsonData;
			//经纪公司代码
			jsonData.put("BrokerID", pErrorConditionalOrder->BrokerID);
			///投资者代码
			jsonData.put("InvestorID", pErrorConditionalOrder->InvestorID);
			///合约代码
			jsonData.put("InstrumentID", pErrorConditionalOrder->InstrumentID);
			///报单引用
			jsonData.put("OrderRef", pErrorConditionalOrder->OrderRef);
			///用户代码
			jsonData.put("UserID", pErrorConditionalOrder->UserID);
			///报单价格条件
			jsonData.put("OrderPriceType", pErrorConditionalOrder->OrderPriceType);
			///买卖方向
			jsonData.put("Direction", pErrorConditionalOrder->Direction);
			///组合开平标志
			jsonData.put("CombOffsetFlag", pErrorConditionalOrder->CombOffsetFlag);
			///组合投机套保标志
			jsonData.put("CombHedgeFlag", pErrorConditionalOrder->CombHedgeFlag);
			///价格
			jsonData.put("LimitPrice", pErrorConditionalOrder->LimitPrice);
			///数量
			jsonData.put("VolumeTotalOriginal", pErrorConditionalOrder->VolumeTotalOriginal);
			///有效期类型
			jsonData.put("TimeCondition", pErrorConditionalOrder->TimeCondition);
			///GTD日期
			jsonData.put("GTDDate", pErrorConditionalOrder->GTDDate);
			///成交量类型
			jsonData.put("VolumeCondition", pErrorConditionalOrder->VolumeCondition);
			///最小成交量
			jsonData.put("MinVolume", pErrorConditionalOrder->MinVolume);
			///触发条件
			jsonData.put("ContingentCondition", pErrorConditionalOrder->ContingentCondition);
			///止损价
			jsonData.put("StopPrice", pErrorConditionalOrder->StopPrice);
			///强平原因
			jsonData.put("ForceCloseReason", pErrorConditionalOrder->ForceCloseReason);
			///自动挂起标志
			jsonData.put("IsAutoSuspend", pErrorConditionalOrder->IsAutoSuspend);
			///业务单元
			jsonData.put("BusinessUnit", pErrorConditionalOrder->BusinessUnit);
			///请求编号
			jsonData.put("RequestID", pErrorConditionalOrder->RequestID);
			///本地报单编号
			jsonData.put("OrderLocalID", pErrorConditionalOrder->OrderLocalID);
			///交易所代码
			jsonData.put("ExchangeID", pErrorConditionalOrder->ExchangeID);
			///会员代码
			jsonData.put("ParticipantID", pErrorConditionalOrder->ParticipantID);
			///客户代码
			jsonData.put("ClientID", pErrorConditionalOrder->ClientID);
			///合约在交易所的代码
			jsonData.put("ExchangeInstID", pErrorConditionalOrder->ExchangeInstID);
			///交易所交易员代码
			jsonData.put("TraderID", pErrorConditionalOrder->TraderID);
			///安装编号
			jsonData.put("InstallID", pErrorConditionalOrder->InstallID);
			///报单提交状态
			jsonData.put("OrderSubmitStatus", pErrorConditionalOrder->OrderSubmitStatus);
			///报单提示序号
			jsonData.put("NotifySequence", pErrorConditionalOrder->NotifySequence);
			///交易日
			jsonData.put("TradingDay", pErrorConditionalOrder->TradingDay);
			///结算编号
			jsonData.put("SettlementID", pErrorConditionalOrder->SettlementID);
			///报单编号
			jsonData.put("OrderSysID", pErrorConditionalOrder->OrderSysID);
			///报单来源
			jsonData.put("OrderSource", pErrorConditionalOrder->OrderSource);
			///报单状态
			jsonData.put("OrderStatus", pErrorConditionalOrder->OrderStatus);
			///报单类型
			jsonData.put("OrderType", pErrorConditionalOrder->OrderType);
			///今成交数量
			jsonData.put("VolumeTraded", pErrorConditionalOrder->VolumeTraded);
			///剩余数量
			jsonData.put("VolumeTotal", pErrorConditionalOrder->VolumeTotal);
			///报单日期
			jsonData.put("InsertDate", pErrorConditionalOrder->InsertDate);
			///委托时间
			jsonData.put("InsertTime", pErrorConditionalOrder->InsertTime);
			///激活时间
			jsonData.put("ActiveTime", pErrorConditionalOrder->ActiveTime);
			///挂起时间
			jsonData.put("SuspendTime", pErrorConditionalOrder->SuspendTime);
			///最后修改时间
			jsonData.put("UpdateTime", pErrorConditionalOrder->UpdateTime);
			///撤销时间
			jsonData.put("CancelTime", pErrorConditionalOrder->CancelTime);
			///最后修改交易所交易员代码
			jsonData.put("ActiveTraderID", pErrorConditionalOrder->ActiveTraderID);
			///结算会员编号
			jsonData.put("ClearingPartID", pErrorConditionalOrder->ClearingPartID);
			///序号
			jsonData.put("SequenceNo", pErrorConditionalOrder->SequenceNo);
			///前置编号
			jsonData.put("FrontID", pErrorConditionalOrder->FrontID);
			///会话编号
			jsonData.put("SessionID", pErrorConditionalOrder->SessionID);
			///用户端产品信息
			jsonData.put("UserProductInfo", pErrorConditionalOrder->UserProductInfo);
			///状态信息
			jsonData.put("StatusMsg", pErrorConditionalOrder->StatusMsg);
			///用户强评标志
			jsonData.put("UserForceClose", pErrorConditionalOrder->UserForceClose);
			///操作用户代码
			jsonData.put("ActiveUserID", pErrorConditionalOrder->ActiveUserID);
			///经纪公司报单编号
			jsonData.put("BrokerOrderSeq", pErrorConditionalOrder->BrokerOrderSeq);
			///相关报单
			jsonData.put("RelativeOrderSysID", pErrorConditionalOrder->RelativeOrderSysID);
			///郑商所成交数量
			jsonData.put("ZCETotalTradedVolume", pErrorConditionalOrder->ZCETotalTradedVolume);
			///错误代码
			jsonData.put("ErrorID", pErrorConditionalOrder->ErrorID);
			///错误信息
			jsonData.put("ErrorMsg", pErrorConditionalOrder->ErrorMsg);
			///互换单标志
			jsonData.put("IsSwapOrder", pErrorConditionalOrder->IsSwapOrder);
			///营业部编号
			jsonData.put("BranchID", pErrorConditionalOrder->BranchID);
			///投资单元代码
			jsonData.put("InvestUnitID", pErrorConditionalOrder->InvestUnitID);
			///资金账号
			jsonData.put("AccountID", pErrorConditionalOrder->AccountID);
			///币种代码
			jsonData.put("CurrencyID", pErrorConditionalOrder->CurrencyID);
			///IP地址
			jsonData.put("IPAddress", pErrorConditionalOrder->IPAddress);
			///Mac地址
			jsonData.put("MacAddress", pErrorConditionalOrder->MacAddress);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///执行宣告通知
void CTPApi::OnRtnExecOrder(CThostFtdcExecOrderField *pExecOrder) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRtnExecOrder")) {
			//构造Data
			boost::property_tree::ptree jsonData;

			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///执行宣告录入错误回报
void CTPApi::OnErrRtnExecOrderInsert(CThostFtdcInputExecOrderField *pInputExecOrder, CThostFtdcRspInfoField *pRspInfo) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnErrRtnExecOrderInsert", pRspInfo)) {
			//构造Data
			boost::property_tree::ptree jsonData;

			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///执行宣告操作错误回报
void CTPApi::OnErrRtnExecOrderAction(CThostFtdcExecOrderActionField *pExecOrderAction, CThostFtdcRspInfoField *pRspInfo) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnErrRtnExecOrderAction", pRspInfo)) {
			//构造Data
			boost::property_tree::ptree jsonData;

			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///询价录入错误回报
void CTPApi::OnErrRtnForQuoteInsert(CThostFtdcInputForQuoteField *pInputForQuote, CThostFtdcRspInfoField *pRspInfo) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnErrRtnForQuoteInsert", pRspInfo)) {
			//构造Data
			boost::property_tree::ptree jsonData;
			///经纪公司代码
			jsonData.put("BrokerID", pInputForQuote->BrokerID);
			///投资者代码
			jsonData.put("InvestorID", pInputForQuote->InvestorID);
			///合约代码
			jsonData.put("InstrumentID", pInputForQuote->InstrumentID);
			///询价引用
			jsonData.put("ForQuoteRef", pInputForQuote->ForQuoteRef);
			///用户代码
			jsonData.put("UserID", pInputForQuote->UserID);
			///交易所代码
			jsonData.put("ExchangeID", pInputForQuote->ExchangeID);
			///投资单元代码
			jsonData.put("InvestUnitID", pInputForQuote->InvestUnitID);
			///IP地址
			jsonData.put("IPAddress", pInputForQuote->IPAddress);
			///Mac地址
			jsonData.put("MacAddress", pInputForQuote->MacAddress);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///报价通知
void CTPApi::OnRtnQuote(CThostFtdcQuoteField *pQuote) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRtnQuote")) {
			//构造Data
			boost::property_tree::ptree jsonData;
			//经纪公司代码
			jsonData.put("BrokerID", pQuote->BrokerID);
			///投资者代码
			jsonData.put("InvestorID", pQuote->InvestorID);
			///合约代码
			jsonData.put("InstrumentID", pQuote->InstrumentID);
			///报价引用
			jsonData.put("QuoteRef", pQuote->QuoteRef);
			///用户代码
			jsonData.put("UserID", pQuote->UserID);
			///卖价格
			jsonData.put("AskPrice", pQuote->AskPrice);
			///买价格
			jsonData.put("BidPrice", pQuote->BidPrice);
			///卖数量
			jsonData.put("AskVolume", pQuote->AskVolume);
			///买数量
			jsonData.put("BidVolume", pQuote->BidVolume);
			///请求编号
			jsonData.put("RequestID", pQuote->RequestID);
			///业务单元
			jsonData.put("BusinessUnit", pQuote->BusinessUnit);
			///卖开平标志
			jsonData.put("AskOffsetFlag", pQuote->AskOffsetFlag);
			///买开平标志
			jsonData.put("BidOffsetFlag", pQuote->BidOffsetFlag);
			///卖投机套保标志
			jsonData.put("AskHedgeFlag", pQuote->AskHedgeFlag);
			///买投机套保标志
			jsonData.put("BidHedgeFlag", pQuote->BidHedgeFlag);
			///本地报价编号
			jsonData.put("QuoteLocalID", pQuote->QuoteLocalID);
			///交易所代码
			jsonData.put("ExchangeID", pQuote->ExchangeID);
			///会员代码
			jsonData.put("ParticipantID", pQuote->ParticipantID);
			///客户代码
			jsonData.put("ClientID", pQuote->ClientID);
			///合约在交易所的代码
			jsonData.put("ExchangeInstID", pQuote->ExchangeInstID);
			///交易所交易员代码
			jsonData.put("TraderID", pQuote->TraderID);
			///安装编号
			jsonData.put("InstallID", pQuote->InstallID);
			///报价提示序号
			jsonData.put("NotifySequence", pQuote->NotifySequence);
			///报价提交状态
			jsonData.put("OrderSubmitStatus", pQuote->OrderSubmitStatus);
			///交易日
			jsonData.put("TradingDay", pQuote->TradingDay);
			///结算编号
			jsonData.put("SettlementID", pQuote->SettlementID);
			///报价编号
			jsonData.put("QuoteSysID", pQuote->QuoteSysID);
			///报单日期
			jsonData.put("InsertDate", pQuote->InsertDate);
			///插入时间
			jsonData.put("InsertTime", pQuote->InsertTime);
			///撤销时间
			jsonData.put("CancelTime", pQuote->CancelTime);
			///报价状态
			jsonData.put("QuoteStatus", pQuote->QuoteStatus);
			///结算会员编号
			jsonData.put("ClearingPartID", pQuote->ClearingPartID);
			///序号
			jsonData.put("SequenceNo", pQuote->SequenceNo);
			///卖方报单编号
			jsonData.put("AskOrderSysID", pQuote->AskOrderSysID);
			///买方报单编号
			jsonData.put("BidOrderSysID", pQuote->BidOrderSysID);
			///前置编号
			jsonData.put("FrontID", pQuote->FrontID);
			///会话编号
			jsonData.put("SessionID", pQuote->SessionID);
			///用户端产品信息
			jsonData.put("UserProductInfo", pQuote->UserProductInfo);
			///状态信息
			jsonData.put("StatusMsg", pQuote->StatusMsg);
			///操作用户代码
			jsonData.put("ActiveUserID", pQuote->ActiveUserID);
			///经纪公司报价编号
			jsonData.put("BrokerQuoteSeq", pQuote->BrokerQuoteSeq);
			///衍生卖报单引用
			jsonData.put("AskOrderRef", pQuote->AskOrderRef);
			///衍生买报单引用
			jsonData.put("BidOrderRef", pQuote->BidOrderRef);
			///应价编号
			jsonData.put("ForQuoteSysID", pQuote->ForQuoteSysID);
			///营业部编号
			jsonData.put("BranchID", pQuote->BranchID);
			///投资单元代码
			jsonData.put("InvestUnitID", pQuote->InvestUnitID);
			///资金账号
			jsonData.put("AccountID", pQuote->AccountID);
			///币种代码
			jsonData.put("CurrencyID", pQuote->CurrencyID);
			///IP地址
			jsonData.put("IPAddress", pQuote->IPAddress);
			///Mac地址
			jsonData.put("MacAddress", pQuote->MacAddress);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///报价录入错误回报
void CTPApi::OnErrRtnQuoteInsert(CThostFtdcInputQuoteField *pInputQuote, CThostFtdcRspInfoField *pRspInfo) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnErrRtnQuoteInsert", pRspInfo)) {
			//构造Data
			boost::property_tree::ptree jsonData;
			///经纪公司代码
			jsonData.put("BrokerID", pInputQuote->BrokerID);
			///投资者代码
			jsonData.put("InvestorID", pInputQuote->InvestorID);
			///合约代码
			///投资者代码
			jsonData.put("InstrumentID", pInputQuote->InstrumentID);
			///报价引用
			jsonData.put("QuoteRef", pInputQuote->QuoteRef);
			///用户代码
			jsonData.put("UserID", pInputQuote->UserID);
			///卖价格
			jsonData.put("AskPrice", pInputQuote->AskPrice);
			///买价格
			jsonData.put("BidPrice", pInputQuote->BidPrice);
			///卖数量
			jsonData.put("AskVolume", pInputQuote->AskVolume);
			///买数量
			jsonData.put("BidVolume", pInputQuote->BidVolume);
			///请求编号
			jsonData.put("RequestID", pInputQuote->RequestID);
			///业务单元
			jsonData.put("BusinessUnit", pInputQuote->BusinessUnit);
			///卖开平标志
			jsonData.put("AskOffsetFlag", pInputQuote->AskOffsetFlag);
			///买开平标志
			jsonData.put("BidOffsetFlag", pInputQuote->BidOffsetFlag);
			///卖投机套保标志
			jsonData.put("AskHedgeFlag", pInputQuote->AskHedgeFlag);
			///买投机套保标志
			jsonData.put("BidHedgeFlag", pInputQuote->BidHedgeFlag);
			///衍生卖报单引用
			jsonData.put("AskOrderRef", pInputQuote->AskOrderRef);
			///衍生买报单引用
			jsonData.put("BidOrderRef", pInputQuote->BidOrderRef);
			///应价编号
			jsonData.put("ForQuoteSysID", pInputQuote->ForQuoteSysID);
			///交易所代码
			jsonData.put("ExchangeID", pInputQuote->ExchangeID);
			///投资单元代码
			jsonData.put("InvestUnitID", pInputQuote->InvestUnitID);
			///交易编码
			jsonData.put("ClientID", pInputQuote->ClientID);
			///IP地址
			jsonData.put("IPAddress", pInputQuote->IPAddress);
			///Mac地址
			jsonData.put("MacAddress", pInputQuote->MacAddress);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///报价操作错误回报
void CTPApi::OnErrRtnQuoteAction(CThostFtdcQuoteActionField *pQuoteAction, CThostFtdcRspInfoField *pRspInfo) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnErrRtnQuoteAction", pRspInfo)) {
			//构造Data
			boost::property_tree::ptree jsonData;
			//经纪公司代码
			jsonData.put("BrokerID", pQuoteAction->BrokerID);
			///投资者代码
			jsonData.put("InvestorID", pQuoteAction->InvestorID);
			///报价操作引用
			jsonData.put("QuoteActionRef", pQuoteAction->QuoteActionRef);
			///报价引用
			jsonData.put("QuoteRef", pQuoteAction->QuoteRef);
			///请求编号
			jsonData.put("RequestID", pQuoteAction->RequestID);
			///前置编号
			jsonData.put("FrontID", pQuoteAction->FrontID);
			///会话编号
			jsonData.put("SessionID", pQuoteAction->SessionID);
			///交易所代码
			jsonData.put("ExchangeID", pQuoteAction->ExchangeID);
			///报价操作编号
			jsonData.put("QuoteSysID", pQuoteAction->QuoteSysID);
			///操作标志
			jsonData.put("ActionFlag", pQuoteAction->ActionFlag);
			///操作日期
			jsonData.put("ActionDate", pQuoteAction->ActionDate);
			///操作时间
			jsonData.put("ActionTime", pQuoteAction->ActionTime);
			///交易所交易员代码
			jsonData.put("TraderID", pQuoteAction->TraderID);
			///安装编号
			jsonData.put("InstallID", pQuoteAction->InstallID);
			///本地报价编号
			jsonData.put("QuoteLocalID", pQuoteAction->QuoteLocalID);
			///操作本地编号
			jsonData.put("ActionLocalID", pQuoteAction->ActionLocalID);
			///会员代码
			jsonData.put("ParticipantID", pQuoteAction->ParticipantID);
			///客户代码
			jsonData.put("ClientID", pQuoteAction->ClientID);
			///业务单元
			jsonData.put("BusinessUnit", pQuoteAction->BusinessUnit);
			///报单操作状态
			jsonData.put("OrderActionStatus", pQuoteAction->OrderActionStatus);
			///用户代码
			jsonData.put("UserID", pQuoteAction->UserID);
			///状态信息
			jsonData.put("StatusMsg", pQuoteAction->StatusMsg);
			///合约代码
			jsonData.put("InstrumentID", pQuoteAction->InstrumentID);
			///营业部编号
			jsonData.put("BranchID", pQuoteAction->BranchID);
			///投资单元代码
			jsonData.put("InvestUnitID", pQuoteAction->InvestUnitID);
			///IP地址
			jsonData.put("IPAddress", pQuoteAction->IPAddress);
			///Mac地址
			jsonData.put("MacAddress", pQuoteAction->MacAddress);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///询价通知
void CTPApi::OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRtnForQuoteRsp")) {
			//构造Data
			boost::property_tree::ptree jsonData;
			///交易日
			jsonData.put("TradingDay", pForQuoteRsp->TradingDay);
			///合约代码
			jsonData.put("InstrumentID", pForQuoteRsp->InstrumentID);
			///询价编号
			jsonData.put("ForQuoteSysID", pForQuoteRsp->ForQuoteSysID);
			///询价时间
			jsonData.put("ForQuoteTime", pForQuoteRsp->ForQuoteTime);
			///业务日期
			jsonData.put("ActionDay", pForQuoteRsp->ActionDay);
			///交易所代码
			jsonData.put("ExchangeID", pForQuoteRsp->ExchangeID);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///保证金监控中心用户令牌
void CTPApi::OnRtnCFMMCTradingAccountToken(CThostFtdcCFMMCTradingAccountTokenField *pCFMMCTradingAccountToken) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRtnCFMMCTradingAccountToken")) {
			//构造Data
			boost::property_tree::ptree jsonData;
			//经纪公司代码
			jsonData.put("BrokerID", pCFMMCTradingAccountToken->BrokerID);
			///经纪公司统一编码
			jsonData.put("ParticipantID", pCFMMCTradingAccountToken->ParticipantID);
			///投资者帐号
			jsonData.put("AccountID", pCFMMCTradingAccountToken->AccountID);
			///密钥编号
			jsonData.put("KeyID", pCFMMCTradingAccountToken->KeyID);
			///动态令牌
			jsonData.put("Token", pCFMMCTradingAccountToken->Token);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///锁定通知
void CTPApi::OnRtnLock(CThostFtdcLockField *pLock) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRtnLock")) {
			//构造Data
			boost::property_tree::ptree jsonData;
			//经纪公司代码
			jsonData.put("BrokerID", pLock->BrokerID);
			///投资者代码
			jsonData.put("InvestorID", pLock->InvestorID);
			///合约代码
			jsonData.put("InstrumentID", pLock->InstrumentID);
			///锁定引用
			jsonData.put("LockRef", pLock->LockRef);
			///用户代码
			jsonData.put("UserID", pLock->UserID);
			///数量
			jsonData.put("Volume", pLock->Volume);
			///请求编号
			jsonData.put("RequestID", pLock->RequestID);
			///业务单元
			jsonData.put("BusinessUnit", pLock->BusinessUnit);
			///锁定类型
			jsonData.put("LockType", pLock->LockType);
			///本地锁定编号
			jsonData.put("LockLocalID", pLock->LockLocalID);
			///交易所代码
			jsonData.put("ExchangeID", pLock->ExchangeID);
			///会员代码
			jsonData.put("ParticipantID", pLock->ParticipantID);
			///客户代码
			jsonData.put("ClientID", pLock->ClientID);
			///合约在交易所的代码
			jsonData.put("ExchangeInstID", pLock->ExchangeInstID);
			///交易所交易员代码
			jsonData.put("TraderID", pLock->TraderID);
			///安装编号
			jsonData.put("InstallID", pLock->InstallID);
			///执行宣告提交状态
			jsonData.put("OrderSubmitStatus", pLock->OrderSubmitStatus);
			///报单提示序号
			jsonData.put("NotifySequence", pLock->NotifySequence);
			///交易日
			jsonData.put("TradingDay", pLock->TradingDay);
			///结算编号
			jsonData.put("SettlementID", pLock->SettlementID);
			///锁定编号
			jsonData.put("LockSysID", pLock->LockSysID);
			///报单日期
			jsonData.put("InsertDate", pLock->InsertDate);
			///插入时间
			jsonData.put("InsertTime", pLock->InsertTime);
			///撤销时间
			jsonData.put("CancelTime", pLock->CancelTime);
			///锁定状态
			jsonData.put("LockStatus", pLock->LockStatus);
			///结算会员编号
			jsonData.put("ClearingPartID", pLock->ClearingPartID);
			///序号
			jsonData.put("SequenceNo", pLock->SequenceNo);
			///前置编号
			jsonData.put("FrontID", pLock->FrontID);
			///会话编号
			jsonData.put("SessionID", pLock->SessionID);
			///用户端产品信息
			jsonData.put("UserProductInfo", pLock->UserProductInfo);
			///状态信息
			jsonData.put("StatusMsg", pLock->StatusMsg);
			///操作用户代码
			jsonData.put("ActiveUserID", pLock->ActiveUserID);
			///经纪公司报单编号
			jsonData.put("BrokerLockSeq", pLock->BrokerLockSeq);
			///营业部编号
			jsonData.put("BranchID", pLock->BranchID);
			///IP地址
			jsonData.put("IPAddress", pLock->IPAddress);
			///Mac地址
			jsonData.put("MacAddress", pLock->MacAddress);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///锁定错误通知
void CTPApi::OnErrRtnLockInsert(CThostFtdcInputLockField *pInputLock, CThostFtdcRspInfoField *pRspInfo) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnErrRtnLockInsert", pRspInfo)) {
			//构造Data
			boost::property_tree::ptree jsonData;
			///经纪公司代码
			jsonData.put("BrokerID", pInputLock->BrokerID);
			///投资者代码
			jsonData.put("InvestorID", pInputLock->InvestorID);
			///合约代码
			jsonData.put("InstrumentID", pInputLock->InstrumentID);
			///锁定引用
			jsonData.put("LockRef", pInputLock->LockRef);
			///用户代码
			jsonData.put("UserID", pInputLock->UserID);
			///数量
			jsonData.put("Volume", pInputLock->Volume);
			///请求编号
			jsonData.put("RequestID", pInputLock->RequestID);
			///业务单元
			jsonData.put("BusinessUnit", pInputLock->BusinessUnit);
			///锁定类型
			jsonData.put("LockType", pInputLock->LockType);
			///交易所代码
			jsonData.put("ExchangeID", pInputLock->ExchangeID);
			///IP地址
			jsonData.put("IPAddress", pInputLock->IPAddress);
			///Mac地址
			jsonData.put("MacAddress", pInputLock->MacAddress);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///批量报单操作错误回报
void CTPApi::OnErrRtnBatchOrderAction(CThostFtdcBatchOrderActionField *pBatchOrderAction, CThostFtdcRspInfoField *pRspInfo) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnErrRtnBatchOrderAction", pRspInfo)) {
			//构造Data
			boost::property_tree::ptree jsonData;
			//经纪公司代码
			jsonData.put("BrokerID", pBatchOrderAction->BrokerID);
			///投资者代码
			jsonData.put("InvestorID", pBatchOrderAction->InvestorID);
			///报单操作引用
			jsonData.put("OrderActionRef", pBatchOrderAction->OrderActionRef);
			///请求编号
			jsonData.put("RequestID", pBatchOrderAction->RequestID);
			///前置编号
			jsonData.put("FrontID", pBatchOrderAction->FrontID);
			///会话编号
			jsonData.put("SessionID", pBatchOrderAction->SessionID);
			///交易所代码
			jsonData.put("ExchangeID", pBatchOrderAction->ExchangeID);
			///操作日期
			jsonData.put("ActionDate", pBatchOrderAction->ActionDate);
			///操作时间
			jsonData.put("ActionTime", pBatchOrderAction->ActionTime);
			///交易所交易员代码
			jsonData.put("TraderID", pBatchOrderAction->TraderID);
			///安装编号
			jsonData.put("InstallID", pBatchOrderAction->InstallID);
			///操作本地编号
			jsonData.put("ActionLocalID", pBatchOrderAction->ActionLocalID);
			///会员代码
			jsonData.put("ParticipantID", pBatchOrderAction->ParticipantID);
			///客户代码
			jsonData.put("ClientID", pBatchOrderAction->ClientID);
			///业务单元
			jsonData.put("BusinessUnit", pBatchOrderAction->BusinessUnit);
			///报单操作状态
			jsonData.put("OrderActionStatus", pBatchOrderAction->OrderActionStatus);
			///用户代码
			jsonData.put("UserID", pBatchOrderAction->UserID);
			///状态信息
			jsonData.put("StatusMsg", pBatchOrderAction->StatusMsg);
			///投资单元代码
			jsonData.put("InvestUnitID", pBatchOrderAction->InvestUnitID);
			///IP地址
			jsonData.put("IPAddress", pBatchOrderAction->IPAddress);
			///Mac地址
			jsonData.put("MacAddress", pBatchOrderAction->MacAddress);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///申请组合通知
void CTPApi::OnRtnCombAction(CThostFtdcCombActionField *pCombAction) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRtnCombAction")) {
			//构造Data
			boost::property_tree::ptree jsonData;
			//经纪公司代码
			jsonData.put("BrokerID", pCombAction->BrokerID);
			///投资者代码
			jsonData.put("InvestorID", pCombAction->InvestorID);
			///合约代码
			jsonData.put("InstrumentID", pCombAction->InstrumentID);
			///组合引用
			jsonData.put("CombActionRef", pCombAction->CombActionRef);
			///用户代码
			jsonData.put("UserID", pCombAction->UserID);
			///买卖方向
			jsonData.put("Direction", pCombAction->Direction);
			///数量
			jsonData.put("Volume", pCombAction->Volume);
			///组合指令方向
			jsonData.put("CombDirection", pCombAction->CombDirection);
			///投机套保标志
			jsonData.put("HedgeFlag", pCombAction->HedgeFlag);
			///本地申请组合编号
			jsonData.put("ActionLocalID", pCombAction->ActionLocalID);
			///交易所代码
			jsonData.put("ExchangeID", pCombAction->ExchangeID);
			///会员代码
			jsonData.put("ParticipantID", pCombAction->ParticipantID);
			///客户代码
			jsonData.put("ClientID", pCombAction->ClientID);
			///合约在交易所的代码
			jsonData.put("ExchangeInstID", pCombAction->ExchangeInstID);
			///交易所交易员代码
			jsonData.put("TraderID", pCombAction->TraderID);
			///安装编号
			jsonData.put("InstallID", pCombAction->InstallID);
			///组合状态
			jsonData.put("ActionStatus", pCombAction->ActionStatus);
			///报单提示序号
			jsonData.put("NotifySequence", pCombAction->NotifySequence);
			///交易日
			jsonData.put("TradingDay", pCombAction->TradingDay);
			///结算编号
			jsonData.put("SettlementID", pCombAction->SettlementID);
			///序号
			jsonData.put("SequenceNo", pCombAction->SequenceNo);
			///前置编号
			jsonData.put("FrontID", pCombAction->FrontID);
			///会话编号
			jsonData.put("SessionID", pCombAction->SessionID);
			///用户端产品信息
			jsonData.put("UserProductInfo", pCombAction->UserProductInfo);
			///状态信息
			jsonData.put("StatusMsg", pCombAction->StatusMsg);
			///IP地址
			jsonData.put("IPAddress", pCombAction->IPAddress);
			///Mac地址
			jsonData.put("MacAddress", pCombAction->MacAddress);
			///组合编号
			jsonData.put("ComTradeID", pCombAction->ComTradeID);
			///营业部编号
			jsonData.put("BranchID", pCombAction->BranchID);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///申请组合录入错误回报
void CTPApi::OnErrRtnCombActionInsert(CThostFtdcInputCombActionField *pInputCombAction, CThostFtdcRspInfoField *pRspInfo) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnErrRtnCombActionInsert", pRspInfo)) {
			//构造Data
			boost::property_tree::ptree jsonData;
			///经纪公司代码
			jsonData.put("BrokerID", pInputCombAction->BrokerID);
			///投资者代码
			jsonData.put("InvestorID", pInputCombAction->InvestorID);
			///合约代码
			jsonData.put("InstrumentID", pInputCombAction->InstrumentID);
			///组合引用
			jsonData.put("CombActionRef", pInputCombAction->CombActionRef);
			///用户代码
			jsonData.put("UserID", pInputCombAction->UserID);
			///买卖方向
			jsonData.put("Direction", pInputCombAction->Direction);
			///数量
			jsonData.put("Volume", pInputCombAction->Volume);
			///组合指令方向
			jsonData.put("CombDirection", pInputCombAction->CombDirection);
			///投机套保标志
			jsonData.put("HedgeFlag", pInputCombAction->HedgeFlag);
			///交易所代码
			jsonData.put("ExchangeID", pInputCombAction->ExchangeID);
			///IP地址
			jsonData.put("IPAddress", pInputCombAction->IPAddress);
			///Mac地址
			jsonData.put("MacAddress", pInputCombAction->MacAddress);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///请求查询签约银行响应
void CTPApi::OnRspQryContractBank(CThostFtdcContractBankField *pContractBank, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRspQryContractBank", pRspInfo, nRequestID, bIsLast)) {
			//构造Data
			boost::property_tree::ptree jsonData;
			//经纪公司代码
			jsonData.put("BrokerID", pContractBank->BrokerID);
			///银行代码
			jsonData.put("BankID", pContractBank->BankID);
			///银行分中心代码
			jsonData.put("BankBrchID", pContractBank->BankBrchID);
			///银行名称
			jsonData.put("BankName", pContractBank->BankName);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///请求查询预埋单响应
void CTPApi::OnRspQryParkedOrder(CThostFtdcParkedOrderField *pParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRspQryParkedOrder", pRspInfo, nRequestID, bIsLast)) {
			//构造Data
			boost::property_tree::ptree jsonData;
			///经纪公司代码
			jsonData.put("BrokerID", pParkedOrder->BrokerID);
			///投资者代码
			jsonData.put("InvestorID", pParkedOrder->InvestorID);
			///合约代码
			jsonData.put("InstrumentID", pParkedOrder->InstrumentID);
			///报单引用
			jsonData.put("OrderRef", pParkedOrder->OrderRef);
			///用户代码
			jsonData.put("UserID", pParkedOrder->UserID);
			///报单价格条件
			jsonData.put("OrderPriceType", pParkedOrder->OrderPriceType);
			///买卖方向
			jsonData.put("Direction", pParkedOrder->Direction);
			///组合开平标志
			jsonData.put("CombOffsetFlag", pParkedOrder->CombOffsetFlag);
			///组合投机套保标志
			jsonData.put("CombHedgeFlag", pParkedOrder->CombHedgeFlag);
			///价格
			jsonData.put("LimitPrice", pParkedOrder->LimitPrice);
			///数量
			jsonData.put("VolumeTotalOriginal", pParkedOrder->VolumeTotalOriginal);
			///有效期类型
			jsonData.put("TimeCondition", pParkedOrder->TimeCondition);
			///GTD日期
			jsonData.put("GTDDate", pParkedOrder->GTDDate);
			///成交量类型
			jsonData.put("VolumeCondition", pParkedOrder->VolumeCondition);
			///最小成交量
			jsonData.put("MinVolume", pParkedOrder->MinVolume);
			///触发条件
			jsonData.put("ContingentCondition", pParkedOrder->ContingentCondition);
			///止损价
			jsonData.put("StopPrice", pParkedOrder->StopPrice);
			///强平原因
			jsonData.put("ForceCloseReason", pParkedOrder->ForceCloseReason);
			///自动挂起标志
			jsonData.put("IsAutoSuspend", pParkedOrder->IsAutoSuspend);
			///业务单元
			jsonData.put("BusinessUnit", pParkedOrder->BusinessUnit);
			///请求编号
			jsonData.put("RequestID", pParkedOrder->RequestID);
			///用户强评标志
			jsonData.put("UserForceClose", pParkedOrder->UserForceClose);
			///交易所代码
			jsonData.put("ExchangeID", pParkedOrder->ExchangeID);
			///预埋报单编号
			jsonData.put("ParkedOrderID", pParkedOrder->ParkedOrderID);
			///用户类型
			jsonData.put("UserType", pParkedOrder->UserType);
			///预埋单状态
			jsonData.put("Status", pParkedOrder->Status);
			///错误代码
			jsonData.put("ErrorID", pParkedOrder->ErrorID);
			///错误信息
			jsonData.put("ErrorMsg", pParkedOrder->ErrorMsg);
			///互换单标志
			jsonData.put("IsSwapOrder", pParkedOrder->IsSwapOrder);
			///资金账号
			jsonData.put("AccountID", pParkedOrder->AccountID);
			///币种代码
			jsonData.put("CurrencyID", pParkedOrder->CurrencyID);
			///交易编码
			jsonData.put("ClientID", pParkedOrder->ClientID);
			///投资单元代码
			jsonData.put("InvestUnitID", pParkedOrder->InvestUnitID);
			///IP地址
			jsonData.put("IPAddress", pParkedOrder->IPAddress);
			///Mac地址
			jsonData.put("MacAddress", pParkedOrder->MacAddress);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///请求查询预埋撤单响应
void CTPApi::OnRspQryParkedOrderAction(CThostFtdcParkedOrderActionField *pParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRspQryParkedOrderAction", pRspInfo, nRequestID, bIsLast)) {
			//构造Data
			boost::property_tree::ptree jsonData;
			///经纪公司代码
			jsonData.put("BrokerID", pParkedOrderAction->BrokerID);
			///投资者代码
			jsonData.put("InvestorID", pParkedOrderAction->InvestorID);
			///报单操作引用
			jsonData.put("OrderActionRef", pParkedOrderAction->OrderActionRef);
			///报单引用
			jsonData.put("OrderRef", pParkedOrderAction->OrderRef);
			///请求编号
			jsonData.put("RequestID", pParkedOrderAction->RequestID);
			///前置编号
			jsonData.put("FrontID", pParkedOrderAction->FrontID);
			///会话编号
			jsonData.put("SessionID", pParkedOrderAction->SessionID);
			///交易所代码
			jsonData.put("ExchangeID", pParkedOrderAction->ExchangeID);
			///报单编号
			jsonData.put("OrderSysID", pParkedOrderAction->OrderSysID);
			///操作标志
			jsonData.put("ActionFlag", pParkedOrderAction->ActionFlag);
			///价格
			jsonData.put("LimitPrice", pParkedOrderAction->LimitPrice);
			///数量变化
			jsonData.put("VolumeChange", pParkedOrderAction->VolumeChange);
			///用户代码
			jsonData.put("UserID", pParkedOrderAction->UserID);
			///合约代码
			jsonData.put("InstrumentID", pParkedOrderAction->InstrumentID);
			///预埋撤单单编号
			jsonData.put("ParkedOrderActionID", pParkedOrderAction->ParkedOrderActionID);
			///用户类型
			jsonData.put("UserType", pParkedOrderAction->UserType);
			///预埋撤单状态
			jsonData.put("Status", pParkedOrderAction->Status);
			///错误代码
			jsonData.put("ErrorID", pParkedOrderAction->ErrorID);
			///错误信息
			jsonData.put("ErrorMsg", pParkedOrderAction->ErrorMsg);
			///投资单元代码
			jsonData.put("InvestUnitID", pParkedOrderAction->InvestUnitID);
			///IP地址
			jsonData.put("IPAddress", pParkedOrderAction->IPAddress);
			///Mac地址
			jsonData.put("MacAddress", pParkedOrderAction->MacAddress);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///请求查询交易通知响应
void CTPApi::OnRspQryTradingNotice(CThostFtdcTradingNoticeField *pTradingNotice, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRspQryTradingNotice", pRspInfo, nRequestID, bIsLast)) {
			//构造Data
			boost::property_tree::ptree jsonData;
			//经纪公司代码
			jsonData.put("BrokerID", pTradingNotice->BrokerID);
			///投资者范围
			jsonData.put("InvestorRange", pTradingNotice->InvestorRange);
			///投资者代码
			jsonData.put("InvestorID", pTradingNotice->InvestorID);
			///序列系列号
			jsonData.put("SequenceSeries", pTradingNotice->SequenceSeries);
			///用户代码
			jsonData.put("UserID", pTradingNotice->UserID);
			///发送时间
			jsonData.put("SendTime", pTradingNotice->SendTime);
			///序列号
			jsonData.put("SequenceNo", pTradingNotice->SequenceNo);
			///消息正文
			jsonData.put("FieldContent", pTradingNotice->FieldContent);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///请求查询经纪公司交易参数响应
void CTPApi::OnRspQryBrokerTradingParams(CThostFtdcBrokerTradingParamsField *pBrokerTradingParams, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRspQryBrokerTradingParams", pRspInfo, nRequestID, bIsLast)) {
			//构造Data
			boost::property_tree::ptree jsonData;
			//经纪公司代码
			jsonData.put("BrokerID", pBrokerTradingParams->BrokerID);
			///投资者代码
			jsonData.put("InvestorID", pBrokerTradingParams->InvestorID);
			///保证金价格类型
			jsonData.put("MarginPriceType", pBrokerTradingParams->MarginPriceType);
			///盈亏算法
			jsonData.put("Algorithm", pBrokerTradingParams->Algorithm);
			///可用是否包含平仓盈利
			jsonData.put("AvailIncludeCloseProfit", pBrokerTradingParams->AvailIncludeCloseProfit);
			///币种代码
			jsonData.put("CurrencyID", pBrokerTradingParams->CurrencyID);
			///期权权利金价格类型
			jsonData.put("OptionRoyaltyPriceType", pBrokerTradingParams->OptionRoyaltyPriceType);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///请求查询经纪公司交易算法响应
void CTPApi::OnRspQryBrokerTradingAlgos(CThostFtdcBrokerTradingAlgosField *pBrokerTradingAlgos, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRspQryBrokerTradingAlgos", pRspInfo, nRequestID, bIsLast)) {
			//构造Data
			boost::property_tree::ptree jsonData;
			//经纪公司代码
			jsonData.put("BrokerID", pBrokerTradingAlgos->BrokerID);
			///交易所代码
			jsonData.put("ExchangeID", pBrokerTradingAlgos->ExchangeID);
			///合约代码
			jsonData.put("InstrumentID", pBrokerTradingAlgos->InstrumentID);
			///持仓处理算法编号
			jsonData.put("HandlePositionAlgoID", pBrokerTradingAlgos->HandlePositionAlgoID);
			///寻找保证金率算法编号
			jsonData.put("FindMarginRateAlgoID", pBrokerTradingAlgos->FindMarginRateAlgoID);
			///资金处理算法编号
			jsonData.put("HandleTradingAccountAlgoID", pBrokerTradingAlgos->HandleTradingAccountAlgoID);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///请求查询监控中心用户令牌
void CTPApi::OnRspQueryCFMMCTradingAccountToken(CThostFtdcQueryCFMMCTradingAccountTokenField *pQueryCFMMCTradingAccountToken, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRspQueryCFMMCTradingAccountToken", pRspInfo, nRequestID, bIsLast)) {
			//构造Data
			boost::property_tree::ptree jsonData;
			//经纪公司代码
			jsonData.put("BrokerID", pQueryCFMMCTradingAccountToken->BrokerID);
			///投资者代码
			jsonData.put("InvestorID", pQueryCFMMCTradingAccountToken->InvestorID);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///银行发起银行资金转期货通知
void CTPApi::OnRtnFromBankToFutureByBank(CThostFtdcRspTransferField *pRspTransfer) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRtnFromBankToFutureByBank")) {
			//构造Data
			boost::property_tree::ptree jsonData;
			///业务功能码
			jsonData.put("TradeCode", pRspTransfer->TradeCode);
			///银行代码
			jsonData.put("BankID", pRspTransfer->BankID);
			///银行分支机构代码
			jsonData.put("BankBranchID", pRspTransfer->BankBranchID);
			///期商代码
			jsonData.put("BrokerID", pRspTransfer->BrokerID);
			///期商分支机构代码
			jsonData.put("BrokerBranchID", pRspTransfer->BrokerBranchID);
			///交易日期
			jsonData.put("TradeDate", pRspTransfer->TradeDate);
			///交易时间
			jsonData.put("TradeTime", pRspTransfer->TradeTime);
			///银行流水号
			jsonData.put("BankSerial", pRspTransfer->BankSerial);
			///交易系统日期 
			jsonData.put("TradingDay", pRspTransfer->TradingDay);
			///银期平台消息流水号
			jsonData.put("PlateSerial", pRspTransfer->PlateSerial);
			///最后分片标志
			jsonData.put("LastFragment", pRspTransfer->LastFragment);
			///会话号
			jsonData.put("SessionID", pRspTransfer->SessionID);
			///客户姓名
			jsonData.put("CustomerName", pRspTransfer->CustomerName);
			///证件类型
			jsonData.put("IdCardType", pRspTransfer->IdCardType);
			///证件号码
			jsonData.put("IdentifiedCardNo", pRspTransfer->IdentifiedCardNo);
			///客户类型
			jsonData.put("CustType", pRspTransfer->CustType);
			///银行帐号
			jsonData.put("BankAccount", pRspTransfer->BankAccount);
			///银行密码
			jsonData.put("BankPassWord", pRspTransfer->BankPassWord);
			///投资者帐号
			jsonData.put("AccountID", pRspTransfer->AccountID);
			///期货密码
			jsonData.put("Password", pRspTransfer->Password);
			///期货公司流水号
			jsonData.put("FutureSerial", pRspTransfer->FutureSerial);
			///安装编号
			jsonData.put("InstallID", pRspTransfer->InstallID);
			///用户标识
			jsonData.put("UserID", pRspTransfer->UserID);
			///验证客户证件号码标志
			jsonData.put("VerifyCertNoFlag", pRspTransfer->VerifyCertNoFlag);
			///币种代码
			jsonData.put("CurrencyID", pRspTransfer->CurrencyID);
			///转帐金额
			jsonData.put("TradeAmount", pRspTransfer->TradeAmount);
			///期货可取金额
			jsonData.put("FutureFetchAmount", pRspTransfer->FutureFetchAmount);
			///费用支付标志
			jsonData.put("FeePayFlag", pRspTransfer->FeePayFlag);
			///应收客户费用
			jsonData.put("CustFee", pRspTransfer->CustFee);
			///应收期货公司费用
			jsonData.put("BrokerFee", pRspTransfer->BrokerFee);
			///发送方给接收方的消息
			jsonData.put("Message", pRspTransfer->Message);
			///摘要
			jsonData.put("Digest", pRspTransfer->Digest);
			///银行帐号类型
			jsonData.put("BankAccType", pRspTransfer->BankAccType);
			///渠道标志
			jsonData.put("DeviceID", pRspTransfer->DeviceID);
			///期货单位帐号类型
			jsonData.put("BankSecuAccType", pRspTransfer->BankSecuAccType);
			///期货公司银行编码
			jsonData.put("BrokerIDByBank", pRspTransfer->BrokerIDByBank);
			///期货单位帐号
			jsonData.put("BankSecuAcc", pRspTransfer->BankSecuAcc);
			///银行密码标志
			jsonData.put("BankPwdFlag", pRspTransfer->BankPwdFlag);
			///期货资金密码核对标志
			jsonData.put("SecuPwdFlag", pRspTransfer->SecuPwdFlag);
			///交易柜员
			jsonData.put("OperNo", pRspTransfer->OperNo);
			///请求编号
			jsonData.put("RequestID", pRspTransfer->RequestID);
			///交易ID
			jsonData.put("TID", pRspTransfer->TID);
			///转账交易状态
			jsonData.put("TransferStatus", pRspTransfer->TransferStatus);
			///错误代码
			jsonData.put("ErrorID", pRspTransfer->ErrorID);
			///错误信息
			jsonData.put("ErrorMsg", pRspTransfer->ErrorMsg);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///银行发起期货资金转银行通知
void CTPApi::OnRtnFromFutureToBankByBank(CThostFtdcRspTransferField *pRspTransfer) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRtnFromFutureToBankByBank")) {
			//构造Data
			boost::property_tree::ptree jsonData;
			///业务功能码
			jsonData.put("TradeCode", pRspTransfer->TradeCode);
			///银行代码
			jsonData.put("BankID", pRspTransfer->BankID);
			///银行分支机构代码
			jsonData.put("BankBranchID", pRspTransfer->BankBranchID);
			///期商代码
			jsonData.put("BrokerID", pRspTransfer->BrokerID);
			///期商分支机构代码
			jsonData.put("BrokerBranchID", pRspTransfer->BrokerBranchID);
			///交易日期
			jsonData.put("TradeDate", pRspTransfer->TradeDate);
			///交易时间
			jsonData.put("TradeTime", pRspTransfer->TradeTime);
			///银行流水号
			jsonData.put("BankSerial", pRspTransfer->BankSerial);
			///交易系统日期 
			jsonData.put("TradingDay", pRspTransfer->TradingDay);
			///银期平台消息流水号
			jsonData.put("PlateSerial", pRspTransfer->PlateSerial);
			///最后分片标志
			jsonData.put("LastFragment", pRspTransfer->LastFragment);
			///会话号
			jsonData.put("SessionID", pRspTransfer->SessionID);
			///客户姓名
			jsonData.put("CustomerName", pRspTransfer->CustomerName);
			///证件类型
			jsonData.put("IdCardType", pRspTransfer->IdCardType);
			///证件号码
			jsonData.put("IdentifiedCardNo", pRspTransfer->IdentifiedCardNo);
			///客户类型
			jsonData.put("CustType", pRspTransfer->CustType);
			///银行帐号
			jsonData.put("BankAccount", pRspTransfer->BankAccount);
			///银行密码
			jsonData.put("BankPassWord", pRspTransfer->BankPassWord);
			///投资者帐号
			jsonData.put("AccountID", pRspTransfer->AccountID);
			///期货密码
			jsonData.put("Password", pRspTransfer->Password);
			///期货公司流水号
			jsonData.put("FutureSerial", pRspTransfer->FutureSerial);
			///安装编号
			jsonData.put("InstallID", pRspTransfer->InstallID);
			///用户标识
			jsonData.put("UserID", pRspTransfer->UserID);
			///验证客户证件号码标志
			jsonData.put("VerifyCertNoFlag", pRspTransfer->VerifyCertNoFlag);
			///币种代码
			jsonData.put("CurrencyID", pRspTransfer->CurrencyID);
			///转帐金额
			jsonData.put("TradeAmount", pRspTransfer->TradeAmount);
			///期货可取金额
			jsonData.put("FutureFetchAmount", pRspTransfer->FutureFetchAmount);
			///费用支付标志
			jsonData.put("FeePayFlag", pRspTransfer->FeePayFlag);
			///应收客户费用
			jsonData.put("CustFee", pRspTransfer->CustFee);
			///应收期货公司费用
			jsonData.put("BrokerFee", pRspTransfer->BrokerFee);
			///发送方给接收方的消息
			jsonData.put("Message", pRspTransfer->Message);
			///摘要
			jsonData.put("Digest", pRspTransfer->Digest);
			///银行帐号类型
			jsonData.put("BankAccType", pRspTransfer->BankAccType);
			///渠道标志
			jsonData.put("DeviceID", pRspTransfer->DeviceID);
			///期货单位帐号类型
			jsonData.put("BankSecuAccType", pRspTransfer->BankSecuAccType);
			///期货公司银行编码
			jsonData.put("BrokerIDByBank", pRspTransfer->BrokerIDByBank);
			///期货单位帐号
			jsonData.put("BankSecuAcc", pRspTransfer->BankSecuAcc);
			///银行密码标志
			jsonData.put("BankPwdFlag", pRspTransfer->BankPwdFlag);
			///期货资金密码核对标志
			jsonData.put("SecuPwdFlag", pRspTransfer->SecuPwdFlag);
			///交易柜员
			jsonData.put("OperNo", pRspTransfer->OperNo);
			///请求编号
			jsonData.put("RequestID", pRspTransfer->RequestID);
			///交易ID
			jsonData.put("TID", pRspTransfer->TID);
			///转账交易状态
			jsonData.put("TransferStatus", pRspTransfer->TransferStatus);
			///错误代码
			jsonData.put("ErrorID", pRspTransfer->ErrorID);
			///错误信息
			jsonData.put("ErrorMsg", pRspTransfer->ErrorMsg);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///银行发起冲正银行转期货通知
void CTPApi::OnRtnRepealFromBankToFutureByBank(CThostFtdcRspRepealField *pRspRepeal) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRtnRepealFromBankToFutureByBank")) {
			//构造Data
			boost::property_tree::ptree jsonData;
			///冲正时间间隔
			jsonData.put("RepealTimeInterval", pRspRepeal->RepealTimeInterval);
			///已经冲正次数
			jsonData.put("RepealedTimes", pRspRepeal->RepealedTimes);
			///银行冲正标志
			jsonData.put("BankRepealFlag", pRspRepeal->BankRepealFlag);
			///期商冲正标志
			jsonData.put("BrokerRepealFlag", pRspRepeal->BrokerRepealFlag);
			///被冲正平台流水号
			jsonData.put("PlateRepealSerial", pRspRepeal->PlateRepealSerial);
			///被冲正银行流水号
			jsonData.put("BankRepealSerial", pRspRepeal->BankRepealSerial);
			///被冲正期货流水号
			jsonData.put("FutureRepealSerial", pRspRepeal->FutureRepealSerial);
			///业务功能码
			jsonData.put("TradeCode", pRspRepeal->TradeCode);
			///银行代码
			jsonData.put("BankID", pRspRepeal->BankID);
			///银行分支机构代码
			jsonData.put("BankBranchID", pRspRepeal->BankBranchID);
			///期商代码
			jsonData.put("BrokerID", pRspRepeal->BrokerID);
			///期商分支机构代码
			jsonData.put("BrokerBranchID", pRspRepeal->BrokerBranchID);
			///交易日期
			jsonData.put("TradeDate", pRspRepeal->TradeDate);
			///交易时间
			jsonData.put("TradeTime", pRspRepeal->TradeTime);
			///银行流水号
			jsonData.put("BankSerial", pRspRepeal->BankSerial);
			///交易系统日期 
			jsonData.put("TradingDay", pRspRepeal->TradingDay);
			///银期平台消息流水号
			jsonData.put("PlateSerial", pRspRepeal->PlateSerial);
			///最后分片标志
			jsonData.put("LastFragment", pRspRepeal->LastFragment);
			///会话号
			jsonData.put("SessionID", pRspRepeal->SessionID);
			///客户姓名
			jsonData.put("CustomerName", pRspRepeal->CustomerName);
			///证件类型
			jsonData.put("IdCardType", pRspRepeal->IdCardType);
			///证件号码
			jsonData.put("IdentifiedCardNo", pRspRepeal->IdentifiedCardNo);
			///客户类型
			jsonData.put("CustType", pRspRepeal->CustType);
			///银行帐号
			jsonData.put("BankAccount", pRspRepeal->BankAccount);
			///银行密码
			jsonData.put("BankPassWord", pRspRepeal->BankPassWord);
			///投资者帐号
			jsonData.put("AccountID", pRspRepeal->AccountID);
			///期货密码
			jsonData.put("Password", pRspRepeal->Password);
			///期货公司流水号
			jsonData.put("FutureSerial", pRspRepeal->FutureSerial);
			///安装编号
			jsonData.put("InstallID", pRspRepeal->InstallID);
			///用户标识
			jsonData.put("UserID", pRspRepeal->UserID);
			///验证客户证件号码标志
			jsonData.put("VerifyCertNoFlag", pRspRepeal->VerifyCertNoFlag);
			///币种代码
			jsonData.put("CurrencyID", pRspRepeal->CurrencyID);
			///转帐金额
			jsonData.put("TradeAmount", pRspRepeal->TradeAmount);
			///期货可取金额
			jsonData.put("FutureFetchAmount", pRspRepeal->FutureFetchAmount);
			///费用支付标志
			jsonData.put("FeePayFlag", pRspRepeal->FeePayFlag);
			///应收客户费用
			jsonData.put("CustFee", pRspRepeal->CustFee);
			///应收期货公司费用
			jsonData.put("BrokerFee", pRspRepeal->BrokerFee);
			///发送方给接收方的消息
			jsonData.put("Message", pRspRepeal->Message);
			///摘要
			jsonData.put("Digest", pRspRepeal->Digest);
			///银行帐号类型
			jsonData.put("BankAccType", pRspRepeal->BankAccType);
			///渠道标志
			jsonData.put("DeviceID", pRspRepeal->DeviceID);
			///期货单位帐号类型
			jsonData.put("BankSecuAccType", pRspRepeal->BankSecuAccType);
			///期货公司银行编码
			jsonData.put("BrokerIDByBank", pRspRepeal->BrokerIDByBank);
			///期货单位帐号
			jsonData.put("BankSecuAcc", pRspRepeal->BankSecuAcc);
			///银行密码标志
			jsonData.put("BankPwdFlag", pRspRepeal->BankPwdFlag);
			///期货资金密码核对标志
			jsonData.put("SecuPwdFlag", pRspRepeal->SecuPwdFlag);
			///交易柜员
			jsonData.put("OperNo", pRspRepeal->OperNo);
			///请求编号
			jsonData.put("RequestID", pRspRepeal->RequestID);
			///交易ID
			jsonData.put("TID", pRspRepeal->TID);
			///转账交易状态
			jsonData.put("TransferStatus", pRspRepeal->TransferStatus);
			///错误代码
			jsonData.put("ErrorID", pRspRepeal->ErrorID);
			///错误信息
			jsonData.put("ErrorMsg", pRspRepeal->ErrorMsg);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///银行发起冲正期货转银行通知
void CTPApi::OnRtnRepealFromFutureToBankByBank(CThostFtdcRspRepealField *pRspRepeal) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRtnRepealFromFutureToBankByBank")) {
			//构造Data
			boost::property_tree::ptree jsonData;
			///冲正时间间隔
			jsonData.put("RepealTimeInterval", pRspRepeal->RepealTimeInterval);
			///已经冲正次数
			jsonData.put("RepealedTimes", pRspRepeal->RepealedTimes);
			///银行冲正标志
			jsonData.put("BankRepealFlag", pRspRepeal->BankRepealFlag);
			///期商冲正标志
			jsonData.put("BrokerRepealFlag", pRspRepeal->BrokerRepealFlag);
			///被冲正平台流水号
			jsonData.put("PlateRepealSerial", pRspRepeal->PlateRepealSerial);
			///被冲正银行流水号
			jsonData.put("BankRepealSerial", pRspRepeal->BankRepealSerial);
			///被冲正期货流水号
			jsonData.put("FutureRepealSerial", pRspRepeal->FutureRepealSerial);
			///业务功能码
			jsonData.put("TradeCode", pRspRepeal->TradeCode);
			///银行代码
			jsonData.put("BankID", pRspRepeal->BankID);
			///银行分支机构代码
			jsonData.put("BankBranchID", pRspRepeal->BankBranchID);
			///期商代码
			jsonData.put("BrokerID", pRspRepeal->BrokerID);
			///期商分支机构代码
			jsonData.put("BrokerBranchID", pRspRepeal->BrokerBranchID);
			///交易日期
			jsonData.put("TradeDate", pRspRepeal->TradeDate);
			///交易时间
			jsonData.put("TradeTime", pRspRepeal->TradeTime);
			///银行流水号
			jsonData.put("BankSerial", pRspRepeal->BankSerial);
			///交易系统日期 
			jsonData.put("TradingDay", pRspRepeal->TradingDay);
			///银期平台消息流水号
			jsonData.put("PlateSerial", pRspRepeal->PlateSerial);
			///最后分片标志
			jsonData.put("LastFragment", pRspRepeal->LastFragment);
			///会话号
			jsonData.put("SessionID", pRspRepeal->SessionID);
			///客户姓名
			jsonData.put("CustomerName", pRspRepeal->CustomerName);
			///证件类型
			jsonData.put("IdCardType", pRspRepeal->IdCardType);
			///证件号码
			jsonData.put("IdentifiedCardNo", pRspRepeal->IdentifiedCardNo);
			///客户类型
			jsonData.put("CustType", pRspRepeal->CustType);
			///银行帐号
			jsonData.put("BankAccount", pRspRepeal->BankAccount);
			///银行密码
			jsonData.put("BankPassWord", pRspRepeal->BankPassWord);
			///投资者帐号
			jsonData.put("AccountID", pRspRepeal->AccountID);
			///期货密码
			jsonData.put("Password", pRspRepeal->Password);
			///期货公司流水号
			jsonData.put("FutureSerial", pRspRepeal->FutureSerial);
			///安装编号
			jsonData.put("InstallID", pRspRepeal->InstallID);
			///用户标识
			jsonData.put("UserID", pRspRepeal->UserID);
			///验证客户证件号码标志
			jsonData.put("VerifyCertNoFlag", pRspRepeal->VerifyCertNoFlag);
			///币种代码
			jsonData.put("CurrencyID", pRspRepeal->CurrencyID);
			///转帐金额
			jsonData.put("TradeAmount", pRspRepeal->TradeAmount);
			///期货可取金额
			jsonData.put("FutureFetchAmount", pRspRepeal->FutureFetchAmount);
			///费用支付标志
			jsonData.put("FeePayFlag", pRspRepeal->FeePayFlag);
			///应收客户费用
			jsonData.put("CustFee", pRspRepeal->CustFee);
			///应收期货公司费用
			jsonData.put("BrokerFee", pRspRepeal->BrokerFee);
			///发送方给接收方的消息
			jsonData.put("Message", pRspRepeal->Message);
			///摘要
			jsonData.put("Digest", pRspRepeal->Digest);
			///银行帐号类型
			jsonData.put("BankAccType", pRspRepeal->BankAccType);
			///渠道标志
			jsonData.put("DeviceID", pRspRepeal->DeviceID);
			///期货单位帐号类型
			jsonData.put("BankSecuAccType", pRspRepeal->BankSecuAccType);
			///期货公司银行编码
			jsonData.put("BrokerIDByBank", pRspRepeal->BrokerIDByBank);
			///期货单位帐号
			jsonData.put("BankSecuAcc", pRspRepeal->BankSecuAcc);
			///银行密码标志
			jsonData.put("BankPwdFlag", pRspRepeal->BankPwdFlag);
			///期货资金密码核对标志
			jsonData.put("SecuPwdFlag", pRspRepeal->SecuPwdFlag);
			///交易柜员
			jsonData.put("OperNo", pRspRepeal->OperNo);
			///请求编号
			jsonData.put("RequestID", pRspRepeal->RequestID);
			///交易ID
			jsonData.put("TID", pRspRepeal->TID);
			///转账交易状态
			jsonData.put("TransferStatus", pRspRepeal->TransferStatus);
			///错误代码
			jsonData.put("ErrorID", pRspRepeal->ErrorID);
			///错误信息
			jsonData.put("ErrorMsg", pRspRepeal->ErrorMsg);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///期货发起银行资金转期货通知
void CTPApi::OnRtnFromBankToFutureByFuture(CThostFtdcRspTransferField *pRspTransfer) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRtnFromBankToFutureByFuture")) {
			//构造Data
			boost::property_tree::ptree jsonData;
			///业务功能码
			jsonData.put("TradeCode", pRspTransfer->TradeCode);
			///银行代码
			jsonData.put("BankID", pRspTransfer->BankID);
			///银行分支机构代码
			jsonData.put("BankBranchID", pRspTransfer->BankBranchID);
			///期商代码
			jsonData.put("BrokerID", pRspTransfer->BrokerID);
			///期商分支机构代码
			jsonData.put("BrokerBranchID", pRspTransfer->BrokerBranchID);
			///交易日期
			jsonData.put("TradeDate", pRspTransfer->TradeDate);
			///交易时间
			jsonData.put("TradeTime", pRspTransfer->TradeTime);
			///银行流水号
			jsonData.put("BankSerial", pRspTransfer->BankSerial);
			///交易系统日期 
			jsonData.put("TradingDay", pRspTransfer->TradingDay);
			///银期平台消息流水号
			jsonData.put("PlateSerial", pRspTransfer->PlateSerial);
			///最后分片标志
			jsonData.put("LastFragment", pRspTransfer->LastFragment);
			///会话号
			jsonData.put("SessionID", pRspTransfer->SessionID);
			///客户姓名
			jsonData.put("CustomerName", pRspTransfer->CustomerName);
			///证件类型
			jsonData.put("IdCardType", pRspTransfer->IdCardType);
			///证件号码
			jsonData.put("IdentifiedCardNo", pRspTransfer->IdentifiedCardNo);
			///客户类型
			jsonData.put("CustType", pRspTransfer->CustType);
			///银行帐号
			jsonData.put("BankAccount", pRspTransfer->BankAccount);
			///银行密码
			jsonData.put("BankPassWord", pRspTransfer->BankPassWord);
			///投资者帐号
			jsonData.put("AccountID", pRspTransfer->AccountID);
			///期货密码
			jsonData.put("Password", pRspTransfer->Password);
			///期货公司流水号
			jsonData.put("FutureSerial", pRspTransfer->FutureSerial);
			///安装编号
			jsonData.put("InstallID", pRspTransfer->InstallID);
			///用户标识
			jsonData.put("UserID", pRspTransfer->UserID);
			///验证客户证件号码标志
			jsonData.put("VerifyCertNoFlag", pRspTransfer->VerifyCertNoFlag);
			///币种代码
			jsonData.put("CurrencyID", pRspTransfer->CurrencyID);
			///转帐金额
			jsonData.put("TradeAmount", pRspTransfer->TradeAmount);
			///期货可取金额
			jsonData.put("FutureFetchAmount", pRspTransfer->FutureFetchAmount);
			///费用支付标志
			jsonData.put("FeePayFlag", pRspTransfer->FeePayFlag);
			///应收客户费用
			jsonData.put("CustFee", pRspTransfer->CustFee);
			///应收期货公司费用
			jsonData.put("BrokerFee", pRspTransfer->BrokerFee);
			///发送方给接收方的消息
			jsonData.put("Message", pRspTransfer->Message);
			///摘要
			jsonData.put("Digest", pRspTransfer->Digest);
			///银行帐号类型
			jsonData.put("BankAccType", pRspTransfer->BankAccType);
			///渠道标志
			jsonData.put("DeviceID", pRspTransfer->DeviceID);
			///期货单位帐号类型
			jsonData.put("BankSecuAccType", pRspTransfer->BankSecuAccType);
			///期货公司银行编码
			jsonData.put("BrokerIDByBank", pRspTransfer->BrokerIDByBank);
			///期货单位帐号
			jsonData.put("BankSecuAcc", pRspTransfer->BankSecuAcc);
			///银行密码标志
			jsonData.put("BankPwdFlag", pRspTransfer->BankPwdFlag);
			///期货资金密码核对标志
			jsonData.put("SecuPwdFlag", pRspTransfer->SecuPwdFlag);
			///交易柜员
			jsonData.put("OperNo", pRspTransfer->OperNo);
			///请求编号
			jsonData.put("RequestID", pRspTransfer->RequestID);
			///交易ID
			jsonData.put("TID", pRspTransfer->TID);
			///转账交易状态
			jsonData.put("TransferStatus", pRspTransfer->TransferStatus);
			///错误代码
			jsonData.put("ErrorID", pRspTransfer->ErrorID);
			///错误信息
			jsonData.put("ErrorMsg", pRspTransfer->ErrorMsg);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///期货发起期货资金转银行通知
void CTPApi::OnRtnFromFutureToBankByFuture(CThostFtdcRspTransferField *pRspTransfer) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRtnFromFutureToBankByFuture")) {
			//构造Data
			boost::property_tree::ptree jsonData;
			///业务功能码
			jsonData.put("TradeCode", pRspTransfer->TradeCode);
			///银行代码
			jsonData.put("BankID", pRspTransfer->BankID);
			///银行分支机构代码
			jsonData.put("BankBranchID", pRspTransfer->BankBranchID);
			///期商代码
			jsonData.put("BrokerID", pRspTransfer->BrokerID);
			///期商分支机构代码
			jsonData.put("BrokerBranchID", pRspTransfer->BrokerBranchID);
			///交易日期
			jsonData.put("TradeDate", pRspTransfer->TradeDate);
			///交易时间
			jsonData.put("TradeTime", pRspTransfer->TradeTime);
			///银行流水号
			jsonData.put("BankSerial", pRspTransfer->BankSerial);
			///交易系统日期 
			jsonData.put("TradingDay", pRspTransfer->TradingDay);
			///银期平台消息流水号
			jsonData.put("PlateSerial", pRspTransfer->PlateSerial);
			///最后分片标志
			jsonData.put("LastFragment", pRspTransfer->LastFragment);
			///会话号
			jsonData.put("SessionID", pRspTransfer->SessionID);
			///客户姓名
			jsonData.put("CustomerName", pRspTransfer->CustomerName);
			///证件类型
			jsonData.put("IdCardType", pRspTransfer->IdCardType);
			///证件号码
			jsonData.put("IdentifiedCardNo", pRspTransfer->IdentifiedCardNo);
			///客户类型
			jsonData.put("CustType", pRspTransfer->CustType);
			///银行帐号
			jsonData.put("BankAccount", pRspTransfer->BankAccount);
			///银行密码
			jsonData.put("BankPassWord", pRspTransfer->BankPassWord);
			///投资者帐号
			jsonData.put("AccountID", pRspTransfer->AccountID);
			///期货密码
			jsonData.put("Password", pRspTransfer->Password);
			///期货公司流水号
			jsonData.put("FutureSerial", pRspTransfer->FutureSerial);
			///安装编号
			jsonData.put("InstallID", pRspTransfer->InstallID);
			///用户标识
			jsonData.put("UserID", pRspTransfer->UserID);
			///验证客户证件号码标志
			jsonData.put("VerifyCertNoFlag", pRspTransfer->VerifyCertNoFlag);
			///币种代码
			jsonData.put("CurrencyID", pRspTransfer->CurrencyID);
			///转帐金额
			jsonData.put("TradeAmount", pRspTransfer->TradeAmount);
			///期货可取金额
			jsonData.put("FutureFetchAmount", pRspTransfer->FutureFetchAmount);
			///费用支付标志
			jsonData.put("FeePayFlag", pRspTransfer->FeePayFlag);
			///应收客户费用
			jsonData.put("CustFee", pRspTransfer->CustFee);
			///应收期货公司费用
			jsonData.put("BrokerFee", pRspTransfer->BrokerFee);
			///发送方给接收方的消息
			jsonData.put("Message", pRspTransfer->Message);
			///摘要
			jsonData.put("Digest", pRspTransfer->Digest);
			///银行帐号类型
			jsonData.put("BankAccType", pRspTransfer->BankAccType);
			///渠道标志
			jsonData.put("DeviceID", pRspTransfer->DeviceID);
			///期货单位帐号类型
			jsonData.put("BankSecuAccType", pRspTransfer->BankSecuAccType);
			///期货公司银行编码
			jsonData.put("BrokerIDByBank", pRspTransfer->BrokerIDByBank);
			///期货单位帐号
			jsonData.put("BankSecuAcc", pRspTransfer->BankSecuAcc);
			///银行密码标志
			jsonData.put("BankPwdFlag", pRspTransfer->BankPwdFlag);
			///期货资金密码核对标志
			jsonData.put("SecuPwdFlag", pRspTransfer->SecuPwdFlag);
			///交易柜员
			jsonData.put("OperNo", pRspTransfer->OperNo);
			///请求编号
			jsonData.put("RequestID", pRspTransfer->RequestID);
			///交易ID
			jsonData.put("TID", pRspTransfer->TID);
			///转账交易状态
			jsonData.put("TransferStatus", pRspTransfer->TransferStatus);
			///错误代码
			jsonData.put("ErrorID", pRspTransfer->ErrorID);
			///错误信息
			jsonData.put("ErrorMsg", pRspTransfer->ErrorMsg);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///系统运行时期货端手工发起冲正银行转期货请求，银行处理完毕后报盘发回的通知
void CTPApi::OnRtnRepealFromBankToFutureByFutureManual(CThostFtdcRspRepealField *pRspRepeal) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRtnRepealFromBankToFutureByFutureManual")) {
			//构造Data
			boost::property_tree::ptree jsonData;

			///冲正时间间隔
			jsonData.put("RepealTimeInterval", pRspRepeal->RepealTimeInterval);
			///已经冲正次数
			jsonData.put("RepealedTimes", pRspRepeal->RepealedTimes);
			///银行冲正标志
			jsonData.put("BankRepealFlag", pRspRepeal->BankRepealFlag);
			///期商冲正标志
			jsonData.put("BrokerRepealFlag", pRspRepeal->BrokerRepealFlag);
			///被冲正平台流水号
			jsonData.put("PlateRepealSerial", pRspRepeal->PlateRepealSerial);
			///被冲正银行流水号
			jsonData.put("BankRepealSerial", pRspRepeal->BankRepealSerial);
			///被冲正期货流水号
			jsonData.put("FutureRepealSerial", pRspRepeal->FutureRepealSerial);
			///业务功能码
			jsonData.put("TradeCode", pRspRepeal->TradeCode);
			///银行代码
			jsonData.put("BankID", pRspRepeal->BankID);
			///银行分支机构代码
			jsonData.put("BankBranchID", pRspRepeal->BankBranchID);
			///期商代码
			jsonData.put("BrokerID", pRspRepeal->BrokerID);
			///期商分支机构代码
			jsonData.put("BrokerBranchID", pRspRepeal->BrokerBranchID);
			///交易日期
			jsonData.put("TradeDate", pRspRepeal->TradeDate);
			///交易时间
			jsonData.put("TradeTime", pRspRepeal->TradeTime);
			///银行流水号
			jsonData.put("BankSerial", pRspRepeal->BankSerial);
			///交易系统日期 
			jsonData.put("TradingDay", pRspRepeal->TradingDay);
			///银期平台消息流水号
			jsonData.put("PlateSerial", pRspRepeal->PlateSerial);
			///最后分片标志
			jsonData.put("LastFragment", pRspRepeal->LastFragment);
			///会话号
			jsonData.put("SessionID", pRspRepeal->SessionID);
			///客户姓名
			jsonData.put("CustomerName", pRspRepeal->CustomerName);
			///证件类型
			jsonData.put("IdCardType", pRspRepeal->IdCardType);
			///证件号码
			jsonData.put("IdentifiedCardNo", pRspRepeal->IdentifiedCardNo);
			///客户类型
			jsonData.put("CustType", pRspRepeal->CustType);
			///银行帐号
			jsonData.put("BankAccount", pRspRepeal->BankAccount);
			///银行密码
			jsonData.put("BankPassWord", pRspRepeal->BankPassWord);
			///投资者帐号
			jsonData.put("AccountID", pRspRepeal->AccountID);
			///期货密码
			jsonData.put("Password", pRspRepeal->Password);
			///期货公司流水号
			jsonData.put("FutureSerial", pRspRepeal->FutureSerial);
			///安装编号
			jsonData.put("InstallID", pRspRepeal->InstallID);
			///用户标识
			jsonData.put("UserID", pRspRepeal->UserID);
			///验证客户证件号码标志
			jsonData.put("VerifyCertNoFlag", pRspRepeal->VerifyCertNoFlag);
			///币种代码
			jsonData.put("CurrencyID", pRspRepeal->CurrencyID);
			///转帐金额
			jsonData.put("TradeAmount", pRspRepeal->TradeAmount);
			///期货可取金额
			jsonData.put("FutureFetchAmount", pRspRepeal->FutureFetchAmount);
			///费用支付标志
			jsonData.put("FeePayFlag", pRspRepeal->FeePayFlag);
			///应收客户费用
			jsonData.put("CustFee", pRspRepeal->CustFee);
			///应收期货公司费用
			jsonData.put("BrokerFee", pRspRepeal->BrokerFee);
			///发送方给接收方的消息
			jsonData.put("Message", pRspRepeal->Message);
			///摘要
			jsonData.put("Digest", pRspRepeal->Digest);
			///银行帐号类型
			jsonData.put("BankAccType", pRspRepeal->BankAccType);
			///渠道标志
			jsonData.put("DeviceID", pRspRepeal->DeviceID);
			///期货单位帐号类型
			jsonData.put("BankSecuAccType", pRspRepeal->BankSecuAccType);
			///期货公司银行编码
			jsonData.put("BrokerIDByBank", pRspRepeal->BrokerIDByBank);
			///期货单位帐号
			jsonData.put("BankSecuAcc", pRspRepeal->BankSecuAcc);
			///银行密码标志
			jsonData.put("BankPwdFlag", pRspRepeal->BankPwdFlag);
			///期货资金密码核对标志
			jsonData.put("SecuPwdFlag", pRspRepeal->SecuPwdFlag);
			///交易柜员
			jsonData.put("OperNo", pRspRepeal->OperNo);
			///请求编号
			jsonData.put("RequestID", pRspRepeal->RequestID);
			///交易ID
			jsonData.put("TID", pRspRepeal->TID);
			///转账交易状态
			jsonData.put("TransferStatus", pRspRepeal->TransferStatus);
			///错误代码
			jsonData.put("ErrorID", pRspRepeal->ErrorID);
			///错误信息
			jsonData.put("ErrorMsg", pRspRepeal->ErrorMsg);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///系统运行时期货端手工发起冲正期货转银行请求，银行处理完毕后报盘发回的通知
void CTPApi::OnRtnRepealFromFutureToBankByFutureManual(CThostFtdcRspRepealField *pRspRepeal) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRtnRepealFromFutureToBankByFutureManual")) {
			//构造Data
			boost::property_tree::ptree jsonData;

			///冲正时间间隔
			jsonData.put("RepealTimeInterval", pRspRepeal->RepealTimeInterval);
			///已经冲正次数
			jsonData.put("RepealedTimes", pRspRepeal->RepealedTimes);
			///银行冲正标志
			jsonData.put("BankRepealFlag", pRspRepeal->BankRepealFlag);
			///期商冲正标志
			jsonData.put("BrokerRepealFlag", pRspRepeal->BrokerRepealFlag);
			///被冲正平台流水号
			jsonData.put("PlateRepealSerial", pRspRepeal->PlateRepealSerial);
			///被冲正银行流水号
			jsonData.put("BankRepealSerial", pRspRepeal->BankRepealSerial);
			///被冲正期货流水号
			jsonData.put("FutureRepealSerial", pRspRepeal->FutureRepealSerial);
			///业务功能码
			jsonData.put("TradeCode", pRspRepeal->TradeCode);
			///银行代码
			jsonData.put("BankID", pRspRepeal->BankID);
			///银行分支机构代码
			jsonData.put("BankBranchID", pRspRepeal->BankBranchID);
			///期商代码
			jsonData.put("BrokerID", pRspRepeal->BrokerID);
			///期商分支机构代码
			jsonData.put("BrokerBranchID", pRspRepeal->BrokerBranchID);
			///交易日期
			jsonData.put("TradeDate", pRspRepeal->TradeDate);
			///交易时间
			jsonData.put("TradeTime", pRspRepeal->TradeTime);
			///银行流水号
			jsonData.put("BankSerial", pRspRepeal->BankSerial);
			///交易系统日期 
			jsonData.put("TradingDay", pRspRepeal->TradingDay);
			///银期平台消息流水号
			jsonData.put("PlateSerial", pRspRepeal->PlateSerial);
			///最后分片标志
			jsonData.put("LastFragment", pRspRepeal->LastFragment);
			///会话号
			jsonData.put("SessionID", pRspRepeal->SessionID);
			///客户姓名
			jsonData.put("CustomerName", pRspRepeal->CustomerName);
			///证件类型
			jsonData.put("IdCardType", pRspRepeal->IdCardType);
			///证件号码
			jsonData.put("IdentifiedCardNo", pRspRepeal->IdentifiedCardNo);
			///客户类型
			jsonData.put("CustType", pRspRepeal->CustType);
			///银行帐号
			jsonData.put("BankAccount", pRspRepeal->BankAccount);
			///银行密码
			jsonData.put("BankPassWord", pRspRepeal->BankPassWord);
			///投资者帐号
			jsonData.put("AccountID", pRspRepeal->AccountID);
			///期货密码
			jsonData.put("Password", pRspRepeal->Password);
			///期货公司流水号
			jsonData.put("FutureSerial", pRspRepeal->FutureSerial);
			///安装编号
			jsonData.put("InstallID", pRspRepeal->InstallID);
			///用户标识
			jsonData.put("UserID", pRspRepeal->UserID);
			///验证客户证件号码标志
			jsonData.put("VerifyCertNoFlag", pRspRepeal->VerifyCertNoFlag);
			///币种代码
			jsonData.put("CurrencyID", pRspRepeal->CurrencyID);
			///转帐金额
			jsonData.put("TradeAmount", pRspRepeal->TradeAmount);
			///期货可取金额
			jsonData.put("FutureFetchAmount", pRspRepeal->FutureFetchAmount);
			///费用支付标志
			jsonData.put("FeePayFlag", pRspRepeal->FeePayFlag);
			///应收客户费用
			jsonData.put("CustFee", pRspRepeal->CustFee);
			///应收期货公司费用
			jsonData.put("BrokerFee", pRspRepeal->BrokerFee);
			///发送方给接收方的消息
			jsonData.put("Message", pRspRepeal->Message);
			///摘要
			jsonData.put("Digest", pRspRepeal->Digest);
			///银行帐号类型
			jsonData.put("BankAccType", pRspRepeal->BankAccType);
			///渠道标志
			jsonData.put("DeviceID", pRspRepeal->DeviceID);
			///期货单位帐号类型
			jsonData.put("BankSecuAccType", pRspRepeal->BankSecuAccType);
			///期货公司银行编码
			jsonData.put("BrokerIDByBank", pRspRepeal->BrokerIDByBank);
			///期货单位帐号
			jsonData.put("BankSecuAcc", pRspRepeal->BankSecuAcc);
			///银行密码标志
			jsonData.put("BankPwdFlag", pRspRepeal->BankPwdFlag);
			///期货资金密码核对标志
			jsonData.put("SecuPwdFlag", pRspRepeal->SecuPwdFlag);
			///交易柜员
			jsonData.put("OperNo", pRspRepeal->OperNo);
			///请求编号
			jsonData.put("RequestID", pRspRepeal->RequestID);
			///交易ID
			jsonData.put("TID", pRspRepeal->TID);
			///转账交易状态
			jsonData.put("TransferStatus", pRspRepeal->TransferStatus);
			///错误代码
			jsonData.put("ErrorID", pRspRepeal->ErrorID);
			///错误信息
			jsonData.put("ErrorMsg", pRspRepeal->ErrorMsg);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///期货发起查询银行余额通知
void CTPApi::OnRtnQueryBankBalanceByFuture(CThostFtdcNotifyQueryAccountField *pNotifyQueryAccount) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRtnQueryBankBalanceByFuture")) {
			//构造Data
			boost::property_tree::ptree jsonData;
			///业务功能码
			jsonData.put("TradeCode", pNotifyQueryAccount->TradeCode);
			///银行代码
			jsonData.put("BankID", pNotifyQueryAccount->BankID);
			///银行分支机构代码
			jsonData.put("BankBranchID", pNotifyQueryAccount->BankBranchID);
			///期商代码
			jsonData.put("BrokerID", pNotifyQueryAccount->BrokerID);
			///期商分支机构代码
			jsonData.put("BrokerBranchID", pNotifyQueryAccount->BrokerBranchID);
			///交易日期
			jsonData.put("TradeDate", pNotifyQueryAccount->TradeDate);
			///交易时间
			jsonData.put("TradeTime", pNotifyQueryAccount->TradeTime);
			///银行流水号
			jsonData.put("BankSerial", pNotifyQueryAccount->BankSerial);
			///交易系统日期 
			jsonData.put("TradingDay", pNotifyQueryAccount->TradingDay);
			///银期平台消息流水号
			jsonData.put("PlateSerial", pNotifyQueryAccount->PlateSerial);
			///最后分片标志
			jsonData.put("LastFragment", pNotifyQueryAccount->LastFragment);
			///会话号
			jsonData.put("SessionID", pNotifyQueryAccount->SessionID);
			///客户姓名
			jsonData.put("CustomerName", pNotifyQueryAccount->CustomerName);
			///证件类型
			jsonData.put("IdCardType", pNotifyQueryAccount->IdCardType);
			///证件号码
			jsonData.put("IdentifiedCardNo", pNotifyQueryAccount->IdentifiedCardNo);
			///客户类型
			jsonData.put("CustType", pNotifyQueryAccount->CustType);
			///银行帐号
			jsonData.put("BankAccount", pNotifyQueryAccount->BankAccount);
			///银行密码
			jsonData.put("BankPassWord", pNotifyQueryAccount->BankPassWord);
			///投资者帐号
			jsonData.put("AccountID", pNotifyQueryAccount->AccountID);
			///期货密码
			jsonData.put("Password", pNotifyQueryAccount->Password);
			///期货公司流水号
			jsonData.put("FutureSerial", pNotifyQueryAccount->FutureSerial);
			///安装编号
			jsonData.put("InstallID", pNotifyQueryAccount->InstallID);
			///用户标识
			jsonData.put("UserID", pNotifyQueryAccount->UserID);
			///验证客户证件号码标志
			jsonData.put("VerifyCertNoFlag", pNotifyQueryAccount->VerifyCertNoFlag);
			///币种代码
			jsonData.put("CurrencyID", pNotifyQueryAccount->CurrencyID);
			///摘要
			jsonData.put("Digest", pNotifyQueryAccount->Digest);
			///银行帐号类型
			jsonData.put("BankAccType", pNotifyQueryAccount->BankAccType);
			///渠道标志
			jsonData.put("DeviceID", pNotifyQueryAccount->DeviceID);
			///期货单位帐号类型
			jsonData.put("BankSecuAccType", pNotifyQueryAccount->BankSecuAccType);
			///期货公司银行编码
			jsonData.put("BrokerIDByBank", pNotifyQueryAccount->BrokerIDByBank);
			///期货单位帐号
			jsonData.put("BankSecuAcc", pNotifyQueryAccount->BankSecuAcc);
			///银行密码标志
			jsonData.put("BankPwdFlag", pNotifyQueryAccount->BankPwdFlag);
			///期货资金密码核对标志
			jsonData.put("SecuPwdFlag", pNotifyQueryAccount->SecuPwdFlag);
			///交易柜员
			jsonData.put("OperNo", pNotifyQueryAccount->OperNo);
			///请求编号
			jsonData.put("RequestID", pNotifyQueryAccount->RequestID);
			///交易ID
			jsonData.put("TID", pNotifyQueryAccount->TID);
			///银行可用金额
			jsonData.put("BankUseAmount", pNotifyQueryAccount->BankUseAmount);
			///银行可取金额
			jsonData.put("BankFetchAmount", pNotifyQueryAccount->BankFetchAmount);
			///错误代码
			jsonData.put("ErrorID", pNotifyQueryAccount->ErrorID);
			///错误信息
			jsonData.put("ErrorMsg", pNotifyQueryAccount->ErrorMsg);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///期货发起银行资金转期货错误回报
void CTPApi::OnErrRtnBankToFutureByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnErrRtnBankToFutureByFuture", pRspInfo)) {
			//构造Data
			boost::property_tree::ptree jsonData;
			///业务功能码
			jsonData.put("TradeCode", pReqTransfer->TradeCode);
			///银行代码
			jsonData.put("BankID", pReqTransfer->BankID);
			///银行分支机构代码
			jsonData.put("BankBranchID", pReqTransfer->BankBranchID);
			///期商代码
			jsonData.put("BrokerID", pReqTransfer->BrokerID);
			///期商分支机构代码
			jsonData.put("BrokerBranchID", pReqTransfer->BrokerBranchID);
			///交易日期
			jsonData.put("TradeDate", pReqTransfer->TradeDate);
			///交易时间
			jsonData.put("TradeTime", pReqTransfer->TradeTime);
			///银行流水号
			jsonData.put("BankSerial", pReqTransfer->BankSerial);
			///交易系统日期 
			jsonData.put("TradingDay", pReqTransfer->TradingDay);
			///银期平台消息流水号
			jsonData.put("PlateSerial", pReqTransfer->PlateSerial);
			///最后分片标志
			jsonData.put("LastFragment", pReqTransfer->LastFragment);
			///会话号
			jsonData.put("SessionID", pReqTransfer->SessionID);
			///客户姓名
			jsonData.put("CustomerName", pReqTransfer->CustomerName);
			///证件类型
			jsonData.put("IdCardType", pReqTransfer->IdCardType);
			///证件号码
			jsonData.put("IdentifiedCardNo", pReqTransfer->IdentifiedCardNo);
			///客户类型
			jsonData.put("CustType", pReqTransfer->CustType);
			///银行帐号
			jsonData.put("BankAccount", pReqTransfer->BankAccount);
			///银行密码
			jsonData.put("BankPassWord", pReqTransfer->BankPassWord);
			///投资者帐号
			jsonData.put("AccountID", pReqTransfer->AccountID);
			///期货密码
			jsonData.put("Password", pReqTransfer->Password);
			///期货公司流水号
			jsonData.put("FutureSerial", pReqTransfer->FutureSerial);
			///安装编号
			jsonData.put("InstallID", pReqTransfer->InstallID);
			///用户标识
			jsonData.put("UserID", pReqTransfer->UserID);
			///验证客户证件号码标志
			jsonData.put("VerifyCertNoFlag", pReqTransfer->VerifyCertNoFlag);
			///币种代码
			jsonData.put("CurrencyID", pReqTransfer->CurrencyID);
			///摘要
			jsonData.put("Digest", pReqTransfer->Digest);
			///银行帐号类型
			jsonData.put("BankAccType", pReqTransfer->BankAccType);
			///渠道标志
			jsonData.put("DeviceID", pReqTransfer->DeviceID);
			///期货单位帐号类型
			jsonData.put("BankSecuAccType", pReqTransfer->BankSecuAccType);
			///期货公司银行编码
			jsonData.put("BrokerIDByBank", pReqTransfer->BrokerIDByBank);
			///期货单位帐号
			jsonData.put("BankSecuAcc", pReqTransfer->BankSecuAcc);
			///银行密码标志
			jsonData.put("BankPwdFlag", pReqTransfer->BankPwdFlag);
			///期货资金密码核对标志
			jsonData.put("SecuPwdFlag", pReqTransfer->SecuPwdFlag);
			///交易柜员
			jsonData.put("OperNo", pReqTransfer->OperNo);
			///请求编号
			jsonData.put("RequestID", pReqTransfer->RequestID);
			///交易ID
			jsonData.put("TID", pReqTransfer->TID);
			///转账交易状态
			jsonData.put("TransferStatus", pReqTransfer->TransferStatus);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///期货发起期货资金转银行错误回报
void CTPApi::OnErrRtnFutureToBankByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnErrRtnFutureToBankByFuture", pRspInfo)) {
			//构造Data
			boost::property_tree::ptree jsonData;
			///业务功能码
			jsonData.put("TradeCode", pReqTransfer->TradeCode);
			///银行代码
			jsonData.put("BankID", pReqTransfer->BankID);
			///银行分支机构代码
			jsonData.put("BankBranchID", pReqTransfer->BankBranchID);
			///期商代码
			jsonData.put("BrokerID", pReqTransfer->BrokerID);
			///期商分支机构代码
			jsonData.put("BrokerBranchID", pReqTransfer->BrokerBranchID);
			///交易日期
			jsonData.put("TradeDate", pReqTransfer->TradeDate);
			///交易时间
			jsonData.put("TradeTime", pReqTransfer->TradeTime);
			///银行流水号
			jsonData.put("BankSerial", pReqTransfer->BankSerial);
			///交易系统日期 
			jsonData.put("TradingDay", pReqTransfer->TradingDay);
			///银期平台消息流水号
			jsonData.put("PlateSerial", pReqTransfer->PlateSerial);
			///最后分片标志
			jsonData.put("LastFragment", pReqTransfer->LastFragment);
			///会话号
			jsonData.put("SessionID", pReqTransfer->SessionID);
			///客户姓名
			jsonData.put("CustomerName", pReqTransfer->CustomerName);
			///证件类型
			jsonData.put("IdCardType", pReqTransfer->IdCardType);
			///证件号码
			jsonData.put("IdentifiedCardNo", pReqTransfer->IdentifiedCardNo);
			///客户类型
			jsonData.put("CustType", pReqTransfer->CustType);
			///银行帐号
			jsonData.put("BankAccount", pReqTransfer->BankAccount);
			///银行密码
			jsonData.put("BankPassWord", pReqTransfer->BankPassWord);
			///投资者帐号
			jsonData.put("AccountID", pReqTransfer->AccountID);
			///期货密码
			jsonData.put("Password", pReqTransfer->Password);
			///期货公司流水号
			jsonData.put("FutureSerial", pReqTransfer->FutureSerial);
			///安装编号
			jsonData.put("InstallID", pReqTransfer->InstallID);
			///用户标识
			jsonData.put("UserID", pReqTransfer->UserID);
			///验证客户证件号码标志
			jsonData.put("VerifyCertNoFlag", pReqTransfer->VerifyCertNoFlag);
			///币种代码
			jsonData.put("CurrencyID", pReqTransfer->CurrencyID);
			///转帐金额
			jsonData.put("TradeAmount", pReqTransfer->TradeAmount);
			///期货可取金额
			jsonData.put("FutureFetchAmount", pReqTransfer->FutureFetchAmount);
			///费用支付标志
			jsonData.put("FeePayFlag", pReqTransfer->FeePayFlag);
			///应收客户费用
			jsonData.put("CustFee", pReqTransfer->CustFee);
			///应收期货公司费用
			jsonData.put("BrokerFee", pReqTransfer->BrokerFee);
			///发送方给接收方的消息
			jsonData.put("Message", pReqTransfer->Message);
			///摘要
			jsonData.put("Digest", pReqTransfer->Digest);
			///银行帐号类型
			jsonData.put("BankAccType", pReqTransfer->BankAccType);
			///渠道标志
			jsonData.put("DeviceID", pReqTransfer->DeviceID);
			///期货单位帐号类型
			jsonData.put("BankSecuAccType", pReqTransfer->BankSecuAccType);
			///期货公司银行编码
			jsonData.put("BrokerIDByBank", pReqTransfer->BrokerIDByBank);
			///期货单位帐号
			jsonData.put("BankSecuAcc", pReqTransfer->BankSecuAcc);
			///银行密码标志
			jsonData.put("BankPwdFlag", pReqTransfer->BankPwdFlag);
			///期货资金密码核对标志
			jsonData.put("SecuPwdFlag", pReqTransfer->SecuPwdFlag);
			///交易柜员
			jsonData.put("OperNo", pReqTransfer->OperNo);
			///请求编号
			jsonData.put("RequestID", pReqTransfer->RequestID);
			///交易ID
			jsonData.put("TID", pReqTransfer->TID);
			///转账交易状态
			jsonData.put("TransferStatus", pReqTransfer->TransferStatus);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///系统运行时期货端手工发起冲正银行转期货错误回报
void CTPApi::OnErrRtnRepealBankToFutureByFutureManual(CThostFtdcReqRepealField *pReqRepeal, CThostFtdcRspInfoField *pRspInfo) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnErrRtnRepealBankToFutureByFutureManual", pRspInfo)) {
			//构造Data
			boost::property_tree::ptree jsonData;
			///冲正时间间隔
			jsonData.put("RepealTimeInterval", pReqRepeal->RepealTimeInterval);
			///已经冲正次数
			jsonData.put("RepealedTimes", pReqRepeal->RepealedTimes);
			///银行冲正标志
			jsonData.put("BankRepealFlag", pReqRepeal->BankRepealFlag);
			///期商冲正标志
			jsonData.put("BrokerRepealFlag", pReqRepeal->BrokerRepealFlag);
			///被冲正平台流水号
			jsonData.put("PlateRepealSerial", pReqRepeal->PlateRepealSerial);
			///被冲正银行流水号
			jsonData.put("BankRepealSerial", pReqRepeal->BankRepealSerial);
			///被冲正期货流水号
			jsonData.put("FutureRepealSerial", pReqRepeal->FutureRepealSerial);
			///业务功能码
			jsonData.put("TradeCode", pReqRepeal->TradeCode);
			///银行代码
			jsonData.put("BankID", pReqRepeal->BankID);
			///银行分支机构代码
			jsonData.put("BankBranchID", pReqRepeal->BankBranchID);
			///期商代码
			jsonData.put("BrokerID", pReqRepeal->BrokerID);
			///期商分支机构代码
			jsonData.put("BrokerBranchID", pReqRepeal->BrokerBranchID);
			///交易日期
			jsonData.put("TradeDate", pReqRepeal->TradeDate);
			///交易时间
			jsonData.put("TradeTime", pReqRepeal->TradeTime);
			///银行流水号
			jsonData.put("BankSerial", pReqRepeal->BankSerial);
			///交易系统日期 
			jsonData.put("TradingDay", pReqRepeal->TradingDay);
			///银期平台消息流水号
			jsonData.put("PlateSerial", pReqRepeal->PlateSerial);
			///最后分片标志
			jsonData.put("LastFragment", pReqRepeal->LastFragment);
			///会话号
			jsonData.put("SessionID", pReqRepeal->SessionID);
			///客户姓名
			jsonData.put("CustomerName", pReqRepeal->CustomerName);
			///证件类型
			jsonData.put("IdCardType", pReqRepeal->IdCardType);
			///证件号码
			jsonData.put("IdentifiedCardNo", pReqRepeal->IdentifiedCardNo);
			///客户类型
			jsonData.put("CustType", pReqRepeal->CustType);
			///银行帐号
			jsonData.put("BankAccount", pReqRepeal->BankAccount);
			///银行密码
			jsonData.put("BankPassWord", pReqRepeal->BankPassWord);
			///投资者帐号
			jsonData.put("AccountID", pReqRepeal->AccountID);
			///期货密码
			jsonData.put("Password", pReqRepeal->Password);
			///期货公司流水号
			jsonData.put("FutureSerial", pReqRepeal->FutureSerial);
			///安装编号
			jsonData.put("InstallID", pReqRepeal->InstallID);
			///用户标识
			jsonData.put("UserID", pReqRepeal->UserID);
			///验证客户证件号码标志
			jsonData.put("VerifyCertNoFlag", pReqRepeal->VerifyCertNoFlag);
			///币种代码
			jsonData.put("CurrencyID", pReqRepeal->CurrencyID);
			///转帐金额
			jsonData.put("TradeAmount", pReqRepeal->TradeAmount);
			///期货可取金额
			jsonData.put("FutureFetchAmount", pReqRepeal->FutureFetchAmount);
			///费用支付标志
			jsonData.put("FeePayFlag", pReqRepeal->FeePayFlag);
			///应收客户费用
			jsonData.put("CustFee", pReqRepeal->CustFee);
			///应收期货公司费用
			jsonData.put("BrokerFee", pReqRepeal->BrokerFee);
			///发送方给接收方的消息
			jsonData.put("Message", pReqRepeal->Message);
			///摘要
			jsonData.put("Digest", pReqRepeal->Digest);
			///银行帐号类型
			jsonData.put("BankAccType", pReqRepeal->BankAccType);
			///渠道标志
			jsonData.put("DeviceID", pReqRepeal->DeviceID);
			///期货单位帐号类型
			jsonData.put("BankSecuAccType", pReqRepeal->BankSecuAccType);
			///期货公司银行编码
			jsonData.put("BrokerIDByBank", pReqRepeal->BrokerIDByBank);
			///期货单位帐号
			jsonData.put("BankSecuAcc", pReqRepeal->BankSecuAcc);
			///银行密码标志
			jsonData.put("BankPwdFlag", pReqRepeal->BankPwdFlag);
			///期货资金密码核对标志
			jsonData.put("SecuPwdFlag", pReqRepeal->SecuPwdFlag);
			///交易柜员
			jsonData.put("OperNo", pReqRepeal->OperNo);
			///请求编号
			jsonData.put("RequestID", pReqRepeal->RequestID);
			///交易ID
			jsonData.put("TID", pReqRepeal->TID);
			///转账交易状态
			jsonData.put("TransferStatus", pReqRepeal->TransferStatus);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///系统运行时期货端手工发起冲正期货转银行错误回报
void CTPApi::OnErrRtnRepealFutureToBankByFutureManual(CThostFtdcReqRepealField *pReqRepeal, CThostFtdcRspInfoField *pRspInfo) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnErrRtnRepealFutureToBankByFutureManual", pRspInfo)) {
			//构造Data
			boost::property_tree::ptree jsonData;
			///冲正时间间隔
			jsonData.put("RepealTimeInterval", pReqRepeal->RepealTimeInterval);
			///已经冲正次数
			jsonData.put("RepealedTimes", pReqRepeal->RepealedTimes);
			///银行冲正标志
			jsonData.put("BankRepealFlag", pReqRepeal->BankRepealFlag);
			///期商冲正标志
			jsonData.put("BrokerRepealFlag", pReqRepeal->BrokerRepealFlag);
			///被冲正平台流水号
			jsonData.put("PlateRepealSerial", pReqRepeal->PlateRepealSerial);
			///被冲正银行流水号
			jsonData.put("BankRepealSerial", pReqRepeal->BankRepealSerial);
			///被冲正期货流水号
			jsonData.put("FutureRepealSerial", pReqRepeal->FutureRepealSerial);
			///业务功能码
			jsonData.put("TradeCode", pReqRepeal->TradeCode);
			///银行代码
			jsonData.put("BankID", pReqRepeal->BankID);
			///银行分支机构代码
			jsonData.put("BankBranchID", pReqRepeal->BankBranchID);
			///期商代码
			jsonData.put("BrokerID", pReqRepeal->BrokerID);
			///期商分支机构代码
			jsonData.put("BrokerBranchID", pReqRepeal->BrokerBranchID);
			///交易日期
			jsonData.put("TradeDate", pReqRepeal->TradeDate);
			///交易时间
			jsonData.put("TradeTime", pReqRepeal->TradeTime);
			///银行流水号
			jsonData.put("BankSerial", pReqRepeal->BankSerial);
			///交易系统日期 
			jsonData.put("TradingDay", pReqRepeal->TradingDay);
			///银期平台消息流水号
			jsonData.put("PlateSerial", pReqRepeal->PlateSerial);
			///最后分片标志
			jsonData.put("LastFragment", pReqRepeal->LastFragment);
			///会话号
			jsonData.put("SessionID", pReqRepeal->SessionID);
			///客户姓名
			jsonData.put("CustomerName", pReqRepeal->CustomerName);
			///证件类型
			jsonData.put("IdCardType", pReqRepeal->IdCardType);
			///证件号码
			jsonData.put("IdentifiedCardNo", pReqRepeal->IdentifiedCardNo);
			///客户类型
			jsonData.put("CustType", pReqRepeal->CustType);
			///银行帐号
			jsonData.put("BankAccount", pReqRepeal->BankAccount);
			///银行密码
			jsonData.put("BankPassWord", pReqRepeal->BankPassWord);
			///投资者帐号
			jsonData.put("AccountID", pReqRepeal->AccountID);
			///期货密码
			jsonData.put("Password", pReqRepeal->Password);
			///期货公司流水号
			jsonData.put("FutureSerial", pReqRepeal->FutureSerial);
			///安装编号
			jsonData.put("InstallID", pReqRepeal->InstallID);
			///用户标识
			jsonData.put("UserID", pReqRepeal->UserID);
			///验证客户证件号码标志
			jsonData.put("VerifyCertNoFlag", pReqRepeal->VerifyCertNoFlag);
			///币种代码
			jsonData.put("CurrencyID", pReqRepeal->CurrencyID);
			///转帐金额
			jsonData.put("TradeAmount", pReqRepeal->TradeAmount);
			///期货可取金额
			jsonData.put("FutureFetchAmount", pReqRepeal->FutureFetchAmount);
			///费用支付标志
			jsonData.put("FeePayFlag", pReqRepeal->FeePayFlag);
			///应收客户费用
			jsonData.put("CustFee", pReqRepeal->CustFee);
			///应收期货公司费用
			jsonData.put("BrokerFee", pReqRepeal->BrokerFee);
			///发送方给接收方的消息
			jsonData.put("Message", pReqRepeal->Message);
			///摘要
			jsonData.put("Digest", pReqRepeal->Digest);
			///银行帐号类型
			jsonData.put("BankAccType", pReqRepeal->BankAccType);
			///渠道标志
			jsonData.put("DeviceID", pReqRepeal->DeviceID);
			///期货单位帐号类型
			jsonData.put("BankSecuAccType", pReqRepeal->BankSecuAccType);
			///期货公司银行编码
			jsonData.put("BrokerIDByBank", pReqRepeal->BrokerIDByBank);
			///期货单位帐号
			jsonData.put("BankSecuAcc", pReqRepeal->BankSecuAcc);
			///银行密码标志
			jsonData.put("BankPwdFlag", pReqRepeal->BankPwdFlag);
			///期货资金密码核对标志
			jsonData.put("SecuPwdFlag", pReqRepeal->SecuPwdFlag);
			///交易柜员
			jsonData.put("OperNo", pReqRepeal->OperNo);
			///请求编号
			jsonData.put("RequestID", pReqRepeal->RequestID);
			///交易ID
			jsonData.put("TID", pReqRepeal->TID);
			///转账交易状态
			jsonData.put("TransferStatus", pReqRepeal->TransferStatus);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///期货发起查询银行余额错误回报
void CTPApi::OnErrRtnQueryBankBalanceByFuture(CThostFtdcReqQueryAccountField *pReqQueryAccount, CThostFtdcRspInfoField *pRspInfo) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnErrRtnQueryBankBalanceByFuture", pRspInfo)) {
			//构造Data
			boost::property_tree::ptree jsonData;
			///业务功能码
			jsonData.put("TradeCode", pReqQueryAccount->TradeCode);
			///银行代码
			jsonData.put("BankID", pReqQueryAccount->BankID);
			///银行分支机构代码
			jsonData.put("BankBranchID", pReqQueryAccount->BankBranchID);
			///期商代码
			jsonData.put("BrokerID", pReqQueryAccount->BrokerID);
			///期商分支机构代码
			jsonData.put("BrokerBranchID", pReqQueryAccount->BrokerBranchID);
			///交易日期
			jsonData.put("TradeDate", pReqQueryAccount->TradeDate);
			///交易时间
			jsonData.put("TradeTime", pReqQueryAccount->TradeTime);
			///银行流水号
			jsonData.put("BankSerial", pReqQueryAccount->BankSerial);
			///交易系统日期 
			jsonData.put("TradingDay", pReqQueryAccount->TradingDay);
			///银期平台消息流水号
			jsonData.put("PlateSerial", pReqQueryAccount->PlateSerial);
			///最后分片标志
			jsonData.put("LastFragment", pReqQueryAccount->LastFragment);
			///会话号
			jsonData.put("SessionID", pReqQueryAccount->SessionID);
			///客户姓名
			jsonData.put("CustomerName", pReqQueryAccount->CustomerName);
			///证件类型
			jsonData.put("IdCardType", pReqQueryAccount->IdCardType);
			///证件号码
			jsonData.put("IdentifiedCardNo", pReqQueryAccount->IdentifiedCardNo);
			///客户类型
			jsonData.put("CustType", pReqQueryAccount->CustType);
			///银行帐号
			jsonData.put("BankAccount", pReqQueryAccount->BankAccount);
			///银行密码
			jsonData.put("BankPassWord", pReqQueryAccount->BankPassWord);
			///投资者帐号
			jsonData.put("AccountID", pReqQueryAccount->AccountID);
			///期货密码
			jsonData.put("Password", pReqQueryAccount->Password);
			///期货公司流水号
			jsonData.put("FutureSerial", pReqQueryAccount->FutureSerial);
			///安装编号
			jsonData.put("InstallID", pReqQueryAccount->InstallID);
			///用户标识
			jsonData.put("UserID", pReqQueryAccount->UserID);
			///验证客户证件号码标志
			jsonData.put("VerifyCertNoFlag", pReqQueryAccount->VerifyCertNoFlag);
			///币种代码
			jsonData.put("CurrencyID", pReqQueryAccount->CurrencyID);
			///摘要
			jsonData.put("Digest", pReqQueryAccount->Digest);
			///银行帐号类型
			jsonData.put("BankAccType", pReqQueryAccount->BankAccType);
			///渠道标志
			jsonData.put("DeviceID", pReqQueryAccount->DeviceID);
			///期货公司银行编码
			jsonData.put("BrokerIDByBank", pReqQueryAccount->BrokerIDByBank);
			///银行密码标志
			jsonData.put("BankPwdFlag", pReqQueryAccount->BankPwdFlag);
			///期货资金密码核对标志
			jsonData.put("SecuPwdFlag", pReqQueryAccount->SecuPwdFlag);
			///交易柜员
			jsonData.put("OperNo", pReqQueryAccount->OperNo);
			///请求编号
			jsonData.put("RequestID", pReqQueryAccount->RequestID);
			///交易ID
			jsonData.put("TID", pReqQueryAccount->TID);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///期货发起冲正银行转期货请求，银行处理完毕后报盘发回的通知
void CTPApi::OnRtnRepealFromBankToFutureByFuture(CThostFtdcRspRepealField *pRspRepeal) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRtnRepealFromBankToFutureByFuture")) {
			//构造Data
			boost::property_tree::ptree jsonData;
			///冲正时间间隔
			jsonData.put("RepealTimeInterval", pRspRepeal->RepealTimeInterval);
			///已经冲正次数
			jsonData.put("RepealedTimes", pRspRepeal->RepealedTimes);
			///银行冲正标志
			jsonData.put("BankRepealFlag", pRspRepeal->BankRepealFlag);
			///期商冲正标志
			jsonData.put("BrokerRepealFlag", pRspRepeal->BrokerRepealFlag);
			///被冲正平台流水号
			jsonData.put("PlateRepealSerial", pRspRepeal->PlateRepealSerial);
			///被冲正银行流水号
			jsonData.put("BankRepealSerial", pRspRepeal->BankRepealSerial);
			///被冲正期货流水号
			jsonData.put("FutureRepealSerial", pRspRepeal->FutureRepealSerial);
			///业务功能码
			jsonData.put("TradeCode", pRspRepeal->TradeCode);
			///银行代码
			jsonData.put("BankID", pRspRepeal->BankID);
			///银行分支机构代码
			jsonData.put("BankBranchID", pRspRepeal->BankBranchID);
			///期商代码
			jsonData.put("BrokerID", pRspRepeal->BrokerID);
			///期商分支机构代码
			jsonData.put("BrokerBranchID", pRspRepeal->BrokerBranchID);
			///交易日期
			jsonData.put("TradeDate", pRspRepeal->TradeDate);
			///交易时间
			jsonData.put("TradeTime", pRspRepeal->TradeTime);
			///银行流水号
			jsonData.put("BankSerial", pRspRepeal->BankSerial);
			///交易系统日期 
			jsonData.put("TradingDay", pRspRepeal->TradingDay);
			///银期平台消息流水号
			jsonData.put("PlateSerial", pRspRepeal->PlateSerial);
			///最后分片标志
			jsonData.put("LastFragment", pRspRepeal->LastFragment);
			///会话号
			jsonData.put("SessionID", pRspRepeal->SessionID);
			///客户姓名
			jsonData.put("CustomerName", pRspRepeal->CustomerName);
			///证件类型
			jsonData.put("IdCardType", pRspRepeal->IdCardType);
			///证件号码
			jsonData.put("IdentifiedCardNo", pRspRepeal->IdentifiedCardNo);
			///客户类型
			jsonData.put("CustType", pRspRepeal->CustType);
			///银行帐号
			jsonData.put("BankAccount", pRspRepeal->BankAccount);
			///银行密码
			jsonData.put("BankPassWord", pRspRepeal->BankPassWord);
			///投资者帐号
			jsonData.put("AccountID", pRspRepeal->AccountID);
			///期货密码
			jsonData.put("Password", pRspRepeal->Password);
			///期货公司流水号
			jsonData.put("FutureSerial", pRspRepeal->FutureSerial);
			///安装编号
			jsonData.put("InstallID", pRspRepeal->InstallID);
			///用户标识
			jsonData.put("UserID", pRspRepeal->UserID);
			///验证客户证件号码标志
			jsonData.put("VerifyCertNoFlag", pRspRepeal->VerifyCertNoFlag);
			///币种代码
			jsonData.put("CurrencyID", pRspRepeal->CurrencyID);
			///转帐金额
			jsonData.put("TradeAmount", pRspRepeal->TradeAmount);
			///期货可取金额
			jsonData.put("FutureFetchAmount", pRspRepeal->FutureFetchAmount);
			///费用支付标志
			jsonData.put("FeePayFlag", pRspRepeal->FeePayFlag);
			///应收客户费用
			jsonData.put("CustFee", pRspRepeal->CustFee);
			///应收期货公司费用
			jsonData.put("BrokerFee", pRspRepeal->BrokerFee);
			///发送方给接收方的消息
			jsonData.put("Message", pRspRepeal->Message);
			///摘要
			jsonData.put("Digest", pRspRepeal->Digest);
			///银行帐号类型
			jsonData.put("BankAccType", pRspRepeal->BankAccType);
			///渠道标志
			jsonData.put("DeviceID", pRspRepeal->DeviceID);
			///期货单位帐号类型
			jsonData.put("BankSecuAccType", pRspRepeal->BankSecuAccType);
			///期货公司银行编码
			jsonData.put("BrokerIDByBank", pRspRepeal->BrokerIDByBank);
			///期货单位帐号
			jsonData.put("BankSecuAcc", pRspRepeal->BankSecuAcc);
			///银行密码标志
			jsonData.put("BankPwdFlag", pRspRepeal->BankPwdFlag);
			///期货资金密码核对标志
			jsonData.put("SecuPwdFlag", pRspRepeal->SecuPwdFlag);
			///交易柜员
			jsonData.put("OperNo", pRspRepeal->OperNo);
			///请求编号
			jsonData.put("RequestID", pRspRepeal->RequestID);
			///交易ID
			jsonData.put("TID", pRspRepeal->TID);
			///转账交易状态
			jsonData.put("TransferStatus", pRspRepeal->TransferStatus);
			///错误代码
			jsonData.put("ErrorID", pRspRepeal->ErrorID);
			///错误信息
			jsonData.put("ErrorMsg", pRspRepeal->ErrorMsg);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///期货发起冲正期货转银行请求，银行处理完毕后报盘发回的通知
void CTPApi::OnRtnRepealFromFutureToBankByFuture(CThostFtdcRspRepealField *pRspRepeal) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRtnRepealFromFutureToBankByFuture")) {
			//构造Data
			boost::property_tree::ptree jsonData;
			///冲正时间间隔
			jsonData.put("RepealTimeInterval", pRspRepeal->RepealTimeInterval);
			///已经冲正次数
			jsonData.put("RepealedTimes", pRspRepeal->RepealedTimes);
			///银行冲正标志
			jsonData.put("BankRepealFlag", pRspRepeal->BankRepealFlag);
			///期商冲正标志
			jsonData.put("BrokerRepealFlag", pRspRepeal->BrokerRepealFlag);
			///被冲正平台流水号
			jsonData.put("PlateRepealSerial", pRspRepeal->PlateRepealSerial);
			///被冲正银行流水号
			jsonData.put("BankRepealSerial", pRspRepeal->BankRepealSerial);
			///被冲正期货流水号
			jsonData.put("FutureRepealSerial", pRspRepeal->FutureRepealSerial);
			///业务功能码
			jsonData.put("TradeCode", pRspRepeal->TradeCode);
			///银行代码
			jsonData.put("BankID", pRspRepeal->BankID);
			///银行分支机构代码
			jsonData.put("BankBranchID", pRspRepeal->BankBranchID);
			///期商代码
			jsonData.put("BrokerID", pRspRepeal->BrokerID);
			///期商分支机构代码
			jsonData.put("BrokerBranchID", pRspRepeal->BrokerBranchID);
			///交易日期
			jsonData.put("TradeDate", pRspRepeal->TradeDate);
			///交易时间
			jsonData.put("TradeTime", pRspRepeal->TradeTime);
			///银行流水号
			jsonData.put("BankSerial", pRspRepeal->BankSerial);
			///交易系统日期 
			jsonData.put("TradingDay", pRspRepeal->TradingDay);
			///银期平台消息流水号
			jsonData.put("PlateSerial", pRspRepeal->PlateSerial);
			///最后分片标志
			jsonData.put("LastFragment", pRspRepeal->LastFragment);
			///会话号
			jsonData.put("SessionID", pRspRepeal->SessionID);
			///客户姓名
			jsonData.put("CustomerName", pRspRepeal->CustomerName);
			///证件类型
			jsonData.put("IdCardType", pRspRepeal->IdCardType);
			///证件号码
			jsonData.put("IdentifiedCardNo", pRspRepeal->IdentifiedCardNo);
			///客户类型
			jsonData.put("CustType", pRspRepeal->CustType);
			///银行帐号
			jsonData.put("BankAccount", pRspRepeal->BankAccount);
			///银行密码
			jsonData.put("BankPassWord", pRspRepeal->BankPassWord);
			///投资者帐号
			jsonData.put("AccountID", pRspRepeal->AccountID);
			///期货密码
			jsonData.put("Password", pRspRepeal->Password);
			///期货公司流水号
			jsonData.put("FutureSerial", pRspRepeal->FutureSerial);
			///安装编号
			jsonData.put("InstallID", pRspRepeal->InstallID);
			///用户标识
			jsonData.put("UserID", pRspRepeal->UserID);
			///验证客户证件号码标志
			jsonData.put("VerifyCertNoFlag", pRspRepeal->VerifyCertNoFlag);
			///币种代码
			jsonData.put("CurrencyID", pRspRepeal->CurrencyID);
			///转帐金额
			jsonData.put("TradeAmount", pRspRepeal->TradeAmount);
			///期货可取金额
			jsonData.put("FutureFetchAmount", pRspRepeal->FutureFetchAmount);
			///费用支付标志
			jsonData.put("FeePayFlag", pRspRepeal->FeePayFlag);
			///应收客户费用
			jsonData.put("CustFee", pRspRepeal->CustFee);
			///应收期货公司费用
			jsonData.put("BrokerFee", pRspRepeal->BrokerFee);
			///发送方给接收方的消息
			jsonData.put("Message", pRspRepeal->Message);
			///摘要
			jsonData.put("Digest", pRspRepeal->Digest);
			///银行帐号类型
			jsonData.put("BankAccType", pRspRepeal->BankAccType);
			///渠道标志
			jsonData.put("DeviceID", pRspRepeal->DeviceID);
			///期货单位帐号类型
			jsonData.put("BankSecuAccType", pRspRepeal->BankSecuAccType);
			///期货公司银行编码
			jsonData.put("BrokerIDByBank", pRspRepeal->BrokerIDByBank);
			///期货单位帐号
			jsonData.put("BankSecuAcc", pRspRepeal->BankSecuAcc);
			///银行密码标志
			jsonData.put("BankPwdFlag", pRspRepeal->BankPwdFlag);
			///期货资金密码核对标志
			jsonData.put("SecuPwdFlag", pRspRepeal->SecuPwdFlag);
			///交易柜员
			jsonData.put("OperNo", pRspRepeal->OperNo);
			///请求编号
			jsonData.put("RequestID", pRspRepeal->RequestID);
			///交易ID
			jsonData.put("TID", pRspRepeal->TID);
			///转账交易状态
			jsonData.put("TransferStatus", pRspRepeal->TransferStatus);
			///错误代码
			jsonData.put("ErrorID", pRspRepeal->ErrorID);
			///错误信息
			jsonData.put("ErrorMsg", pRspRepeal->ErrorMsg);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///期货发起银行资金转期货应答
void CTPApi::OnRspFromBankToFutureByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRspFromBankToFutureByFuture", pRspInfo, nRequestID, bIsLast)) {
			//构造Data
			boost::property_tree::ptree jsonData;
			///业务功能码
			jsonData.put("TradeCode", pReqTransfer->TradeCode);
			///银行代码
			jsonData.put("BankID", pReqTransfer->BankID);
			///银行分支机构代码
			jsonData.put("BankBranchID", pReqTransfer->BankBranchID);
			///期商代码
			jsonData.put("BrokerID", pReqTransfer->BrokerID);
			///期商分支机构代码
			jsonData.put("BrokerBranchID", pReqTransfer->BrokerBranchID);
			///交易日期
			jsonData.put("TradeDate", pReqTransfer->TradeDate);
			///交易时间
			jsonData.put("TradeTime", pReqTransfer->TradeTime);
			///银行流水号
			jsonData.put("BankSerial", pReqTransfer->BankSerial);
			///交易系统日期 
			jsonData.put("TradingDay", pReqTransfer->TradingDay);
			///银期平台消息流水号
			jsonData.put("PlateSerial", pReqTransfer->PlateSerial);
			///最后分片标志
			jsonData.put("LastFragment", pReqTransfer->LastFragment);
			///会话号
			jsonData.put("SessionID", pReqTransfer->SessionID);
			///客户姓名
			jsonData.put("CustomerName", pReqTransfer->CustomerName);
			///证件类型
			jsonData.put("IdCardType", pReqTransfer->IdCardType);
			///证件号码
			jsonData.put("IdentifiedCardNo", pReqTransfer->IdentifiedCardNo);
			///客户类型
			jsonData.put("CustType", pReqTransfer->CustType);
			///银行帐号
			jsonData.put("BankAccount", pReqTransfer->BankAccount);
			///银行密码
			jsonData.put("BankPassWord", pReqTransfer->BankPassWord);
			///投资者帐号
			jsonData.put("AccountID", pReqTransfer->AccountID);
			///期货密码
			jsonData.put("Password", pReqTransfer->Password);
			///期货公司流水号
			jsonData.put("FutureSerial", pReqTransfer->FutureSerial);
			///安装编号
			jsonData.put("InstallID", pReqTransfer->InstallID);
			///用户标识
			jsonData.put("UserID", pReqTransfer->UserID);
			///验证客户证件号码标志
			jsonData.put("VerifyCertNoFlag", pReqTransfer->VerifyCertNoFlag);
			///币种代码
			jsonData.put("CurrencyID", pReqTransfer->CurrencyID);
			///转帐金额
			jsonData.put("TradeAmount", pReqTransfer->TradeAmount);
			///期货可取金额
			jsonData.put("FutureFetchAmount", pReqTransfer->FutureFetchAmount);
			///费用支付标志
			jsonData.put("FeePayFlag", pReqTransfer->FeePayFlag);
			///应收客户费用
			jsonData.put("CustFee", pReqTransfer->CustFee);
			///应收期货公司费用
			jsonData.put("BrokerFee", pReqTransfer->BrokerFee);
			///发送方给接收方的消息
			jsonData.put("Message", pReqTransfer->Message);
			///摘要
			jsonData.put("Digest", pReqTransfer->Digest);
			///银行帐号类型
			jsonData.put("BankAccType", pReqTransfer->BankAccType);
			///渠道标志
			jsonData.put("DeviceID", pReqTransfer->DeviceID);
			///期货单位帐号类型
			jsonData.put("BankSecuAccType", pReqTransfer->BankSecuAccType);
			///期货公司银行编码
			jsonData.put("BrokerIDByBank", pReqTransfer->BrokerIDByBank);
			///期货单位帐号
			jsonData.put("BankSecuAcc", pReqTransfer->BankSecuAcc);
			///银行密码标志
			jsonData.put("BankPwdFlag", pReqTransfer->BankPwdFlag);
			///期货资金密码核对标志
			jsonData.put("SecuPwdFlag", pReqTransfer->SecuPwdFlag);
			///交易柜员
			jsonData.put("OperNo", pReqTransfer->OperNo);
			///请求编号
			jsonData.put("RequestID", pReqTransfer->RequestID);
			///交易ID
			jsonData.put("TID", pReqTransfer->TID);
			///转账交易状态
			jsonData.put("TransferStatus", pReqTransfer->TransferStatus);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///期货发起期货资金转银行应答
void CTPApi::OnRspFromFutureToBankByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRspFromFutureToBankByFuture", pRspInfo, nRequestID, bIsLast)) {
			//构造Data
			boost::property_tree::ptree jsonData;
			///业务功能码
			jsonData.put("TradeCode", pReqTransfer->TradeCode);
			///银行代码
			jsonData.put("BankID", pReqTransfer->BankID);
			///银行分支机构代码
			jsonData.put("BankBranchID", pReqTransfer->BankBranchID);
			///期商代码
			jsonData.put("BrokerID", pReqTransfer->BrokerID);
			///期商分支机构代码
			jsonData.put("BrokerBranchID", pReqTransfer->BrokerBranchID);
			///交易日期
			jsonData.put("TradeDate", pReqTransfer->TradeDate);
			///交易时间
			jsonData.put("TradeTime", pReqTransfer->TradeTime);
			///银行流水号
			jsonData.put("BankSerial", pReqTransfer->BankSerial);
			///交易系统日期 
			jsonData.put("TradingDay", pReqTransfer->TradingDay);
			///银期平台消息流水号
			jsonData.put("PlateSerial", pReqTransfer->PlateSerial);
			///最后分片标志
			jsonData.put("LastFragment", pReqTransfer->LastFragment);
			///会话号
			jsonData.put("SessionID", pReqTransfer->SessionID);
			///客户姓名
			jsonData.put("CustomerName", pReqTransfer->CustomerName);
			///证件类型
			jsonData.put("IdCardType", pReqTransfer->IdCardType);
			///证件号码
			jsonData.put("IdentifiedCardNo", pReqTransfer->IdentifiedCardNo);
			///客户类型
			jsonData.put("CustType", pReqTransfer->CustType);
			///银行帐号
			jsonData.put("BankAccount", pReqTransfer->BankAccount);
			///银行密码
			jsonData.put("BankPassWord", pReqTransfer->BankPassWord);
			///投资者帐号
			jsonData.put("AccountID", pReqTransfer->AccountID);
			///期货密码
			jsonData.put("Password", pReqTransfer->Password);
			///期货公司流水号
			jsonData.put("FutureSerial", pReqTransfer->FutureSerial);
			///安装编号
			jsonData.put("InstallID", pReqTransfer->InstallID);
			///用户标识
			jsonData.put("UserID", pReqTransfer->UserID);
			///验证客户证件号码标志
			jsonData.put("VerifyCertNoFlag", pReqTransfer->VerifyCertNoFlag);
			///币种代码
			jsonData.put("CurrencyID", pReqTransfer->CurrencyID);
			///转帐金额
			jsonData.put("TradeAmount", pReqTransfer->TradeAmount);
			///期货可取金额
			jsonData.put("FutureFetchAmount", pReqTransfer->FutureFetchAmount);
			///费用支付标志
			jsonData.put("FeePayFlag", pReqTransfer->FeePayFlag);
			///应收客户费用
			jsonData.put("CustFee", pReqTransfer->CustFee);
			///应收期货公司费用
			jsonData.put("BrokerFee", pReqTransfer->BrokerFee);
			///发送方给接收方的消息
			jsonData.put("Message", pReqTransfer->Message);
			///摘要
			jsonData.put("Digest", pReqTransfer->Digest);
			///银行帐号类型
			jsonData.put("BankAccType", pReqTransfer->BankAccType);
			///渠道标志
			jsonData.put("DeviceID", pReqTransfer->DeviceID);
			///期货单位帐号类型
			jsonData.put("BankSecuAccType", pReqTransfer->BankSecuAccType);
			///期货公司银行编码
			jsonData.put("BrokerIDByBank", pReqTransfer->BrokerIDByBank);
			///期货单位帐号
			jsonData.put("BankSecuAcc", pReqTransfer->BankSecuAcc);
			///银行密码标志
			jsonData.put("BankPwdFlag", pReqTransfer->BankPwdFlag);
			///期货资金密码核对标志
			jsonData.put("SecuPwdFlag", pReqTransfer->SecuPwdFlag);
			///交易柜员
			jsonData.put("OperNo", pReqTransfer->OperNo);
			///请求编号
			jsonData.put("RequestID", pReqTransfer->RequestID);
			///交易ID
			jsonData.put("TID", pReqTransfer->TID);
			///转账交易状态
			jsonData.put("TransferStatus", pReqTransfer->TransferStatus);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///期货发起查询银行余额应答
void CTPApi::OnRspQueryBankAccountMoneyByFuture(CThostFtdcReqQueryAccountField *pReqQueryAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRspQueryBankAccountMoneyByFuture", pRspInfo, nRequestID, bIsLast)) {
			//构造Data
			boost::property_tree::ptree jsonData;
			///业务功能码
			jsonData.put("TradeCode", pReqQueryAccount->TradeCode);
			///银行代码
			jsonData.put("BankID", pReqQueryAccount->BankID);
			///银行分支机构代码
			jsonData.put("BankBranchID", pReqQueryAccount->BankBranchID);
			///期商代码
			jsonData.put("BrokerID", pReqQueryAccount->BrokerID);
			///期商分支机构代码
			jsonData.put("BrokerBranchID", pReqQueryAccount->BrokerBranchID);
			///交易日期
			jsonData.put("TradeDate", pReqQueryAccount->TradeDate);
			///交易时间
			jsonData.put("TradeTime", pReqQueryAccount->TradeTime);
			///银行流水号
			jsonData.put("BankSerial", pReqQueryAccount->BankSerial);
			///交易系统日期 
			jsonData.put("TradingDay", pReqQueryAccount->TradingDay);
			///银期平台消息流水号
			jsonData.put("PlateSerial", pReqQueryAccount->PlateSerial);
			///最后分片标志
			jsonData.put("LastFragment", pReqQueryAccount->LastFragment);
			///会话号
			jsonData.put("SessionID", pReqQueryAccount->SessionID);
			///客户姓名
			jsonData.put("CustomerName", pReqQueryAccount->CustomerName);
			///证件类型
			jsonData.put("IdCardType", pReqQueryAccount->IdCardType);
			///证件号码
			jsonData.put("IdentifiedCardNo", pReqQueryAccount->IdentifiedCardNo);
			///客户类型
			jsonData.put("CustType", pReqQueryAccount->CustType);
			///银行帐号
			jsonData.put("BankAccount", pReqQueryAccount->BankAccount);
			///银行密码
			jsonData.put("BankPassWord", pReqQueryAccount->BankPassWord);
			///投资者帐号
			jsonData.put("AccountID", pReqQueryAccount->AccountID);
			///期货密码
			jsonData.put("Password", pReqQueryAccount->Password);
			///期货公司流水号
			jsonData.put("FutureSerial", pReqQueryAccount->FutureSerial);
			///安装编号
			jsonData.put("InstallID", pReqQueryAccount->InstallID);
			///用户标识
			jsonData.put("UserID", pReqQueryAccount->UserID);
			///验证客户证件号码标志
			jsonData.put("VerifyCertNoFlag", pReqQueryAccount->VerifyCertNoFlag);
			///币种代码
			jsonData.put("CurrencyID", pReqQueryAccount->CurrencyID);
			///摘要
			jsonData.put("Digest", pReqQueryAccount->Digest);
			///银行帐号类型
			jsonData.put("BankAccType", pReqQueryAccount->BankAccType);
			///渠道标志
			jsonData.put("DeviceID", pReqQueryAccount->DeviceID);
			///期货公司银行编码
			jsonData.put("BrokerIDByBank", pReqQueryAccount->BrokerIDByBank);
			///银行密码标志
			jsonData.put("BankPwdFlag", pReqQueryAccount->BankPwdFlag);
			///期货资金密码核对标志
			jsonData.put("SecuPwdFlag", pReqQueryAccount->SecuPwdFlag);
			///交易柜员
			jsonData.put("OperNo", pReqQueryAccount->OperNo);
			///请求编号
			jsonData.put("RequestID", pReqQueryAccount->RequestID);
			///交易ID
			jsonData.put("TID", pReqQueryAccount->TID);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///银行发起银期开户通知
void CTPApi::OnRtnOpenAccountByBank(CThostFtdcOpenAccountField *pOpenAccount) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRtnOpenAccountByBank")) {
			//构造Data
			boost::property_tree::ptree jsonData;
			///业务功能码
			jsonData.put("TradeCode", pOpenAccount->TradeCode);
			///银行代码
			jsonData.put("BankID", pOpenAccount->BankID);
			///银行分支机构代码
			jsonData.put("BankBranchID", pOpenAccount->BankBranchID);
			///期商代码
			jsonData.put("BrokerID", pOpenAccount->BrokerID);
			///期商分支机构代码
			jsonData.put("BrokerBranchID", pOpenAccount->BrokerBranchID);
			///交易日期
			jsonData.put("TradeDate", pOpenAccount->TradeDate);
			///交易时间
			jsonData.put("TradeTime", pOpenAccount->TradeTime);
			///银行流水号
			jsonData.put("BankSerial", pOpenAccount->BankSerial);
			///交易系统日期 
			jsonData.put("TradingDay", pOpenAccount->TradingDay);
			///银期平台消息流水号
			jsonData.put("PlateSerial", pOpenAccount->PlateSerial);
			///最后分片标志
			jsonData.put("LastFragment", pOpenAccount->LastFragment);
			///会话号
			jsonData.put("SessionID", pOpenAccount->SessionID);
			///客户姓名
			jsonData.put("CustomerName", pOpenAccount->CustomerName);
			///证件类型
			jsonData.put("IdCardType", pOpenAccount->IdCardType);
			///证件号码
			jsonData.put("IdentifiedCardNo", pOpenAccount->IdentifiedCardNo);
			///性别
			jsonData.put("Gender", pOpenAccount->Gender);
			///国家代码
			jsonData.put("CountryCode", pOpenAccount->CountryCode);
			///客户类型
			jsonData.put("CustType", pOpenAccount->CustType);
			///地址
			jsonData.put("Address", pOpenAccount->Address);
			///邮编
			jsonData.put("ZipCode", pOpenAccount->ZipCode);
			///电话号码
			jsonData.put("Telephone", pOpenAccount->Telephone);
			///手机
			jsonData.put("MobilePhone", pOpenAccount->MobilePhone);
			///传真
			jsonData.put("Fax", pOpenAccount->Fax);
			///电子邮件
			jsonData.put("EMail", pOpenAccount->EMail);
			///资金账户状态
			jsonData.put("MoneyAccountStatus", pOpenAccount->MoneyAccountStatus);
			///银行帐号
			jsonData.put("BankAccount", pOpenAccount->BankAccount);
			///银行密码
			jsonData.put("BankPassWord", pOpenAccount->BankPassWord);
			///投资者帐号
			jsonData.put("AccountID", pOpenAccount->AccountID);
			///期货密码
			jsonData.put("Password", pOpenAccount->Password);
			///银行帐号类型
			jsonData.put("BankAccType", pOpenAccount->BankAccType);
			///安装编号
			jsonData.put("InstallID", pOpenAccount->InstallID);
			///验证客户证件号码标志
			jsonData.put("VerifyCertNoFlag", pOpenAccount->VerifyCertNoFlag);
			///币种代码
			jsonData.put("CurrencyID", pOpenAccount->CurrencyID);
			///期货公司银行编码
			jsonData.put("BrokerIDByBank", pOpenAccount->BrokerIDByBank);
			///银行密码标志
			jsonData.put("BankPwdFlag", pOpenAccount->BankPwdFlag);
			///期货资金密码核对标志
			jsonData.put("SecuPwdFlag", pOpenAccount->SecuPwdFlag);
			///交易ID
			jsonData.put("TID", pOpenAccount->TID);
			///摘要
			jsonData.put("Digest", pOpenAccount->Digest);
			///错误代码
			jsonData.put("ErrorID", pOpenAccount->ErrorID);
			///错误信息
			jsonData.put("ErrorMsg", pOpenAccount->ErrorMsg);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///银行发起银期销户通知
void CTPApi::OnRtnCancelAccountByBank(CThostFtdcCancelAccountField *pCancelAccount) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRtnCancelAccountByBank")) {
			//构造Data
			boost::property_tree::ptree jsonData;
			///业务功能码
			jsonData.put("TradeCode", pCancelAccount->TradeCode);
			///银行代码
			jsonData.put("BankID", pCancelAccount->BankID);
			///银行分支机构代码
			jsonData.put("BankBranchID", pCancelAccount->BankBranchID);
			///期商代码
			jsonData.put("BrokerID", pCancelAccount->BrokerID);
			///期商分支机构代码
			jsonData.put("BrokerBranchID", pCancelAccount->BrokerBranchID);
			///交易日期
			jsonData.put("TradeDate", pCancelAccount->TradeDate);
			///交易时间
			jsonData.put("TradeTime", pCancelAccount->TradeTime);
			///银行流水号
			jsonData.put("BankSerial", pCancelAccount->BankSerial);
			///交易系统日期 
			jsonData.put("TradingDay", pCancelAccount->TradingDay);
			///银期平台消息流水号
			jsonData.put("PlateSerial", pCancelAccount->PlateSerial);
			///最后分片标志
			jsonData.put("LastFragment", pCancelAccount->LastFragment);
			///会话号
			jsonData.put("SessionID", pCancelAccount->SessionID);
			///客户姓名
			jsonData.put("CustomerName", pCancelAccount->CustomerName);
			///证件类型
			jsonData.put("IdCardType", pCancelAccount->IdCardType);
			///证件号码
			jsonData.put("IdentifiedCardNo", pCancelAccount->IdentifiedCardNo);
			///性别
			jsonData.put("Gender", pCancelAccount->Gender);
			///国家代码
			jsonData.put("CountryCode", pCancelAccount->CountryCode);
			///客户类型
			jsonData.put("CustType", pCancelAccount->CustType);
			///地址
			jsonData.put("Address", pCancelAccount->Address);
			///邮编
			jsonData.put("ZipCode", pCancelAccount->ZipCode);
			///电话号码
			jsonData.put("Telephone", pCancelAccount->Telephone);
			///手机
			jsonData.put("MobilePhone", pCancelAccount->MobilePhone);
			///传真
			jsonData.put("Fax", pCancelAccount->Fax);
			///电子邮件
			jsonData.put("EMail", pCancelAccount->EMail);
			///资金账户状态
			jsonData.put("MoneyAccountStatus", pCancelAccount->MoneyAccountStatus);
			///银行帐号
			jsonData.put("BankAccount", pCancelAccount->BankAccount);
			///银行密码
			jsonData.put("BankPassWord", pCancelAccount->BankPassWord);
			///投资者帐号
			jsonData.put("AccountID", pCancelAccount->AccountID);
			///期货密码
			jsonData.put("Password", pCancelAccount->Password);
			///银行帐号类型
			jsonData.put("BankAccType", pCancelAccount->BankAccType);
			///安装编号
			jsonData.put("InstallID", pCancelAccount->InstallID);
			///验证客户证件号码标志
			jsonData.put("VerifyCertNoFlag", pCancelAccount->VerifyCertNoFlag);
			///币种代码
			jsonData.put("CurrencyID", pCancelAccount->CurrencyID);
			///期货公司银行编码
			jsonData.put("BrokerIDByBank", pCancelAccount->BrokerIDByBank);
			///银行密码标志
			jsonData.put("BankPwdFlag", pCancelAccount->BankPwdFlag);
			///期货资金密码核对标志
			jsonData.put("SecuPwdFlag", pCancelAccount->SecuPwdFlag);
			///交易ID
			jsonData.put("TID", pCancelAccount->TID);
			///摘要
			jsonData.put("Digest", pCancelAccount->Digest);
			///错误代码
			jsonData.put("ErrorID", pCancelAccount->ErrorID);
			///错误信息
			jsonData.put("ErrorMsg", pCancelAccount->ErrorMsg);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

///银行发起变更银行账号通知
void CTPApi::OnRtnChangeAccountByBank(CThostFtdcChangeAccountField *pChangeAccount) {
	try
	{
		boost::property_tree::ptree jsonObject;
		if (BuildDefaultJsonObject(jsonObject, "OnRtnChangeAccountByBank")) {
			//构造Data
			boost::property_tree::ptree jsonData;
			///业务功能码
			jsonData.put("TradeCode", pChangeAccount->TradeCode);
			///银行代码
			jsonData.put("BankID", pChangeAccount->BankID);
			///银行分支机构代码
			jsonData.put("BankBranchID", pChangeAccount->BankBranchID);
			///期商代码
			jsonData.put("BrokerID", pChangeAccount->BrokerID);
			///期商分支机构代码
			jsonData.put("BrokerBranchID", pChangeAccount->BrokerBranchID);
			///交易日期
			jsonData.put("TradeDate", pChangeAccount->TradeDate);
			///交易时间
			jsonData.put("TradeTime", pChangeAccount->TradeTime);
			///银行流水号
			jsonData.put("BankSerial", pChangeAccount->BankSerial);
			///交易系统日期 
			jsonData.put("TradingDay", pChangeAccount->TradingDay);
			///银期平台消息流水号
			jsonData.put("PlateSerial", pChangeAccount->PlateSerial);
			///最后分片标志
			jsonData.put("LastFragment", pChangeAccount->LastFragment);
			///会话号
			jsonData.put("SessionID", pChangeAccount->SessionID);
			///客户姓名
			jsonData.put("CustomerName", pChangeAccount->CustomerName);
			///证件类型
			jsonData.put("IdCardType", pChangeAccount->IdCardType);
			///证件号码
			jsonData.put("IdentifiedCardNo", pChangeAccount->IdentifiedCardNo);
			///性别
			jsonData.put("Gender", pChangeAccount->Gender);
			///国家代码
			jsonData.put("CountryCode", pChangeAccount->CountryCode);
			///客户类型
			jsonData.put("CustType", pChangeAccount->CustType);
			///地址
			jsonData.put("Address", pChangeAccount->Address);
			///邮编
			jsonData.put("ZipCode", pChangeAccount->ZipCode);
			///电话号码
			jsonData.put("Telephone", pChangeAccount->Telephone);
			///手机
			jsonData.put("MobilePhone", pChangeAccount->MobilePhone);
			///传真
			jsonData.put("Fax", pChangeAccount->Fax);
			///电子邮件
			jsonData.put("EMail", pChangeAccount->EMail);
			///资金账户状态
			jsonData.put("MoneyAccountStatus", pChangeAccount->MoneyAccountStatus);
			///银行帐号
			jsonData.put("BankAccount", pChangeAccount->BankAccount);
			///银行密码
			jsonData.put("BankPassWord", pChangeAccount->BankPassWord);
			///新银行帐号
			jsonData.put("NewBankAccount", pChangeAccount->NewBankAccount);
			///新银行密码
			jsonData.put("NewBankPassWord", pChangeAccount->NewBankPassWord);
			///投资者帐号
			jsonData.put("AccountID", pChangeAccount->AccountID);
			///期货密码
			jsonData.put("Password", pChangeAccount->Password);
			///银行帐号类型
			jsonData.put("BankAccType", pChangeAccount->BankAccType);
			///安装编号
			jsonData.put("InstallID", pChangeAccount->InstallID);
			///验证客户证件号码标志
			jsonData.put("VerifyCertNoFlag", pChangeAccount->VerifyCertNoFlag);
			///币种代码
			jsonData.put("CurrencyID", pChangeAccount->CurrencyID);
			///期货公司银行编码
			jsonData.put("BrokerIDByBank", pChangeAccount->BrokerIDByBank);
			///银行密码标志
			jsonData.put("BankPwdFlag", pChangeAccount->BankPwdFlag);
			///期货资金密码核对标志
			jsonData.put("SecuPwdFlag", pChangeAccount->SecuPwdFlag);
			///交易ID
			jsonData.put("TID", pChangeAccount->TID);
			///摘要
			jsonData.put("Digest", pChangeAccount->Digest);
			///错误代码
			jsonData.put("ErrorID", pChangeAccount->ErrorID);
			///错误信息
			jsonData.put("ErrorMsg", pChangeAccount->ErrorMsg);
			///添加Data
			jsonObject.push_back(std::make_pair("Data", jsonData));
			///获取jsonString
			std::string jsonString;
			if (XUtil::json_to_str(jsonString, jsonObject)) {
				//发送数据
				m_pSpi->OnRecv(jsonString);
				return;
			}
		}
	}
	catch (...)
	{
	}
	m_pSpi->OnClose();
};

//------------------------------------------------------------------------------
//
//int main(int argc, char* argv[])
//{
//	// Check command line arguments.
//	if (argc != 4)
//	{
//		std::cerr <<
//			"Usage: CTPApi <address> <port> <threads>\n" <<
//			"Example:\n" <<
//			"    CTPApi 0.0.0.0 8080 1\n";
//		return EXIT_FAILURE;
//	}
//	auto const address = boost::asio::ip::make_address(argv[1]);
//	auto const port = static_cast<unsigned short>(std::atoi(argv[2]));
//	auto const threads = std::max<int>(1, std::atoi(argv[3]));
//
//	// The io_context is required for all I/O
//	boost::asio::io_context ioc{ threads };
//
//	// Create and launch a listening port
//	std::make_shared<listener>(ioc, tcp::endpoint{ address, port })->run();
//
//	// Run the I/O service on the requested number of threads
//	/*std::vector<std::thread> v;
//	v.reserve(threads);
//	for (auto i = threads; i > 0; --i)
//		v.emplace_back(
//			[&ioc]
//	{
//		ioc.run();
//	});*/
//	boost::thread_group worker_threads;
//	for (auto i = threads; i > 0; --i)
//		worker_threads.create_thread(
//			[&ioc]
//	{
//		ioc.run();
//	});
//
//	//ioc.run();
//	std::string cmd;
//	do {
//		std::cout << "please input command:(q=quit)" << std::endl;
//		std::cin >> cmd;
//	} while (cmd != "q", (char)0);
//	
//	ioc.stop();
//
//	worker_threads.join_all();
//
//	return EXIT_SUCCESS;
//}