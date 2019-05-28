#include "MTApi.h"
#include "MDApi.h"
#include <XUtil/XController.h>
#include <XUtil/XXml.hpp>
#include <MDDataSet.hpp>
#include <IndicatorSet.hpp>

namespace MTP {

CMDApi theMDApi;

IMDApi& IMDApi::Instance() 
{
    //static CMDApi _inst;
    return theMDApi;
}

CMDApi::CMDApi()
{

}

CMDApi::~CMDApi()
{
    
}

void CMDApi::RegisterSpi(IMDSpi* pSpi)
{
    spi_ = pSpi;
}

bool CMDApi::start(char* xml, int xmlflag)
{
    if(!Base::init(xml, xmlflag)) {
        return false;
    }
    
	boost::property_tree::ptree& mdcfg = cfg();
	IMDSet::Instance().init((char*)&mdcfg, (int)XUtil::XML_FLAG_PTREE);

    if (theMTApi.down_mode() == UpDownMode::WORK_MODE_IPC) {
		//
	}
	else {
		service().post(boost::bind(&CMDApi::on_connect_event, this));
	}
    return true;
}

void CMDApi::stop()
{
    if (theMTApi.down_mode() == UpDownMode::WORK_MODE_IPC) {
		//
	}
	else {
		//service().run_until(boost::bind(&CMDApi::on_disconnect_event, this));
	}

	IMDSet::Instance().term();
    Base::term();
}

boost::asio::io_service &CMDApi::service()
{
    return theMTApi.service();
}

void CMDApi::Subscribe(char* xml, int xmlflag)
{
    const char* str = R"([{"Exchagne":"CFFEX", "Product":"IF", "Code":"IF1905", "Period":1, "Indicator":[
        {"Name":"K"}
        {"Name":"MA", "input":[{"N"=5}]}
    ]}])";
    std::shared_ptr<boost::property_tree::ptree> params = std::make_shared<boost::property_tree::ptree>();
    XUtil::cfg_from_xml(xml, xmlflag, *params);
	service().post(boost::bind(&CMDApi::on_subscribe_event, this, params));
}

void CMDApi::UnSubscribe(char* xml, int xmlflag)
{
    std::shared_ptr<boost::property_tree::ptree> params = std::make_shared<boost::property_tree::ptree>();
    XUtil::cfg_from_xml(xml, xmlflag, *params);
	service().post(boost::bind(&CMDApi::on_unsubscribe_event, this, params));
}

void CMDApi::on_connect_event()
{
	/*boost::property_tree::ptree& cfg = theMDApi.cfg();
    const std::string ip = cfg.get<std::string>("ip", "");
    const size_t port = cfg.get<size_t>("port", 0);
    auto peer_ptr = NetBase::connect<ws_clt_t>(ip, port, 5000);
    if(peer_ptr) {
        peer_ = peer_ptr->id();
    }*/
}

void CMDApi::on_disconnect_event()
{
    // if(peer_) {
    //     theMTApi.close(peer_);
    //     peer_ = 0;
    // }
}

    void CMDApi::on_subscribe_event(const std::shared_ptr<boost::property_tree::ptree>& params)
    {
        //不用订阅，外部主动调用MakeCommoditySetPtr MakeCalcDataSetPtr MakeBufferSetPtr即可
        // if (theMTApi.down_mode() == WORK_MODE_IPC) {
        //     return;
        //     BOOST_FOREACH (const boost::property_tree::ptree::value_type &one, *params)
        //     {
        //         const boost::property_tree::ptree& xml_one = one.second;
        //         std::string exchange = xml_one.get<std::string>(IDFS_Exchange, "");
        //         std::string product = xml_one.get<std::string>(IDFS_Product, "");
        //         std::string commodity = xml_one.get<std::string>(IDFS_Code, "");
        //         PERIODTYPE period = (PERIODTYPE)xml_one.get<int>(IDFS_Period, 0);
        //         DWTYPE dwtype = (DWTYPE)xml_one.get<int>(IDFS_DWType, 0);
        //         auto commodity_ptr = MakeCommoditySetPtr(exchange.c_str(), product.c_str(), commodity.c_str(), FindFlag::FindOrCreate);
        //         auto calcdata_ptr = MakeCalcDataSetPtr(exchange.c_str(), product.c_str(), commodity.c_str(), period, dwtype, FindFlag::FindOrCreate);
        //         auto opt = xml_one.get_child_optional(IDFS_Indicator);
        //         if (opt)
        //         {
        //             BOOST_FOREACH (const boost::property_tree::ptree::value_type &idx_one, opt.get())
        //             {
        //                 const boost::property_tree::ptree& xml_idx_one = idx_one.second;
        //                 std::string name = xml_one.get<std::string>(IDFS_Name, "");
        //                 InputSet input;
        //                 auto opt_input = xml_one.get_child_optional("input");
        //                 if(opt_input) {
        //                     const boost::property_tree::ptree& xml_input = opt_input.get();
        //                     DataSetConvEx conv;
        //                     conv.from_xml_ex<InputSet>(xml_input, &input);
        //                 }
        //                 MakeBufferSetPtr(name.c_str(), &input, calcdata_ptr.get(), FindFlag::FindOrCreate);
        //             }
        //         }
        //     }
        // } else {
        //     auto future = sendRemoteRequest(peer_, "Subscribe", params);
        //     auto response = future.get();
        // }
    }

	void CMDApi::on_unsubscribe_event(const std::shared_ptr<boost::property_tree::ptree>& params)
    {

    }

}
