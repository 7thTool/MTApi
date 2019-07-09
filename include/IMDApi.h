#pragma once

#include "DataDef.h"
#include "IDataSet.h"
#include "IMTApi.h"

namespace MTP {

class IMDSpi
{
public:
    //通知状态改变，包括0开始 1连接、2登陆、3完成、reason为0表示成功，其他表示错误，error为错误消息
    virtual void on_market_update(int state, int reason, const char* error) {} 
    virtual void on_exchange_update(char* xml, int xmlflag = 0) = 0;// { } //通知市场信息已经变化，这里只在市场信息发生变化才会调用
	virtual void on_product_update(char* xml, int xmlflag = 0) = 0;// { } //通知产品信息已经变化，这里只在产品信息发生变化才会调用
    virtual void on_commodity_update(char* xml, int xmlflag = 0) = 0;// { } //通知商品信息已经变化，这里只在商品信息发生变化才会调用
};

/**
 * IMDApi行情源API接口定义，负责处理客户端和行情服务的数据交互。
 */
class MTAPI_API IMDApi
{
public:
	static IMDApi& Instance();

    virtual void RegisterSpi(IMDSpi* pSpi) = 0;

    //virtual void Subscribe(char* xml, int xmlflag = 0) = 0;
	//virtual void UnSubscribe(char* xml, int xmlflag = 0) = 0;
    
protected:
	virtual ~IMDApi() {}
};

}
