#pragma once
#ifndef __MDAPI_H
#define __MDAPI_H

#include <IMDApi.h>
#include <XUtil/XApi.hpp>
#include <XNet/XServer.hpp>
#include <IMDSet.h>

namespace MTP {

class CMDApi
    : public XUtil::XApi
    , public IMDApi
{
    typedef XUtil::XApi Base;
protected:
    IMDSpi* spi_ = nullptr;
public:
    CMDApi();
    virtual ~CMDApi();
    
    virtual void RegisterSpi(IMDSpi* pSpi);
    inline IMDSpi* Spi() { return spi_; }

    bool start(char* xml, int xmlflag = 0);
    void stop();

	inline boost::asio::io_service &service();
    
    virtual void Subscribe(char* xml, int xmlflag = 0);
	virtual void UnSubscribe(char* xml, int xmlflag = 0);

protected:
    //Service 线程内运行
    void on_connect_event();
    void on_disconnect_event();
    void on_subscribe_event(const std::shared_ptr<boost::property_tree::ptree>& params);
	void on_unsubscribe_event(const std::shared_ptr<boost::property_tree::ptree>& params);
};

extern CMDApi theMDApi;

}

#endif // __MDAPI_H
