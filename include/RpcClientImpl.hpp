#pragma once
#ifndef __H_SRC_RPC_CLIENT_IMPL_HPP__
#define __H_SRC_RPC_CLIENT_IMPL_HPP__

#include <IpcBase.hpp>
#include <DataSetConvEx.hpp>

namespace MTP {

template<class Server, class TBase>
class RpcClientImpl : public TBase
{
	typedef RpcClientImpl<Server,TBase> This;
	typedef TBase Base;
protected:
	Server* srv_;
	std::shared_ptr<boost::property_tree::ptree> result_;
public:
	RpcClientImpl(Server* srv):Base(),srv_(srv)
	{
		result_ = std::make_shared<boost::property_tree::ptree>();
	}

	~RpcClientImpl()
	{
	}

	
	void on_add_exchange(const std::shared_ptr<boost::property_tree::ptree>& params)
	{
		srv_->service().post(boost::bind(&Server::on_add_exchange, srv_, params));
		//srv_->on_add_exchange(params);
	}

	void on_remove_exchange(const std::shared_ptr<boost::property_tree::ptree>& params)
	{
		srv_->service().post(boost::bind(&Server::on_remove_exchange, srv_, params));
		//srv_->on_remove_exchange(params);
	}

	void on_commodity_update(const std::shared_ptr<boost::property_tree::ptree>& params)
	{
		srv_->service().post(boost::bind(&Server::on_commodity_update, srv_, params));
		//srv_->on_commodity_update(params);
	}

	void on_commodity_status(const std::shared_ptr<boost::property_tree::ptree>& params)
	{
		srv_->service().post(boost::bind(&Server::on_commodity_status, srv_, params));
		//srv_->on_commodity_status(params);
	}

	auto send_request(const std::string& method, std::shared_ptr<boost::property_tree::ptree> params)
	{
		return sendRemoteRequest((size_t)mq_server_.get(), method, params);
	}

	void on_update_user(const std::shared_ptr<boost::property_tree::ptree>& params)
	{
		srv_->service().post(boost::bind(&Server::on_update_user, srv_, params));
		//srv_->on_update_user(params);
	}

	void on_update_order(const std::shared_ptr<boost::property_tree::ptree>& params)
	{
		srv_->service().post(boost::bind(&Server::on_update_order, srv_, params));
		//srv_->on_update_order(params);
	}

	void on_update_trade(const std::shared_ptr<boost::property_tree::ptree>& params)
	{
		srv_->service().post(boost::bind(&Server::on_update_trade, srv_, params));
		//srv_->on_update_trade(params);
	}

	void _handleRequest (size_t peer, const std::string& method, const std::shared_ptr<boost::property_tree::ptree>& params, size_t id) 
	{
		if(method == "on_add_exchange") {
			this->on_add_exchange(params);
		} else if(method == "on_remove_exchange") {
			this->on_remove_exchange(params);
		} else if(method == "on_commodity_update") {
			this->on_commodity_update(params);
		} else if(method == "on_commodity_status") {
			this->on_commodity_status(params);
		}
		else 
		if(method == "on_update_user") {
			this->on_update_user(params);
		} else if(method == "on_update_order") {
			this->on_update_order(params);
		} else if(method == "on_update_trade") {
			this->on_update_trade(params);
		}
	}
};

}

#endif //__H_SRC_RPC_CLIENT_IMPL_HPP__
