#ifndef __H_TDMATCHHANDLER_HPP__
#define __H_TDMATCHHANDLER_HPP__

#include <XUtil/XService.hpp>
#include <XUtil/XXmlRpc.hpp>
#include <XUtil/XDateTime.hpp>
//#include "../Token/Token.hpp"

namespace MTP {

template<class Server>
class MTHandler
	: public XUtil::XService
	, public XUtil::XXmlRpc<MTHandler<Server>>
{
private:
	typedef MTHandler<Server> This;
	typedef XUtil::XService Base;
	typedef XUtil::XXmlRpc<MTHandler<Server>> RpcBase;
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
	std::shared_ptr<boost::property_tree::ptree> result_;
public:
	MTHandler(Server* srv) : Base(), RpcBase(), srv_(srv)
	{
		//method_handlers_["request_login"] = {false, std::bind(&This::handle_login_request, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)};
		//method_handlers_["request_order"] = {false, std::bind(&This::handle_order_request, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)};
		result_ = std::make_shared<boost::property_tree::ptree>();
	}

	~MTHandler()
	{
	}

	bool start()
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
	//以下是josnrpc回调，运行在rpc线程里，即MTHandler线程里
	
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

#endif //__H_MTHandler_HPP__
