#ifndef __H_TDHANDLER_HPP__
#define __H_TDHANDLER_HPP__

#include <XUtil/XService.hpp>
#include <XUtil/XXmlRpc.hpp>
#include "../include/Order.hpp"
#include "../include/TDMQBase.hpp"
#include <XUtil/XDateTime.hpp>
//#include "../Token/Token.hpp"

namespace MTP {

template<class Server>
class TDHandler
	: public XUtil::XService
	, public XUtil::XXmlRpc<TDHandler<Server>>
{
private:
	typedef TDHandler<Server> This;
	typedef XUtil::XService Base;
	typedef XUtil::XXmlRpc<TDHandler<Server>> RpcBase;
	Server* srv_;
	struct UserInfo {
		std::string user_id;
		size_t peer_id;
	};
	std::map<const char*,std::shared_ptr<UserInfo>,XUtil::strless> users_;
	//Token* token_;
	using method_handler = std::function<void(size_t,const std::shared_ptr<boost::property_tree::ptree>&,size_t)>;
    struct HandlerInfo
	{
		bool sync;
		method_handler handler;
	};
	std::map<std::string, HandlerInfo> method_handlers_;
	std::string send_msg_;
	std::shared_ptr<boost::property_tree::ptree> result_;
public:
	TDHandler(Server* srv) : Base(), RpcBase(), srv_(srv)
	{
		send_msg_.reserve(TD_MQ_MATCH_MAX_MSG_SIZE);
		method_handlers_["request_login"] = {false, std::bind(&This::handle_login_request, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)};
		method_handlers_["request_order"] = {false, std::bind(&This::handle_order_request, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)};
		result_ = std::make_shared<boost::property_tree::ptree>();
	}

	~TDHandler()
	{
	}

	bool start()
	{
		//token_ = token;

		Base::start();

		//service().post(boost::bind(&This::handle_order_receive, this));
		service().post(boost::bind(&This::test, this));
		
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
	//以下是josnrpc回调，运行在rpc线程里，即TDHandler线程里

	bool order_verify(Order& order)
	{
		return true;
	}

	void order_to_db(Order& order) 
	{
		//
	}

	bool xml_to_order(const boost::property_tree::ptree& xml, Order& order)
	{
		strncpy(order.user_id,xml.get<std::string>("user_id","").c_str(),_countof(order.user_id)-1);
        strncpy(order.code,xml.get<std::string>("code","").c_str(),_countof(order.code)-1);
        order.action = (OrderActionType)xml.get<int>("action",0);
        order.direction = (OrderDirectionType)xml.get<int>("direction",0);
        order.price_type = (OrderPriceType)xml.get<int>("price_type",0);
        order.price = (uint64_t)(xml.get<double>("price",0)*OrderPriceRate);
        order.volume = xml.get<uint32_t>("volume",0);
		order.date = XUtil::NowDateTime(&order.time);
        //order.timestamp  = boost::posix_time::to_iso_string(boost::posix_time::microsec_clock::universal_time());
        //order.id;
        //order.status;
		return true;
	}
	
	bool order_to_xml(const Order& order, boost::property_tree::ptree& xml)
	{
		xml.put("user_id",order.user_id);
		xml.put("code",order.code);
		xml.put("action",order.action);
		xml.put("direction",order.direction);
		xml.put("price_type",order.price_type);
		xml.put("price",order.price);
		xml.put("volume",order.volume);
		xml.put("time",order.time);
		xml.put("id",order.id);
		xml.put("status",order.status);
		return true;
	}
	
	void handle_login_request(size_t peer, const std::shared_ptr<boost::property_tree::ptree>& params, size_t id) {
		std::shared_ptr<boost::property_tree::ptree> result = result_;
		std::string user_id = params->get<std::string>("user_id", "");
		auto it = users_.find(user_id.c_str());
		if(it != users_.end()) {
			it->second->peer_id = peer;
		} else {
			std::shared_ptr<UserInfo> user = std::make_shared<UserInfo>();
			user->user_id = user_id;
			user->peer_id = peer;
			users_[user->user_id.c_str()] = user;
		}
		sendResponse(peer, id, result, false);
	}

	void handle_order_request(size_t peer, const std::shared_ptr<boost::property_tree::ptree>& params, size_t id) {
		std::shared_ptr<boost::property_tree::ptree> result = result_;
		send_msg_.resize(TD_MQ_MATCH_MAX_MSG_SIZE);
		char* msg = (char*)send_msg_.data();
		size_t msg_len = sizeof(TDMQ_MATCH_HEADER) + sizeof(Order);
		TDMQ_MATCH_HEADER* header = (TDMQ_MATCH_HEADER*)msg;
		strcpy(header->name, TD_MQ_MATCH_RESULT_NAME);
		Order* order = (Order*)(header + 1);
		xml_to_order(*params,*order);
		if(order_verify(*order)) {
			//int random_variable = std::rand();
			//order->action = OrderActionType::Do;
			//order->price += random_variable%3 * OrderPriceRate;
			//order->direction = OrderDirectionType::Buy;
			//订单进入排队中
			//result->put("status","排队中");
			//资金冻结、证券冻结、订单落库,生成订单id
			order_to_db(*order);
			srv_->mq_send_match(msg, msg_len, 0);
			sendResponse(peer, id, result, false);
		} else {
			sendResponse(peer, id, result, true);
		}
	}
	
	void test()
	{
		const std::string str_request_login = R"({
	"jsonrpc":"2.0",
	"method":"request_login",
	"params":{
		"user_id":"123456",
		"password":"123456"
	},
	"id":123456
	})";

	const std::vector<std::string> str_request_order = {R"({
	"jsonrpc":"2.0",
	"method":"request_order",
	"params":{
		"user_id":"123456",
		"password":"123456",
		"code":"123456",
		"action":1,
		"direction":0,
		"price_type":0,
		"price":6.89,
		"volume":1000
	},
	"id":123456
	})"
	,R"({
	"jsonrpc":"2.0",
	"method":"request_order",
	"params":{
		"user_id":"123456",
		"password":"123456",
		"code":"123456",
		"action":1,
		"direction":0,
		"price_type":0,
		"price":6.88,
		"volume":1000
	},
	"id":123456
	})"
	,R"({
	"jsonrpc":"2.0",
	"method":"request_order",
	"params":{
		"user_id":"123456",
		"password":"123456",
		"code":"123456",
		"action":1,
		"direction":0,
		"price_type":0,
		"price":6.87,
		"volume":1000
	},
	"id":123456
	})"
	,R"({
	"jsonrpc":"2.0",
	"method":"request_order",
	"params":{
		"user_id":"123456",
		"password":"123456",
		"code":"123456",
		"action":1,
		"direction":1,
		"price_type":0,
		"price":6.87,
		"volume":100000
	},
	"id":123456
	})"
	,R"({
	"jsonrpc":"2.0",
	"method":"request_order",
	"params":{
		"user_id":"123456",
		"password":"123456",
		"code":"123456",
		"action":1,
		"direction":1,
		"price_type":0,
		"price":6.90,
		"volume":1000
	},
	"id":123456
	})"
	};

	for(size_t i = 0; i < str_request_order.size(); i++) {
		on_link_message(1024, str_request_order[i]);
	}
	
		//service().post(boost::bind(&This::test, this));

	}
	
	void on_link_open(size_t peer)
	{
		
	}

	void on_link_close(size_t peer)
	{
		
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

#endif //__H_TDHandler_HPP__
