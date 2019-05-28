#pragma once
#ifndef __H_MDSRCRPCHANDLER_HPP__
#define __H_MDSRCRPCHANDLER_HPP__

#include <XUtil/XService.hpp>
#include <XUtil/XXmlRpc.hpp>
//#include "../Token/Token.hpp"
#include <MDDataSet.hpp>
#include <DataSetConv.hpp>

namespace MTP {

template<class Server>
class MDSrcRpcHandler
	: public XUtil::XService
	, public XUtil::XXmlRpc<MDSrcRpcHandler<Server>>
{
private:
	typedef XUtil::XService Base;
	typedef XUtil::XXmlRpc<MDSrcRpcHandler<Server>> RpcBase;
	Server* srv_;
	//Token* token_;
	using method_handler = std::function<void(size_t,const std::shared_ptr<boost::property_tree::ptree>&,size_t)>;
    struct HandlerInfo
	{
		bool sync;
		method_handler handler;
	};
	std::map<std::string, HandlerInfo> method_handlers_;
	std::shared_ptr<boost::property_tree::ptree> result_;
    struct PeerInfo
    {
        bool is_sub = false;
    };
	//std::map<const char*,CommoditySetEx*> sub_sets_;
	//std::multimap<size_t,CommoditySetEx*> peer_sub_sets_;
    std::map<size_t,PeerInfo> peers_;
public:
	MDSrcRpcHandler(Server* srv) : Base(), RpcBase(), srv_(srv)
	{
		method_handlers_["request_exchange"] = {false, std::bind(&MDSrcRpcHandler::on_request_exchange, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)};
		method_handlers_["request_product"] = {false, std::bind(&MDSrcRpcHandler::on_request_product, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)};
		method_handlers_["request_commodity"] = {false, std::bind(&MDSrcRpcHandler::on_request_commodity, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)};
		method_handlers_["request_subscribe"] = {false, std::bind(&MDSrcRpcHandler::on_request_subscribe, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)};
		method_handlers_["request_unsubscribe"] = {false, std::bind(&MDSrcRpcHandler::on_request_unsubscribe, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)};
		result_ = std::make_shared<boost::property_tree::ptree>();
	}

	~MDSrcRpcHandler()
	{
	}

	bool start(
		//RegToken* token
	)
	{
		//token_ = token;

		Base::start();
		
		return true;
	}

	void stop()
	{
		Base::stop();
		
		//token_ = nullptr;
	
		RpcBase::clear();
	}

	inline boost::asio::io_service &service() { return Base::service(); }

	bool verify(std::shared_ptr<boost::property_tree::ptree>& params)
	{
		return false;
	}

public:
    //一下是MDSrc数据变化回调
	
	void on_add_exchange(const std::vector<ExchangeInfoPtr> exchanges)
	{
		for(auto it : peers_) {
            sendRemoteNotification(it.first, "on_add_exchange", nullptr);
        }
	}

	void on_remove_exchange(const std::vector<ExchangeInfoPtr> exchanges)
	{
		for(auto it : peers_) {
            sendRemoteNotification(it.first, "on_remove_exchange", nullptr);
        }
	}

	void on_commodity_update(const CommodityInfoPtr& commodity)
	{
		for(auto it : peers_) {
            sendRemoteNotification(it.first, "on_commodity_update", nullptr);
        }
	}

	void on_commodity_status(const CommodityInfoPtr& commodity)
	{
		for(auto it : peers_) {
            sendRemoteNotification(it.first, "on_commodity_status", nullptr);
        }
	}

	//以下是josnrpc回调，运行在rpc线程里，即MDSrcRpcHandler线程里
	
	inline void on_request_exchange(size_t peer, const std::shared_ptr<boost::property_tree::ptree>& params, size_t id) {
		// size_t exchange_count = MarketDataSet::Instance().GetBufferSize(0);
		// std::shared_ptr<boost::property_tree::ptree> result = std::make_shared<boost::property_tree::ptree>();
		// for(size_t i = 0; i < exchange_count; i++)
		// {
		// 	ExchangeInfo* exchange =	(ExchangeInfo*)MarketDataSet::Instance().GetBufferValue(0,i);
		// 	if(exchange) {
		// 		boost::property_tree::ptree item;
		// 		item.put("MarketType", exchange->MarketType);
		// 		item.put("Exchange", exchange->Exchange);
		// 		item.put("Name", exchange->Name);
		// 		//item.put("ProductCount", exchange->ProductCount);
		// 		//item.put("CommodityCount", exchange->CommodityCount);
		// 		//item.put("PeriodsCount", exchange->PeriodsCount);
		// 		boost::property_tree::ptree periods;
		// 		for(size_t j = 0; j < exchange->PeriodsCount; j++)
		// 		{
		// 			boost::property_tree::ptree period;
		// 			period.put("from",exchange->Periods[0][0]);
		// 			period.put("to", exchange->Periods[0][1]);
		// 			periods.push_back(std::make_pair("",period));
		// 		}
		// 		item.push_back(std::make_pair("Periods", periods));
		// 		item.put("TimeZone", exchange->TimeZone);
		// 		item.put("TradingDay", exchange->TradingDay);
		// 		item.put("Date", exchange->Date);
		// 		item.put("Time", exchange->Time);
		// 		item.put("Status", exchange->Status);
		// 		item.put("EnterReason", exchange->EnterReason);
		// 		result->push_back(std::make_pair("",item));
		// 	}
		// }
		//std::string msg = DataSetConv(MDDataSetConv).to_json(&MarketDataSet::Instance());
		std::stringstream ss;
		IExchangeSet* exchanges[8] = { 0 };
		size_t exchange_count = IMarketDataSet::Instance().Ref_All_Exchange(exchanges, 8);
		for(size_t i = 0; i < exchange_count; i++)
		{
				/*size_t product_count = exchanges[i]->GetBufferSize(IDX_EXCHANGE_BUFFER_ProductInfo);
				for(size_t j = 0; j < product_count; j++)
				{
						//
				}
				size_t commodity_count = exchanges[i]->GetBufferSize(IDX_EXCHANGE_BUFFER_CommodityInfo);
				for(size_t j = 0; j < commodity_count; j++)
				{
						//
				}*/
				ss << DataSetConv(MDDataSetConv).to_json(exchanges[i]);
		}
		std::string msg = ss.str();
		_sendMessage(peer, msg);
		/*std::shared_ptr<boost::property_tree::ptree> result = params;
		if(db_handler_->Login(*params, *result)) {
			//签名token
			sign(result);
			//发送登录回应
			sendResponse(peer, id, result, false);
			//写入reids
			//redis_handler_->Login(*params, *result);
		} else {
			sendResponse(peer, id, result, true);
		}*/
	}
	
	inline void on_request_product(size_t peer, const std::shared_ptr<boost::property_tree::ptree>& params, size_t id) {
		/*std::shared_ptr<boost::property_tree::ptree> result = result_;
		if(db_handler_->Logout(*params, *result)) {
			//写入redis
			sendResponse(peer, id, result, false);
		} else {
			sendResponse(peer, id, result, true);
		}*/
	}

	inline void on_request_commodity(size_t peer, const std::shared_ptr<boost::property_tree::ptree>& params, size_t id) {
	}
	
	inline void on_request_subscribe(size_t peer, const std::shared_ptr<boost::property_tree::ptree>& params, size_t id) {
	}
	
	inline void on_request_unsubscribe(size_t peer, const std::shared_ptr<boost::property_tree::ptree>& params, size_t id) {
	}
	
	void on_link_open(size_t peer)
	{
		peers_[peer];
	}

	void on_link_close(size_t peer)
	{
		peers_.erase(peer);
	}

	void on_link_message(size_t peer, const std::string &buffer)
	{
		this->_handleMessage(peer, buffer);
	}

	inline void sendResponse (size_t peer, size_t id, const std::shared_ptr<boost::property_tree::ptree>& result, bool error) {
		this->_sendResponse(peer, id, result, error);
	}

    void _sendMessage (size_t peer, const std::string& msg) {
		srv_->link_send(peer, msg.data(), msg.size());
    }

	void _handleRequest (size_t peer, const std::string& method, const std::shared_ptr<boost::property_tree::ptree>& params, size_t id) {
		//return sendResponse(peer, id, params, false);
		if (id == RpcBase::INVALID_REQUEST_ID) {
			return;
		}
		auto it = method_handlers_.find(method);
		if(it != method_handlers_.end()) {
			HandlerInfo& info = it->second;
			info.handler(peer, params, id);
		}
	}
};

}

#endif //__H_MDSRCRPCHANDLER_HPP__


