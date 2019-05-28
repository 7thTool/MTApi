#pragma once

#include <XUtil/XServer.hpp>
#include <XUtil/XController.h>
#include <XNet/XServer.hpp>
#include <IMDSet.h>
#include "CTPMarket.h"
#include "CTPTrade.h"
#include "RpcServer.hpp"
#include "IpcServer.hpp"
#include "RpcServerImpl.hpp"

namespace MTP {

class MDCTPSrc
	: public XUtil::XServer<MDCTPSrc>
{
	typedef XUtil::XServer<MDCTPSrc> Base;
	typedef CTPMarket<MDCTPSrc> MDSrcHandler;
	typedef CTPTrade<MDCTPSrc> TDSrcHandler;
	class SrcRpcServer : public RpcServerImpl<MDCTPSrc,RpcServer<SrcRpcServer>>
	{
		typedef RpcServerImpl<MDCTPSrc,RpcServer<SrcRpcServer>> Base;
	public:
		using Base::Base;
	};
	class SrcIpcServer : public RpcServerImpl<MDCTPSrc,IpcServer<SrcIpcServer>>
	{
		typedef RpcServerImpl<MDCTPSrc,IpcServer<SrcIpcServer>> Base;
	public:
		using Base::Base;
	};
  private:
	boost::asio::deadline_timer timer_;
	CTPUserLoginInfo logininfo_;
	std::shared_ptr<MDSrcHandler> md_src_handler_;
	std::map<std::string,std::shared_ptr<TDSrcHandler>> td_src_handlers_;
	std::shared_ptr<SrcRpcServer> rpc_server_;
	std::shared_ptr<SrcIpcServer> ipc_server_;
	UpDownMode up_mode_ = UpDownMode::WORK_MODE_RPC;
	std::vector<std::shared_ptr<XUtil::XService>> services_;
	enum Status { STOP, INIT, RUN };
	Status status_;
  public:
	MDCTPSrc() : Base(), timer_(service()), status_(Status::STOP)
	{
    	
	}

	const char *name()
	{
		return "MDCTPSrc";
	}

	void init(int argc, char *argv[])
	{
		status_ = Status::INIT;

		Base::init(argc, argv);

		boost::property_tree::ptree& cfg = theApp.cfg();
		up_mode_ = (UpDownMode)cfg.get<int>("up_mode");

		IMDSet::Instance().init((char*)&cfg, (int)XUtil::XML_FLAG_PTREE);

		size_t max_thread_count = std::thread::hardware_concurrency();
		
		// services_.resize(max_thread_count);
		// for (size_t i = 0; i < services_.size(); i++)
		// {
		// 	services_[i] = std::make_shared<XUtil::XService>();
		// 	services_[i]->start();
		// }

		md_src_handler_ = std::make_shared<MDSrcHandler>(this);
		auto opt_md_src_handler = cfg.get_child_optional(MD_SRC_HANDLER_NAME);
		if(opt_md_src_handler) {
			boost::property_tree::ptree& cfg_md_src_handler = opt_md_src_handler.get();
			auto opt_data = cfg_md_src_handler.get_child_optional("data");
			if(opt_data) {
				boost::property_tree::ptree& cfg_data = opt_data.get();
				md_src_handler_->start((char*)&cfg_data, XUtil::XML_FLAG_PTREE);
			}
		} else {
			ASSERT(0);
		}

		if(up_mode_ == UpDownMode::WORK_MODE_IPC) {
			ipc_server_ = std::make_shared<SrcIpcServer>(this);
			ipc_server_->start((char*)&cfg, (int)XUtil::XML_FLAG_PTREE);
		} else {
			rpc_server_ = std::make_shared<SrcRpcServer>(this);
			rpc_server_->start((char*)&cfg, (int)XUtil::XML_FLAG_PTREE);
		}

		register_timer();

		status_ = Status::RUN;
	}

	void term()
	{
		IMDSet::Instance().term();
		Base::term();
	}

	inline bool is_run() { return status_ == Status::RUN; }
	
	void stop()
	{
		status_ = Status::STOP;

		unregister_timer();

		Base::stop();

		for (auto it : td_src_handlers_)
		{
			it.second->set_offline();
		}
		if(md_src_handler_)
			md_src_handler_->stop();
		if(up_mode_ == UpDownMode::WORK_MODE_IPC) {
			if(ipc_server_)
				ipc_server_->stop();
		} else {
			if(rpc_server_)
				rpc_server_->stop();
		}

		for (size_t i = 0; i < services_.size(); i++)
		{
			services_[i]->stop();
		}

		rpc_server_.reset();
		td_src_handlers_.clear();
		md_src_handler_.reset();

		services_.clear();
	}

  protected:
	void register_timer() 
	{
		timer_.expires_from_now(boost::posix_time::milliseconds(200));
		timer_.async_wait(std::bind(&MDCTPSrc::on_timer, this));
	}

	void unregister_timer()
	{
		boost::system::error_code ec;
		timer_.cancel(ec);
	}

	void on_timer()
	{
		if (!is_run()) {
			return;
		}

		register_timer();
	}

public:
	//基础接口
	inline boost::asio::io_service& service() { return Base::service(); }
	inline boost::asio::io_service& select_service() {
		static size_t last_service_index = 0;
		return services_[last_service_index++%services_.size()]->service();
	}

	void on_add_exchange(const std::vector<ExchangeInfoPtr> exchanges)
	{
		if(up_mode_ == UpDownMode::WORK_MODE_IPC) {
			ipc_server_->post_add_exchange(exchanges);
		} else {
			rpc_server_->post_add_exchange(exchanges);
		}
	}

	void on_remove_exchange(const std::vector<ExchangeInfoPtr> exchanges)
	{
		if(up_mode_ == UpDownMode::WORK_MODE_IPC) {
			ipc_server_->post_remove_exchange(exchanges);
		} else {
			rpc_server_->post_remove_exchange(exchanges);
		}
	}

	void on_commodity_update(const CommodityInfoPtr& commodity)
	{
		if(up_mode_ == UpDownMode::WORK_MODE_IPC) {
			ipc_server_->post_commodity_update(commodity);
		} else {
			rpc_server_->post_commodity_update(commodity);
		}
	}

	void on_commodity_status(const CommodityInfoPtr& commodity)
	{
		if(up_mode_ == UpDownMode::WORK_MODE_IPC) {
			ipc_server_->post_commodity_status(commodity);
		} else {
			rpc_server_->post_commodity_status(commodity);
		}
	}

	inline void on_request (size_t peer, const std::string& method, const std::shared_ptr<boost::property_tree::ptree>& params, size_t id)
	{
		const std::string& userid = params->get<std::string>("userid","");
		if(userid.empty()) {
			post_response(peer, id, params);
			return;
		}
		std::shared_ptr<TDSrcHandler> td_src_handler;
		auto it = td_src_handlers_.find(userid);
		if(it != td_src_handlers_.end()) {
			if(method == "user_login") {
				post_response(peer, id, params);
				return;
			} else if(method == "user_logout") {
				td_src_handlers_.erase(it);
				td_src_handler.reset();
				post_response(peer, id, params);
				return;
			}
		} else {
			if(method == "user_login") {
				CTPUserLoginInfo logininfo;
				if(parse_logininfo(logininfo, *params))
				{
					td_src_handler = std::make_shared<TDSrcHandler>(this,peer,logininfo);
					td_src_handlers_[userid] = td_src_handler;
				    post_response(peer, id, params);
				    return;
				}
			}
		}
		if(td_src_handler) {
			td_src_handler->on_request(peer, method, params, id);
		}
	}

	inline void post_response(size_t peer, size_t id, const std::shared_ptr<boost::property_tree::ptree>& data, bool error = false)
	{
		if(up_mode_ == UpDownMode::WORK_MODE_IPC) {
			ipc_server_->post_response(peer, id, data, error);
		} else {
			//rpc_server_->post_response(peer, id, data, error);
		}
	}

	inline void on_user_update (size_t peer, const std::shared_ptr<boost::property_tree::ptree>& data)
	{
        if(up_mode_ == UpDownMode::WORK_MODE_IPC) {
			ipc_server_->post_notification(peer, "on_user_update", data);
		} else {
			//rpc_server_->post_notification(peer, "on_user_update", data);
		}
    }

	// inline bool didReceiveCall (size_t peer, const std::string& method, const std::shared_ptr<boost::property_tree::ptree>& params, std::shared_ptr<boost::property_tree::ptree>& result) {
    //     const std::string& userid = params->get<std::string>("userid","");
	// 	if(userid.empty()) {
	// 		return false;
	// 	}
	// 	std::shared_ptr<TDSrcHandler> td_src_handler;
	// 	auto it = td_src_handlers_.find(userid);
	// 	if(it != td_src_handlers_.end()) {
	// 		if(method == "user_login") {
	// 			result = params;
	// 			return true;
	// 		} else if(method == "user_logout") {
	// 			td_src_handlers_.erase(it);
	// 			td_src_handler.reset();
	// 			result = params;
	// 			return true;
	// 		}
	// 	} else {
	// 		if(method == "user_login") {
	// 			CTPUserLoginInfo logininfo;
	// 			if(parse_logininfo(logininfo, *params))
	// 			{
	// 				td_src_handler = std::make_shared<TDSrcHandler>(this,peer,logininfo);
	// 				td_src_handlers_[userid] = td_src_handler;
	// 			    result = params;
	// 			    return true;
	// 			}
	// 		}
	// 	}
	// 	if(td_src_handler) {
	// 		return td_src_handler->didReceiveCall(peer, method, params, result);
	// 	}
	// 	return false;
    // }
};

}

