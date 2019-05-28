#pragma once

#include <IMTApi.h>
#include <IMDApi.h>
#include <ITDApi.h>
#include <IMDDataSet.h>
#include <ITDDataSet.h>
#include <IndicatorSet.hpp>

namespace MTP {

template<class Server>
class CMTApiMgr : public IMDSpi
{
    typedef CMTApiMgr<Server> This;
protected:
    Server* srv_;
public:
    CMTApiMgr(Server* srv):srv_(srv) {}
    ~CMTApiMgr() {}

	bool Start(char* xml, int xmlflag = 0)
    {
		IMDApi::Instance().RegisterSpi(this);
		bool ret = IMTApi::Instance().Start(xml, xmlflag);
        return ret;
    }

	void Stop()
    {
		IMTApi::Instance().Stop();
    }
    
    inline std::vector<IExchangeSetPtr> Ref_All_Exchange()
    {
        return MakeAllExchangeSetPtr();
    }

    inline std::vector<IProductSetPtr> Ref_All_Product()
    {
        return MakeAllProductSetPtr();
    }

    inline std::vector<IProductSetPtr> Ref_All_Product(IExchangeSetPtr exchange_ptr)
    {
        return MakeAllProductSetPtr(exchange_ptr);
    }

    inline std::vector<ICommoditySetPtr> Ref_All_Commodity()
    {
        return MakeAllCommoditySetPtr();
    }

    inline std::vector<ICommoditySetPtr> Ref_All_Commodity(IProductSetPtr product_ptr)
    {
        return MakeAllCommoditySetPtr(product_ptr);
    }

    inline std::vector<ICommoditySetPtr> Ref_All_Commodity(IExchangeSetPtr exchange_ptr)
    {
        return MakeAllCommoditySetPtr(exchange_ptr);
    }

    inline std::vector<ICalculatorSetPtr> Ref_All_Calculator()
    {
        return MakeAllCalculatorSetPtr();
    }

    inline std::vector<IBufferSetPtr> Ref_All_Buffer()
    {
        return MakeAllBufferSetPtr();
    }

    inline IExchangeSetPtr Ref(const char* exchange, FindFlag flag = FindFlag::FindOrCreate)
    {
        return MakeExchangeSetPtr(exchange, flag);
    }

    inline IProductSetPtr Ref(const char* exchange, const char* product, FindFlag flag = FindFlag::FindOrCreate)
    {
        return MakeProductSetPtr(exchange, product, flag);
    }
    
    inline ICommoditySetPtr Ref(const char* exchange, const char* product, const char* code, FindFlag flag = FindFlag::FindOrCreate)
    {
        return MakeCommoditySetPtr(exchange, product, code, flag);
    }

    inline ICalcDataSetPtr Ref(const char* exchange, const char* product, const char* code, PERIODTYPE period, DWTYPE dwtype, FindFlag flag = FindFlag::FindOrCreate)
    {
        return MakeCalcDataSetPtr(exchange, product, code, period, dwtype, flag);
    }
    
    inline ICalculatorSetPtr Ref(const char* name, const IInputSet* input, FindFlag flag = FindFlag::FindOrCreate)
    {
        return MakeCalculatorSetPtr(name, input, flag);
    }

    inline IBufferSetPtr Ref(const char* name, const IInputSet* input, IDataSet* calcdata, FindFlag flag = FindFlag::FindOrCreate)
    {
        return MakeBufferSetPtr(name, input, calcdata, flag);
    }

	virtual void on_exchange_update(IDataSet* dataset, UpdateFlag flag) 
    { 
        // DataSetConv conv;
        // std::cout << conv.to_json(dataset) << std::endl;
        // std::shared_ptr<CDataSet> dataset_ptr = std::make_shared<CDataSet>(*dataset);
        // std::cout << conv.to_json(dataset_ptr.get()) << std::endl;

        // std::vector<IExchangeSetPtr> exchange_ptrs = Ref_All_Exchange();
        // for (auto& exchange_ptr : exchange_ptrs)
        // {
        // }
    }

	virtual void on_product_update(IDataSet* dataset, UpdateFlag flag) 
    { 
        // std::vector<IProductSetPtr> product_ptrs = Ref_All_Product();
        // for (auto& product_ptr : product_ptrs)
        // {
        // }
    }

    virtual void on_commodity_update(IDataSet* dataset, UpdateFlag flag) 
    {
        // DataSetConv conv;
        // std::cout << conv.to_json(dataset) << std::endl;
    }
};

}


