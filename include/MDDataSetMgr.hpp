#pragma once

#include <IMDDataSet.h>

namespace MTP
{

template <class Server, class ExchangeSet, class ProductSet, class CommoditySet, class CalcDataSet>
class CMDDataSetMgr
{
protected:
    Server *srv_;
    struct IExchangeSetPtrLess
    {
        bool operator()(const IExchangeSet *const &x, const IExchangeSet *const &y) const
        {
            return *x < *y;
        }
    };
    struct ExchangeSetEx : public ExchangeSet
    {
        using ExchangeSet::ExchangeSet;
        int ref = 0;
    };
    typedef std::map<const IExchangeSet *, ExchangeSetEx *, IExchangeSetPtrLess> ExchangeSets;
    ExchangeSets exchangesets_;
    std::shared_mutex exchange_mutex_;
    struct IProductSetPtrLess
    {
        bool operator()(const IProductSet *const &x, const IProductSet *const &y) const
        {
            return *x < *y;
        }
    };
    struct ProductSetEx : public ProductSet
    {
        using ProductSet::ProductSet;
        int ref = 0;
    };
    typedef std::map<const IProductSet *, ProductSetEx *, IProductSetPtrLess> ProductSets;
    ProductSets productsets_;
    std::shared_mutex product_mutex_;
    struct ICommoditySetPtrLess
    {
        bool operator()(const ICommoditySet *const &x, const ICommoditySet *const &y) const
        {
            return *x < *y;
        }
    };
    struct CommoditySetEx : public CommoditySet
    {
        using CommoditySet::CommoditySet;
        int ref = 0;
    };
    typedef std::map<const ICommoditySet *, CommoditySetEx *, ICommoditySetPtrLess> CommoditySets;
    CommoditySets commoditysets_;
    std::shared_mutex commodity_mutex_;
    struct ICalcDataSetPtrLess
    {
        bool operator()(const ICalcDataSet *const &x, const ICalcDataSet *const &y) const
        {
            return *x < *y;
        }
    };
    struct CalcDataSetEx : public CalcDataSet
    {
        using CalcDataSet::CalcDataSet;
        ICommoditySet * commodityset = nullptr;
        int ref = 0;
    };
    typedef std::map<const ICalcDataSet *, CalcDataSetEx *, ICalcDataSetPtrLess> CalcDataSets;
    CalcDataSets calcdatasets_;
    std::shared_mutex calcdata_mutex_;

public:
    CMDDataSetMgr(Server *srv) : srv_(srv) {}
    ~CMDDataSetMgr() {}

    inline void Clear()
    {
        RemoveExchange(nullptr);
    }

    inline void RemoveExchange(const char* exchange)
    {
        RemoveProduct(exchange, nullptr);
        std::unique_lock<std::shared_mutex> w_lock(exchange_mutex_);
        if(exchange && exchange[0])
        {
            ExchangeSetInfo datasetinfo(exchange);
            auto it = exchangesets_.find(&datasetinfo);
            if (it != exchangesets_.end())
            {
                exchangesets_.erase(it);
            }
        }
        else
        {
            exchangesets_.clear();
        }
    }

    inline void RemoveProduct(const char* exchange, const char* product)
    {
        RemoveCalcData(exchange, product, nullptr);
        RemoveCommodity(exchange, product, nullptr);
        std::unique_lock<std::shared_mutex> w_lock(product_mutex_);
        if(exchange && exchange[0])
        {
            // ProductSetInfo datasetinfo(exchange, product);
            // auto pr = productsets_.equal_range(&datasetinfo);
            // productsets_.erase(pr.first, pr.second);
            for(auto it = productsets_.begin(); it != productsets_.end(); )
            {
                if(strcmp(it->second->Exchange(),exchange) == 0) {
                    if(product && product[0]) {
                        if(strcmp(it->second->Product(),product) == 0) {
                            it = productsets_.erase(it);
                            break;
                        }
                    } else {
                        it = productsets_.erase(it);
                        continue;
                    }
                } 
                ++it;
            }
        }
        else
        {
            productsets_.clear();
        }
    }

    inline void RemoveCommodity(const char* exchange, const char* product, const char* code)
    {
        std::unique_lock<std::shared_mutex> w_lock(commodity_mutex_);
        if(exchange && exchange[0])
        {
            // CommoditySetInfo datasetinfo(exchange, product, code);
            // auto pr = commoditysets_.equal_range(&datasetinfo);
            // commoditysets_.erase(pr.first, pr.second);
            for(auto it = commoditysets_.begin(); it != commoditysets_.end(); )
            {
                if(strcmp(it->second->Exchange(),exchange) == 0) {
                    if(product && product[0]) {
                        if(strcmp(it->second->Product(),product) == 0) {
                            if(code && code[0]) {
                                if(strcmp(it->second->Code(),code) == 0) {
                                    it = commoditysets_.erase(it);
                                    break;
                                } 
                            } else {
                                it = commoditysets_.erase(it);
                                continue;
                            }
                        }
                    } else {
                        it = commoditysets_.erase(it);
                        continue;
                    }
                } 
                ++it;
            }
        }
        else
        {
            commoditysets_.clear();
        }
    }

    inline void RemoveCalcData(const char* exchange, const char* product, const char* code)
    {
        std::unique_lock<std::shared_mutex> w_lock(calcdata_mutex_);
        if(exchange && exchange[0]) {
            // CalcDataSetInfo datasetinfo(exchange, product, code, (PERIODTYPE)0, (DWTYPE)0);
            // auto pr = calcdatasets_.equal_range(&datasetinfo);
            // calcdatasets_.erase(pr.first, pr.second);
            for(auto it = calcdatasets_.begin(); it != calcdatasets_.end(); )
            {
                if(strcmp(it->second->Exchange(),exchange) == 0) {
                    if(product && product[0]) {
                        if(strcmp(it->second->Product(),product) == 0) {
                            if(code && code[0]) {
                                if(strcmp(it->second->Code(),code) == 0) {
                                    it = calcdatasets_.erase(it);
                                    continue;
                                } 
                            } else {
                                it = calcdatasets_.erase(it);
                                continue;
                            }
                        }
                    } else {
                        it = calcdatasets_.erase(it);
                        continue;
                    }
                } 
                ++it;
            }
        }
        else
        {
            calcdatasets_.clear();
        }
    }

    IExchangeSet *Ref(const char *exchange, FindFlag flag = FindFlag::FindOrCreate)
    {
        if (flag == FindFlag::FindOrCreate || flag == FindFlag::FindOnly)
        {
            std::shared_lock<std::shared_mutex> lock(exchange_mutex_);
            ExchangeSetInfo datasetinfo(exchange);
            auto it = exchangesets_.find(&datasetinfo);
            if (it != exchangesets_.end())
            {
                it->second->ref++;
                return it->second;
            }
        }
        if (flag == FindFlag::FindOrCreate || flag == FindFlag::CreateOnly)
        {
            std::unique_lock<std::shared_mutex> lock(exchange_mutex_);
            auto exchange_ptr = srv_->New(exchange);
            if (exchange_ptr)
            {
                exchange_ptr->ref = 1;
                exchangesets_[exchange_ptr] = exchange_ptr;
                return exchange_ptr;
            }
        }
        return nullptr;
    }

    void Release(IExchangeSet *dataset)
    {
        return; //不释放
        ExchangeSetEx* exchangeset = dynamic_cast<ExchangeSetEx*>(dataset);
        if (!exchangeset)
        {
            return;
        }
        std::unique_lock<std::shared_mutex> lock(exchange_mutex_);
        int ref = --exchangeset->ref;
        auto it = exchangesets_.find(dataset);
        if (it != exchangesets_.end())
        {
            if (ref == 0)
            {
                exchangesets_.erase(it);
            }
        }
        if (ref == 0)
        {
            delete exchangeset;
        }
    }

    IProductSet *Ref(const char *exchange, const char *product, FindFlag flag = FindFlag::FindOrCreate)
    {
        if (flag == FindFlag::FindOrCreate || flag == FindFlag::FindOnly)
        {
            std::shared_lock<std::shared_mutex> lock(product_mutex_);
            ProductSetInfo datasetinfo(exchange, product);
            auto it = productsets_.find(&datasetinfo);
            if (it != productsets_.end())
            {
                it->second->ref++;
                return it->second;
            }
        }
        if (flag == FindFlag::FindOrCreate || flag == FindFlag::CreateOnly)
        {
            std::unique_lock<std::shared_mutex> lock(product_mutex_);
            auto product_ptr = srv_->New(exchange, product);
            if (product_ptr)
            {
                product_ptr->ref = 1;
                productsets_[product_ptr] = product_ptr;
                return product_ptr;
            }
        }
        return nullptr;
    }

    void Release(IProductSet *dataset)
    {
        return; //不释放
        ProductSetEx* productset = dynamic_cast<ProductSetEx*>(dataset);
        if (!productset)
        {
            return;
        }
        std::unique_lock<std::shared_mutex> lock(product_mutex_);
        int ref = --productset->ref;
        auto it = productsets_.find(dataset);
        if (it != productsets_.end())
        {
            if (ref == 0)
            {
                productsets_.erase(it);
            }
        }
        if (ref == 0)
        {
            delete productset;
        }
    }

    ICommoditySet *Ref(const char *exchange, const char *product, const char *code, FindFlag flag = FindFlag::FindOrCreate)
    {
        if (flag == FindFlag::FindOrCreate || flag == FindFlag::FindOnly)
        {
            std::shared_lock<std::shared_mutex> lock(commodity_mutex_);
            CommoditySetInfo datasetinfo(exchange, product, code);
            auto it = commoditysets_.find(&datasetinfo);
            if (it != commoditysets_.end())
            {
                it->second->ref++;
                return it->second;
            }
        }
        if (flag == FindFlag::FindOrCreate || flag == FindFlag::CreateOnly)
        {
            std::unique_lock<std::shared_mutex> lock(commodity_mutex_);
            auto commodity_ptr = srv_->New(exchange, product, code);
            if (commodity_ptr)
            {
                commodity_ptr->ref = 1;
                commoditysets_[commodity_ptr] = commodity_ptr;
                return commodity_ptr;
            }
        }
        return nullptr;
    }

    void Release(ICommoditySet *dataset)
    {
        return; //不释放
        CommoditySetEx* commodityset = dynamic_cast<CommoditySetEx*>(dataset);
        if (!dataset)
        {
            return;
        }
        std::unique_lock<std::shared_mutex> lock(commodity_mutex_);
        int ref = --commodityset->ref;
        auto it = commoditysets_.find(dataset);
        if (it != commoditysets_.end())
        {
            if (ref == 0)
            {
                commoditysets_.erase(it);
            }
        }
        if (ref == 0)
        {
            delete commodityset;
        }
    }

    ICalcDataSet *Ref(const char *exchange, const char *product, const char *code, PERIODTYPE period, DWTYPE dwtype, FindFlag flag = FindFlag::FindOrCreate)
    {
        if (flag == FindFlag::FindOrCreate || flag == FindFlag::FindOnly)
        {
            std::shared_lock<std::shared_mutex> lock(calcdata_mutex_);
            CalcDataSetInfo datasetinfo(exchange, product, code, period, dwtype);
            auto it = calcdatasets_.find(&datasetinfo);
            if (it != calcdatasets_.end())
            {
                it->second->ref++;
                return it->second;
            }
        }
        if (flag == FindFlag::FindOrCreate || flag == FindFlag::CreateOnly)
        {
            boost::unique_lock<std::shared_mutex> w_lock(calcdata_mutex_);
            auto calcdata_ptr = srv_->New(exchange, product, code, period, dwtype);
            if (calcdata_ptr)
            {
                calcdata_ptr->commodityset = Ref(exchange, product, code);
                calcdata_ptr->ref = 1;
                calcdatasets_[calcdata_ptr] = calcdata_ptr;
                return calcdata_ptr;
            }
        }
        return nullptr;
    }

    void Release(ICalcDataSet *dataset)
    {
        CalcDataSetEx* calcdataset = dynamic_cast<CalcDataSetEx*>(dataset);
        if (!dataset)
        {
            return;
        }
        std::unique_lock<std::shared_mutex> lock(calcdata_mutex_);
        int ref = --calcdataset->ref;
        auto it = calcdatasets_.find(dataset);
        if (it != calcdatasets_.end())
        {
            if (ref == 0)
            {
                calcdatasets_.erase(it);
            }
        }
        if (ref == 0)
        {
            Release(calcdataset->commodityset);
            delete calcdataset;
        }
    }
};

} // namespace MTP
