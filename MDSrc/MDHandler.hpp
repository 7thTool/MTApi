#ifndef __H_MDHANDLER_HPP__
#define __H_MDHANDLER_HPP__

#include <XUtil/XService.hpp>
#include <XUtil/XXmlRpc.hpp>
//#include "../Token/Token.hpp"

namespace MTP {

template<class Server>
class MDHandler
	: public XUtil::XService
	, public XUtil::XXmlRpc<MDHandler<Server>>
{
private:
	typedef XUtil::XService Base;
	typedef XUtil::XXmlRpc<MDHandler<Server>> RpcBase;
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
public:
	MDHandler(Server* srv) : Base(), RpcBase(), srv_(srv)
	{
		//method_handlers_["login"] = {false, std::bind(&MDHandler::didReceive_Login, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)};
		//method_handlers_["logout"] = {false, std::bind(&MDHandler::didReceive_Logout, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)};
		result_ = std::make_shared<boost::property_tree::ptree>();
	}

	~MDHandler()
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
	//以下是josnrpc回调，运行在rpc线程里，即MDHandler线程里
	
	// inline void didReceive_Login(size_t peer, const std::shared_ptr<boost::property_tree::ptree>& params, size_t id) {
	// 	std::shared_ptr<boost::property_tree::ptree> result = params;
	// 	if(db_handler_->Login(*params, *result)) {
	// 		//签名token
	// 		sign(result);
	// 		//发送登录回应
	// 		sendResponse(peer, id, result, false);
	// 		//写入reids
	// 		//redis_handler_->Login(*params, *result);
	// 	} else {
	// 		sendResponse(peer, id, result, true);
	// 	}
	// }
	
	// inline void didReceive_Logout(size_t peer, const std::shared_ptr<boost::property_tree::ptree>& params, size_t id) {
	// 	std::shared_ptr<boost::property_tree::ptree> result = result_;
	// 	if(db_handler_->Logout(*params, *result)) {
	// 		//写入redis
	// 		sendResponse(peer, id, result, false);
	// 	} else {
	// 		sendResponse(peer, id, result, true);
	// 	}
	// }
	
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

#endif //__H_MDHANDLER_HPP__
