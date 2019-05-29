#include "MTApi.h"
#include "TDApi.h"
#include <XUtil/XXml.hpp>
#include <TDDataSet.hpp>

namespace MTP {

CTDApi theTDApi;

ITDApi& ITDApi::Instance() 
{
    //static CTDApi _inst;
    return theTDApi;
}

CTDApi::CTDApi()
{

}

CTDApi::~CTDApi()
{
    
}

void CTDApi::RegisterSpi(ITDSpi* pSpi)
{
    spi_ = pSpi;
}

bool CTDApi::start(char* xml, int xmlflag)
{
    if(!Base::init(xml, xmlflag)) {
        return false;
    }
    
	boost::property_tree::ptree& tdcfg = cfg();
	ITDSet::Instance().init((char*)&tdcfg, (int)XUtil::XML_FLAG_PTREE);

    if (theMTApi.down_mode() == UpDownMode::WORK_MODE_IPC) {
		//
	}
	else {
		//service().post(boost::bind(&CTDApi::on_connect_event, this));
	}
    return true;
}

void CTDApi::stop()
{
    if (theMTApi.down_mode() == UpDownMode::WORK_MODE_IPC) {
		//
	}
	else {
		//service().run_until(boost::bind(&CTDApi::on_disconnect_event, this));
	}

	ITDSet::Instance().term();
    Base::term();
}

boost::asio::io_service &CTDApi::service()
{
    return theMTApi.service();
}

void CTDApi::AddUser(char* xml, int xmlflag)
{
	std::shared_ptr<boost::property_tree::ptree> params = std::make_shared<boost::property_tree::ptree>();
	XUtil::cfg_from_xml(xml, xmlflag, *params);
    //theMTApi.request("user_login", params);
	service().post(boost::bind(&CMTApi::on_add_user, &theMTApi, params));
	//theMTApi.on_add_user(params);
}

void CTDApi::RemoveUser(char* xml, int xmlflag)
{
	std::shared_ptr<boost::property_tree::ptree> params = std::make_shared<boost::property_tree::ptree>();
	XUtil::cfg_from_xml(xml, xmlflag, *params);
    //theMTApi.request("user_logout", params);
	service().post(boost::bind(&CMTApi::on_remove_user, &theMTApi, params));
	//theMTApi.on_remove_user(params);
}

void CTDApi::Request(char* xml, int xmlflag)
{
	theMTApi.td_request(xml, xmlflag);
    //std::shared_ptr<boost::property_tree::ptree> params = std::make_shared<boost::property_tree::ptree>();
	//XUtil::cfg_from_xml(xml, xmlflag, *params);
	//service().post(boost::bind(&CMTApi::on_request, &theMTApi, params));
}

}
