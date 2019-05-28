#pragma once

#include <TDBase.hpp>
//#include <TDSHMBase.hpp>
#include <TDDataSet.hpp>
#include <TDSrcBase.hpp>
#include "CTPTraderApi.h"
#include <XUtil/XApi.hpp>
#include <XUtil/XStr.hpp>
#include <XUtil/XLogger.hpp>
#include <XUtil/XState.hpp>

namespace MTP {

template<class Server>
class CTPTrade
	: public CTPTraderApi
{
	typedef CTPTrade<Server> This;
	typedef CTPTraderApi Base;
protected:
	Server* srv_ = nullptr;
	size_t peer_ = 0;
	std::string trading_day_;
	std::string old_trading_day_;
	boost::posix_time::ptime last_instate_time_;
	//std::mutex mutex_;
	//std::condition_variable cv_;
	//int nRequestID_ = 0;
	//CThostFtdcRspInfoField RspInfo_ = {0};
	//bool bIsLast_ = false;
	std::shared_ptr<boost::property_tree::ptree> result_;

public:
	CTPTrade(Server* srv, size_t peer, const CTPUserLoginInfo& logininfo) : Base(logininfo)
	//, SHMBase()
	, srv_(srv), peer_(peer)
	, result_(std::make_shared<boost::property_tree::ptree>())
	{
		set_online();
	}

	~CTPTrade()
	{
		set_offline();
	}

	void set_online()
	{
		last_instate_time_ = boost::posix_time::microsec_clock::local_time();
		const auto& flowpath = srv_->data_path().string();
		Base::start((char *)flowpath.c_str());
	}

	void set_offline() 
	{
		Base::stop();
		ResetAllState();
	}

	void on_idle()
	{
		if (!IsStateful(CTP_STAT_MAX)) {
			boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();
			const boost::posix_time::time_duration td = now - last_instate_time_;
			if (td.total_seconds() > 90) { //初始化超时,重新初始化
				set_offline();
				set_online();
			}
		}
	}

	void on_request (size_t peer, const std::string& method, const std::shared_ptr<boost::property_tree::ptree>& params, size_t id)
	{
		int rlt = -1;
		if(method == "user_login") {
			ASSERT(0); //外部应该处理了
		} else if(method == "user_logout") {
			ASSERT(0); //外部应该处理了
		} else if(method == "ReqUserPasswordUpdate") {
			CThostFtdcUserPasswordUpdateField req = { 0 };
			BuildDefaultRequest(req);
			///原来的口令
			snprintf(req.OldPassword, _countof(req.OldPassword), "%s", params->get<std::string>("OldPassword", "").c_str());
			///新的口令
			snprintf(req.NewPassword, _countof(req.NewPassword), "%s", params->get<std::string>("NewPassword", "").c_str());
			DoRequest(api_->ReqUserPasswordUpdate, req, id);
		}
		if (rlt)
		{
			params->put("ErrorID", rlt);
			//params->put("ErrorMsg", "");
			params->put("IsLast", true);
			srv_->post_response(peer, id, params, true);
			return;
		}
		else
		{
			//
		}
	}
	// bool didReceiveCall (size_t peer, const std::string& method, const std::shared_ptr<boost::property_tree::ptree>& p, std::shared_ptr<boost::property_tree::ptree>& result) 
	// {
	// 	std::unique_lock<std::mutex> lock(mutex_);

	// 	boost::property_tree::ptree& params = *p;

	// 	nRequestID_ = 0;
	// 	memset(&RspInfo_, 0, sizeof(RspInfo_));
	// 	bIsLast_ = false;
	// 	result_->clear();
	// 	boost::property_tree::ptree& data = *result_;

	// 	int rlt = -1;
	// 	if(method == "user_login") {
	// 		ASSERT(0); //外部应该处理了
	// 	} else if(method == "user_logout") {
	// 		ASSERT(0); //外部应该处理了
	// 	} else if(method == "ReqUserPasswordUpdate") {
	// 		CThostFtdcUserPasswordUpdateField req = { 0 };
	// 		BuildDefaultRequest(req);
	// 		///原来的口令
	// 		snprintf(req.OldPassword, _countof(req.OldPassword), "%s", params.get<std::string>("OldPassword", "").c_str());
	// 		///新的口令
	// 		snprintf(req.NewPassword, _countof(req.NewPassword), "%s", params.get<std::string>("NewPassword", "").c_str());
	// 		nRequestID_ = NextRequestID();
	// 		DoRequest(api_->ReqUserPasswordUpdate, req, nRequestID_);
	// 	}
	// 	if (rlt)
	// 	{
	// 		//data.put("broker_id", userinfo_.BrokerID);
	// 		data.put("userid", userinfo_.UserID);
	// 		//data.put("ErrorMsg", pRspInfo->ErrorMsg);
	// 		data.put("ErrorID", rlt);
	// 		data.put("IsLast", true);
	// 		return false;
	// 	}
	// 	else
	// 	{
	// 		if (!cv_.wait_for(lock, std::chrono::milliseconds(3000),
	// 			[this]() { return bIsLast_; })) {
				
	// 		}
	// 		if(!bIsLast_ || IsErrorRspInfo(&RspInfo_))
	// 		{
	// 			//
	// 		}
	// 	}
		
	// 	return true;
	// }

protected:

	template<typename TRequest>
	void BuildDefaultRequest(TRequest& req) 
	{
		memcpy(req.BrokerID, m_UserInfo.BrokerID,sizeof(req.BrokerID));
		memcpy(req.UserID, m_UserInfo.UserID, sizeof(req.UserID));
	}
	template<typename TRequest>
	void BuildDefaultAccountRequest(TRequest& req)
	{
		memcpy(req.BrokerID, m_UserInfo.BrokerID, sizeof(req.BrokerID));
		memcpy(req.AccountID, m_UserInfo.UserID, sizeof(req.AccountID));
	}
	template<typename TRequest>
	void BuildDefaultInvestorRequest(TRequest& req)
	{
		memcpy(req.BrokerID, m_UserInfo.BrokerID, sizeof(req.BrokerID));
		memcpy(req.InvestorID, m_UserInfo.UserID, sizeof(req.InvestorID));
	}
	template<typename TRequest>
	void BuildDefaultTradeRequest(TRequest& req)
	{
		memcpy(req.BrokerID, m_UserInfo.BrokerID, sizeof(req.BrokerID));
		memcpy(req.UserID, m_UserInfo.UserID, sizeof(req.UserID));
		memcpy(req.InvestorID, m_UserInfo.UserID, sizeof(req.InvestorID));

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

	bool BuildDefaultJsonObject(boost::property_tree::ptree & data, const CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
	{
		try
		{
			//data.put("broker_id", userinfo_.BrokerID);
			data.put("userid", userinfo_.UserID);
			if (pRspInfo) {
				data.put("ErrorMsg", pRspInfo->ErrorMsg);
				data.put("ErrorID", pRspInfo->ErrorID);
			}
			else {
				//data.put("ErrorMsg", "");
				data.put("ErrorID", 0);
			}
			data.put("IsLast", bIsLast);
			return true;
		}
		catch (...)
		{
		}
		return false;
	}

	enum InfoFlag
	{
		COMPLEX,
		SIMPLE,
	};

	void BuildUserInfo(boost::property_tree::ptree & data, InfoFlag flag = InfoFlag::COMPLEX)
	{
		data.put("userid", userinfo_.UserID);
		data.put("status", IsStateful(CTP_STAT_MAX)?"loading":"online");
		if(flag==InfoFlag::SIMPLE) {
			return;
		}
		boost::property_tree::ptree account_array;
		for(size_t i = 0; i < 1; i++)
		{
			boost::property_tree::ptree account;
			account.put("accountid", m_TradingAccount.AccountID);
			account.put("balance", m_TradingAccount.Balance);
			account.put("frozen", m_TradingAccount.FrozenMargin + m_TradingAccount.FrozenCash + m_TradingAccount.FrozenCommission);
			//经纪公司代码
			account.put("BrokerID", m_TradingAccount.BrokerID);
			///投资者帐号
			account.put("AccountID", m_TradingAccount.AccountID);
			///上次质押金额
			account.put("PreMortgage", m_TradingAccount.PreMortgage);
			///上次信用额度
			account.put("PreCredit", m_TradingAccount.PreCredit);
			///上次存款额
			account.put("PreDeposit", m_TradingAccount.PreDeposit);
			///上次结算准备金
			account.put("PreBalance", m_TradingAccount.PreBalance);
			///上次占用的保证金
			account.put("PreMargin", m_TradingAccount.PreMargin);
			///利息基数
			account.put("InterestBase", m_TradingAccount.InterestBase);
			///利息收入
			account.put("Interest", m_TradingAccount.Interest);
			///入金金额
			account.put("Deposit", m_TradingAccount.Deposit);
			///出金金额
			account.put("Withdraw", m_TradingAccount.Withdraw);
			///冻结的保证金
			account.put("FrozenMargin", m_TradingAccount.FrozenMargin);
			///冻结的资金
			account.put("FrozenCash", m_TradingAccount.FrozenCash);
			///冻结的手续费
			account.put("FrozenCommission", m_TradingAccount.FrozenCommission);
			///当前保证金总额
			account.put("CurrMargin", m_TradingAccount.CurrMargin);
			///资金差额
			account.put("CashIn", m_TradingAccount.CashIn);
			///手续费
			account.put("Commission", m_TradingAccount.Commission);
			///平仓盈亏
			account.put("CloseProfit", m_TradingAccount.CloseProfit);
			///持仓盈亏
			account.put("PositionProfit", m_TradingAccount.PositionProfit);
			///期货结算准备金
			account.put("Balance", m_TradingAccount.Balance);
			///可用资金
			account.put("Available", m_TradingAccount.Available);
			///可取资金
			account.put("WithdrawQuota", m_TradingAccount.WithdrawQuota);
			///基本准备金
			account.put("Reserve", m_TradingAccount.Reserve);
			///交易日
			account.put("TradingDay", m_TradingAccount.TradingDay);
			///结算编号
			account.put("SettlementID", m_TradingAccount.SettlementID);
			///信用额度
			account.put("Credit", m_TradingAccount.Credit);
			///质押金额
			account.put("Mortgage", m_TradingAccount.Mortgage);
			///交易所保证金
			account.put("ExchangeMargin", m_TradingAccount.ExchangeMargin);
			///投资者交割保证金
			account.put("DeliveryMargin", m_TradingAccount.DeliveryMargin);
			///交易所交割保证金
			account.put("ExchangeDeliveryMargin", m_TradingAccount.ExchangeDeliveryMargin);
			///保底期货结算准备金
			account.put("ReserveBalance", m_TradingAccount.ReserveBalance);
			///币种代码
			account.put("CurrencyID", m_TradingAccount.CurrencyID);
			///上次货币质入金额
			account.put("PreFundMortgageIn", m_TradingAccount.PreFundMortgageIn);
			///上次货币质出金额
			account.put("PreFundMortgageOut", m_TradingAccount.PreFundMortgageOut);
			///货币质入金额
			account.put("FundMortgageIn", m_TradingAccount.FundMortgageIn);
			///货币质出金额
			account.put("FundMortgageOut", m_TradingAccount.FundMortgageOut);
			///货币质押余额
			account.put("FundMortgageAvailable", m_TradingAccount.FundMortgageAvailable);
			///可质押货币金额
			account.put("MortgageableFund", m_TradingAccount.MortgageableFund);
			///特殊产品占用保证金
			account.put("SpecProductMargin", m_TradingAccount.SpecProductMargin);
			///特殊产品冻结保证金
			account.put("SpecProductFrozenMargin", m_TradingAccount.SpecProductFrozenMargin);
			///特殊产品手续费
			account.put("SpecProductCommission", m_TradingAccount.SpecProductCommission);
			///特殊产品冻结手续费
			account.put("SpecProductFrozenCommission", m_TradingAccount.SpecProductFrozenCommission);
			///特殊产品持仓盈亏
			account.put("SpecProductPositionProfit", m_TradingAccount.SpecProductPositionProfit);
			///特殊产品平仓盈亏
			account.put("SpecProductCloseProfit", m_TradingAccount.SpecProductCloseProfit);
			///根据持仓盈亏算法计算的特殊产品持仓盈亏
			account.put("SpecProductPositionProfitByAlg", m_TradingAccount.SpecProductPositionProfitByAlg);
			///特殊产品交易所保证金
			account.put("SpecProductExchangeMargin", m_TradingAccount.SpecProductExchangeMargin);
			///业务类型
			account.put("BizType", m_TradingAccount.BizType);
			///添加Data
			account_array.push_back(std::make_pair("", account));
		}
		data.push_back(std::make_pair("account", account_array));
	}
	
	int NextRequest()
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
			//CTP_STAT_ORDER, //委托
			//CTP_STAT_TRADE, //成交
			//CTP_STAT_POSITION, //持仓
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
	
	//以下是CTPTraderApi回调函数

	///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
	virtual void OnFrontConnected()
	{
		Base::OnFrontConnected();
	}

	///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
	///@param nReason 错误原因
	///        0x1001 网络读失败
	///        0x1002 网络写失败
	///        0x2001 接收心跳超时
	///        0x2002 发送心跳失败
	///        0x2003 收到错误报文
	virtual void OnFrontDisconnected(int nReason)
	{
		Base::OnFrontDisconnected(nReason);
		// std::lock_guard<std::mutex> lock(mutex_);
		// boost::property_tree::ptree& data = *result_;
		// RspInfo_.ErrorID = nReason;
		// //RspInfo_.ErrorMsg = 
		// if (BuildDefaultJsonObject(data, &RspInfo_, 0, true)) {
		// 	//
		// }
		// cv_.notify_one();
		std::shared_ptr<boost::property_tree::ptree> user = std::make_shared<boost::property_tree::ptree>();
		BuildUserInfo(*user, InfoFlag::SIMPLE);
		srv_->on_user_update(peer_, user);
	}

	void OnLogin()
	{
		LOG4I("on_trade_update %s", m_UserInfo.TradingDay);
		old_trading_day_ = trading_day_;
		trading_day_ = m_UserInfo.TradingDay;
		if(old_trading_day_ != trading_day_) {
			//std::shared_ptr<boost::property_tree::ptree> user = std::make_shared<boost::property_tree::ptree>();
			//BuildUserInfo(*user, InfoFlag::SIMPLE);
			//srv_->on_user_update(peer_, user);
		}
	}
	
	void OnNormal()
	{
		//如果交易日变化才要重新初始化数据
		LOG4I("on_trade_init from %s to %s", old_trading_day_.c_str(), trading_day_.c_str());
		if (old_trading_day_ != trading_day_) {
			//
		}
		//std::lock_guard<std::mutex> lock(mutex_);
		//cv_.notify_one();
		std::shared_ptr<boost::property_tree::ptree> user = std::make_shared<boost::property_tree::ptree>();
		BuildUserInfo(*user);
		srv_->on_user_update(peer_, user);
	}
	
	///错误应答
	virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
	{
		// std::lock_guard<std::mutex> lock(mutex_);
		std::shared_ptr<boost::property_tree::ptree> data = result_;
		if (BuildDefaultJsonObject(*data, pRspInfo, nRequestID, bIsLast)) {
				
		}
		if(bIsLast) {
			//cv_.notify_one();
		}
		srv_->post_response(peer_, nRequestID, data, IsErrorRspInfo(pRspInfo));
	}

	///用户口令更新请求响应
	virtual void OnRspUserPasswordUpdate(CThostFtdcUserPasswordUpdateField *pUserPasswordUpdate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
	{
		// std::lock_guard<std::mutex> lock(mutex_);
		try
		{
			std::shared_ptr<boost::property_tree::ptree> data = result_;
			if (BuildDefaultJsonObject(*data, pRspInfo, nRequestID, bIsLast)) {
				///经纪公司代码
				data->put("BrokerID", pUserPasswordUpdate->BrokerID);
				///用户代码
				data->put("UserID", pUserPasswordUpdate->UserID);
				///经纪公司代码
				data->put("OldPassword", pUserPasswordUpdate->OldPassword);
				///经纪公司代码
				data->put("NewPassword", pUserPasswordUpdate->NewPassword);
			}
			srv_->post_response(peer_, nRequestID, data, IsErrorRspInfo(pRspInfo));
		}
		catch (...)
		{
		}
		if(bIsLast) {
			// cv_.notify_one();
		}
	}
};

}