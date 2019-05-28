#ifndef _H_CTP_H_
#define _H_CTP_H_

#include "CTPCommon.h"

class CTPMdApi : public CThostFtdcMdSpi
{
private:
	CTPUserLoginInfo m_UserInfo;
	CThostFtdcMdApi* m_pApi;
	CThostFtdcMdSpi* m_pSpi;
public:
	CTPMdApi(const CTPUserLoginInfo& userInfo, CThostFtdcMdSpi* pSpi);
	~CTPMdApi();

	CThostFtdcMdApi* getApi();

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
	virtual void OnHeartBeatWarning(int nTimeLapse) {
		//if (m_pSpi) m_pSpi->OnHeartBeatWarning(nTimeLapse);
	};


	///登录请求响应
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		//if (m_pSpi) m_pSpi->OnRspUserLogin(pRspUserLogin, pRspInfo, nRequestID, bIsLast);
	};

	///登出请求响应
	virtual void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		//if (m_pSpi) m_pSpi->OnRspUserLogout(pUserLogout, pRspInfo, nRequestID, bIsLast);
	};

	///错误应答
	virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		//if (m_pSpi) m_pSpi->OnRspError(pRspInfo, nRequestID, bIsLast);
	};

	///订阅行情应答
	virtual void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		if (m_pSpi) m_pSpi->OnRspSubMarketData(pSpecificInstrument, pRspInfo, nRequestID, bIsLast);
	};

	///取消订阅行情应答
	virtual void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		if (m_pSpi) m_pSpi->OnRspUnSubMarketData(pSpecificInstrument, pRspInfo, nRequestID, bIsLast);
	};

	///订阅询价应答
	virtual void OnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		if (m_pSpi) m_pSpi->OnRspSubForQuoteRsp(pSpecificInstrument, pRspInfo, nRequestID, bIsLast);
	};

	///取消订阅询价应答
	virtual void OnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		if (m_pSpi) m_pSpi->OnRspUnSubForQuoteRsp(pSpecificInstrument, pRspInfo, nRequestID, bIsLast);
	};

	///深度行情通知
	virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData) {
		if (m_pSpi) m_pSpi->OnRtnDepthMarketData(pDepthMarketData);
	};

	///询价通知
	virtual void OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp) {
		if (m_pSpi) m_pSpi->OnRtnForQuoteRsp(pForQuoteRsp);
	};
};

class CTPTraderApi : public CThostFtdcTraderSpi
{
private:
	CTPUserLoginInfo m_UserInfo;
	CThostFtdcTraderApi* m_pApi;
	CThostFtdcTraderSpi* m_pSpi;
public:
	CTPTraderApi(const CTPUserLoginInfo& userInfo, CThostFtdcTraderSpi* pSpi);
	~CTPTraderApi();

	CThostFtdcTraderApi* getApi();

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
	virtual void OnHeartBeatWarning(int nTimeLapse) {
		if (m_pSpi) m_pSpi->OnHeartBeatWarning(nTimeLapse);
	};

	///客户端认证响应
	virtual void OnRspAuthenticate(CThostFtdcRspAuthenticateField *pRspAuthenticateField, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		if (m_pSpi) m_pSpi->OnRspAuthenticate(pRspAuthenticateField, pRspInfo, nRequestID, bIsLast);
	};


	///登录请求响应
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		if (m_pSpi) m_pSpi->OnRspUserLogin(pRspUserLogin, pRspInfo, nRequestID, bIsLast);
	};

	///登出请求响应
	virtual void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		if (m_pSpi) m_pSpi->OnRspUserLogout(pUserLogout, pRspInfo, nRequestID, bIsLast);
	};

	///用户口令更新请求响应
	virtual void OnRspUserPasswordUpdate(CThostFtdcUserPasswordUpdateField *pUserPasswordUpdate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		if (m_pSpi) m_pSpi->OnRspUserPasswordUpdate(pUserPasswordUpdate, pRspInfo, nRequestID, bIsLast);
	};

	///资金账户口令更新请求响应
	virtual void OnRspTradingAccountPasswordUpdate(CThostFtdcTradingAccountPasswordUpdateField *pTradingAccountPasswordUpdate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		if (m_pSpi) m_pSpi->OnRspTradingAccountPasswordUpdate(pTradingAccountPasswordUpdate, pRspInfo, nRequestID, bIsLast);
	};

	///报单录入请求响应
	virtual void OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		if (m_pSpi) m_pSpi->OnRspOrderInsert(pInputOrder, pRspInfo, nRequestID, bIsLast);
	};

	///预埋单录入请求响应
	virtual void OnRspParkedOrderInsert(CThostFtdcParkedOrderField *pParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		if (m_pSpi) m_pSpi->OnRspParkedOrderInsert(pParkedOrder, pRspInfo, nRequestID, bIsLast);
	};

	///预埋撤单录入请求响应
	virtual void OnRspParkedOrderAction(CThostFtdcParkedOrderActionField *pParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		if (m_pSpi) m_pSpi->OnRspParkedOrderAction(pParkedOrderAction, pRspInfo, nRequestID, bIsLast);
	};

	///报单操作请求响应
	virtual void OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		if (m_pSpi) m_pSpi->OnRspOrderAction(pInputOrderAction, pRspInfo, nRequestID, bIsLast);
	};

	///查询最大报单数量响应
	virtual void OnRspQueryMaxOrderVolume(CThostFtdcQueryMaxOrderVolumeField *pQueryMaxOrderVolume, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		if (m_pSpi) m_pSpi->OnRspQueryMaxOrderVolume(pQueryMaxOrderVolume, pRspInfo, nRequestID, bIsLast);
	};

	///投资者结算结果确认响应
	virtual void OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		if (m_pSpi) m_pSpi->OnRspSettlementInfoConfirm(pSettlementInfoConfirm, pRspInfo, nRequestID, bIsLast);
	};

	///删除预埋单响应
	virtual void OnRspRemoveParkedOrder(CThostFtdcRemoveParkedOrderField *pRemoveParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		if (m_pSpi) m_pSpi->OnRspRemoveParkedOrder(pRemoveParkedOrder, pRspInfo, nRequestID, bIsLast);
	};

	///删除预埋撤单响应
	virtual void OnRspRemoveParkedOrderAction(CThostFtdcRemoveParkedOrderActionField *pRemoveParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		if (m_pSpi) m_pSpi->OnRspRemoveParkedOrderAction(pRemoveParkedOrderAction, pRspInfo, nRequestID, bIsLast);
	};

	///执行宣告录入请求响应
	virtual void OnRspExecOrderInsert(CThostFtdcInputExecOrderField *pInputExecOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		if (m_pSpi) m_pSpi->OnRspExecOrderInsert(pInputExecOrder, pRspInfo, nRequestID, bIsLast);
	};

	///执行宣告操作请求响应
	virtual void OnRspExecOrderAction(CThostFtdcInputExecOrderActionField *pInputExecOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		if (m_pSpi) m_pSpi->OnRspExecOrderAction(pInputExecOrderAction, pRspInfo, nRequestID, bIsLast);
	};

	///询价录入请求响应
	virtual void OnRspForQuoteInsert(CThostFtdcInputForQuoteField *pInputForQuote, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		if (m_pSpi) m_pSpi->OnRspForQuoteInsert(pInputForQuote, pRspInfo, nRequestID, bIsLast);
	};

	///报价录入请求响应
	virtual void OnRspQuoteInsert(CThostFtdcInputQuoteField *pInputQuote, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		if (m_pSpi) m_pSpi->OnRspQuoteInsert(pInputQuote, pRspInfo, nRequestID, bIsLast);
	};

	///报价操作请求响应
	virtual void OnRspQuoteAction(CThostFtdcInputQuoteActionField *pInputQuoteAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		if (m_pSpi) m_pSpi->OnRspQuoteAction(pInputQuoteAction, pRspInfo, nRequestID, bIsLast);
	};

	///锁定应答
	virtual void OnRspLockInsert(CThostFtdcInputLockField *pInputLock, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		if (m_pSpi) m_pSpi->OnRspLockInsert(pInputLock, pRspInfo, nRequestID, bIsLast);
	};

	///批量报单操作请求响应
	virtual void OnRspBatchOrderAction(CThostFtdcInputBatchOrderActionField *pInputBatchOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		if (m_pSpi) m_pSpi->OnRspBatchOrderAction(pInputBatchOrderAction, pRspInfo, nRequestID, bIsLast);
	};

	///申请组合录入请求响应
	virtual void OnRspCombActionInsert(CThostFtdcInputCombActionField *pInputCombAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		if (m_pSpi) m_pSpi->OnRspCombActionInsert(pInputCombAction, pRspInfo, nRequestID, bIsLast);
	};

	///请求查询报单响应
	virtual void OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		if (m_pSpi) m_pSpi->OnRspQryOrder(pOrder, pRspInfo, nRequestID, bIsLast);
	};

	///请求查询成交响应
	virtual void OnRspQryTrade(CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		if (m_pSpi) m_pSpi->OnRspQryTrade(pTrade, pRspInfo, nRequestID, bIsLast);
	};

	///请求查询投资者持仓响应
	virtual void OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		if (m_pSpi) m_pSpi->OnRspQryInvestorPosition(pInvestorPosition, pRspInfo, nRequestID, bIsLast);
	};

	///请求查询资金账户响应
	virtual void OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		if (m_pSpi) m_pSpi->OnRspQryTradingAccount(pTradingAccount, pRspInfo, nRequestID, bIsLast);
	};

	///请求查询投资者响应
	virtual void OnRspQryInvestor(CThostFtdcInvestorField *pInvestor, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		if (m_pSpi) m_pSpi->OnRspQryInvestor(pInvestor, pRspInfo, nRequestID, bIsLast);
	};

	///请求查询交易编码响应
	virtual void OnRspQryTradingCode(CThostFtdcTradingCodeField *pTradingCode, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		if (m_pSpi) m_pSpi->OnRspQryTradingCode(pTradingCode, pRspInfo, nRequestID, bIsLast);
	};

	///请求查询合约保证金率响应
	virtual void OnRspQryInstrumentMarginRate(CThostFtdcInstrumentMarginRateField *pInstrumentMarginRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		if (m_pSpi) m_pSpi->OnRspQryInstrumentMarginRate(pInstrumentMarginRate, pRspInfo, nRequestID, bIsLast);
	};

	///请求查询合约手续费率响应
	virtual void OnRspQryInstrumentCommissionRate(CThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		if (m_pSpi) m_pSpi->OnRspQryInstrumentCommissionRate(pInstrumentCommissionRate, pRspInfo, nRequestID, bIsLast);
	};

	///请求查询交易所响应
	virtual void OnRspQryExchange(CThostFtdcExchangeField *pExchange, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		if (m_pSpi) m_pSpi->OnRspQryExchange(pExchange, pRspInfo, nRequestID, bIsLast);
	};

	///请求查询产品响应
	virtual void OnRspQryProduct(CThostFtdcProductField *pProduct, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		if (m_pSpi) m_pSpi->OnRspQryProduct(pProduct, pRspInfo, nRequestID, bIsLast);
	};

	///请求查询合约响应
	virtual void OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		if (m_pSpi) m_pSpi->OnRspQryInstrument(pInstrument, pRspInfo, nRequestID, bIsLast);
	};

	///请求查询行情响应
	virtual void OnRspQryDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		if (m_pSpi) m_pSpi->OnRspQryDepthMarketData(pDepthMarketData, pRspInfo, nRequestID, bIsLast);
	};

	///请求查询投资者结算结果响应
	virtual void OnRspQrySettlementInfo(CThostFtdcSettlementInfoField *pSettlementInfo, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		if (m_pSpi) m_pSpi->OnRspQrySettlementInfo(pSettlementInfo, pRspInfo, nRequestID, bIsLast);
	};

	///请求查询转帐银行响应
	virtual void OnRspQryTransferBank(CThostFtdcTransferBankField *pTransferBank, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		if (m_pSpi) m_pSpi->OnRspQryTransferBank(pTransferBank, pRspInfo, nRequestID, bIsLast);
	};

	///请求查询投资者持仓明细响应
	virtual void OnRspQryInvestorPositionDetail(CThostFtdcInvestorPositionDetailField *pInvestorPositionDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		if (m_pSpi) m_pSpi->OnRspQryInvestorPositionDetail(pInvestorPositionDetail, pRspInfo, nRequestID, bIsLast);
	};

	///请求查询客户通知响应
	virtual void OnRspQryNotice(CThostFtdcNoticeField *pNotice, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		if (m_pSpi) m_pSpi->OnRspQryNotice(pNotice, pRspInfo, nRequestID, bIsLast);
	};

	///请求查询结算信息确认响应
	virtual void OnRspQrySettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		if (m_pSpi) m_pSpi->OnRspQrySettlementInfoConfirm(pSettlementInfoConfirm, pRspInfo, nRequestID, bIsLast);
	};

	///请求查询投资者持仓明细响应
	virtual void OnRspQryInvestorPositionCombineDetail(CThostFtdcInvestorPositionCombineDetailField *pInvestorPositionCombineDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		if (m_pSpi) m_pSpi->OnRspQryInvestorPositionCombineDetail(pInvestorPositionCombineDetail, pRspInfo, nRequestID, bIsLast);
	};

	///查询保证金监管系统经纪公司资金账户密钥响应
	virtual void OnRspQryCFMMCTradingAccountKey(CThostFtdcCFMMCTradingAccountKeyField *pCFMMCTradingAccountKey, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		if (m_pSpi) m_pSpi->OnRspQryCFMMCTradingAccountKey(pCFMMCTradingAccountKey, pRspInfo, nRequestID, bIsLast);
	};

	///请求查询仓单折抵信息响应
	virtual void OnRspQryEWarrantOffset(CThostFtdcEWarrantOffsetField *pEWarrantOffset, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		if (m_pSpi) m_pSpi->OnRspQryEWarrantOffset(pEWarrantOffset, pRspInfo, nRequestID, bIsLast);
	};

	///请求查询投资者品种/跨品种保证金响应
	virtual void OnRspQryInvestorProductGroupMargin(CThostFtdcInvestorProductGroupMarginField *pInvestorProductGroupMargin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		if (m_pSpi) m_pSpi->OnRspQryInvestorProductGroupMargin(pInvestorProductGroupMargin, pRspInfo, nRequestID, bIsLast);
	};

	///请求查询交易所保证金率响应
	virtual void OnRspQryExchangeMarginRate(CThostFtdcExchangeMarginRateField *pExchangeMarginRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		if (m_pSpi) m_pSpi->OnRspQryExchangeMarginRate(pExchangeMarginRate, pRspInfo, nRequestID, bIsLast);
	};

	///请求查询交易所调整保证金率响应
	virtual void OnRspQryExchangeMarginRateAdjust(CThostFtdcExchangeMarginRateAdjustField *pExchangeMarginRateAdjust, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		if (m_pSpi) m_pSpi->OnRspQryExchangeMarginRateAdjust(pExchangeMarginRateAdjust, pRspInfo, nRequestID, bIsLast);
	};

	///请求查询汇率响应
	virtual void OnRspQryExchangeRate(CThostFtdcExchangeRateField *pExchangeRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		if (m_pSpi) m_pSpi->OnRspQryExchangeRate(pExchangeRate, pRspInfo, nRequestID, bIsLast);
	};

	///请求查询二级代理操作员银期权限响应
	virtual void OnRspQrySecAgentACIDMap(CThostFtdcSecAgentACIDMapField *pSecAgentACIDMap, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		if (m_pSpi) m_pSpi->OnRspQrySecAgentACIDMap(pSecAgentACIDMap, pRspInfo, nRequestID, bIsLast);
	};

	///请求查询产品报价汇率
	virtual void OnRspQryProductExchRate(CThostFtdcProductExchRateField *pProductExchRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		if (m_pSpi) m_pSpi->OnRspQryProductExchRate(pProductExchRate, pRspInfo, nRequestID, bIsLast);
	};

	///请求查询产品组
	virtual void OnRspQryProductGroup(CThostFtdcProductGroupField *pProductGroup, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		if (m_pSpi) m_pSpi->OnRspQryProductGroup(pProductGroup, pRspInfo, nRequestID, bIsLast);
	};

	///请求查询做市商合约手续费率响应
	virtual void OnRspQryMMInstrumentCommissionRate(CThostFtdcMMInstrumentCommissionRateField *pMMInstrumentCommissionRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		if (m_pSpi) m_pSpi->OnRspQryMMInstrumentCommissionRate(pMMInstrumentCommissionRate, pRspInfo, nRequestID, bIsLast);
	};

	///请求查询做市商期权合约手续费响应
	virtual void OnRspQryMMOptionInstrCommRate(CThostFtdcMMOptionInstrCommRateField *pMMOptionInstrCommRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		if (m_pSpi) m_pSpi->OnRspQryMMOptionInstrCommRate(pMMOptionInstrCommRate, pRspInfo, nRequestID, bIsLast);
	};

	///请求查询报单手续费响应
	virtual void OnRspQryInstrumentOrderCommRate(CThostFtdcInstrumentOrderCommRateField *pInstrumentOrderCommRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		if (m_pSpi) m_pSpi->OnRspQryInstrumentOrderCommRate(pInstrumentOrderCommRate, pRspInfo, nRequestID, bIsLast);
	};

	///请求查询期权交易成本响应
	virtual void OnRspQryOptionInstrTradeCost(CThostFtdcOptionInstrTradeCostField *pOptionInstrTradeCost, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		if (m_pSpi) m_pSpi->OnRspQryOptionInstrTradeCost(pOptionInstrTradeCost, pRspInfo, nRequestID, bIsLast);
	};

	///请求查询期权合约手续费响应
	virtual void OnRspQryOptionInstrCommRate(CThostFtdcOptionInstrCommRateField *pOptionInstrCommRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		if (m_pSpi) m_pSpi->OnRspQryOptionInstrCommRate(pOptionInstrCommRate, pRspInfo, nRequestID, bIsLast);
	};

	///请求查询执行宣告响应
	virtual void OnRspQryExecOrder(CThostFtdcExecOrderField *pExecOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		if (m_pSpi) m_pSpi->OnRspQryExecOrder(pExecOrder, pRspInfo, nRequestID, bIsLast);
	};

	///请求查询询价响应
	virtual void OnRspQryForQuote(CThostFtdcForQuoteField *pForQuote, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		if (m_pSpi) m_pSpi->OnRspQryForQuote(pForQuote, pRspInfo, nRequestID, bIsLast);
	};

	///请求查询报价响应
	virtual void OnRspQryQuote(CThostFtdcQuoteField *pQuote, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		if (m_pSpi) m_pSpi->OnRspQryQuote(pQuote, pRspInfo, nRequestID, bIsLast);
	};

	///请求查询锁定应答
	virtual void OnRspQryLock(CThostFtdcLockField *pLock, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		if (m_pSpi) m_pSpi->OnRspQryLock(pLock, pRspInfo, nRequestID, bIsLast);
	};

	///请求查询锁定证券仓位应答
	virtual void OnRspQryLockPosition(CThostFtdcLockPositionField *pLockPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		if (m_pSpi) m_pSpi->OnRspQryLockPosition(pLockPosition, pRspInfo, nRequestID, bIsLast);
	};

	///请求查询ETF期权合约手续费响应
	virtual void OnRspQryETFOptionInstrCommRate(CThostFtdcETFOptionInstrCommRateField *pETFOptionInstrCommRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		if (m_pSpi) m_pSpi->OnRspQryETFOptionInstrCommRate(pETFOptionInstrCommRate, pRspInfo, nRequestID, bIsLast);
	};

	///请求查询投资者分级
	virtual void OnRspQryInvestorLevel(CThostFtdcInvestorLevelField *pInvestorLevel, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		if (m_pSpi) m_pSpi->OnRspQryInvestorLevel(pInvestorLevel, pRspInfo, nRequestID, bIsLast);
	};

	///请求查询E+1日行权冻结响应
	virtual void OnRspQryExecFreeze(CThostFtdcExecFreezeField *pExecFreeze, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		if (m_pSpi) m_pSpi->OnRspQryExecFreeze(pExecFreeze, pRspInfo, nRequestID, bIsLast);
	};

	///请求查询组合合约安全系数响应
	virtual void OnRspQryCombInstrumentGuard(CThostFtdcCombInstrumentGuardField *pCombInstrumentGuard, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		if (m_pSpi) m_pSpi->OnRspQryCombInstrumentGuard(pCombInstrumentGuard, pRspInfo, nRequestID, bIsLast);
	};

	///请求查询申请组合响应
	virtual void OnRspQryCombAction(CThostFtdcCombActionField *pCombAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		if (m_pSpi) m_pSpi->OnRspQryCombAction(pCombAction, pRspInfo, nRequestID, bIsLast);
	};

	///请求查询转帐流水响应
	virtual void OnRspQryTransferSerial(CThostFtdcTransferSerialField *pTransferSerial, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		if (m_pSpi) m_pSpi->OnRspQryTransferSerial(pTransferSerial, pRspInfo, nRequestID, bIsLast);
	};

	///请求查询银期签约关系响应
	virtual void OnRspQryAccountregister(CThostFtdcAccountregisterField *pAccountregister, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		if (m_pSpi) m_pSpi->OnRspQryAccountregister(pAccountregister, pRspInfo, nRequestID, bIsLast);
	};

	///错误应答
	virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		if (m_pSpi) m_pSpi->OnRspError(pRspInfo, nRequestID, bIsLast);
	};

	///报单通知
	virtual void OnRtnOrder(CThostFtdcOrderField *pOrder) {
		if (m_pSpi) m_pSpi->OnRtnOrder(pOrder);
	};

	///成交通知
	virtual void OnRtnTrade(CThostFtdcTradeField *pTrade) {
		if (m_pSpi) m_pSpi->OnRtnTrade(pTrade);
	};

	///报单录入错误回报
	virtual void OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo) {
		if (m_pSpi) m_pSpi->OnErrRtnOrderInsert(pInputOrder, pRspInfo);
	};

	///报单操作错误回报
	virtual void OnErrRtnOrderAction(CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo) {
		if (m_pSpi) m_pSpi->OnErrRtnOrderAction(pOrderAction, pRspInfo);
	};

	///合约交易状态通知
	virtual void OnRtnInstrumentStatus(CThostFtdcInstrumentStatusField *pInstrumentStatus) {
		if (m_pSpi) m_pSpi->OnRtnInstrumentStatus(pInstrumentStatus);
	};

	///交易所公告通知
	virtual void OnRtnBulletin(CThostFtdcBulletinField *pBulletin) {
		if (m_pSpi) m_pSpi->OnRtnBulletin(pBulletin);
	};

	///交易通知
	virtual void OnRtnTradingNotice(CThostFtdcTradingNoticeInfoField *pTradingNoticeInfo) {
		if (m_pSpi) m_pSpi->OnRtnTradingNotice(pTradingNoticeInfo);
	};

	///提示条件单校验错误
	virtual void OnRtnErrorConditionalOrder(CThostFtdcErrorConditionalOrderField *pErrorConditionalOrder) {
		if (m_pSpi) m_pSpi->OnRtnErrorConditionalOrder(pErrorConditionalOrder);
	};

	///执行宣告通知
	virtual void OnRtnExecOrder(CThostFtdcExecOrderField *pExecOrder) {
		if (m_pSpi) m_pSpi->OnRtnExecOrder(pExecOrder);
	};

	///执行宣告录入错误回报
	virtual void OnErrRtnExecOrderInsert(CThostFtdcInputExecOrderField *pInputExecOrder, CThostFtdcRspInfoField *pRspInfo) {
		if (m_pSpi) m_pSpi->OnErrRtnExecOrderInsert(pInputExecOrder, pRspInfo);
	};

	///执行宣告操作错误回报
	virtual void OnErrRtnExecOrderAction(CThostFtdcExecOrderActionField *pExecOrderAction, CThostFtdcRspInfoField *pRspInfo) {
		if (m_pSpi) m_pSpi->OnErrRtnExecOrderAction(pExecOrderAction, pRspInfo);
	};

	///询价录入错误回报
	virtual void OnErrRtnForQuoteInsert(CThostFtdcInputForQuoteField *pInputForQuote, CThostFtdcRspInfoField *pRspInfo) {
		if (m_pSpi) m_pSpi->OnErrRtnForQuoteInsert(pInputForQuote, pRspInfo);
	};

	///报价通知
	virtual void OnRtnQuote(CThostFtdcQuoteField *pQuote) {
		if (m_pSpi) m_pSpi->OnRtnQuote(pQuote);
	};

	///报价录入错误回报
	virtual void OnErrRtnQuoteInsert(CThostFtdcInputQuoteField *pInputQuote, CThostFtdcRspInfoField *pRspInfo) {
		if (m_pSpi) m_pSpi->OnErrRtnQuoteInsert(pInputQuote, pRspInfo);
	};

	///报价操作错误回报
	virtual void OnErrRtnQuoteAction(CThostFtdcQuoteActionField *pQuoteAction, CThostFtdcRspInfoField *pRspInfo) {
		if (m_pSpi) m_pSpi->OnErrRtnQuoteAction(pQuoteAction, pRspInfo);
	};

	///询价通知
	virtual void OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp) {
		if (m_pSpi) m_pSpi->OnRtnForQuoteRsp(pForQuoteRsp);
	};

	///保证金监控中心用户令牌
	virtual void OnRtnCFMMCTradingAccountToken(CThostFtdcCFMMCTradingAccountTokenField *pCFMMCTradingAccountToken) {
		if (m_pSpi) m_pSpi->OnRtnCFMMCTradingAccountToken(pCFMMCTradingAccountToken);
	};

	///锁定通知
	virtual void OnRtnLock(CThostFtdcLockField *pLock) {
		if (m_pSpi) m_pSpi->OnRtnLock(pLock);
	};

	///锁定错误通知
	virtual void OnErrRtnLockInsert(CThostFtdcInputLockField *pInputLock, CThostFtdcRspInfoField *pRspInfo) {
		if (m_pSpi) m_pSpi->OnErrRtnLockInsert(pInputLock, pRspInfo);
	};

	///批量报单操作错误回报
	virtual void OnErrRtnBatchOrderAction(CThostFtdcBatchOrderActionField *pBatchOrderAction, CThostFtdcRspInfoField *pRspInfo) {
		if (m_pSpi) m_pSpi->OnErrRtnBatchOrderAction(pBatchOrderAction, pRspInfo);
	};

	///申请组合通知
	virtual void OnRtnCombAction(CThostFtdcCombActionField *pCombAction) {
		if (m_pSpi) m_pSpi->OnRtnCombAction(pCombAction);
	};

	///申请组合录入错误回报
	virtual void OnErrRtnCombActionInsert(CThostFtdcInputCombActionField *pInputCombAction, CThostFtdcRspInfoField *pRspInfo) {
		if (m_pSpi) m_pSpi->OnErrRtnCombActionInsert(pInputCombAction, pRspInfo);
	};

	///请求查询签约银行响应
	virtual void OnRspQryContractBank(CThostFtdcContractBankField *pContractBank, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		if (m_pSpi) m_pSpi->OnRspQryContractBank(pContractBank, pRspInfo, nRequestID, bIsLast);
	};

	///请求查询预埋单响应
	virtual void OnRspQryParkedOrder(CThostFtdcParkedOrderField *pParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		if (m_pSpi) m_pSpi->OnRspQryParkedOrder(pParkedOrder, pRspInfo, nRequestID, bIsLast);
	};

	///请求查询预埋撤单响应
	virtual void OnRspQryParkedOrderAction(CThostFtdcParkedOrderActionField *pParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		if (m_pSpi) m_pSpi->OnRspQryParkedOrderAction(pParkedOrderAction, pRspInfo, nRequestID, bIsLast);
	};

	///请求查询交易通知响应
	virtual void OnRspQryTradingNotice(CThostFtdcTradingNoticeField *pTradingNotice, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		if (m_pSpi) m_pSpi->OnRspQryTradingNotice(pTradingNotice, pRspInfo, nRequestID, bIsLast);
	};

	///请求查询经纪公司交易参数响应
	virtual void OnRspQryBrokerTradingParams(CThostFtdcBrokerTradingParamsField *pBrokerTradingParams, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		if (m_pSpi) m_pSpi->OnRspQryBrokerTradingParams(pBrokerTradingParams, pRspInfo, nRequestID, bIsLast);
	};

	///请求查询经纪公司交易算法响应
	virtual void OnRspQryBrokerTradingAlgos(CThostFtdcBrokerTradingAlgosField *pBrokerTradingAlgos, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		if (m_pSpi) m_pSpi->OnRspQryBrokerTradingAlgos(pBrokerTradingAlgos, pRspInfo, nRequestID, bIsLast);
	};

	///请求查询监控中心用户令牌
	virtual void OnRspQueryCFMMCTradingAccountToken(CThostFtdcQueryCFMMCTradingAccountTokenField *pQueryCFMMCTradingAccountToken, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		if (m_pSpi) m_pSpi->OnRspQueryCFMMCTradingAccountToken(pQueryCFMMCTradingAccountToken, pRspInfo, nRequestID, bIsLast);
	};

	///银行发起银行资金转期货通知
	virtual void OnRtnFromBankToFutureByBank(CThostFtdcRspTransferField *pRspTransfer) {
		if (m_pSpi) m_pSpi->OnRtnFromBankToFutureByBank(pRspTransfer);
	};

	///银行发起期货资金转银行通知
	virtual void OnRtnFromFutureToBankByBank(CThostFtdcRspTransferField *pRspTransfer) {
		if (m_pSpi) m_pSpi->OnRtnFromFutureToBankByBank(pRspTransfer);
	};

	///银行发起冲正银行转期货通知
	virtual void OnRtnRepealFromBankToFutureByBank(CThostFtdcRspRepealField *pRspRepeal) {
		if (m_pSpi) m_pSpi->OnRtnRepealFromBankToFutureByBank(pRspRepeal);
	};

	///银行发起冲正期货转银行通知
	virtual void OnRtnRepealFromFutureToBankByBank(CThostFtdcRspRepealField *pRspRepeal) {
		if (m_pSpi) m_pSpi->OnRtnRepealFromFutureToBankByBank(pRspRepeal);
	};

	///期货发起银行资金转期货通知
	virtual void OnRtnFromBankToFutureByFuture(CThostFtdcRspTransferField *pRspTransfer) {
		if (m_pSpi) m_pSpi->OnRtnFromBankToFutureByFuture(pRspTransfer);
	};

	///期货发起期货资金转银行通知
	virtual void OnRtnFromFutureToBankByFuture(CThostFtdcRspTransferField *pRspTransfer) {
		if (m_pSpi) m_pSpi->OnRtnFromFutureToBankByFuture(pRspTransfer);
	};

	///系统运行时期货端手工发起冲正银行转期货请求，银行处理完毕后报盘发回的通知
	virtual void OnRtnRepealFromBankToFutureByFutureManual(CThostFtdcRspRepealField *pRspRepeal) {
		if (m_pSpi) m_pSpi->OnRtnRepealFromBankToFutureByFutureManual(pRspRepeal);
	};

	///系统运行时期货端手工发起冲正期货转银行请求，银行处理完毕后报盘发回的通知
	virtual void OnRtnRepealFromFutureToBankByFutureManual(CThostFtdcRspRepealField *pRspRepeal) {
		if (m_pSpi) m_pSpi->OnRtnRepealFromFutureToBankByFutureManual(pRspRepeal);
	};

	///期货发起查询银行余额通知
	virtual void OnRtnQueryBankBalanceByFuture(CThostFtdcNotifyQueryAccountField *pNotifyQueryAccount) {
		if (m_pSpi) m_pSpi->OnRtnQueryBankBalanceByFuture(pNotifyQueryAccount);
	};

	///期货发起银行资金转期货错误回报
	virtual void OnErrRtnBankToFutureByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo) {
		if (m_pSpi) m_pSpi->OnErrRtnBankToFutureByFuture(pReqTransfer, pRspInfo);
	};

	///期货发起期货资金转银行错误回报
	virtual void OnErrRtnFutureToBankByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo) {
		if (m_pSpi) m_pSpi->OnErrRtnFutureToBankByFuture(pReqTransfer, pRspInfo);
	};

	///系统运行时期货端手工发起冲正银行转期货错误回报
	virtual void OnErrRtnRepealBankToFutureByFutureManual(CThostFtdcReqRepealField *pReqRepeal, CThostFtdcRspInfoField *pRspInfo) {
		if (m_pSpi) m_pSpi->OnErrRtnRepealBankToFutureByFutureManual(pReqRepeal, pRspInfo);
	};

	///系统运行时期货端手工发起冲正期货转银行错误回报
	virtual void OnErrRtnRepealFutureToBankByFutureManual(CThostFtdcReqRepealField *pReqRepeal, CThostFtdcRspInfoField *pRspInfo) {
		if (m_pSpi) m_pSpi->OnErrRtnRepealFutureToBankByFutureManual(pReqRepeal, pRspInfo);
	};

	///期货发起查询银行余额错误回报
	virtual void OnErrRtnQueryBankBalanceByFuture(CThostFtdcReqQueryAccountField *pReqQueryAccount, CThostFtdcRspInfoField *pRspInfo) {
		if (m_pSpi) m_pSpi->OnErrRtnQueryBankBalanceByFuture(pReqQueryAccount, pRspInfo);
	};

	///期货发起冲正银行转期货请求，银行处理完毕后报盘发回的通知
	virtual void OnRtnRepealFromBankToFutureByFuture(CThostFtdcRspRepealField *pRspRepeal) {
		if (m_pSpi) m_pSpi->OnRtnRepealFromBankToFutureByFuture(pRspRepeal);
	};

	///期货发起冲正期货转银行请求，银行处理完毕后报盘发回的通知
	virtual void OnRtnRepealFromFutureToBankByFuture(CThostFtdcRspRepealField *pRspRepeal) {
		if (m_pSpi) m_pSpi->OnRtnRepealFromFutureToBankByFuture(pRspRepeal);
	};

	///期货发起银行资金转期货应答
	virtual void OnRspFromBankToFutureByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		if (m_pSpi) m_pSpi->OnRspFromBankToFutureByFuture(pReqTransfer, pRspInfo, nRequestID, bIsLast);
	};

	///期货发起期货资金转银行应答
	virtual void OnRspFromFutureToBankByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		if (m_pSpi) m_pSpi->OnRspFromFutureToBankByFuture(pReqTransfer, pRspInfo, nRequestID, bIsLast);
	};

	///期货发起查询银行余额应答
	virtual void OnRspQueryBankAccountMoneyByFuture(CThostFtdcReqQueryAccountField *pReqQueryAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		if (m_pSpi) m_pSpi->OnRspQueryBankAccountMoneyByFuture(pReqQueryAccount, pRspInfo, nRequestID, bIsLast);
	};

	///银行发起银期开户通知
	virtual void OnRtnOpenAccountByBank(CThostFtdcOpenAccountField *pOpenAccount) {
		if (m_pSpi) m_pSpi->OnRtnOpenAccountByBank(pOpenAccount);
	};

	///银行发起银期销户通知
	virtual void OnRtnCancelAccountByBank(CThostFtdcCancelAccountField *pCancelAccount) {
		if (m_pSpi) m_pSpi->OnRtnCancelAccountByBank(pCancelAccount);
	};

	///银行发起变更银行账号通知
	virtual void OnRtnChangeAccountByBank(CThostFtdcChangeAccountField *pChangeAccount) {
		if (m_pSpi) m_pSpi->OnRtnChangeAccountByBank(pChangeAccount);
	};

};

#endif//_H_CTP_H_