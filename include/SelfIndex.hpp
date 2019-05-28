#pragma once
#include "DataDefEx.h"
#include <XUtil/XStr.hpp>

namespace MTP {

// 1. OpenInterest:持仓量加权。大部分品种的指数是使用的此方法，如IF000
// 2. Weight:权重法。权重参数可以任意设置，但在这里一般是让权重和等于1。如南华商品指数
// 3. Price:价格法。纯粹是简单价格指数，所以权重参数被忽略。如IF888这种主力合约行情

    struct LegData {
        // 权重，可能在外部已经指定好，也可以先求和，然后再算权重
        double weight = 0.;
        // 是否计算过
        bool calculate = false;
    };

template<class Base, class CommodityPtr>
class MDSelfIndex : public Base
{
protected:
    std::map<CommodityPtr,LegData> commoditys_;
    double weight_ = 0;
public:
    //MDSelfIndex(const std::string& code, std::vector<CommodityPtr> commoditys);

    std::map<CommodityPtr,LegData> GetCommoditys() { return commoditys_; }

	void Calculate(CommodityPtr& commodity)
    {

    }
};

template<class Base, class CommodityPtr>
class MDSelfIndexWithOpenInterest : public MDSelfIndex<Base,CommodityPtr>
{
    typedef MDSelfIndex<Base,CommodityPtr> Base;
public:
	void Calculate(CommodityPtr& commodity)
    {
        this->TradingDay = commodity->TradingDay;
        this->Date = commodity->Date;
        this->Time = commodity->Time;
        this->OpenInterest += commodity->OpenInterest;
        this->Volume += commodity->Volume;
        this->LastPrice = 0.;
        for(auto& it : commoditys_)
        {
            // 生成权重，对于指定权重的数据不需要修改，如指数的成份股
            it.second.weight = it.first->OpenInterest / this->weight_;
            // 生成新价格
            this->LastPrice += it.first->LastPrice*it.second.weight;
        }
    }
};

template<class Base, class CommodityPtr>
class MDSelfIndexWithWeight : public MDSelfIndex<Base,CommodityPtr>
{
    typedef MDSelfIndex<Base,CommodityPtr> Base;
public:
	void Calculate(CommodityPtr& commodity)
    {
        this->TradingDay = commodity->TradingDay;
        this->Date = commodity->Date;
        this->Time = commodity->Time;
        this->OpenInterest += commodity->OpenInterest;
        this->Volume += commodity->Volume;
        this->LastPrice = 0.;
        for(auto& it : commoditys_)
        {
            // 生成新价格
            this->LastPrice += it.first->LastPrice*it.second.weight;
        }
    }
};

template<class Base, class CommodityPtr>
class MDSelfIndexWithPrice : public MDSelfIndex<Base,CommodityPtr>
{
    typedef MDSelfIndex<Base,CommodityPtr> Base;
public:
	void Calculate(CommodityPtr& commodity)
    {
        this->TradingDay = commodity->TradingDay;
        this->Date = commodity->Date;
        this->Time = commodity->Time;
        this->OpenInterest += commodity->OpenInterest;
        this->Volume += commodity->Volume;
        this->LastPrice = 0.;
        for(auto& it : commoditys_)
        {
            // 生成新价格
            this->LastPrice += it.first->LastPrice;
        }
    }
};

}