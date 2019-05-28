#ifndef __H_REGHANDLER_HPP__
#define __H_REGHANDLER_HPP__

#include <XUtil/XService.hpp>
#include <XUtil/XXmlRpc.hpp>
#include "RegDBHandler.hpp"
#include "RegRedisHandler.hpp"
#include "../RegToken/RegToken.hpp"

class RegHandler
	: public XUtil::XService
	, public XUtil::XXmlRpc<RegHandler>
{
public:
	class Server
	{
	public:
		//基础接口
		virtual boost::asio::io_service &service() = 0;

		//网络接口
		virtual void link_close(size_t peer) = 0;
		virtual void link_send(size_t peer, const char* buf, size_t len) = 0;
	};
private:
	typedef XUtil::XService Base;
	typedef XUtil::XXmlRpc<RegHandler> RpcBase;
	Server* srv_;
	RegDBHandler* db_handler_;
	RegRedisHandler* redis_handler_;
	RegToken* token_;
	using method_handler = std::function<void(size_t,const std::shared_ptr<boost::property_tree::ptree>&,size_t)>;
    struct HandlerInfo
	{
		bool sync;
		method_handler handler;
	};
	std::map<std::string, HandlerInfo> method_handlers_;
	std::shared_ptr<boost::property_tree::ptree> result_;
public:
	RegHandler(Server* srv) : Base(), RpcBase(), srv_(srv)
	{
		method_handlers_["register"] = {false, std::bind(&RegHandler::didReceive_Register, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)};
		method_handlers_["unregister"] = {false, std::bind(&RegHandler::didReceive_UnRegister, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)};
		method_handlers_["activate"] = {false, std::bind(&RegHandler::didReceive_Activate, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)};
		method_handlers_["login"] = {false, std::bind(&RegHandler::didReceive_Login, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)};
		method_handlers_["logout"] = {false, std::bind(&RegHandler::didReceive_Logout, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)};
		result_ = std::make_shared<boost::property_tree::ptree>();
	}

	~RegHandler()
	{
	}

	bool start(RegDBHandler* db_handler, RegRedisHandler* redis_handler, RegToken* token)
	{
		db_handler_ = db_handler;
		redis_handler_ = redis_handler;
		token_ = token;

		Base::start();
		
		return true;
	}

	void stop()
	{
		Base::stop();
		
		token_ = nullptr;
		redis_handler_ = nullptr;
		db_handler_ = nullptr;
		
		RpcBase::clear();
	}

	inline boost::asio::io_service &service() { return Base::service(); }

	bool sign(std::shared_ptr<boost::property_tree::ptree>& result)
	{
		std::string token;
		// Create the json payload
		time_t t_exp;
    	//struct tm *p_exp;
		time(&t_exp); /*当前time_t类型UTC时间*/
		//p_exp = localtime(&t_exp); /*转换为本地的tm结构的时间按*/
		//t_exp = mktime(p_exp); /*重新转换为time_t类型的UTC时间，这里有一个时区的转换*/
		t_exp += 60*60*30;   //增加60*60*30毫秒，即30分钟后过期
		if(token_->sign({
			{"sub", "subject"}
		, {"exp", t_exp}
		, {"user_id", result->get<uint64_t>("user_id", 0)}
		}
		, token)) {
			result->put("token", token);
			return true;
		}
		return false;
	}

	bool verify(std::shared_ptr<boost::property_tree::ptree>& params)
	{
		return false;
	}

public:
	//以下是josnrpc回调，运行在rpc线程里，即RegHandler线程里
	
	inline void didReceive_Register(size_t peer, const std::shared_ptr<boost::property_tree::ptree>& params, size_t id) {
		std::shared_ptr<boost::property_tree::ptree> result = result_;
		if(db_handler_->Register(*params, *result)) {
			sendResponse(peer, id, result, false);
		} else {
			sendResponse(peer, id, result, true);
		}
	}
	
	inline void didReceive_UnRegister(size_t peer, const std::shared_ptr<boost::property_tree::ptree>& params, size_t id) {
		std::shared_ptr<boost::property_tree::ptree> result = result_;
		if(db_handler_->UnRegister(*params, *result)) {
			sendResponse(peer, id, result, false);
		} else {
			sendResponse(peer, id, result, true);
		}
	}
	
	inline void didReceive_Activate(size_t peer, const std::shared_ptr<boost::property_tree::ptree>& params, size_t id) {
		std::shared_ptr<boost::property_tree::ptree> result = result_;
		if(db_handler_->Activate(*params, *result)) {
			sendResponse(peer, id, result, false);
			//redis_handler_->Activate(*params, *result);
		} else {
			sendResponse(peer, id, result, true);
		}
	}
	
	inline void didReceive_Login(size_t peer, const std::shared_ptr<boost::property_tree::ptree>& params, size_t id) {
		std::shared_ptr<boost::property_tree::ptree> result = params;
		if(db_handler_->Login(*params, *result)) {
			//签名token
			sign(result);
			//发送登录回应
			sendResponse(peer, id, result, false);
			//写入reids
			//redis_handler_->Login(*params, *result);
		} else {
			sendResponse(peer, id, result, true);
		}
	}
	
	inline void didReceive_Logout(size_t peer, const std::shared_ptr<boost::property_tree::ptree>& params, size_t id) {
		std::shared_ptr<boost::property_tree::ptree> result = result_;
		if(db_handler_->Logout(*params, *result)) {
			//写入redis
			sendResponse(peer, id, result, false);
		} else {
			sendResponse(peer, id, result, true);
		}
	}
	
	void on_link_open(size_t peer)
	{
		
	}

	void on_link_close(size_t peer)
	{
		
	}

	void on_link_message(size_t peer, const std::string &buffer)
	{
		_handleMessage(peer, buffer);
	}

	inline void sendResponse (size_t peer, size_t id, const std::shared_ptr<boost::property_tree::ptree>& result, bool error) {
		_sendResponse(peer, id, result, error);
	}

    void _sendMessage (size_t peer, const std::string& msg) {
		srv_->link_send(peer, msg.data(), msg.size());
    }

	void _handleRequest (size_t peer, const std::string& method, const std::shared_ptr<boost::property_tree::ptree>& params, size_t id) {
		if (id == INVALID_REQUEST_ID) {
			return;
		}
		std::map<std::string, HandlerInfo>::iterator it = method_handlers_.find(method);
		if(it != method_handlers_.end()) {
			HandlerInfo& info = it->second;
			info.handler(peer, params, id);
		}
	}
};

#endif //__H_REGHANDLER_HPP__
