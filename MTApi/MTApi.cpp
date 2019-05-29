#include "MTApi.h"
#include <XUtil/XApp.cpp>
#include <XUtil/XController.h>
#include "MDApi.h"
#include "TDApi.h"
#include <MDDataSet.hpp>
#include <TDDataSet.hpp>
#include <IndicatorSet.hpp>
#include <IIndicatorApi.h>

namespace MTP {

CMTApi theMTApi;

IMTApi& IMTApi::Instance() 
{
    return theMTApi;
}

CMTApi::CMTApi()
{

}

CMTApi::~CMTApi()
{
    
}

bool CMTApi::Start(char* xml, int xmlflag)
{
    if(!Base::start(xml, xmlflag)) {
        return false;
    }

	boost::property_tree::ptree& cfg = theApp.cfg();
	up_mode_ = (UpDownMode)cfg.get<int>("up_mode");
	down_mode_ = (UpDownMode)cfg.get<int>("down_mode");

    on_start();
	//service().post(boost::bind(&CMTApi::on_start, this));
    return true;
}

void CMTApi::Stop()
{
    on_stop();
    //service().post(boost::bind(&CMTApi::on_stop, this));
    Base::stop();
}

void CMTApi::on_start()
{
	boost::property_tree::ptree& cfg = theApp.cfg();

    IIndicatorApi::Instance().Start((char*)&cfg, (int)XUtil::XML_FLAG_PTREE);

    theMDApi.start((char*)&cfg, (int)XUtil::XML_FLAG_PTREE);
    theTDApi.start((char*)&cfg, (int)XUtil::XML_FLAG_PTREE);

    XUtil::XController::instance().start((char*)&cfg, (int)XUtil::XML_FLAG_PTREE);

	if(theMTApi.up_mode() == UpDownMode::WORK_MODE_IPC) {
		ipc_server_ = std::make_shared<SrcIpcServer>(this);
		ipc_server_->start((char*)&cfg, (int)XUtil::XML_FLAG_PTREE);
	} else if (theMTApi.up_mode() == WORK_MODE_RPC) {
		rpc_server_ = std::make_shared<SrcRpcServer>(this);
		rpc_server_->start((char*)&cfg, (int)XUtil::XML_FLAG_PTREE);
	}

	if (theMTApi.down_mode() == WORK_MODE_IPC) {
		ipc_client_ = std::make_shared<ApiIpcClient>(this);
		ipc_client_->start((char*)&cfg, (int)XUtil::XML_FLAG_PTREE);
	}
	else if (theMTApi.down_mode() == WORK_MODE_RPC) {
        rpc_client_ = std::make_shared<ApiRpcClient>(this);
		rpc_client_->start((char*)&cfg, (int)XUtil::XML_FLAG_PTREE);
	} else {
        auto opt_module = cfg_.get_child_optional("module");
        if (opt_module) {
            boost::property_tree::ptree &cfg_module = opt_module.get();
            BOOST_FOREACH (const boost::property_tree::ptree::value_type &cfgi, cfg_module)
            {
                std::string name = cfgi.second.get<std::string>("name", "");
                std::string md_name = "md_" + name;
                md_api_ = (IMDApi*)XUtil::XController::instance().GetHandler(md_name.c_str());
                if(md_api_) {
                    md_api_->RegisterSpi(this);
                }
                std::string td_name = "td_" + name;
                td_api_ = (ITDApi*)XUtil::XController::instance().GetHandler(td_name.c_str());
                if(td_api_) {
                    td_api_->RegisterSpi(this);
                }
            }
        }
    }
}

void CMTApi::on_stop()
{
    if(md_api_) {
        md_api_->RegisterSpi(nullptr);
    }
    if(td_api_) {
        td_api_->RegisterSpi(nullptr);
    }

	if(theMTApi.up_mode() == UpDownMode::WORK_MODE_IPC) {
		ipc_server_->stop();
	} else if (theMTApi.up_mode() == UpDownMode::WORK_MODE_RPC) {
		rpc_server_->stop();
	}

    XUtil::XController::instance().stop();

    IIndicatorApi::Instance().Stop();

    theTDApi.stop();
    theMDApi.stop();

    if (theMTApi.down_mode() == WORK_MODE_IPC) {
		ipc_client_->stop();
	}
	else if (theMTApi.down_mode() == WORK_MODE_RPC) {
        rpc_client_->stop();
	}
    
	rpc_server_.reset();
    ipc_server_.reset();
    rpc_client_.reset();
    ipc_client_.reset();
}

void CMTApi::on_exchange_update(IDataSet* dataset)
{
    const char* exchange = (const char*)dataset->GetFieldValue(IDF_Exchange);
            LOG4I("on_exchange_update %s", exchange);
            IMarketDataSet::OnUpdateExchange(exchange);
            IExchangeSetPtr exchange_ptr = MakeExchangeSetPtr(exchange, FindFlag::FindOrCreate);
            if (exchange_ptr)
            {
                IIndicatorApi::Instance().on_exchange_update(exchange_ptr.get());
                IMDSpi* spi = theMDApi.Spi();
                if (spi)
                {
                    spi->on_exchange_update(exchange_ptr.get());
                }
            }
            
		if(theMTApi.up_mode() == UpDownMode::WORK_MODE_IPC) {
			ipc_server_->post_exchange_update(dataset);
		} else if(theMTApi.up_mode() == UpDownMode::WORK_MODE_RPC) {
	        rpc_server_->post_exchange_update(dataset);
		}
}

void CMTApi::on_product_update(IDataSet* dataset)
{
    const char* exchange = (const char*)dataset->GetFieldValue(IDF_Exchange);
    const char* product = (const char*)dataset->GetFieldValue(IDF_Product);
            LOG4I("on_product_update %s %s", exchange, product);
            IMarketDataSet::OnUpdateProduct(exchange, product);
            IProductSetPtr product_ptr = MakeProductSetPtr(exchange, product, FindFlag::FindOrCreate);
            if (product_ptr)
            {
                IIndicatorApi::Instance().on_product_update(product_ptr.get());
                IMDSpi* spi = theMDApi.Spi();
                if (spi)
                {
                    spi->on_product_update(product_ptr.get());
                }
            }

        if(theMTApi.up_mode() == UpDownMode::WORK_MODE_IPC) {
			ipc_server_->post_product_update(dataset);
		} else if(theMTApi.up_mode() == UpDownMode::WORK_MODE_RPC) {
			rpc_server_->post_product_update(dataset);
		}
}

void CMTApi::on_commodity_update(IDataSet* dataset)
{
    const char* exchange = (const char*)dataset->GetFieldValue(IDF_Exchange);
        const char* product = (const char*)dataset->GetFieldValue(IDF_Product);
        const char* commodity = (const char*)dataset->GetFieldValue(IDF_Code);
        LOG4I("on_commodity_update %s %s %s", exchange, product, commodity);
        IMarketDataSet::OnUpdateMarket(exchange, product, commodity);
        ICommoditySetPtr commodity_ptr = MakeCommoditySetPtr(exchange, product, commodity, FindFlag::FindOnly);
        if (commodity_ptr)
        {
            IIndicatorApi::Instance().on_commodity_update(commodity_ptr.get());
            IMDSpi* spi = theMDApi.Spi();
            if (spi)
            {
                spi->on_commodity_update(commodity_ptr.get());
            }
        }

        if(theMTApi.up_mode() == UpDownMode::WORK_MODE_IPC) {
			ipc_server_->post_commodity_update(dataset);
		} else if(theMTApi.up_mode() == UpDownMode::WORK_MODE_RPC) {
			rpc_server_->post_commodity_update(dataset);
		}
}

	void CMTApi::on_exchange_update(const std::shared_ptr<boost::property_tree::ptree>& params)
	{
        //BOOST_FOREACH (const boost::property_tree::ptree::value_type &one, *params)
        {
            //ExchangeInfo exchange;
            //xml_to_exchange(one.second, exchange);
            //ExchangeSetWrapper<IDataSet, ExchangeInfo *, ProductInfo *, CommodityInfo *> exchange_set(&exchange);
            std::string exchange = params->get<std::string>(IDFS_Exchange, "");
            LOG4I("on_add_exchange %s", exchange.c_str());
            IMarketDataSet::OnUpdateExchange(exchange.c_str());
            IExchangeSetPtr exchange_ptr = MakeExchangeSetPtr(exchange.c_str(), FindFlag::FindOrCreate);
            if (exchange_ptr)
            {
                IIndicatorApi::Instance().on_exchange_update(exchange_ptr.get());
                IMDSpi* spi = theMDApi.Spi();
                if (spi)
                {
                    spi->on_exchange_update(exchange_ptr.get());
                }
            }
        }
    }

	void CMTApi::on_product_update(const std::shared_ptr<boost::property_tree::ptree>& params)
    {
        //BOOST_FOREACH (const boost::property_tree::ptree::value_type &one, *params)
        {
            std::string exchange = params->get<std::string>(IDFS_Exchange, "");
            std::string product = params->get<std::string>(IDFS_Product, "");
            LOG4I("on_product_update %s %s", exchange.c_str(), product.c_str());
            IProductSetPtr product_ptr = MakeProductSetPtr(exchange.c_str(), product.c_str(), FindFlag::FindOnly);
            IMarketDataSet::OnUpdateProduct(exchange.c_str(), product.c_str());
            if (product_ptr)
            {
                IIndicatorApi::Instance().on_product_update(product_ptr.get());
                IMDSpi* spi = theMDApi.Spi();
                if (spi)
                {
                    spi->on_product_update(product_ptr.get());
                }
            }
        }
    }

	void CMTApi::on_commodity_update(const std::shared_ptr<boost::property_tree::ptree>& params)
	{
        std::string exchange = params->get<std::string>(IDFS_Exchange, "");
        std::string product = params->get<std::string>(IDFS_Product, "");
        std::string commodity = params->get<std::string>(IDFS_Code, "");
        LOG4I("on_commodity_update %s %s %s", exchange.c_str(), product.c_str(), commodity.c_str());
        IMarketDataSet::OnUpdateMarket(exchange.c_str(), product.c_str(), commodity.c_str());
        ICommoditySetPtr commodity_ptr = MakeCommoditySetPtr(exchange.c_str(), product.c_str(), commodity.c_str(), FindFlag::FindOnly);
        if (commodity_ptr)
        {
            IIndicatorApi::Instance().on_commodity_update(commodity_ptr.get());
            IMDSpi* spi = theMDApi.Spi();
            if (spi)
            {
                spi->on_commodity_update(commodity_ptr.get());
            }
        }
    }

    void CMTApi::on_add_user(const std::shared_ptr<boost::property_tree::ptree>& params)
    {
        std::string userid = params->get<std::string>("userid", "");
        LOG4I("on_add_user %s", userid.c_str());
        ITradeDataSet::OnAddUser((char*)params.get(), (int)XUtil::XML_FLAG_PTREE);
        ITradeUserSetPtr user_ptr = MakeTradeUserSetPtr(userid.c_str(), FindFlag::FindOnly);
        if (user_ptr)
        {
            IIndicatorApi::Instance().on_user_update(user_ptr.get(), UpdateFlag::Add);
            ITDSpi* spi = theTDApi.Spi();
            if (spi)
            {
                spi->on_user_update(user_ptr.get(), UpdateFlag::Add);
            }
        }
    }

	void CMTApi::on_remove_user(const std::shared_ptr<boost::property_tree::ptree>& params)
    {
        std::string userid = params->get<std::string>("userid", "");
        LOG4I("on_remove_user %s", userid.c_str());
        ITradeUserSetPtr user_ptr = MakeTradeUserSetPtr(userid.c_str(), FindFlag::FindOnly);
        ITradeDataSet::OnRemoveUser((char*)params.get(), (int)XUtil::XML_FLAG_PTREE);
        if (user_ptr)
        {
            IIndicatorApi::Instance().on_user_update(user_ptr.get(), UpdateFlag::Remove);
            ITDSpi* spi = theTDApi.Spi();
            if (spi)
            {
                spi->on_user_update(user_ptr.get(), UpdateFlag::Remove);
            }
        }
    }

    void CMTApi::on_update_user(const std::shared_ptr<boost::property_tree::ptree>& params)
    {
        std::string userid = params->get<std::string>("userid", "");
        LOG4I("on_update_user %s", userid.c_str());
        ITradeDataSet::OnUpdateUser((char*)params.get(), (int)XUtil::XML_FLAG_PTREE);
        ITradeUserSetPtr user_ptr = MakeTradeUserSetPtr(userid.c_str(), FindFlag::FindOnly);
        if (user_ptr)
        {
            IIndicatorApi::Instance().on_user_update(user_ptr.get(), UpdateFlag::Update);
            ITDSpi* spi = theTDApi.Spi();
            if (spi)
            {
                spi->on_user_update(user_ptr.get(), UpdateFlag::Update);
            }
        }
    }
    
	void CMTApi::on_update_order(const std::shared_ptr<boost::property_tree::ptree>& params)
    {
        std::string userid = params->get<std::string>("userid", "");
        std::string tradeid = params->get<std::string>("tradeid", "");
        LOG4I("on_update_order %s %s", userid.c_str(), tradeid.c_str());
        ITradeDataSet::OnUpdateOrder((char*)params.get(), (int)XUtil::XML_FLAG_PTREE);
        IOrderSetPtr order_ptr = MakeOrderSetPtr(userid.c_str(), tradeid.c_str(), FindFlag::FindOnly);
        if (order_ptr)
        {
            IIndicatorApi::Instance().on_order_update(order_ptr.get(), UpdateFlag::Update);
            ITDSpi* spi = theTDApi.Spi();
            if (spi)
            {
                spi->on_order_update(order_ptr.get(), UpdateFlag::Update);
            }
        }
    }
    
	void CMTApi::on_update_trade(const std::shared_ptr<boost::property_tree::ptree>& params)
    {
        std::string userid = params->get<std::string>("userid", "");
        std::string tradeid = params->get<std::string>("tradeid", "");
        std::string exchange = params->get<std::string>("exchange", "");
        std::string product = params->get<std::string>("product", "");
        std::string code = params->get<std::string>("code", "");
        LOG4I("on_update_trade %s %s %s %s %s", userid.c_str(), tradeid.c_str(), exchange.c_str(), product.c_str(), code.c_str());
        ITradeDataSet::OnUpdateTrade((char*)params.get(), (int)XUtil::XML_FLAG_PTREE);
        ITradeSetPtr trade_ptr = MakeTradeSetPtr(userid.c_str(), tradeid.c_str(), FindFlag::FindOnly);
        if (trade_ptr)
        {
            IIndicatorApi::Instance().on_trade_update(trade_ptr.get(), UpdateFlag::Update);
            ITDSpi* spi = theTDApi.Spi();
            if (spi)
            {
                spi->on_trade_update(trade_ptr.get(), UpdateFlag::Update);
                //IPositionSetPtr position_ptr = MakePositionSetPtr(userid.c_str(), exchange.c_str(), product.c_str(), code.c_str(), FindFlag::FindOnly);
            }
        }
    }

    void CMTApi::td_request (char* xml, int xmlflag)
    {
        // if (theMTApi.down_mode() == WORK_MODE_IPC) {
        //     ipc_client_->send_request(method, params);
        // }
        // else if (theMTApi.down_mode() == WORK_MODE_RPC) {
        //     //NetBase::post_packet();
        // } else {
        //     if(td_api_) {
        //         //td_api_->Request();
        //     }
        // }
    }

    void CMTApi::on_request (size_t peer, const std::string& method, const std::shared_ptr<boost::property_tree::ptree>& params, size_t id)
    {
        //
    }
}
