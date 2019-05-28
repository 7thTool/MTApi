#pragma once

#include <MDBase.hpp>
//#include <MDSHMBase.hpp>
#include <MDDataSet.hpp>
#include <MDSrcBase.hpp>
#include "CTPMarketApi.h"
#include <XUtil/XApi.hpp>
#include <XUtil/XStr.hpp>
#include <XUtil/XLogger.hpp>
#include <XUtil/XState.hpp>

//#define TEST_SHM_EXCHANGE_REMOVE_ADD

namespace MTP {

template<class Server>
class CTPMarket
    : public MDSrcBase<CTPMarket<Server>,ExchangeInfo,ProductInfo,CommodityInfo>
	//, public MDSHMBase<CTPMarket<Server>>
    , public CTPOwner
	, public XUtil::XStateEx<CTP_STAT_MAX+1>
{
	typedef CTPMarket<Server> This;
	typedef MDSrcBase<CTPMarket<Server>,ExchangeInfo,ProductInfo,CommodityInfo> Base;
	//typedef MDSHMBase<CTPMarket<Server>> SHMBase;
protected:
	Server* srv_;
	//std::recursive_mutex> mutex_;
	std::shared_ptr<CTPTradeApi> td_api_;
	std::shared_ptr<CTPMarketApi> md_api_;
	boost::asio::deadline_timer timer_;
	//boost::posix_time::ptime last_instate_time_;
	std::map<std::string,std::shared_ptr<ExchangeSetEx>> exchangesets_;

public:
	CTPMarket(Server* srv) : Base()
	//, SHMBase()
	, srv_(srv), timer_(service())
	, exchangesets_{ {XFE_SH,nullptr}, {XFE_NY,nullptr}, {XFE_DL,nullptr}, {XFE_ZZ,nullptr}, {XFE_CF,nullptr} }
	{
		
	}
	
    bool start(char* xml, int xmlflag = 0)
    {
        Base::start(xml, xmlflag);

		//SHMBase::init();

		if (!parse_logininfo(logininfo_, cfg_)) {
			return false;
		}

		bool reinit = false;
		//从共享内存恢复
		init_exchangeset();
		for(auto it = exchangesets_.begin(); it != exchangesets_.end(); ++it)
		{
			auto exchange_set = it->second;
			if (exchange_set)
			{
				ExchangeInfo& exchange = exchange_set->get();
#ifndef TEST_SHM_EXCHANGE_REMOVE_ADD
		 		trading_day_ = XUtil::tostr(exchange.TradingDay);
#endif//
				on_add_exchange(std::make_shared<ExchangeInfo>(exchange));
				size_t product_count = exchange_set->GetFieldSize(IDF_PRODUCTINFO);
				for (size_t j = 0; j < product_count; j++)
				{
					ProductInfo *product = (ProductInfo *)exchange_set->GetFieldValue(IDF_PRODUCTINFO, j);
					on_add_product(std::make_shared<ProductInfo>(*product));
				}
				size_t commodity_count = exchange_set->GetFieldSize(IDF_COMMODITYINFO);
				for (size_t j = 0; j < commodity_count; j++)
				{
					CommodityInfo *commodity = (CommodityInfo *)exchange_set->GetFieldValue(IDF_COMMODITYINFO, j);
					on_add_commodity(std::make_shared<CommodityInfo>(*commodity));
				}
			} 
			else
			{
				reinit = true;
				clear_exchangeset();
				break;
			}
		}
		// const std::set<const char*,XUtil::strless> ExchangeIDs = {
		// 	XFE_SH,
		// 	XFE_NY,
		// 	XFE_DL,
		// 	XFE_ZZ,
		// 	XFE_CF
		// };
		// auto exchanges = SHMMarketData::instance().get_exchange_list();
		// reinit = exchanges.empty();
		// for(size_t i = 0; i < exchanges.size(); i++) {
		// 	const char* exchange = exchanges[i]->Exchange;
		// 	if (ExchangeIDs.find(exchange) == ExchangeIDs.end()) {
		// 		continue;
		// 	}
		// 	SHMExchangePtr exchange_ptr = exchanges[i];
		// 	if (exchange_ptr) {
		// 		trading_day_ = XUtil::tostr(exchange_ptr->TradingDay);
		// 		ExchangeInfoPtr exchange = std::make_shared<ExchangeInfo>(*exchange_ptr);
		// 		//exchanges_.emplace_back(exchange);
		// 		on_add_exchange(exchange);
		// 		auto products = SHMMarketData::instance().get_product_list(exchange_ptr->Exchange);
		// 		for(size_t j = 0; j < products.size(); j++) {
		// 			ProductInfoPtr& product = std::make_shared<ProductInfo>(*products[j]);
		// 			//products_.emplace_back(product);
		// 			on_add_product(product);
		// 		}
		// 		auto commoditys = SHMMarketData::instance().get_commodity_list(exchange_ptr->Exchange);
		// 		for(size_t j = 0; j < commoditys.size(); j++) {
		// 			CommodityInfoPtr& commodity = std::make_shared<CommodityInfo>(*commoditys[j]);
		// 			//commoditys_.emplace_back(commodity);
		// 			on_add_commodity(commodity);
		// 		}
		// 	}
		// }
		if(!reinit) {
			SetState({CTP_STAT_EXCHANGE,CTP_STAT_PRODUCT,CTP_STAT_INSTRUMENT,CTP_STAT_DEPTHMARKETDATA,CTP_STAT_INSTRUMENTSTATUS});
		}

		//std::string str_req_login = "{ \"RequestApi\":\"ReqLogin\", \"Data\": {\"BrokerID\":\"9999\", \"UserID\":\"115569\", \"Password\":\"654321\",\"FrontAddress\":\"tcp://180.168.146.187:10001\",\"MdFrontAddress\":\"tcp://180.168.146.187:10011\" } }";

		service().post(boost::bind(&This::set_online, this));
		
		return true;
    }

    void stop()
    {
		unregister_timer();
		service().post(boost::bind(&This::set_offline, this));
		
		Base::stop();

		clear_exchangeset();
		exchangesets_.clear();

		//SHMBase::clear();
        
		ResetAllState();
	}

	void register_timer() 
	{
		timer_.expires_from_now(boost::posix_time::milliseconds(90*1000));
		timer_.async_wait(boost::bind(&This::on_timer, this, boost::asio::placeholders::error));
	}

	void unregister_timer()
	{
		boost::system::error_code ec;
		timer_.cancel(ec);
	}

	//以下是当前线程回调函数
protected:
	
	void set_online()
	{
		const auto& flowpath = data_path().string();

		td_api_ = std::make_shared<CTPTradeApi>(this);
		td_api_->start((char *)flowpath.c_str());

		md_api_ = std::make_shared<CTPMarketApi>(this);
		md_api_->start((char *)flowpath.c_str());
	}

	void set_offline() 
	{
		if (md_api_) {
			md_api_->stop();
			md_api_.reset();
			//md_subscribes_.clear();
		}

		if (td_api_) {
			td_api_->stop();
			td_api_.reset();
		}
	}

	void on_timer(const boost::system::error_code& ec)
	{
		if (ec) {
			return;
		}

		if (!td_api_) {
			return;
		}
		//if (!last_instate_time_.is_not_a_date_time()) {
			if (!IsStateful(CTP_STAT_MAX)) {
				//boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();
				//const boost::posix_time::time_duration td = now - last_instate_time_;
				//if (td.total_seconds() > 90) { //初始化超时,重新初始化
					set_offline();
					set_online();
				//}
			}
		//}
	}

	void init_exchangeset()
	{
		for(auto it = exchangesets_.begin(); it != exchangesets_.end(); ++it)
		{
			ASSERT(!it->second);
			it->second = std::dynamic_pointer_cast<ExchangeSetEx>(MakeExchangeSetPtr(it->first.c_str()));//dynamic_cast<ExchangeSetEx*>(IExchangeSet::Ref(it->first.c_str()));
		}
	}

	void clear_exchangeset()
	{
		for(auto& it : exchangesets_)
		{
			if(it.second) {
				it.second.reset();
			}
		}
	}

	std::shared_ptr<ExchangeSetEx> find_exchangeset(const char* exchange) {
		auto it = exchangesets_.find(exchange);
		if (it != exchangesets_.end()) {
			return it->second;
		}
		return nullptr;
	}

	void on_src_date(const std::string& date)
	{
		LOG4I("on_market_update %s", date.c_str());
		old_trading_day_ = trading_day_;
		trading_day_ = date;
		if(old_trading_day_ != trading_day_) {
			if(!exchanges_.empty()) {
				clear_exchangeset();
				MarketDataSetEx::Instance().RemoveExchange(&exchanges_[0], exchanges_.size());
				//SHMMarketData::instance().remove_exchange_ptr(&exchanges_[0], exchanges_.size());
				srv_->on_remove_exchange(exchanges_);
			}
			ResetAllState();
		}
	}

	void on_src_init()
	{
		//如果交易日变化才要重新初始化数据
		LOG4I("on_market_init from %s to %s", old_trading_day_.c_str(), trading_day_.c_str());
		if (old_trading_day_ != trading_day_) {
			uint32_t holiday[] = { 20180101 };
			MarketDataSetEx::Instance().AddExchange(trading_day_.c_str(), &exchanges_[0], exchanges_.size()
			, &products_[0], products_.size(), &commoditys_[0], commoditys_.size());
			init_exchangeset();
			//SHMMarketData::instance().init_exchange_ptr(&exchanges_[0], exchanges_.size()
			//, &products_[0], products_.size(), &commoditys_[0], commoditys_.size());
			srv_->on_add_exchange(exchanges_);
		}
		//重新订阅行情
		//等待行情也连上
		while (!md_api_->IsOk())
		{
			std::this_thread::yield();
		}
		md_api_->SubscribeCommodity(commoditys_);
	}

	void on_commodity_update(const CommodityInfoPtr& ptr)
	{
		CommodityInfoPtr commodity = Base::on_commodity_update(ptr);
		if(commodity)
		{
			if (IsStateful(CTP_STAT_MAX)) {
				MarketDataSetEx::Instance().UpdateMarket(commodity.get());
				// auto shm_commodity = SHMMarketData::instance().get_commodity(commodity->Exchange, commodity->Product, commodity->Code);
				// if (shm_commodity) {
				// 	shm_commodity->UpdateMarket(commodity);
				// }
				srv_->on_commodity_update(commodity);
			}
		}
	}

	void on_commodity_status(const CommodityInfoPtr& update)
	{
		if(update)
		{
			if(update->Code[0]) {
				CommodityInfoPtr commodity = Base::on_commodity_status(update);
				// if (IsStateful(CTP_STAT_MAX)) {
				// 	auto shm_commodity = SHMMarketData::instance().get_commodity(commodity->Exchange, commodity->Product, commodity->Code);
				// 	if (shm_commodity) {
				// 		shm_commodity->UpdateStatus(commodity);
				// 	}
				// }
			}
			else if (update->Product[0]) {
				ProductInfoPtr commodity = Base::on_product_status(update);
				// if (IsStateful(CTP_STAT_MAX)) {
				// 	auto shm_product = SHMMarketData::instance().get_product(commodity->Exchange, commodity->Product);
				// 	if (shm_product) {
				// 		shm_product->UpdateStatus(commodity);
				// 	}
				// }
			}
			else {
				ExchangeInfoPtr commodity = Base::on_exchange_status(update);
				// if (IsStateful(CTP_STAT_MAX)) {
				// 	auto shm_exchange = SHMMarketData::instance().get_exchange(commodity->Exchange);
				// 	if (shm_exchange) {
				// 		shm_exchange->UpdateStatus(commodity);
				// 	}
				// }
			}
			if (IsStateful(CTP_STAT_MAX)) {
				MarketDataSetEx::Instance().UpdateStatus(update.get());
				srv_->on_commodity_status(update);
			}
		}
	}

	//以下是CTPOwner回调函数
protected:
	bool IsNeedRequestAndSet(int nStatus)
	{
		//std::unique_lock<std::recursive_mutex> lock(mutex_);
		if (IsStateful(nStatus) || IsInStateful(nStatus))
		{
			return false;
		}
		else
		{
			SetInState(nStatus);
			return true;
		}
	}

	void on_connect() 
	{
		//std::unique_lock<std::recursive_mutex> lock(mutex_);
		//last_instate_time_ = boost::posix_time::microsec_clock::local_time();
		register_timer();
	}

	void on_disconnect() 
	{
		//std::unique_lock<std::recursive_mutex> lock(mutex_);
		//last_instate_time_ = boost::posix_time::microsec_clock::local_time();
	}

	void on_market_date(const char* date)
	{
		service().post(boost::bind(&This::on_src_date, this, std::string(date)));
	}

	void on_normal()
	{
	}

	void on_market_init()
	{
		//std::unique_lock<std::recursive_mutex> lock(mutex_);
		service().post(boost::bind(&This::on_src_init, this));
		SetState(CTP_STAT_MAX);
	}

	void OnRspQryExchange(CThostFtdcExchangeField *pExchange, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
	{
		//std::unique_lock<std::recursive_mutex> lock(mutex_);
		ASSERT(IsInStateful(CTP_STAT_EXCHANGE) && !IsStateful(CTP_STAT_EXCHANGE));
		ExchangeInfoPtr exchange;
		if(pExchange) {
			exchange = std::make_shared<ExchangeInfo>();
			CTPExchange2Exchange(*pExchange, exchange);
		}
		service().post(boost::bind(&This::on_add_exchange, this, exchange));
		if (bIsLast)
		{
			SetState(CTP_STAT_EXCHANGE);
		}
	}

	void OnRspQryProduct(CThostFtdcProductField *pProduct, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
	{
		//std::unique_lock<std::recursive_mutex> lock(mutex_);
		ASSERT(IsInStateful(CTP_STAT_PRODUCT) && !IsStateful(CTP_STAT_PRODUCT));
		ProductInfoPtr product;
		if(pProduct) {
			product = std::make_shared<ProductInfo>();
			CTPProduct2Product(*pProduct, product);
		}
		service().post(boost::bind(&This::on_add_product, this, product));
		if (bIsLast)
		{
			SetState(CTP_STAT_PRODUCT);
		}
	}

	void OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
	{
		//std::unique_lock<std::recursive_mutex> lock(mutex_);
		ASSERT(IsInStateful(CTP_STAT_INSTRUMENT) && !IsStateful(CTP_STAT_INSTRUMENT));
		CommodityInfoPtr commodity;
		if(pInstrument) { 
			commodity = std::make_shared<CommodityInfo>();
			CTPInstrument2Commodity(*pInstrument, commodity);
		}
		service().post(boost::bind(&This::on_add_commodity, this, commodity));
		if (bIsLast)
		{
			SetState(CTP_STAT_INSTRUMENT);
		}
	}

	void OnRspQryDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
	{
		//std::unique_lock<std::recursive_mutex> lock(mutex_);
		ASSERT(IsInStateful(CTP_STAT_DEPTHMARKETDATA) && !IsStateful(CTP_STAT_DEPTHMARKETDATA));
		CommodityInfoPtr commodity;
		if(pDepthMarketData) {
			commodity = std::make_shared<CommodityInfo>();
			CTPMarketData2Commodity(*pDepthMarketData, commodity);
		}
		service().post(boost::bind(&This::on_init_commodity, this, commodity));
		if (bIsLast)
		{
			SetState(CTP_STAT_DEPTHMARKETDATA);
		}
	}

	void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
	{
		if (pDepthMarketData)
		{
			CommodityInfoPtr commodity = std::make_shared<CommodityInfo>();
			CTPMarketData2Commodity(*pDepthMarketData, commodity);
			service().post(boost::bind(&This::on_commodity_update, this, commodity));
		}
	}

	void OnRtnInstrumentStatus(CThostFtdcInstrumentStatusField *pInstrumentStatus)
	{
		if (pInstrumentStatus)
		{
            CommodityInfoPtr commodity = std::make_shared<CommodityInfo>();
			CTPInstrumentStatus2Commodity(*pInstrumentStatus, commodity);
			service().post(boost::bind(&This::on_commodity_status, this, commodity));
		}
	}

	int CTPExchange2Exchange(const CThostFtdcExchangeField &ctp_exchange, ExchangeInfoPtr &exchange)
	{
		exchange->MarketType = XMT_FUTURES;
		strncpy(exchange->Exchange, ctp_exchange.ExchangeID, MAX_EXCHANGE_LENGTH);
		XUtil::gbk2utf8(ctp_exchange.ExchangeName, strlen(ctp_exchange.ExchangeName)+1, exchange->Name, MAX_NAME_LENGTH);
		exchange->PeriodsCount = _countof(exchange->Periods);
		GetPeriods(exchange->Exchange, exchange->Periods, &exchange->PeriodsCount);
		exchange->TradingDay = XUtil::strto<uint32_t>(trading_day_);
		ASSERT(exchange->TradingDay);
		/*exchange->Date = exchange->TradingDay;
					if (std::strcmp(exchanges[i].ExchangeID, XFE_ZZ) == 0)
					{
						exchange->Time = CTPTime2Time(td_api_->rsp_logininfo().CZCETime);
					}
					else if (std::strcmp(exchanges[i].ExchangeID, XFE_CF) == 0) {
						exchange->Time = CTPTime2Time(td_api_->rsp_logininfo().FFEXTime);
					}
					else if (std::strcmp(exchanges[i].ExchangeID, XFE_DL) == 0) {
						exchange->Time = CTPTime2Time(td_api_->rsp_logininfo().DCETime);
					}
					else if (std::strcmp(exchanges[i].ExchangeID, XFE_SH) == 0) {
						exchange->Time = CTPTime2Time(td_api_->rsp_logininfo().SHFETime);
					}
					else if (std::strcmp(exchanges[i].ExchangeID, XFE_NY) == 0) {
						exchange->Time = CTPTime2Time(td_api_->rsp_logininfo().INETime);
					}
					else if (std::strcmp(exchanges[i].ExchangeID, XSE_SH) == 0) {
						exchange->Time = CTPTime2Time(td_api_->rsp_logininfo().SHFETime);
					}
					else if (std::strcmp(exchanges[i].ExchangeID, XSE_SZ) == 0) {
						exchange->Time = CTPTime2Time(td_api_->rsp_logininfo().SHFETime);
					}
					else {
						ASSERT(0);
						exchange->Time = CTPTime2Time(td_api_->rsp_logininfo().SHFETime);
					}*/
		return 1;
	}

	int CTPProduct2Product(const CThostFtdcProductField &ctp_product, ProductInfoPtr &product)
	{
		product->MarketType = XMT_FUTURES;
		product->ProductType = PRODUCT_TYPE_SYSTEM;
		strncpy(product->Exchange, ctp_product.ExchangeID, MAX_EXCHANGE_LENGTH);
		strncpy(product->Product, ctp_product.ProductID, MAX_PRODUCT_LENGTH);
		XUtil::gbk2utf8(ctp_product.ProductName, strlen(ctp_product.ProductName)+1, product->Name, MAX_NAME_LENGTH);
		product->PeriodsCount = _countof(product->Periods);
		GetPeriods(product->Exchange, product->Periods, &product->PeriodsCount);
		product->Digits = GetDecimalDigits(ctp_product.PriceTick);
		product->PriceDigits = GetDecimalDigits(ctp_product.PriceTick);
		//product->LotSize = ctp_product.VolumeMultiple;
		return 1;
	}

	int CTPInstrument2Commodity(const CThostFtdcInstrumentField &ctp_instrument, CommodityInfoPtr &commodity)
	{
		commodity->MarketType = XMT_FUTURES;
		commodity->ProductType = PRODUCT_TYPE_SYSTEM;
		strncpy(commodity->Exchange, ctp_instrument.ExchangeID, MAX_EXCHANGE_LENGTH);
		strncpy(commodity->Product, ctp_instrument.ProductID, MAX_PRODUCT_LENGTH);
		strncpy(commodity->Code, ctp_instrument.InstrumentID, MAX_CODE_LENGTH);
		XUtil::gbk2utf8(ctp_instrument.InstrumentName, strlen(ctp_instrument.InstrumentName)+1, commodity->Name, MAX_NAME_LENGTH);
		commodity->Digits = GetDecimalDigits(ctp_instrument.PriceTick);
		commodity->PriceDigits = GetDecimalDigits(ctp_instrument.PriceTick);
		//commodity->LotSize = ctp_instrument.VolumeMultiple;
		return 1;
	}

	int CTPMarketData2Commodity(const CThostFtdcDepthMarketDataField &ctp_market_data, CommodityInfoPtr &commodity)
	{
		/*strncpy(commodity->Exchange, ctp_market_data.ExchangeID, MAX_EXCHANGE_LENGTH);
		for (size_t i = 0; i < MAX_PRODUCT_LENGTH; i++)
		{
			if (!std::isalpha(ctp_market_data.InstrumentID[i])) {
				strncpy(commodity->Product, ctp_market_data.InstrumentID, i);
				break;
			}
		}*/
		strncpy(commodity->Code, ctp_market_data.InstrumentID, MAX_CODE_LENGTH);
		commodity->PreSettlementPrice = ctp_market_data.PreSettlementPrice;
		commodity->PreClosePrice = ctp_market_data.PreClosePrice;
		commodity->PreOpenInterest = ctp_market_data.PreOpenInterest;
		commodity->UpperLimitPrice = ctp_market_data.UpperLimitPrice;
		commodity->LowerLimitPrice = ctp_market_data.LowerLimitPrice;
		commodity->PreDelta = ctp_market_data.PreDelta;
		commodity->Date = CTPDate2Date(ctp_market_data.ActionDay);
		commodity->Time = CTPTime2Time(ctp_market_data.UpdateTime);
		if(!commodity->Date || !commodity->Time) {
			commodity->Date = date_;
			commodity->Time = time_;
		}
		ASSERT(commodity->Date && commodity->Time); //测试发现Date Time在ctp新交易日初始化时会是0
		commodity->Open = ctp_market_data.OpenPrice;
		commodity->High = ctp_market_data.HighestPrice;
		commodity->Low = ctp_market_data.LowestPrice;
		commodity->Price = ctp_market_data.LastPrice;
		if(!IsZeroFloat(ctp_market_data.ClosePrice) && !IsOverFloat(ctp_market_data.ClosePrice)) {
			commodity->Price = ctp_market_data.ClosePrice;
		}
		commodity->Volume = ctp_market_data.Volume;
		commodity->Amount = ctp_market_data.Turnover;
		commodity->SettlementPrice = ctp_market_data.SettlementPrice;
		commodity->OpenInterest = ctp_market_data.OpenInterest;
		commodity->CurrDelta = ctp_market_data.CurrDelta;
		for (size_t i = 0; i < XMT_DATA_LEVEL; i++)
		{
			switch (i)
			{
			case 0:
				///申买价一
				commodity->BidPrice[i] = ctp_market_data.BidPrice1;
				///申买量一
				commodity->BidVolume[i] = ctp_market_data.BidVolume1;
				///申卖价一
				commodity->AskPrice[i] = ctp_market_data.AskPrice1;
				///申卖量一
				commodity->AskVolume[i] = ctp_market_data.AskVolume1;
				break;
			case 1:
				///申买价二
				commodity->BidPrice[i] = ctp_market_data.BidPrice2;
				///申买量二
				commodity->BidVolume[i] = ctp_market_data.BidVolume2;
				///申卖价二
				commodity->AskPrice[i] = ctp_market_data.AskPrice2;
				///申卖量二
				commodity->AskVolume[i] = ctp_market_data.AskVolume2;
				break;
			case 2:
				///申买价三
				commodity->BidPrice[i] = ctp_market_data.BidPrice3;
				///申买量三
				commodity->BidVolume[i] = ctp_market_data.BidVolume3;
				///申卖价三
				commodity->AskPrice[i] = ctp_market_data.AskPrice3;
				///申卖量三
				commodity->AskVolume[i] = ctp_market_data.AskVolume3;
				break;
			case 3:
				///申买价四
				commodity->BidPrice[i] = ctp_market_data.BidPrice4;
				///申买量四
				commodity->BidVolume[i] = ctp_market_data.BidVolume4;
				///申卖价四
				commodity->AskPrice[i] = ctp_market_data.AskPrice4;
				///申卖量四
				commodity->AskVolume[i] = ctp_market_data.AskVolume4;
				break;
			case 4:
				///申买价五
				commodity->BidPrice[i] = ctp_market_data.BidPrice5;
				///申买量五
				commodity->BidVolume[i] = ctp_market_data.BidVolume5;
				///申卖价五
				commodity->AskPrice[i] = ctp_market_data.AskPrice5;
				///申卖量五
				commodity->AskVolume[i] = ctp_market_data.AskVolume5;
				break;
			default:
				break;
			}
		}
		return 1;
	}

	int CTPInstrumentStatus2Commodity(const CThostFtdcInstrumentStatusField &ctp_instrument_status, CommodityInfoPtr &commodity)
	{
		strncpy(commodity->Exchange, ctp_instrument_status.ExchangeID, MAX_EXCHANGE_LENGTH);
		strncpy(commodity->Product, ctp_instrument_status.ExchangeInstID, MAX_PRODUCT_LENGTH);
		//strncpy(commodity->Code, ctp_instrument_status.InstrumentID, MAX_CODE_LENGTH);
		commodity->Status = ctp_instrument_status.InstrumentStatus;
		commodity->EnterReason = ctp_instrument_status.EnterReason;
		return 1;
	}
};

}