#pragma once
#ifndef _H_ITDAPI_H_
#define _H_ITDAPI_H_

#include "IDataSet.h"
#include "IMTApi.h"

namespace MTP {

class ITDSpi
{
public:
	virtual void on_user_update(size_t peer, char* xml, int xmlflag = 0) { }
	virtual void on_push(size_t peer, const char* method, char* xml, int xmlflag = 0) = 0;
	virtual void on_response(size_t peer, const char* method, char* xml, int xmlflag = 0, size_t id = 0) { }
};

/**
 * TDApi客户端API接口定义，负责处理客户端和交易服务端的数据交互。
 */
class MTAPI_API ITDApi
{
public:
	static ITDApi& Instance();

    virtual void register_spi(ITDSpi* spi, int spiflag) = 0;

    virtual void add_user(size_t peer, char* xml, int xmlflag = 0) = 0;
    virtual void remove_user(size_t peer, char* xml, int xmlflag = 0) = 0;

	virtual void request(size_t peer, const char* method, char* xml, int xmlflag = 0, size_t id = 0) = 0;
    
protected:
	virtual ~ITDApi() {}
};

}

#endif//_H_ITDAPI_H_
