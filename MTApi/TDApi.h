#ifndef __TDAPI_H
#define __TDAPI_H

#include <ITDApi.h>
#include <XUtil/XApi.hpp>
#include <XUtil/XUtil.hpp>

namespace MTP {

class CTDApi
    : public XUtil::XApi 
    , public ITDApi
{
    typedef XUtil::XApi Base;
protected:
    ITDSpi* spi_ = nullptr;
    size_t peer_ = 0;
public:
    CTDApi();
    virtual ~CTDApi();
    
    virtual void RegisterSpi(ITDSpi* pSpi);
    inline ITDSpi* Spi() { return spi_; }

    bool start(char* xml, int xmlflag = 0);
    void stop();

	inline boost::asio::io_service &service();

    virtual void AddUser(char* xml, int xmlflag = 0);
    virtual void RemoveUser(char* xml, int xmlflag = 0);
    
    virtual void Request(char* xml, int xmlflag = 0);
};

extern CTDApi theTDApi;

}

#endif // __TDAPI_H
