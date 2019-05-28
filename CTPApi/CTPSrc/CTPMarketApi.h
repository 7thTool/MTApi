#pragma once

#include "CTPTraderApi.h"

namespace MTP {

class CTPMarketApi;
class CTPTradeApi;

	class CTPOwner
	{
	public:
		friend class CTPMarketApi;
		friend class CTPTradeApi;
	protected:
		CTPUserLoginInfo logininfo_;
		std::atomic<int> requestid_; //最后一次请求ID
	public:
		CTPOwner();
		virtual ~CTPOwner();

		inline const CTPUserLoginInfo& logininfo() { return logininfo_; }

		int gen_requestid();
		int get_requestid();

	protected:
		virtual bool IsNeedRequestAndSet(int nStatus) { return false; }

		virtual void on_connect() {}
		virtual void on_disconnect() {}

		virtual void on_market_date(const char* date) {}

		virtual void OnRspQryExchange(CThostFtdcExchangeField *pExchange, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

		virtual void OnRspQryProduct(CThostFtdcProductField *pProduct, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

		virtual void OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

		virtual void OnRspQryDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

		virtual void on_market_init() {}

		virtual void on_normal() {}

		virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData) { }

		virtual void OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp) {}

		virtual void OnRtnInstrumentStatus(CThostFtdcInstrumentStatusField *pInstrumentStatus) {}
	};

class CTPMarketApi 
	: public CThostFtdcMdSpi
	, public XUtil::XStateEx<CTP_STAT_MAX>
{
private:
	CTPOwner* owner_;
	CThostFtdcMdApi* api_;

	CTPUserLoginInfo userinfo_;
	CThostFtdcRspUserLoginField rsp_userinfo_;	//登录信息

	std::vector<const char*> subscribes_; //订阅信息

public:
	CTPMarketApi(CTPOwner* owner);
	~CTPMarketApi();

	void start(char* str);
	void stop();

	inline CThostFtdcMdApi* api() { return api_; }

	inline const CThostFtdcRspUserLoginField& rsp_logininfo() { return rsp_userinfo_; }

	bool IsOk();

	template<class Ty>
	void SubscribeInstrument(Ty *pInstrument, int nCount)
	{
		ASSERT(IsOk());
		UnSubscribe();
		if (pInstrument && nCount) {
			for (int i = 0; i < nCount; i++)
			{
				subscribes_.push_back(pInstrument[i].InstrumentID);
			}
			api_->SubscribeMarketData((char**)&subscribes_[0], subscribes_.size());
		}
	}

	template<class Ty>
	void SubscribeCommodity(const std::vector<Ty>& commoditys)
	{
		ASSERT(IsOk());
		UnSubscribe();
		if (!commoditys.empty()) {
			for (int i = 0; i < commoditys.size(); i++)
			{
				subscribes_.push_back(commoditys[i]->Code);
			}
			api_->SubscribeMarketData((char**)&subscribes_[0], subscribes_.size());
		}
	}

	void Subscribe(const char *code[], size_t count)
	{
		ASSERT(IsOk());
		UnSubscribe();
		if (code && count) {
			api_->SubscribeMarketData((char**)code, count);
		}
	}

	void UnSubscribe()
	{
		if (!subscribes_.empty()) {
			api_->UnSubscribeMarketData((char**)&subscribes_[0], subscribes_.size());
			subscribes_.clear();
		}
	}

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
	virtual void OnHeartBeatWarning(int nTimeLapse) {}

	///登录请求响应
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///登出请求响应
	virtual void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///错误应答
	virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///深度行情通知
	virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);
};

class CTPTradeApi: public CTPTraderApi
{
	typedef CTPTraderApi Base;
private:
	//boost::recursive_mutex mutex_;
	CTPOwner* owner_;
public:
	CTPTradeApi(CTPOwner* owner);
	~CTPTradeApi();
		
	bool start(char *str);
	void stop();

	virtual int NextRequestID();
	virtual int NextRequest();
	virtual int Request(int nStatus, int nRequestID);
	virtual void OnLogin();
	virtual void OnNormal();

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

	///合约交易状态通知
	virtual void OnRtnInstrumentStatus(CThostFtdcInstrumentStatusField *pInstrumentStatus);
	
	///请求查询交易所响应
	virtual void OnRspQryExchange(CThostFtdcExchangeField *pExchange, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询产品响应
	virtual void OnRspQryProduct(CThostFtdcProductField *pProduct, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询合约响应
	virtual void OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询行情响应
	virtual void OnRspQryDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
};

}
