#pragma once

#include <XUtil/XServer.hpp>
#include <XUtil/XController.h>
#include <IMDSet.h>
#include <IMDApi.h>
#include <ITDApi.h>
#include "CTPMarket.h"
#include "CTPTrade.h"

namespace MTP {

class CTPApi
	: public XUtil::XServer<CTPApi>
    , public IMDApi
    , public ITDApi
{
	typedef XUtil::XServer<CTPApi> Base;
	typedef CTPMarket<CTPApi> MDSrcHandler;
	typedef CTPTrade<CTPApi> TDSrcHandler;
  private:
	CTPUserLoginInfo logininfo_;
	std::shared_ptr<MDSrcHandler> md_src_handler_;
	std::map<std::string,std::shared_ptr<TDSrcHandler>> td_src_handlers_;
	std::vector<std::shared_ptr<XUtil::XService>> services_;
	enum Status { STOP, INIT, RUN };
	Status status_;
  public:
	CTPApi() : Base(), status_(Status::STOP)
	{
    	
	}

	const char *name()
	{
		return "CTPApi";
	}

	void init(int argc, char *argv[])
	{
		status_ = Status::INIT;

		Base::init(argc, argv);

		boost::property_tree::ptree& cfg = theApp.cfg();
	
		//IMDSet::Instance().init((char*)&cfg, (int)XUtil::XML_FLAG_PTREE); 由外部初始化

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

        XUtil::XController::instance().SetHandler("CTPApi", this);

		status_ = Status::RUN;
	}

	void term()
	{
        XUtil::XController::instance().SetHandler("CTPApi", nullptr);
		//IMDSet::Instance().term(); 由外部释放
		Base::term();
	}

	inline bool is_run() { return status_ == Status::RUN; }
	
	void stop()
	{
		status_ = Status::STOP;

		Base::stop();

		for (auto it : td_src_handlers_)
		{
			it.second->set_offline();
		}
		if(md_src_handler_)
			md_src_handler_->stop();

		for (size_t i = 0; i < services_.size(); i++)
		{
			services_[i]->stop();
		}

		td_src_handlers_.clear();
		md_src_handler_.reset();

		services_.clear();
	}

    //IMDApi
    virtual void RegisterSpi(IMDSpi* pSpi) {}

    //ITDApi
    virtual void RegisterSpi(ITDSpi* pSpi) {}

    virtual void AddUser(char* xml, int xmlflag = 0) {}
    virtual void RemoveUser(char* xml, int xmlflag = 0) {}
	virtual void Request(char* xml, int xmlflag = 0) {}

public:
	//基础接口
	inline boost::asio::io_service& service() { return Base::service(); }
	inline boost::asio::io_service& select_service() {
		static size_t last_service_index = 0;
		return services_[last_service_index++%services_.size()]->service();
	}

	void on_add_exchange(const std::vector<ExchangeInfoPtr> exchanges)
	{
		
	}

	void on_remove_exchange(const std::vector<ExchangeInfoPtr> exchanges)
	{
		
	}

	void on_commodity_update(const CommodityInfoPtr& commodity)
	{
		
	}

	void on_commodity_status(const CommodityInfoPtr& commodity)
	{
		
	}

	inline void on_request (size_t peer, const std::string& method, const std::shared_ptr<boost::property_tree::ptree>& params, size_t id)
	{
		// const std::string& userid = params->get<std::string>("userid","");
		// if(userid.empty()) {
		// 	post_response(peer, id, params);
		// 	return;
		// }
		// std::shared_ptr<TDSrcHandler> td_src_handler;
		// auto it = td_src_handlers_.find(userid);
		// if(it != td_src_handlers_.end()) {
		// 	if(method == "user_login") {
		// 		post_response(peer, id, params);
		// 		return;
		// 	} else if(method == "user_logout") {
		// 		td_src_handlers_.erase(it);
		// 		td_src_handler.reset();
		// 		post_response(peer, id, params);
		// 		return;
		// 	}
		// } else {
		// 	if(method == "user_login") {
		// 		CTPUserLoginInfo logininfo;
		// 		if(parse_logininfo(logininfo, *params))
		// 		{
		// 			td_src_handler = std::make_shared<TDSrcHandler>(this,peer,logininfo);
		// 			td_src_handlers_[userid] = td_src_handler;
		// 		    post_response(peer, id, params);
		// 		    return;
		// 		}
		// 	}
		// }
		// if(td_src_handler) {
		// 	td_src_handler->on_request(peer, method, params, id);
		// }
	}

	inline void post_response(size_t peer, size_t id, const std::shared_ptr<boost::property_tree::ptree>& data, bool error = false)
	{
		//
	}

	inline void on_user_update (size_t peer, const std::shared_ptr<boost::property_tree::ptree>& data)
	{
        
    }
};

}

