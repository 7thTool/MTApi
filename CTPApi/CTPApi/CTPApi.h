#ifndef _H_CTPAPI_H_
#define _H_CTPAPI_H_

#include "CTPCommon.h"
#include "CTPSessionApi.h"
#include <XUtil/XXml.hpp>
#include <atomic>

class CTPApi
	: public CTPSessionApi
	, public CThostFtdcMdSpi
	, public CThostFtdcTraderSpi
{
private:
	CTPUserLoginInfo mUserInfo;
	CTPTraderApi* m_pTradeApi;
	CTPMdApi* m_pMdApi;
	CTPSessionSpi* m_pSpi;
	std::atomic<bool> m_CloseFlag;

public:
	CTPApi();
	~CTPApi();

	void OpenApi(const CTPUserLoginInfo& userInfo);
	void CloseApi();
	bool IsOpenApi();

	virtual void RegisterSpi(CTPSessionSpi* pSpi);

	virtual bool IsOpen() { return IsOpenApi(); }
	virtual bool Open(const std::string& data) {
		BOOST_ASSERT(!IsOpen());
		CTPUserLoginInfo userInfo;
		if (parseUserInfoFromString(userInfo, data)) {
			OpenApi(userInfo);
			return true;
		}
		else {
			Close();
			return false;
		}
	}
	virtual void Send(const std::string& data);
	virtual void Close() { CloseApi(); }

protected:
	template<typename TRequest>
	void BuildDefaultRequest(TRequest& req) 
	{
		memcpy(req.BrokerID, mUserInfo.BrokerID,sizeof(req.BrokerID));
		memcpy(req.UserID, mUserInfo.UserID, sizeof(req.UserID));
	}
	template<typename TRequest>
	void BuildDefaultAccountRequest(TRequest& req)
	{
		memcpy(req.BrokerID, mUserInfo.BrokerID, sizeof(req.BrokerID));
		memcpy(req.AccountID, mUserInfo.UserID, sizeof(req.AccountID));
	}
	template<typename TRequest>
	void BuildDefaultInvestorRequest(TRequest& req)
	{
		memcpy(req.BrokerID, mUserInfo.BrokerID, sizeof(req.BrokerID));
		memcpy(req.InvestorID, mUserInfo.UserID, sizeof(req.InvestorID));
	}
	template<typename TRequest>
	void BuildDefaultTradeRequest(TRequest& req)
	{
		memcpy(req.BrokerID, mUserInfo.BrokerID, sizeof(req.BrokerID));
		memcpy(req.UserID, mUserInfo.UserID, sizeof(req.UserID));
		memcpy(req.InvestorID, mUserInfo.UserID, sizeof(req.InvestorID));

	}

	template<typename TRequest>
	void BuildAccountRequest(TRequest& req, const boost::property_tree::ptree & jsonData)
	{
		BuildDefaultAccountRequest(req);
		snprintf(req.BrokerID, _countof(req.BrokerID), "%s", jsonData.get<std::string>("BrokerID","").c_str());
		snprintf(req.AccountID, _countof(req.AccountID), "%s", jsonData.get<std::string>("AccountID","").c_str());
	}
	template<typename TRequest>
	void BuildInvestorRequest(TRequest& req, const boost::property_tree::ptree & jsonData)
	{
		BuildDefaultInvestorRequest(req);
		snprintf(req.BrokerID, _countof(req.BrokerID), "%s", jsonData.get<std::string>("BrokerID","").c_str());
		snprintf(req.InvestorID, _countof(req.InvestorID), "%s", jsonData.get<std::string>("InvestorID","").c_str());
	}
	template<typename TRequest>
	void BuildTradeRequest(TRequest& req, const boost::property_tree::ptree & jsonData)
	{
		BuildDefaultTradeRequest(req);
		snprintf(req.BrokerID, _countof(req.BrokerID), "%s", jsonData.get<std::string>("BrokerID","").c_str());
		snprintf(req.UserID, _countof(req.UserID), "%s", jsonData.get<std::string>("UserID","").c_str());
		snprintf(req.InvestorID, _countof(req.InvestorID), "%s", jsonData.get<std::string>("InvestorID","").c_str());
	}
	template<typename TRequest>
	void BuildTradeAccountRequest(TRequest& req, const boost::property_tree::ptree & jsonData)
	{
		BuildDefaultTradeRequest(req);
		snprintf(req.BrokerID, _countof(req.BrokerID), "%s", jsonData.get<std::string>("BrokerID","").c_str());
		snprintf(req.UserID, _countof(req.UserID), "%s", jsonData.get<std::string>("UserID","").c_str());
		snprintf(req.AccountID, _countof(req.InvestorID), "%s", jsonData.get<std::string>("AccountID","").c_str());
		snprintf(req.InvestorID, _countof(req.InvestorID), "%s", jsonData.get<std::string>("InvestorID","").c_str());
	}
	template<typename TRequest>
	void BuildInstrumentRequest(TRequest& req, const boost::property_tree::ptree & jsonData)
	{
		///交易所代码
		snprintf(req.ExchangeID, _countof(req.ExchangeID), "%s", jsonData.get<std::string>("ExchangeID","").c_str());
		///合约代码
		snprintf(req.InstrumentID, _countof(req.InstrumentID), "%s", jsonData.get<std::string>("InstrumentID","").c_str());
	}

	int HandleRequestResult(int nResult, const char* pRequestApi = nullptr, int nRequestID = 0);

	template<typename TRequest>
	void BuildDefaultResponse(const TRequest& req, const boost::property_tree::ptree & jsonData)
	{
		///经纪公司代码
		jsonData.put("BrokerID", req.BrokerID);
		///资金账号
		jsonData.put("AccountID", req.AccountID);
	}
	template<typename TRequest>
	void BuildAccountReturn(const TRequest* pRet, boost::property_tree::ptree & jsonData)
	{
		///经纪公司代码
		jsonData.put("BrokerID", pRet->BrokerID);
		///用户代码
		jsonData.put("UserID", pRet->UserID);
	}
	template<typename TRequest>
	void BuildInvestorReturn(const TRequest* pRet, boost::property_tree::ptree & jsonData)
	{
		///经纪公司代码
		jsonData.put("BrokerID", pRet->BrokerID);
		///投资者代码
		jsonData.put("InvestorID", pRet->InvestorID);
	}
	template<typename TRequest>
	void BuildTradeReturn(const TRequest* pRet, boost::property_tree::ptree & jsonData)
	{
		///经纪公司代码
		jsonData.put("BrokerID", pRet->BrokerID);
		///用户代码
		jsonData.put("UserID", pRet->UserID);
		///投资者代码
		jsonData.put("InvestorID", pRet->InvestorID);
	}
	template<typename TRequest>
	void BuildTradeAccountReturn(const TRequest* pRet, boost::property_tree::ptree & jsonData)
	{
		///经纪公司代码
		jsonData.put("BrokerID", pRet->BrokerID);
		///用户代码
		jsonData.put("UserID", pRet->UserID);
		///资金账号
		jsonData.put("AccountID", pRet->AccountID);
		///投资者代码
		jsonData.put("InvestorID", pRet->InvestorID);
	}
	template<typename TRequest>
	void BuildInstrumentReturn(const TRequest* pRet, boost::property_tree::ptree & jsonData)
	{
		///交易所代码
		jsonData.put("ExchangeID", pRet->ExchangeID);
		///合约代码
		jsonData.put("InstrumentID", pRet->InstrumentID);
	}

	bool BuildDefaultJsonObject(boost::property_tree::ptree & jsonObject
		, const char* pReturnApi, const CThostFtdcRspInfoField *pRspInfo = nullptr, int nRequestID = 0, bool bIsLast = true);

public:
	///订阅行情
	virtual int SubscribeMarketData(const boost::property_tree::ptree & jsonObject, int nRequestID);

	///退订行情
	virtual int UnSubscribeMarketData(const boost::property_tree::ptree & jsonObject, int nRequestID);

	///订阅行情应答
	virtual void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///取消订阅行情应答
	virtual void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///深度行情通知
	virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);

	///客户端认证请求
	int ReqAuthenticate(const boost::property_tree::ptree & jsonObject, int nRequestID);

	///用户登录请求
	int ReqUserLogin(const boost::property_tree::ptree & jsonObject, int nRequestID);


	///登出请求
	int ReqUserLogout(const boost::property_tree::ptree & jsonObject, int nRequestID);

	///用户口令更新请求
	int ReqUserPasswordUpdate(const boost::property_tree::ptree & jsonObject, int nRequestID);

	///资金账户口令更新请求
	int ReqTradingAccountPasswordUpdate(const boost::property_tree::ptree & jsonObject, int nRequestID);

	///报单录入请求
	int ReqOrderInsert(const boost::property_tree::ptree & jsonObject, int nRequestID);

	///预埋单录入请求
	int ReqParkedOrderInsert(const boost::property_tree::ptree & jsonObject, int nRequestID);

	///预埋撤单录入请求
	int ReqParkedOrderAction(const boost::property_tree::ptree & jsonObject, int nRequestID);

	///报单操作请求
	int ReqOrderAction(const boost::property_tree::ptree & jsonObject, int nRequestID);

	///查询最大报单数量请求
	int ReqQueryMaxOrderVolume(const boost::property_tree::ptree & jsonObject, int nRequestID);

	///投资者结算结果确认
	int ReqSettlementInfoConfirm(const boost::property_tree::ptree & jsonObject, int nRequestID);

	///请求删除预埋单
	int ReqRemoveParkedOrder(const boost::property_tree::ptree & jsonObject, int nRequestID);

	///请求删除预埋撤单
	int ReqRemoveParkedOrderAction(const boost::property_tree::ptree & jsonObject, int nRequestID);

	///执行宣告录入请求
	int ReqExecOrderInsert(const boost::property_tree::ptree & jsonObject, int nRequestID);

	///执行宣告操作请求
	int ReqExecOrderAction(const boost::property_tree::ptree & jsonObject, int nRequestID);

	///询价录入请求
	int ReqForQuoteInsert(const boost::property_tree::ptree & jsonObject, int nRequestID);

	///报价录入请求
	int ReqQuoteInsert(const boost::property_tree::ptree & jsonObject, int nRequestID);

	///报价操作请求
	int ReqQuoteAction(const boost::property_tree::ptree & jsonObject, int nRequestID);

	///锁定请求
	int ReqLockInsert(const boost::property_tree::ptree & jsonObject, int nRequestID);

	///批量报单操作请求
	int ReqBatchOrderAction(const boost::property_tree::ptree & jsonObject, int nRequestID);

	///申请组合录入请求
	int ReqCombActionInsert(const boost::property_tree::ptree & jsonObject, int nRequestID);

	///请求查询报单
	int ReqQryOrder(const boost::property_tree::ptree & jsonObject, int nRequestID);

	///请求查询成交
	int ReqQryTrade(const boost::property_tree::ptree & jsonObject, int nRequestID);

	///请求查询投资者持仓
	int ReqQryInvestorPosition(const boost::property_tree::ptree & jsonObject, int nRequestID);

	///请求查询资金账户
	int ReqQryTradingAccount(const boost::property_tree::ptree & jsonObject, int nRequestID);

	///请求查询投资者
	int ReqQryInvestor(const boost::property_tree::ptree & jsonObject, int nRequestID);

	///请求查询交易编码
	int ReqQryTradingCode(const boost::property_tree::ptree & jsonObject, int nRequestID);

	///请求查询合约保证金率
	int ReqQryInstrumentMarginRate(const boost::property_tree::ptree & jsonObject, int nRequestID);

	///请求查询合约手续费率
	int ReqQryInstrumentCommissionRate(const boost::property_tree::ptree & jsonObject, int nRequestID);

	///请求查询交易所
	int ReqQryExchange(const boost::property_tree::ptree & jsonObject, int nRequestID);

	///请求查询产品
	int ReqQryProduct(const boost::property_tree::ptree & jsonObject, int nRequestID);

	///请求查询合约
	int ReqQryInstrument(const boost::property_tree::ptree & jsonObject, int nRequestID);

	///请求查询行情
	int ReqQryDepthMarketData(const boost::property_tree::ptree & jsonObject, int nRequestID);

	///请求查询投资者结算结果
	int ReqQrySettlementInfo(const boost::property_tree::ptree & jsonObject, int nRequestID);

	///请求查询转帐银行
	int ReqQryTransferBank(const boost::property_tree::ptree & jsonObject, int nRequestID);

	///请求查询投资者持仓明细
	int ReqQryInvestorPositionDetail(const boost::property_tree::ptree & jsonObject, int nRequestID);

	///请求查询客户通知
	int ReqQryNotice(const boost::property_tree::ptree & jsonObject, int nRequestID);

	///请求查询结算信息确认
	int ReqQrySettlementInfoConfirm(const boost::property_tree::ptree & jsonObject, int nRequestID);

	///请求查询投资者持仓明细
	int ReqQryInvestorPositionCombineDetail(const boost::property_tree::ptree & jsonObject, int nRequestID);

	///请求查询保证金监管系统经纪公司资金账户密钥
	int ReqQryCFMMCTradingAccountKey(const boost::property_tree::ptree & jsonObject, int nRequestID);

	///请求查询仓单折抵信息
	int ReqQryEWarrantOffset(const boost::property_tree::ptree & jsonObject, int nRequestID);

	///请求查询投资者品种/跨品种保证金
	int ReqQryInvestorProductGroupMargin(const boost::property_tree::ptree & jsonObject, int nRequestID);

	///请求查询交易所保证金率
	int ReqQryExchangeMarginRate(const boost::property_tree::ptree & jsonObject, int nRequestID);

	///请求查询交易所调整保证金率
	int ReqQryExchangeMarginRateAdjust(const boost::property_tree::ptree & jsonObject, int nRequestID);

	///请求查询汇率
	int ReqQryExchangeRate(const boost::property_tree::ptree & jsonObject, int nRequestID);

	///请求查询二级代理操作员银期权限
	int ReqQrySecAgentACIDMap(const boost::property_tree::ptree & jsonObject, int nRequestID);

	///请求查询产品报价汇率
	int ReqQryProductExchRate(const boost::property_tree::ptree & jsonObject, int nRequestID);

	///请求查询产品组
	int ReqQryProductGroup(const boost::property_tree::ptree & jsonObject, int nRequestID);

	///请求查询做市商合约手续费率
	int ReqQryMMInstrumentCommissionRate(const boost::property_tree::ptree & jsonObject, int nRequestID);

	///请求查询做市商期权合约手续费
	int ReqQryMMOptionInstrCommRate(const boost::property_tree::ptree & jsonObject, int nRequestID);

	///请求查询报单手续费
	int ReqQryInstrumentOrderCommRate(const boost::property_tree::ptree & jsonObject, int nRequestID);

	///请求查询期权交易成本
	int ReqQryOptionInstrTradeCost(const boost::property_tree::ptree & jsonObject, int nRequestID);

	///请求查询期权合约手续费
	int ReqQryOptionInstrCommRate(const boost::property_tree::ptree & jsonObject, int nRequestID);

	///请求查询执行宣告
	int ReqQryExecOrder(const boost::property_tree::ptree & jsonObject, int nRequestID);

	///请求查询询价
	int ReqQryForQuote(const boost::property_tree::ptree & jsonObject, int nRequestID);

	///请求查询报价
	int ReqQryQuote(const boost::property_tree::ptree & jsonObject, int nRequestID);

	///请求查询锁定
	int ReqQryLock(const boost::property_tree::ptree & jsonObject, int nRequestID);

	///请求查询锁定证券仓位
	int ReqQryLockPosition(const boost::property_tree::ptree & jsonObject, int nRequestID);

	///请求查询ETF期权合约手续费
	int ReqQryETFOptionInstrCommRate(const boost::property_tree::ptree & jsonObject, int nRequestID);

	///请求查询投资者分级
	int ReqQryInvestorLevel(const boost::property_tree::ptree & jsonObject, int nRequestID);

	///请求查询E+1日行权冻结
	int ReqQryExecFreeze(const boost::property_tree::ptree & jsonObject, int nRequestID);

	///请求查询组合合约安全系数
	int ReqQryCombInstrumentGuard(const boost::property_tree::ptree & jsonObject, int nRequestID);

	///请求查询申请组合
	int ReqQryCombAction(const boost::property_tree::ptree & jsonObject, int nRequestID);

	///请求查询转帐流水
	int ReqQryTransferSerial(const boost::property_tree::ptree & jsonObject, int nRequestID);

	///请求查询银期签约关系
	int ReqQryAccountregister(const boost::property_tree::ptree & jsonObject, int nRequestID);

	///请求查询签约银行
	int ReqQryContractBank(const boost::property_tree::ptree & jsonObject, int nRequestID);

	///请求查询预埋单
	int ReqQryParkedOrder(const boost::property_tree::ptree & jsonObject, int nRequestID);

	///请求查询预埋撤单
	int ReqQryParkedOrderAction(const boost::property_tree::ptree & jsonObject, int nRequestID);

	///请求查询交易通知
	int ReqQryTradingNotice(const boost::property_tree::ptree & jsonObject, int nRequestID);

	///请求查询经纪公司交易参数
	int ReqQryBrokerTradingParams(const boost::property_tree::ptree & jsonObject, int nRequestID);

	///请求查询经纪公司交易算法
	int ReqQryBrokerTradingAlgos(const boost::property_tree::ptree & jsonObject, int nRequestID);

	///请求查询监控中心用户令牌
	int ReqQueryCFMMCTradingAccountToken(const boost::property_tree::ptree & jsonObject, int nRequestID);

	///期货发起银行资金转期货请求
	int ReqFromBankToFutureByFuture(const boost::property_tree::ptree & jsonObject, int nRequestID);

	///期货发起期货资金转银行请求
	int ReqFromFutureToBankByFuture(const boost::property_tree::ptree & jsonObject, int nRequestID);

	///期货发起查询银行余额请求
	int ReqQueryBankAccountMoneyByFuture(const boost::property_tree::ptree & jsonObject, int nRequestID);

public:
	///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
	virtual void OnFrontConnected();

	///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
	///@param nReason 错误原因
	///        0x1001 网络读失败
	///        0x1002 网络写失败
	///        0x2001 接收心跳超时
	///        0x2002 发送心跳失败
	///        0x2003 收到错误报文
	virtual void OnFrontDisconnected(int nReason);

	///心跳超时警告。当长时间未收到报文时，该方法被调用。
	///@param nTimeLapse 距离上次接收报文的时间
	virtual void OnHeartBeatWarning(int nTimeLapse);

	///客户端认证响应
	virtual void OnRspAuthenticate(CThostFtdcRspAuthenticateField *pRspAuthenticateField, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);


	///登录请求响应
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///登出请求响应
	virtual void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///用户口令更新请求响应
	virtual void OnRspUserPasswordUpdate(CThostFtdcUserPasswordUpdateField *pUserPasswordUpdate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///资金账户口令更新请求响应
	virtual void OnRspTradingAccountPasswordUpdate(CThostFtdcTradingAccountPasswordUpdateField *pTradingAccountPasswordUpdate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///报单录入请求响应
	virtual void OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///预埋单录入请求响应
	virtual void OnRspParkedOrderInsert(CThostFtdcParkedOrderField *pParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///预埋撤单录入请求响应
	virtual void OnRspParkedOrderAction(CThostFtdcParkedOrderActionField *pParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///报单操作请求响应
	virtual void OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///查询最大报单数量响应
	virtual void OnRspQueryMaxOrderVolume(CThostFtdcQueryMaxOrderVolumeField *pQueryMaxOrderVolume, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///投资者结算结果确认响应
	virtual void OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///删除预埋单响应
	virtual void OnRspRemoveParkedOrder(CThostFtdcRemoveParkedOrderField *pRemoveParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///删除预埋撤单响应
	virtual void OnRspRemoveParkedOrderAction(CThostFtdcRemoveParkedOrderActionField *pRemoveParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///执行宣告录入请求响应
	virtual void OnRspExecOrderInsert(CThostFtdcInputExecOrderField *pInputExecOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///执行宣告操作请求响应
	virtual void OnRspExecOrderAction(CThostFtdcInputExecOrderActionField *pInputExecOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///询价录入请求响应
	virtual void OnRspForQuoteInsert(CThostFtdcInputForQuoteField *pInputForQuote, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///报价录入请求响应
	virtual void OnRspQuoteInsert(CThostFtdcInputQuoteField *pInputQuote, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///报价操作请求响应
	virtual void OnRspQuoteAction(CThostFtdcInputQuoteActionField *pInputQuoteAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///锁定应答
	virtual void OnRspLockInsert(CThostFtdcInputLockField *pInputLock, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///批量报单操作请求响应
	virtual void OnRspBatchOrderAction(CThostFtdcInputBatchOrderActionField *pInputBatchOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///申请组合录入请求响应
	virtual void OnRspCombActionInsert(CThostFtdcInputCombActionField *pInputCombAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询报单响应
	virtual void OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询成交响应
	virtual void OnRspQryTrade(CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询投资者持仓响应
	virtual void OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询资金账户响应
	virtual void OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询投资者响应
	virtual void OnRspQryInvestor(CThostFtdcInvestorField *pInvestor, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询交易编码响应
	virtual void OnRspQryTradingCode(CThostFtdcTradingCodeField *pTradingCode, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询合约保证金率响应
	virtual void OnRspQryInstrumentMarginRate(CThostFtdcInstrumentMarginRateField *pInstrumentMarginRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询合约手续费率响应
	virtual void OnRspQryInstrumentCommissionRate(CThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询交易所响应
	virtual void OnRspQryExchange(CThostFtdcExchangeField *pExchange, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询产品响应
	virtual void OnRspQryProduct(CThostFtdcProductField *pProduct, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询合约响应
	virtual void OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询行情响应
	virtual void OnRspQryDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询投资者结算结果响应
	virtual void OnRspQrySettlementInfo(CThostFtdcSettlementInfoField *pSettlementInfo, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询转帐银行响应
	virtual void OnRspQryTransferBank(CThostFtdcTransferBankField *pTransferBank, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询投资者持仓明细响应
	virtual void OnRspQryInvestorPositionDetail(CThostFtdcInvestorPositionDetailField *pInvestorPositionDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询客户通知响应
	virtual void OnRspQryNotice(CThostFtdcNoticeField *pNotice, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询结算信息确认响应
	virtual void OnRspQrySettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询投资者持仓明细响应
	virtual void OnRspQryInvestorPositionCombineDetail(CThostFtdcInvestorPositionCombineDetailField *pInvestorPositionCombineDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///查询保证金监管系统经纪公司资金账户密钥响应
	virtual void OnRspQryCFMMCTradingAccountKey(CThostFtdcCFMMCTradingAccountKeyField *pCFMMCTradingAccountKey, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询仓单折抵信息响应
	virtual void OnRspQryEWarrantOffset(CThostFtdcEWarrantOffsetField *pEWarrantOffset, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询投资者品种/跨品种保证金响应
	virtual void OnRspQryInvestorProductGroupMargin(CThostFtdcInvestorProductGroupMarginField *pInvestorProductGroupMargin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询交易所保证金率响应
	virtual void OnRspQryExchangeMarginRate(CThostFtdcExchangeMarginRateField *pExchangeMarginRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询交易所调整保证金率响应
	virtual void OnRspQryExchangeMarginRateAdjust(CThostFtdcExchangeMarginRateAdjustField *pExchangeMarginRateAdjust, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询汇率响应
	virtual void OnRspQryExchangeRate(CThostFtdcExchangeRateField *pExchangeRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询二级代理操作员银期权限响应
	virtual void OnRspQrySecAgentACIDMap(CThostFtdcSecAgentACIDMapField *pSecAgentACIDMap, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询产品报价汇率
	virtual void OnRspQryProductExchRate(CThostFtdcProductExchRateField *pProductExchRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询产品组
	virtual void OnRspQryProductGroup(CThostFtdcProductGroupField *pProductGroup, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询做市商合约手续费率响应
	virtual void OnRspQryMMInstrumentCommissionRate(CThostFtdcMMInstrumentCommissionRateField *pMMInstrumentCommissionRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询做市商期权合约手续费响应
	virtual void OnRspQryMMOptionInstrCommRate(CThostFtdcMMOptionInstrCommRateField *pMMOptionInstrCommRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询报单手续费响应
	virtual void OnRspQryInstrumentOrderCommRate(CThostFtdcInstrumentOrderCommRateField *pInstrumentOrderCommRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询期权交易成本响应
	virtual void OnRspQryOptionInstrTradeCost(CThostFtdcOptionInstrTradeCostField *pOptionInstrTradeCost, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询期权合约手续费响应
	virtual void OnRspQryOptionInstrCommRate(CThostFtdcOptionInstrCommRateField *pOptionInstrCommRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询执行宣告响应
	virtual void OnRspQryExecOrder(CThostFtdcExecOrderField *pExecOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询询价响应
	virtual void OnRspQryForQuote(CThostFtdcForQuoteField *pForQuote, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询报价响应
	virtual void OnRspQryQuote(CThostFtdcQuoteField *pQuote, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询锁定应答
	virtual void OnRspQryLock(CThostFtdcLockField *pLock, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询锁定证券仓位应答
	virtual void OnRspQryLockPosition(CThostFtdcLockPositionField *pLockPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询ETF期权合约手续费响应
	virtual void OnRspQryETFOptionInstrCommRate(CThostFtdcETFOptionInstrCommRateField *pETFOptionInstrCommRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询投资者分级
	virtual void OnRspQryInvestorLevel(CThostFtdcInvestorLevelField *pInvestorLevel, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询E+1日行权冻结响应
	virtual void OnRspQryExecFreeze(CThostFtdcExecFreezeField *pExecFreeze, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询组合合约安全系数响应
	virtual void OnRspQryCombInstrumentGuard(CThostFtdcCombInstrumentGuardField *pCombInstrumentGuard, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询申请组合响应
	virtual void OnRspQryCombAction(CThostFtdcCombActionField *pCombAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询转帐流水响应
	virtual void OnRspQryTransferSerial(CThostFtdcTransferSerialField *pTransferSerial, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询银期签约关系响应
	virtual void OnRspQryAccountregister(CThostFtdcAccountregisterField *pAccountregister, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///错误应答
	virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///报单通知
	virtual void OnRtnOrder(CThostFtdcOrderField *pOrder);

	///成交通知
	virtual void OnRtnTrade(CThostFtdcTradeField *pTrade);

	///报单录入错误回报
	virtual void OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo);

	///报单操作错误回报
	virtual void OnErrRtnOrderAction(CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo);

	///合约交易状态通知
	virtual void OnRtnInstrumentStatus(CThostFtdcInstrumentStatusField *pInstrumentStatus);

	///交易所公告通知
	virtual void OnRtnBulletin(CThostFtdcBulletinField *pBulletin);

	///交易通知
	virtual void OnRtnTradingNotice(CThostFtdcTradingNoticeInfoField *pTradingNoticeInfo);

	///提示条件单校验错误
	virtual void OnRtnErrorConditionalOrder(CThostFtdcErrorConditionalOrderField *pErrorConditionalOrder);

	///执行宣告通知
	virtual void OnRtnExecOrder(CThostFtdcExecOrderField *pExecOrder);

	///执行宣告录入错误回报
	virtual void OnErrRtnExecOrderInsert(CThostFtdcInputExecOrderField *pInputExecOrder, CThostFtdcRspInfoField *pRspInfo);

	///执行宣告操作错误回报
	virtual void OnErrRtnExecOrderAction(CThostFtdcExecOrderActionField *pExecOrderAction, CThostFtdcRspInfoField *pRspInfo);

	///询价录入错误回报
	virtual void OnErrRtnForQuoteInsert(CThostFtdcInputForQuoteField *pInputForQuote, CThostFtdcRspInfoField *pRspInfo);

	///报价通知
	virtual void OnRtnQuote(CThostFtdcQuoteField *pQuote);

	///报价录入错误回报
	virtual void OnErrRtnQuoteInsert(CThostFtdcInputQuoteField *pInputQuote, CThostFtdcRspInfoField *pRspInfo);

	///报价操作错误回报
	virtual void OnErrRtnQuoteAction(CThostFtdcQuoteActionField *pQuoteAction, CThostFtdcRspInfoField *pRspInfo);

	///询价通知
	virtual void OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp);

	///保证金监控中心用户令牌
	virtual void OnRtnCFMMCTradingAccountToken(CThostFtdcCFMMCTradingAccountTokenField *pCFMMCTradingAccountToken);

	///锁定通知
	virtual void OnRtnLock(CThostFtdcLockField *pLock);

	///锁定错误通知
	virtual void OnErrRtnLockInsert(CThostFtdcInputLockField *pInputLock, CThostFtdcRspInfoField *pRspInfo);

	///批量报单操作错误回报
	virtual void OnErrRtnBatchOrderAction(CThostFtdcBatchOrderActionField *pBatchOrderAction, CThostFtdcRspInfoField *pRspInfo);

	///申请组合通知
	virtual void OnRtnCombAction(CThostFtdcCombActionField *pCombAction);

	///申请组合录入错误回报
	virtual void OnErrRtnCombActionInsert(CThostFtdcInputCombActionField *pInputCombAction, CThostFtdcRspInfoField *pRspInfo);

	///请求查询签约银行响应
	virtual void OnRspQryContractBank(CThostFtdcContractBankField *pContractBank, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询预埋单响应
	virtual void OnRspQryParkedOrder(CThostFtdcParkedOrderField *pParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询预埋撤单响应
	virtual void OnRspQryParkedOrderAction(CThostFtdcParkedOrderActionField *pParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询交易通知响应
	virtual void OnRspQryTradingNotice(CThostFtdcTradingNoticeField *pTradingNotice, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询经纪公司交易参数响应
	virtual void OnRspQryBrokerTradingParams(CThostFtdcBrokerTradingParamsField *pBrokerTradingParams, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询经纪公司交易算法响应
	virtual void OnRspQryBrokerTradingAlgos(CThostFtdcBrokerTradingAlgosField *pBrokerTradingAlgos, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询监控中心用户令牌
	virtual void OnRspQueryCFMMCTradingAccountToken(CThostFtdcQueryCFMMCTradingAccountTokenField *pQueryCFMMCTradingAccountToken, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///银行发起银行资金转期货通知
	virtual void OnRtnFromBankToFutureByBank(CThostFtdcRspTransferField *pRspTransfer);

	///银行发起期货资金转银行通知
	virtual void OnRtnFromFutureToBankByBank(CThostFtdcRspTransferField *pRspTransfer);

	///银行发起冲正银行转期货通知
	virtual void OnRtnRepealFromBankToFutureByBank(CThostFtdcRspRepealField *pRspRepeal);

	///银行发起冲正期货转银行通知
	virtual void OnRtnRepealFromFutureToBankByBank(CThostFtdcRspRepealField *pRspRepeal);

	///期货发起银行资金转期货通知
	virtual void OnRtnFromBankToFutureByFuture(CThostFtdcRspTransferField *pRspTransfer);

	///期货发起期货资金转银行通知
	virtual void OnRtnFromFutureToBankByFuture(CThostFtdcRspTransferField *pRspTransfer);

	///系统运行时期货端手工发起冲正银行转期货请求，银行处理完毕后报盘发回的通知
	virtual void OnRtnRepealFromBankToFutureByFutureManual(CThostFtdcRspRepealField *pRspRepeal);

	///系统运行时期货端手工发起冲正期货转银行请求，银行处理完毕后报盘发回的通知
	virtual void OnRtnRepealFromFutureToBankByFutureManual(CThostFtdcRspRepealField *pRspRepeal);

	///期货发起查询银行余额通知
	virtual void OnRtnQueryBankBalanceByFuture(CThostFtdcNotifyQueryAccountField *pNotifyQueryAccount);

	///期货发起银行资金转期货错误回报
	virtual void OnErrRtnBankToFutureByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo);

	///期货发起期货资金转银行错误回报
	virtual void OnErrRtnFutureToBankByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo);

	///系统运行时期货端手工发起冲正银行转期货错误回报
	virtual void OnErrRtnRepealBankToFutureByFutureManual(CThostFtdcReqRepealField *pReqRepeal, CThostFtdcRspInfoField *pRspInfo);

	///系统运行时期货端手工发起冲正期货转银行错误回报
	virtual void OnErrRtnRepealFutureToBankByFutureManual(CThostFtdcReqRepealField *pReqRepeal, CThostFtdcRspInfoField *pRspInfo);

	///期货发起查询银行余额错误回报
	virtual void OnErrRtnQueryBankBalanceByFuture(CThostFtdcReqQueryAccountField *pReqQueryAccount, CThostFtdcRspInfoField *pRspInfo);

	///期货发起冲正银行转期货请求，银行处理完毕后报盘发回的通知
	virtual void OnRtnRepealFromBankToFutureByFuture(CThostFtdcRspRepealField *pRspRepeal);

	///期货发起冲正期货转银行请求，银行处理完毕后报盘发回的通知
	virtual void OnRtnRepealFromFutureToBankByFuture(CThostFtdcRspRepealField *pRspRepeal);

	///期货发起银行资金转期货应答
	virtual void OnRspFromBankToFutureByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///期货发起期货资金转银行应答
	virtual void OnRspFromFutureToBankByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///期货发起查询银行余额应答
	virtual void OnRspQueryBankAccountMoneyByFuture(CThostFtdcReqQueryAccountField *pReqQueryAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///银行发起银期开户通知
	virtual void OnRtnOpenAccountByBank(CThostFtdcOpenAccountField *pOpenAccount);

	///银行发起银期销户通知
	virtual void OnRtnCancelAccountByBank(CThostFtdcCancelAccountField *pCancelAccount);

	///银行发起变更银行账号通知
	virtual void OnRtnChangeAccountByBank(CThostFtdcChangeAccountField *pChangeAccount);
};

#endif//_H_CTPAPI_H_