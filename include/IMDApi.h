#pragma once

#include "DataDef.h"
#include "IDataSet.h"
#include "IMTApi.h"

namespace MTP {

class IMDSpi
{
public:
    //virtual void on_state_update(IDataSet* dataset, UpdateFlag flag) { }
	virtual void on_exchange_update(IDataSet* dataset, UpdateFlag flag) { }
	virtual void on_product_update(IDataSet* dataset, UpdateFlag flag) { }
    virtual void on_commodity_update(IDataSet* dataset, UpdateFlag flag) { }
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
