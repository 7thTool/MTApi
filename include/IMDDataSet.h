#pragma once
#ifndef _H_IMDDATASET_H_
#define _H_IMDDATASET_H_

#include <DataDef.h>
#include <IMDSet.h>
#include <IDataSet.h>

namespace MTP {

    class IMarketDataSet;
    class IExchangeSet;
    class IProductSet;
    class ICommoditySet;
  
class MDSET_API IMarketDataSet : public IDataSet
{
public:
    static IMarketDataSet& Instance();

	static size_t Ref_All_Exchange(IExchangeSet** exchange, size_t max_count);
	static size_t Ref_All_Product(IProductSet** product, size_t max_count);
	static size_t Ref_All_Commodity(ICommoditySet** commodity, size_t max_count);

	static void OnAddExchange(const char* exchange);
	static void OnRemoveExchange(const char* exchange);
	static void OnUpdateExchange(const char* exchange);
	static void OnUpdateProduct(const char* exchange, const char* product);
	static void OnUpdateMarket(const char* exchange, const char* product, const char* code);
	static void OnUpdateStatus(const char* exchange, const char* product, const char* code);
};

class MDSET_API IExchangeSet : public IDataSet
{
public:
    static IExchangeSet* Ref(const char* exchange, FindFlag flag = FindFlag::FindOrCreate);
    static void Release(IExchangeSet* dataset);

	size_t Ref_All_Product(IProductSet** product, size_t max_count);
	size_t Ref_All_Commodity(ICommoditySet** commodity, size_t max_count);

	virtual const size_t Type() const { return DATASET_TYPE_EXCHANGE; }

	virtual const char* Exchange() const { return ""; }

    bool Less(const IDataSet* r) const
    {
		if(!r) {
			return false;
		}
		const size_t type = Type(), r_type = r->Type();
		if (type < r_type)
            return true;
        else if(type > r_type)
			return false;

		const IExchangeSet* rc = dynamic_cast<const IExchangeSet*>(r);
		
			int cmp = 0;
			const char *exchange = Exchange(), *r_exchange = rc->Exchange();
			cmp = strcmp(exchange, r_exchange);
			if(cmp < 0) {
				return true;
			} else if(cmp > 0) {
				return false;
			}
			return false;
    };

    bool Equal(const IDataSet* r) const
    {
		if(!r) {
			return false;
		}
		if(Type() != r->Type()) {
			return false;
		}
		
		const IExchangeSet* rc = dynamic_cast<const IExchangeSet*>(r);
		
        return (strcmp(Exchange(),rc->Exchange()) == 0);
    }
};

class ExchangeSetInfo : public IExchangeSet
{
protected:
	const std::string exchange_;
public:
	ExchangeSetInfo(const char* exchange)
	:exchange_(exchange) { }

	virtual const char* Exchange() const { return exchange_.c_str(); }
};

class MDSET_API IProductSet : public IDataSet
{
public:
    static IProductSet* Ref(const char* exchange, const char* product, FindFlag flag = FindFlag::FindOrCreate);
    static void Release(IProductSet* dataset);

	size_t Ref_All_Commodity(ICommoditySet** commodity, size_t max_count);

	virtual const size_t Type() const { return DATASET_TYPE_PRODUCT; }

	virtual const char* Exchange() const { return ""; }
	virtual const char* Product() const { return ""; }
	
    bool Less(const IDataSet* r) const
    {
		if(!r) {
			return false;
		}
		const size_t type = Type(), r_type = r->Type();
		if (type < r_type)
            return true;
        else if(type > r_type)
			return false;

		const IProductSet* rc = dynamic_cast<const IProductSet*>(r);
		
			int cmp = 0;
			const char *exchange = Exchange(), *r_exchange = rc->Exchange();
			cmp = strcmp(exchange, r_exchange);
			if(cmp < 0) {
				return true;
			} else if(cmp > 0) {
				return false;
			}
			const char *product = Product(), *r_product = rc->Product();
			cmp = strcmp(product, r_product);
			if(cmp < 0) {
				return true;
			} else if(cmp > 0) {
				return false;
			}
			return false;
    };

    bool Equal(const IDataSet* r) const
    {
		if(!r) {
			return false;
		}
		if(Type() != r->Type()) {
			return false;
		}
		
		const IProductSet* rc = dynamic_cast<const IProductSet*>(r);
		
        return (strcmp(Exchange(),rc->Exchange()) == 0 && strcmp(Product(),rc->Product()) == 0);
    }
};

class ProductSetInfo : public IProductSet
{
protected:
	const std::string exchange_;
	const std::string product_;
public:
	ProductSetInfo(const char* exchange, const char* product, FindFlag flag = FindFlag::FindOrCreate)
	:exchange_(exchange),product_(product) { }

	virtual const char* Exchange() const { return exchange_.c_str(); }
	virtual const char* Product() const { return product_.c_str(); }
};

class MDSET_API ICommoditySet : public IDataSet
{
public:
    static ICommoditySet* Ref(const char* exchange, const char* product, const char* code, FindFlag flag = FindFlag::FindOrCreate);
    static void Release(ICommoditySet* dataset);

	virtual const size_t Type() const { return DATASET_TYPE_COMMODITY; }

	virtual const char* Exchange() const { return ""; }
	virtual const char* Product() const { return ""; }
	virtual const char* Code() const { return ""; }

    bool Less(const IDataSet* r) const
    {
		if(!r) {
			return false;
		}
		const size_t type = Type(), r_type = r->Type();
		if (type < r_type)
            return true;
        else if(type > r_type)
			return false;

		const ICommoditySet* rc = dynamic_cast<const ICommoditySet*>(r);
		
			int cmp = 0;
			const char *exchange = Exchange(), *r_exchange = rc->Exchange();
			cmp = strcmp(exchange, r_exchange);
			if(cmp < 0) {
				return true;
			} else if(cmp > 0) {
				return false;
			}
			const char *product = Product(), *r_product = rc->Product();
			cmp = strcmp(product, r_product);
			if(cmp < 0) {
				return true;
			} else if(cmp > 0) {
				return false;
			}
			const char *code = Code(), *r_code = rc->Code();
			cmp = strcmp(code, r_code);
			if(cmp < 0) {
				return true;
			} else if(cmp > 0) {
				return false;
			}
			return false;
    };

    bool Equal(const IDataSet* r) const
    {
		if(!r) {
			return false;
		}
		if(Type() != r->Type()) {
			return false;
		}
		
		const ICommoditySet* rc = dynamic_cast<const ICommoditySet*>(r);
		
        return (strcmp(Exchange(),rc->Exchange()) == 0 && strcmp(Product(),rc->Product()) == 0 && strcmp(Code(),rc->Code()) == 0);
    }
};

class CommoditySetInfo : public ICommoditySet
{
protected:
	const std::string exchange_;
	const std::string product_;
	const std::string code_;
public:
	CommoditySetInfo(const char* exchange, const char* product, const char* code)
	:exchange_(exchange),product_(product),code_(code) { }

	virtual const char* Exchange() const { return exchange_.c_str(); }
	virtual const char* Product() const { return product_.c_str(); }
	virtual const char* Code() const { return code_.c_str(); }
};

class MDSET_API ICalcDataSet : public IDataSet
{
public:
    static ICalcDataSet* Ref(const char* exchange, const char* product, const char* code, PERIODTYPE period, DWTYPE dwtype, FindFlag flag = FindFlag::FindOrCreate);
    static void Release(ICalcDataSet* dataset);

public:
	virtual const size_t Type() const { return DATASET_TYPE_CALCDATA; }

	//virtual const char* Symbol() const { return ""; }

	virtual const char* Exchange() const { return ""; }
	virtual const char* Product() const { return ""; }
	virtual const char* Code() const { return ""; }
	virtual PERIODTYPE Period() const { return CYC_MAX; }
	virtual DWTYPE DWType() const { return DW_NONE; }

    bool Less(const IDataSet* r) const
    {
		if(!r) {
			return false;
		}
		const size_t type = Type(), r_type = r->Type();
		if (type < r_type)
            return true;
        else if(type > r_type)
			return false;

		const ICalcDataSet* rc = dynamic_cast<const ICalcDataSet*>(r);
		//const char *symbol = Symbol(), *r_symbol = rc->Symbol();
		//return strcmp(symbol, r_symbol) < 0;
		
			const PERIODTYPE period = Period(), r_period = rc->Period();
			if(period < r_period) {
				return true;
			} else if(period > r_period) {
				return false;
			} else {
				const DWTYPE dwtype = DWType(), r_dwtype = rc->DWType();
				if(dwtype < r_dwtype) {
					return true;
				} else if(dwtype > r_dwtype) {
					return false;
				}
			}

			int cmp = 0;
			const char *exchange = Exchange(), *r_exchange = rc->Exchange();
			cmp = strcmp(exchange, r_exchange);
			if(cmp < 0) {
				return true;
			} else if(cmp > 0) {
				return false;
			}
			const char *product = Product(), *r_product = rc->Product();
			cmp = strcmp(product, r_product);
			if(cmp < 0) {
				return true;
			} else if(cmp > 0) {
				return false;
			}
			const char *code = Code(), *r_code = rc->Code();
			cmp = strcmp(code, r_code);
			if(cmp < 0) {
				return true;
			} else if(cmp > 0) {
				return false;
			}
			return false;
    };

    bool Equal(const IDataSet* r) const
    {
		if(!r) {
			return false;
		}
		if(Type() != r->Type()) {
			return false;
		}
		
		const ICalcDataSet* rc = dynamic_cast<const ICalcDataSet*>(r);
		//const char *symbol = Symbol(), r_symbol = rc->Symbol();
		//return strcmp(symbol, r_symbol) == 0;
		
        return (Period() == rc->Period() && DWType() == rc->DWType()
		 && strcmp(Exchange(),rc->Exchange()) == 0 && strcmp(Product(),rc->Product()) == 0 && strcmp(Code(),rc->Code()) == 0);
    }
};

class CalcDataSetInfo : public ICalcDataSet
{
protected:
	const std::string exchange_;
	const std::string product_;
	const std::string code_;
	PERIODTYPE period_;
	DWTYPE dwtype_;
public:
	CalcDataSetInfo(const char* exchange, const char* product, const char* code, PERIODTYPE period, DWTYPE dwtype)
	:exchange_(exchange),product_(product),code_(code),period_(period),dwtype_(dwtype) { }

	virtual const char* Exchange() const { return exchange_.c_str(); }
	virtual const char* Product() const { return product_.c_str(); }
	virtual const char* Code() const { return code_.c_str(); }
	virtual PERIODTYPE Period() const { return period_; }
	virtual DWTYPE DWType() const { return dwtype_; }
};

typedef std::shared_ptr<IExchangeSet> IExchangeSetPtr;
typedef std::shared_ptr<IProductSet> IProductSetPtr;
typedef std::shared_ptr<ICommoditySet> ICommoditySetPtr;
typedef std::shared_ptr<ICalcDataSet> ICalcDataSetPtr;

inline IExchangeSetPtr MakeExchangeSetPtr(const char* exchange, FindFlag flag = FindFlag::FindOrCreate) { 
	return IExchangeSetPtr(IExchangeSet::Ref(exchange,flag),&IExchangeSet::Release);
}
inline std::vector<IExchangeSetPtr> MakeAllExchangeSetPtr() { 
    std::vector<IExchangeSetPtr> exchange_ptrs;
    size_t count = IMarketDataSet::Ref_All_Exchange(nullptr, 0);
	IExchangeSet** ptrs = (IExchangeSet**)alloca(sizeof(IExchangeSet*)*count);
	count = IMarketDataSet::Ref_All_Exchange(ptrs, count);
    exchange_ptrs.resize(count);
    for(size_t i = 0; i < count; i++) 
	{
		exchange_ptrs[i] = IExchangeSetPtr(ptrs[i],&IExchangeSet::Release);
	}
	return exchange_ptrs;
}

inline IProductSetPtr MakeProductSetPtr(const char* exchange, const char* product, FindFlag flag = FindFlag::FindOrCreate) { 
	return IProductSetPtr(IProductSet::Ref(exchange,product,flag),&IProductSet::Release);
}
inline std::vector<IProductSetPtr> MakeAllProductSetPtr(IExchangeSetPtr exchange_ptr) { 
    std::vector<IProductSetPtr> product_ptrs;
	size_t count = exchange_ptr->Ref_All_Product(nullptr, 0);
	IProductSet** ptrs = (IProductSet**)alloca(sizeof(IProductSet*) * count);
	count = exchange_ptr->Ref_All_Product(ptrs, count);
    product_ptrs.resize(count);
    for(size_t i = 0; i < count; i++) 
	{
		product_ptrs[i] = IProductSetPtr(ptrs[i],&IProductSet::Release);
	}
	return product_ptrs;
}
inline std::vector<IProductSetPtr> MakeAllProductSetPtr() { 
    std::vector<IProductSetPtr> product_ptrs;
	size_t count = IMarketDataSet::Ref_All_Product(nullptr, 0);
	IProductSet** ptrs = (IProductSet**)alloca(sizeof(IProductSet*) * count);
	count = IMarketDataSet::Ref_All_Product(ptrs, count);
    product_ptrs.resize(count);
    for(size_t i = 0; i < count; i++) 
	{
		product_ptrs[i] = IProductSetPtr(ptrs[i],&IProductSet::Release);
	}
	return product_ptrs;
}

inline ICommoditySetPtr MakeCommoditySetPtr(const char* exchange, const char* product, const char* code, FindFlag flag = FindFlag::FindOrCreate) { 
	return ICommoditySetPtr(ICommoditySet::Ref(exchange,product,code,flag),&ICommoditySet::Release);
}
inline std::vector<ICommoditySetPtr> MakeAllCommoditySetPtr() { 
    std::vector<ICommoditySetPtr> commodity_ptrs;
	size_t count = IMarketDataSet::Ref_All_Commodity(nullptr, 0);
	ICommoditySet** ptrs = (ICommoditySet**)alloca(sizeof(ICommoditySet*) * count);
	count = IMarketDataSet::Ref_All_Commodity(ptrs, count);
    commodity_ptrs.resize(count);
    for(size_t i = 0; i < count; i++) 
	{
		commodity_ptrs[i] = ICommoditySetPtr(ptrs[i],&ICommoditySet::Release);
	}
	return commodity_ptrs;
}
inline std::vector<ICommoditySetPtr> MakeAllCommoditySetPtr(IExchangeSetPtr exchange_ptr) { 
    std::vector<ICommoditySetPtr> commodity_ptrs;
	size_t count = exchange_ptr->Ref_All_Commodity(nullptr, 0);
	ICommoditySet** ptrs = (ICommoditySet**)alloca(sizeof(ICommoditySet*) * count);
	count = exchange_ptr->Ref_All_Commodity(ptrs, count);
    commodity_ptrs.resize(count);
    for(size_t i = 0; i < count; i++) 
	{
		commodity_ptrs[i] = ICommoditySetPtr(ptrs[i],&ICommoditySet::Release);
	}
	return commodity_ptrs;
}
inline std::vector<ICommoditySetPtr> MakeAllCommoditySetPtr(IProductSetPtr product_ptr) { 
    std::vector<ICommoditySetPtr> commodity_ptrs;
	size_t count = product_ptr->Ref_All_Commodity(nullptr, 0);
	ICommoditySet** ptrs = (ICommoditySet**)alloca(sizeof(ICommoditySet*) * count);
	count = product_ptr->Ref_All_Commodity(ptrs, count);
    commodity_ptrs.resize(count);
    for(size_t i = 0; i < count; i++) 
	{
		commodity_ptrs[i] = ICommoditySetPtr(ptrs[i],&ICommoditySet::Release);
	}
	return commodity_ptrs;
}

inline ICalcDataSetPtr MakeCalcDataSetPtr(const char* exchange, const char* product, const char* code, PERIODTYPE period, DWTYPE dwtype, FindFlag flag = FindFlag::FindOrCreate) { 
	return ICalcDataSetPtr(ICalcDataSet::Ref(exchange,product,code,period,dwtype,flag),&ICalcDataSet::Release);
}

}

#endif//_H_IMDDATASET_H_
