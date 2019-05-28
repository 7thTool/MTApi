#pragma once

#include <DataDef.h>
#include <IDataSet.h>
#include <FieldDef.h>

#ifdef WIN32
#ifdef Indicator_EXPORTS
#define INDICATOR_API __declspec(dllexport)
#else
#define INDICATOR_API __declspec(dllimport)
#endif//
#else //LINUX
#define	INDICATOR_API 
#endif//

#if defined(__cplusplus)
extern "C" {
#endif//
//
#if defined(__cplusplus)
}

namespace MTP {

/**
 * IIndicatorApi Indicator全局对象，负责处理客户端和Indicator的交互。
 */
class INDICATOR_API IIndicatorApi
{
public:
	static IIndicatorApi& Instance();

	virtual bool Start(char* xml, int xmlflag = 0) = 0;
	virtual void Stop() = 0;

	virtual void on_exchange_update(IDataSet* dataset, UpdateFlag flag) = 0;
	virtual void on_product_update(IDataSet* dataset, UpdateFlag flag) = 0;
    virtual void on_commodity_update(IDataSet* dataset, UpdateFlag flag) = 0;

    virtual void on_user_update(IDataSet* dataset, UpdateFlag flag) = 0;
	virtual void on_order_update(IDataSet* dataset, UpdateFlag flag) = 0;
	virtual void on_trade_update(IDataSet* dataset, UpdateFlag flag) = 0;

protected:
	virtual ~IIndicatorApi() {}
};

}

#endif//
