#pragma once
#ifndef _H_ITDAPI_H_
#define _H_ITDAPI_H_

#include "IDataSet.h"
#include "IMTApi.h"

namespace MTP {

class ITDSpi
{
public:
    //virtual void on_state_update(IDataSet* dataset, UpdateFlag flag) { }
	virtual void on_user_update(IDataSet* dataset, UpdateFlag flag) { }
	virtual void on_order_update(IDataSet* dataset, UpdateFlag flag) { }
	virtual void on_trade_update(IDataSet* dataset, UpdateFlag flag) { }
};

/**
 * TDApi客户端API接口定义，负责处理客户端和交易服务端的数据交互。
 */
class MTAPI_API ITDApi
{
public:
	static ITDApi& Instance();

    virtual void RegisterSpi(ITDSpi* pSpi) = 0;

    virtual void AddUser(char* xml, int xmlflag = 0) = 0;
    virtual void RemoveUser(char* xml, int xmlflag = 0) = 0;
	virtual void Request(char* xml, int xmlflag = 0) = 0;
    
protected:
	virtual ~ITDApi() {}
};

}

#endif//_H_ITDAPI_H_
