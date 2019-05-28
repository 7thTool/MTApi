#pragma once
#ifndef _H_MDDATASETEX_HPP_
#define _H_MDDATASETEX_HPP_

#include <IMDDataSet.h>
#include <DataSetConv.hpp>
#include <MDBase.hpp>

namespace MTP {

    template<class Base, class ExchangePtr, class ProductPtr, class CommodityPtr>
    class ExchangeSetWrapper : public Base
    {
    protected:
        ExchangePtr exchange_;
        std::vector<ProductPtr> products_;
        std::vector<CommodityPtr> commoditys_;
    public:
        ExchangeSetWrapper(ExchangePtr exchange):exchange_(exchange)
        {

        }
        ~ExchangeSetWrapper() 
        {

        }

        ExchangeInfo& get() { return *this->exchange_; };

		const char* Exchange() const { return exchange_->Exchange; }

        //快照字段
        size_t GetFieldInfo(FieldInfo* info, size_t count) {
            static const size_t ids[] = {
                IDF_MarketType,
                IDF_Exchange,
                IDF_Name,
                IDF_TimeZone,
                IDF_TradingDay,
                IDF_DATE,
                IDF_TIME,
                IDF_Status,
                IDF_EnterReason,
                IDF_Periods,
                IDF_PRODUCTINFO,
                IDF_COMMODITYINFO
            };
            if(!info || !count) 
                return _countof(ids);
        
            size_t index = 0;
            for(; index < _countof(ids) && index < count; ++index)
            {
                info[index].id = ids[index];
                GetFieldInfoById(info[index].id, &info[index]);
            }
            return index;
        }
        virtual size_t GetFieldInfoById(size_t id, FieldInfo* info = nullptr)
        {
            if(!info) {
                return 0;
            }
            info->id = id;
            switch(id) {
                case IDF_MarketType:
                {
                    info->type = FIELD_TYPE_INT8;
                }
                break;
                case IDF_Exchange:
                {
                    info->type = FIELD_TYPE_STRING;
                }
                break;
                case IDF_Name:
                {
                    info->type = FIELD_TYPE_STRING;
                }
                break;
                case IDF_TimeZone:
                {
                    info->type = FIELD_TYPE_INT8;
                }
                break;
                case IDF_TradingDay:
                {
                    info->type = FIELD_TYPE_INT32;
                }
                break;
                case IDF_DATE:
                {
                    info->type = FIELD_TYPE_INT32;
                }
                break;
                case IDF_TIME:
                {
                    info->type = FIELD_TYPE_INT32;
                }
                break;
                case IDF_Status:
                {
                    info->type = FIELD_TYPE_INT8;
                }
                break;
                case IDF_EnterReason:
                {
                    info->type = FIELD_TYPE_INT8;
                }
                break;
                case IDF_Periods:
                {
                    info->type = FIELD_TYPE_INT16;
                }
                break;
                case IDF_PRODUCTINFO:
                {
                    info->type = FIELD_TYPE_PRODUCT;
                }
                break;
                case IDF_COMMODITYINFO:
                {
                    info->type = FIELD_TYPE_COMMODITY;
                }
                break;
                default:
                {
                    return 0;
                }
                break;
            }
            return id;
        }
        
        size_t GetFieldSize(size_t id)
        {
            switch(id) {
                case IDF_MarketType:
                case IDF_Exchange:
                case IDF_Name:
                case IDF_TimeZone:
                case IDF_TradingDay:
                case IDF_DATE:
                case IDF_TIME:
                case IDF_Status:
                case IDF_EnterReason:
                {
                    return 1;
                }
                break;
                case IDF_Periods:
                {
                    return exchange_->PeriodsCount * 2;
                }
                break;
                case IDF_PRODUCTINFO:
                {
                    return products_.size();
                }
                break;
                case IDF_COMMODITYINFO:
                {
                    return commoditys_.size();
                }
                break;
            }
            return 0;
        }

        void * GetFieldValue(size_t id, size_t offset = 0) {
            if(!exchange_) {
                return nullptr;
            }
            switch(id) {
                case IDF_MarketType:
                {
                    return &exchange_->MarketType;
                }
                break;
                case IDF_Exchange:
                {
                    return &exchange_->Exchange;
                }
                break;
                case IDF_Name:
                {
                    return &exchange_->Name;
                }
                break;
                case IDF_TimeZone:
                {
                    return &exchange_->TimeZone;
                }
                break;
                case IDF_TradingDay:
                {
                    return &exchange_->TradingDay;
                }
                break;
                case IDF_DATE:
                {
                    return &exchange_->Date;
                }
                break;
                case IDF_TIME:
                {
                    return &exchange_->Time;
                }
                break;
                case IDF_Status:
                {
                    return &exchange_->Status;
                }
                break;
                case IDF_EnterReason:
                {
                    return &exchange_->EnterReason;
                }
                break;
                case IDF_Periods:
                {
                    return &(exchange_->Periods[offset/2][offset%2]);
                }
                break;
                case IDF_PRODUCTINFO:
                {
                    if(offset >= products_.size()) {
                        break;
                    }
                    return &(*products_[offset]);
                }
                break;
                case IDF_COMMODITYINFO:
                {
                    if(offset >= commoditys_.size()) {
                        break;
                    }
                    return &(*commoditys_[offset]);
                }
                break;
            }
            return nullptr;
        }
    };

    template<class Base, class ProductPtr, class CommodityPtr>
    class ProductSetWrapper : public Base
    {
    protected:
        ProductPtr product_;
        std::vector<CommodityPtr> commoditys_;
    public:
        ProductSetWrapper(ProductPtr product):product_(product)
        {

        }
        
        ~ProductSetWrapper()
        {
            
        }

        ProductInfo& get() { return *this->product_; }

		const char* Exchange() const { return product_->Exchange; }
		const char* Product() const { return product_->Product; }

        //快照字段
        size_t GetFieldInfo(FieldInfo* info, size_t count)
        {
            static const size_t ids[] = {
                IDF_MarketType,
                IDF_Exchange,
                IDF_ProductType,
                IDF_Product,
                IDF_Name,
                IDF_Digits,
                IDF_PriceDigits,
                IDF_Status,
                IDF_EnterReason,
                IDF_Periods,
                IDF_COMMODITYINFO
            };
            if(!info || !count) 
                return _countof(ids);
        
            size_t index = 0;
            for(; index < _countof(ids) && index < count; ++index)
            {
                info[index].id = ids[index];
                GetFieldInfoById(info[index].id, &info[index]);
            }
            return index;
        }
        
        virtual size_t GetFieldInfoById(size_t id, FieldInfo* info = nullptr)
        {
            if(!info) {
                return 0;
            }
            info->id = id;
            switch(id) {
                case IDF_MarketType:
                {
                    info->type = FIELD_TYPE_INT8;
                }
                break;
                case IDF_Exchange:
                {
                    info->type = FIELD_TYPE_STRING;
                }
                break;
                case IDF_ProductType:
                {
                    info->type = FIELD_TYPE_INT8;
                }
                break;
                case IDF_Product:
                {
                    info->type = FIELD_TYPE_STRING;
                }
                break;
                case IDF_Name:
                {
                    info->type = FIELD_TYPE_STRING;
                }
                break;
                case IDF_Digits:
                {
                    info->type = FIELD_TYPE_INT8;
                }
                break;
                case IDF_PriceDigits:
                {
                    info->type = FIELD_TYPE_INT8;
                }
                break;
                case IDF_Status:
                {
                    info->type = FIELD_TYPE_INT8;
                }
                break;
                case IDF_EnterReason:
                {
                    info->type = FIELD_TYPE_INT8;
                }
                break;
                case IDF_Periods:
                {
                    info->type = FIELD_TYPE_INT16;
                }
                break;
                case IDF_COMMODITYINFO:
                {
                    info->type = FIELD_TYPE_COMMODITY;
                }
                break;
                default:
                {
                    return 0;
                }
                break;
            }
            return id;
        }
        size_t GetFieldSize(size_t id)
        {
            switch(id) {
                case IDF_MarketType:
                case IDF_Exchange:
                case IDF_ProductType:
                case IDF_Product:
                case IDF_Name:
                case IDF_Digits:
                case IDF_PriceDigits:
                case IDF_Status:
                case IDF_EnterReason:
                {
                    return 1;
                }
                break;
                case IDF_Periods:
                {
                    return product_->PeriodsCount * 2;
                }
                break;
                case IDF_COMMODITYINFO:
                {
                    return commoditys_.size();
                }
                break;
            }
            return 0;
        }
        //
        void * GetFieldValue(size_t id, size_t offset = 0) {
            switch(id) {
                case IDF_MarketType:
                {
                    return &product_->MarketType;
                }
                break;
                case IDF_Exchange:
                {
                    return product_->Exchange;
                }
                break;
                case IDF_ProductType:
                {
                    return &product_->ProductType;
                }
                break;
                case IDF_Product:
                {
                    return product_->Product;
                }
                break;
                case IDF_Name:
                {
                    return product_->Name;
                }
                break;
                case IDF_Digits:
                {
                    return &product_->Digits;
                }
                break;
                case IDF_PriceDigits:
                {
                    return &product_->PriceDigits;
                }
                break;
                case IDF_Status:
                {
                    return &product_->Status;
                }
                break;
                case IDF_EnterReason:
                {
                    return &product_->EnterReason;
                }
                break;
                case IDF_Periods:
                {
                    return &(product_->Periods[offset/2][offset%2]);
                }
                break;
                case IDF_COMMODITYINFO:
                {
                    if(offset >= commoditys_.size()) {
                        break;
                    }
                    return &(*commoditys_[offset]);
                }
                break;
            }
            return nullptr;
        }
    };

    template<class Base, class CommodityPtr>
    class CommoditySetWrapper : public Base
    {
    protected:
        CommodityPtr commodity_;
    public:
        CommoditySetWrapper(CommodityPtr commodity):commodity_(commodity)
        {

        }

        ~CommoditySetWrapper()
        {
            
        }

        CommodityInfo& get() { 
            return *this->commodity_; 
        }

		const char* Exchange() const { return commodity_->Exchange; }
		const char* Product() const { return commodity_->Product; }
		const char* Code() const { return commodity_->Code; }

        //快照字段
        size_t GetFieldInfo(FieldInfo* info, size_t count)
        {
            static const size_t ids[] = {
                IDF_MarketType,
                IDF_Exchange,
                IDF_ProductType,
                IDF_Product,
                IDF_Code,
                IDF_Name,
                IDF_Digits,
                IDF_PriceDigits,
                IDF_PREV_SETTLEMENT,
                IDF_PREV_CLOSE,
                IDF_PreOpenInterest,
                IDF_CEILING_PRICE,
                IDF_FLOOR_PRICE,
                IDF_PreDelta,
                IDF_Status,
                IDF_EnterReason,
                IDF_DATE,
                IDF_TIME,
                IDF_OPEN,
                IDF_HIGH,
                IDF_LOW,
                IDF_CLOSE,
                IDF_LAST_VOLUME,
                IDF_LastAmount,
                IDF_VOLUME,
                IDF_AMOUNT,
                IDF_SETTLEMENT,
                IDF_INTEREST,
                IDF_CurrDelta,
                IDF_Periods,
                IDF_BidPrice,
                IDF_AskPrice,
                IDF_BidVolume,
                IDF_AskVolume
#ifdef COMMODITY_EX_FLAG
                , IDF_BidVolDiff
                , IDF_AskVolDiff
#endif
            };
            if(!info || !count) 
                return _countof(ids);
        
            size_t index = 0;
            for(; index < _countof(ids) && index < count; ++index)
            {
                info[index].id = ids[index];
                GetFieldInfoById(info[index].id, &info[index]);
            }
            return index;
        }
        
        virtual size_t GetFieldInfoById(size_t id, FieldInfo* info = nullptr)
        {
            if(!info) {
                return 0;
            }
            info->id = id;
            switch(id) {
                case IDF_MarketType:
                {
                    info->type = FIELD_TYPE_INT8;
                }
                break;
                case IDF_Exchange:
                {
                    info->type = FIELD_TYPE_STRING;
                }
                break;
                case IDF_ProductType:
                {
                    info->type = FIELD_TYPE_INT8;
                }
                break;
                case IDF_Product:
                {
                    info->type = FIELD_TYPE_STRING;
                }
                break;
                case IDF_Code:
                {
                    info->type = FIELD_TYPE_STRING;
                }
                break;
                case IDF_Name:
                {
                    info->type = FIELD_TYPE_STRING;
                }
                break;
                case IDF_Digits:
                {
                    info->type = FIELD_TYPE_INT8;
                }
                break;
                case IDF_PriceDigits:
                {
                    info->type = FIELD_TYPE_INT8;
                }
                break;
                case IDF_PREV_SETTLEMENT:
                {
                    info->type = FIELD_TYPE_DOUBLE;
                }
                break;
                case IDF_PREV_CLOSE:
                {
                    info->type = FIELD_TYPE_DOUBLE;
                }
                break;
                case IDF_PreOpenInterest:
                {
                    info->type = FIELD_TYPE_DOUBLE;
                }
                break;
                case IDF_CEILING_PRICE:
                {
                    info->type = FIELD_TYPE_DOUBLE;
                }
                break;
                case IDF_FLOOR_PRICE:
                {
                    info->type = FIELD_TYPE_DOUBLE;
                }
                break;
                case IDF_PreDelta:
                {
                    info->type = FIELD_TYPE_DOUBLE;
                }
                break;
                case IDF_Status:
                {
                    info->type = FIELD_TYPE_INT8;
                }
                break;
                case IDF_EnterReason:
                {
                    info->type = FIELD_TYPE_INT8;
                }
                break;
                case IDF_DATE:
                {
                    info->type = FIELD_TYPE_INT32;
                }
                break;
                case IDF_TIME:
                {
                    info->type = FIELD_TYPE_INT32;
                }
                break;
                case IDF_OPEN:
                {
                    info->type = FIELD_TYPE_DOUBLE;
                }
                break;
                case IDF_HIGH:
                {
                    info->type = FIELD_TYPE_DOUBLE;
                }
                break;
                case IDF_LOW:
                {
                    info->type = FIELD_TYPE_DOUBLE;
                }
                break;
                case IDF_CLOSE:
                {
                    info->type = FIELD_TYPE_DOUBLE;
                }
                break;
                case IDF_LAST_VOLUME:
                {
                    info->type = FIELD_TYPE_INT32;
                }
                break;
                case IDF_LastAmount:
                {
                    info->type = FIELD_TYPE_DOUBLE;
                }
                break;
                case IDF_VOLUME:
                {
                    info->type = FIELD_TYPE_INT32;
                }
                break;
                case IDF_AMOUNT:
                {
                    info->type = FIELD_TYPE_DOUBLE;
                }
                break;
                case IDF_SETTLEMENT:
                {
                    info->type = FIELD_TYPE_DOUBLE;
                }
                break;
                case IDF_INTEREST:
                {
                    info->type = FIELD_TYPE_INT32;
                }
                break;
                case IDF_CurrDelta:
                {
                    info->type = FIELD_TYPE_INT8;
                }
                break;
                case IDF_Periods:
                {
                    info->type = FIELD_TYPE_INT16;
                }
                break;
                case IDF_BidPrice:
                {
                    info->type = FIELD_TYPE_DOUBLE;
                }
                break;
                case IDF_AskPrice:
                {
                    info->type = FIELD_TYPE_DOUBLE;
                }
                break;
                case IDF_BidVolume:
                {
                    info->type = FIELD_TYPE_INT32;
                }
                break;
                case IDF_AskVolume:
                {
                    info->type = FIELD_TYPE_INT32;
                }
                break;
#ifdef COMMODITY_EX_FLAG
                case IDF_BidVolDiff:
                {
                    info->type = FIELD_TYPE_INT32;
                }
                break;
                case IDF_AskVolDiff:
                {
                    info->type = FIELD_TYPE_INT32;
                }
                break;
#endif
                default:
                {
                    return 0;
                }
                break;
            }
            return id;
        }
        size_t GetFieldSize(size_t id)
        {
            switch(id) {
                case IDF_MarketType:
                case IDF_Exchange:
                case IDF_ProductType:
                case IDF_Product:
                case IDF_Code:
                case IDF_Name:
                case IDF_Digits:
                case IDF_PriceDigits:
                case IDF_PREV_SETTLEMENT:
                case IDF_PREV_CLOSE:
                case IDF_PreOpenInterest:
                case IDF_CEILING_PRICE:
                case IDF_FLOOR_PRICE:
                case IDF_PreDelta:
                case IDF_Status:
                case IDF_EnterReason:
                case IDF_DATE:
                case IDF_TIME:
                case IDF_OPEN:
                case IDF_HIGH:
                case IDF_LOW:
                case IDF_CLOSE:
                case IDF_LAST_VOLUME:
                case IDF_LastAmount:
                case IDF_VOLUME:
                case IDF_AMOUNT:
                case IDF_SETTLEMENT:
                case IDF_INTEREST:
                case IDF_CurrDelta:
                {
                    return 1;
                }
                break;
                case IDF_Periods:
                {
                    return commodity_->PeriodsCount * 2;
                }
                break;
                case IDF_BidPrice:
                {
                    return 1;
                }
                break;
                case IDF_AskPrice:
                {
                    return 1;
                }
                break;
                case IDF_BidVolume:
                {
                    return 1;
                }
                break;
                case IDF_AskVolume:
                {
                    return 1;
                }
                break;
#ifdef COMMODITY_EX_FLAG
                case IDF_BidVolDiff:
                {
                    return 1;
                }
                break;
                case IDF_AskVolDiff:
                {
                    return 1;
                }
                break;
#endif
            }
            return 0;
        }
        void * GetFieldValue(size_t id, size_t offset = 0)
        {
            switch(id) {
                case IDF_MarketType:
                {
                    return &commodity_->MarketType;
                }
                break;
                case IDF_Exchange:
                {
                    return commodity_->Exchange;
                }
                break;
                case IDF_ProductType:
                {
                    return &commodity_->ProductType;
                }
                break;
                case IDF_Product:
                {
                    return commodity_->Product;
                }
                break;
                case IDF_Code:
                {
                    return commodity_->Code;
                }
                break;
                case IDF_Name:
                {
                    return commodity_->Name;
                }
                break;
                case IDF_Digits:
                {
                    return &commodity_->Digits;
                }
                break;
                case IDF_PriceDigits:
                {
                    return &commodity_->PriceDigits;
                }
                break;
                case IDF_PREV_SETTLEMENT:
                {
                    return &commodity_->PreSettlementPrice;
                }
                break;
                case IDF_PREV_CLOSE:
                {
                    return &commodity_->PreClosePrice;
                }
                break;
                case IDF_PreOpenInterest:
                {
                    return &commodity_->PreOpenInterest;
                }
                break;
                case IDF_CEILING_PRICE:
                {
                    return &commodity_->UpperLimitPrice;
                }
                break;
                case IDF_FLOOR_PRICE:
                {
                    return &commodity_->LowerLimitPrice;
                }
                break;
                case IDF_PreDelta:
                {
                    return &commodity_->PreDelta;
                }
                break;
                case IDF_Status:
                {
                    return &commodity_->Status;
                }
                break;
                case IDF_EnterReason:
                {
                    return &commodity_->EnterReason;
                }
                break;
                case IDF_DATE:
                {
                    return &commodity_->Date;
                }
                break;
                case IDF_TIME:
                {
                    return &commodity_->Time;
                }
                break;
                case IDF_OPEN:
                {
                    return &commodity_->Open;
                }
                break;
                case IDF_HIGH:
                {
                    return &commodity_->High;
                }
                break;
                case IDF_LOW:
                {
                    return &commodity_->Low;
                }
                break;
                case IDF_CLOSE:
                {
                    return &commodity_->Price;
                }
                break;
                case IDF_LAST_VOLUME:
                {
                    return &commodity_->LastVolume;
                }
                break;
                case IDF_LastAmount:
                {
                    return &commodity_->LastAmount;
                }
                break;
                case IDF_VOLUME:
                {
                    return &commodity_->Volume;
                }
                break;
                case IDF_AMOUNT:
                {
                    return &commodity_->Amount;
                }
                break;
                case IDF_SETTLEMENT:
                {
                    return &commodity_->SettlementPrice;
                }
                break;
                case IDF_INTEREST:
                {
                    return &commodity_->OpenInterest;
                }
                break;
                case IDF_CurrDelta:
                {
                    return &commodity_->CurrDelta;
                }
                break;
                case IDF_Periods:
                {
                    return &(commodity_->Periods[offset/2][offset%2]);
                }
                break;
                case IDF_BidPrice:
                {
                    return &(commodity_->BidPrice[offset]);
                }
                break;
                case IDF_AskPrice:
                {
                    return &(commodity_->AskPrice[offset]);
                }
                break;
                case IDF_BidVolume:
                {
                    return &(commodity_->BidVolume[offset]);
                }
                break;
                case IDF_AskVolume:
                {
                    return &(commodity_->AskVolume[offset]);
                }
                break;
#ifdef COMMODITY_EX_FLAG
                case IDF_BidVolDiff:
                {
                    return &(commodity_->BidVolDiff[offset]);
                }
                break;
                case IDF_AskVolDiff:
                {
                    return &(commodity_->AskVolDiff[offset]);
                }
                break;
#endif
            }
            return nullptr;
        }
    };

    template<class TBase, class TCommodityPtr>
    class MDCalcDataSetWrapper : public CommoditySetWrapper<TBase,TCommodityPtr>
    {
        typedef CommoditySetWrapper<TBase,TCommodityPtr> Base;
    protected:
	    PERIODTYPE period_;
	    DWTYPE dwtype_;
	    tagCalcParam param_;
        std::vector<KDATA> kdatas_;
        bool update_flag_ = false; //是否发生变化
    public:
        MDCalcDataSetWrapper(TCommodityPtr commodity, PERIODTYPE period, DWTYPE dwtype):Base(commodity),period_(period),dwtype_(dwtype)
        {
            param_.Period = period;
            param_.PeriodEx = 0;
            param_.BasePeriod = CalcBasePeriod(period, 0, &param_.Multiple);
        }

        const char* Exchange() const { return commodity_->Exchange; }
        const char* Product() const { return commodity_->Product; }
        const char* Code() const { return commodity_->Code; }
        PERIODTYPE Period() const { return period_; }
        DWTYPE DWType() const { return dwtype_; }
        
        //快照字段
        size_t GetFieldInfo(FieldInfo* info, size_t count)
        {
            size_t base_index = Base::GetFieldInfo(info, count);
            static const size_t ids[] = {
                IDF_Period
                , IDF_DWType
                , IDF_DATE
                , IDF_TIME
                , IDF_OPEN
                , IDF_HIGH
                , IDF_LOW
                , IDF_CLOSE
                , IDF_VOLUME
                , IDF_AMOUNT
                , IDF_BARGAIN
                , IDF_INTEREST
            };
            if(!info || !count) 
                return base_index + _countof(ids);
        
            size_t index = 0;
            for(; index < _countof(ids) && index < count; ++index)
            {
                info[index].id = ids[index];
                GetFieldInfoById(info[index].id, &info[index]);
            }
            return base_index + index;
        }
        
        virtual size_t GetFieldInfoById(size_t id, FieldInfo* info = nullptr)
        {
            size_t base_id = Base::GetFieldInfoById(id, info);
            if(!base_id) {
                return 0;
            }
            if(!info) {
                return 0;
            }
            info->id = id;
            switch(id) {
                case IDF_MarketType:
                {
                    info->type = FIELD_TYPE_INT8;
                }
                break;
                case IDF_DATE:
                {
                    info->type = FIELD_TYPE_INT32;
                }
                break;
                case IDF_TIME:
                {
                    info->type = FIELD_TYPE_INT32;
                }
                break;
                case IDF_OPEN:
                {
                    info->type = FIELD_TYPE_DOUBLE;
                }
                break;
                case IDF_HIGH:
                {
                    info->type = FIELD_TYPE_DOUBLE;
                }
                break;
                case IDF_LOW:
                {
                    info->type = FIELD_TYPE_DOUBLE;
                }
                break;
                case IDF_CLOSE:
                {
                    info->type = FIELD_TYPE_DOUBLE;
                }
                break;
                case IDF_VOLUME:
                {
                    info->type = FIELD_TYPE_INT32;
                }
                break;
                case IDF_AMOUNT:
                {
                    info->type = FIELD_TYPE_DOUBLE;
                }
                break;
                case IDF_BARGAIN:
                {
                    info->type = FIELD_TYPE_INT32;
                }
                break;
                case IDF_INTEREST:
                {
                    info->type = FIELD_TYPE_INT32;
                }
                break;
                default:
                {
                    return 0;
                }
                break;
            }
            return id;
        }
        size_t GetFieldSize(size_t id)
        {
            switch(id)
            {
                case IDF_Period:
                {
                    return 1;
                }
                break;
                case IDF_DWType:
                {
                    return 1;
                }
                break;
                case IDF_DATE:
                case IDF_TIME:
                case IDF_OPEN:
                case IDF_HIGH:
                case IDF_LOW:
                case IDF_CLOSE:
                case IDF_VOLUME:
                case IDF_AMOUNT:
                case IDF_BARGAIN:
                case IDF_INTEREST:
                {
                    switch (period_)
                    {
                    case CYC_TICK:
                        return commodity_->ticks_.size();
                        break;
                    case CYC_5SEC:
                        return commodity_->rt_kdatas_[period_].size();
                        break;
                    case CYC_ANYSEC:
                        {
                            size_t old_size = kdatas_.size();
                            size_t new_size = commodity_->rt_kdatas_[period_].size();
                            if(!update_flag_ && old_size == new_size) {
                                return kdatas_.size();
                            }
                            kdatas_.resize(commodity_->rt_kdatas_[period_].size());
                            kdatas_.resize(CalcSecondData(&param_, &kdatas_[0], kdatas_.size()
                            , &commodity_->rt_kdatas_[period_][0], commodity_->rt_kdatas_[period_].size()));
                            update_flag_ = false;
                        }
                        break;
                    case CYC_1MIN:
                        return commodity_->rt_kdatas_[period_].size();
                        break;
                    case CYC_5MIN:
                    case CYC_15MIN:
                    case CYC_30MIN:
                    case CYC_60MIN:
                    case CYC_ANYMIN:
                        {
                            size_t old_size = kdatas_.size();
                            size_t new_size = commodity_->rt_kdatas_[period_].size();
                            if(!update_flag_ && old_size == new_size) {
                                return kdatas_.size();
                            }
                            kdatas_.resize(commodity_->rt_kdatas_[period_].size());
                            kdatas_.resize(CalcMinuteData(&param_, &kdatas_[0], kdatas_.size()
                            , &commodity_->rt_kdatas_[period_][0], commodity_->rt_kdatas_[period_].size()));
                            update_flag_ = false;
                        }
                        break;
                    case CYC_DAY:
                        return 1;
                        break;
                    case CYC_WEEK:
                    case CYC_MONTH:
                    case CYC_SEASON:
                    case CYC_YEAR:
                    case CYC_ANYDAY:
                        break;
                    default:
                        break;
                    }
                    return 0;
                }
                break;
            }
            return Base::GetFieldSize(id);
        }
        void * GetFieldValue(size_t id, size_t offset = 0)
        {
            switch(id)
            {
                case IDF_Period:
                {
                return &(this->period_);
                }
                break;
                case IDF_DWType:
                {
                    return &(this->dwtype_);
                }
                break;
                case IDF_DATE:
                case IDF_TIME:
                case IDF_OPEN:
                case IDF_HIGH:
                case IDF_LOW:
                case IDF_CLOSE:
                case IDF_VOLUME:
                case IDF_AMOUNT:
                case IDF_BARGAIN:
                case IDF_INTEREST:
                {
                    KDATA* pdata = nullptr;
                    switch (period_)
                    {
                    case CYC_TICK:
                        {
                            switch(id)
                            {
                                case IDF_DATE:
                                {
                                    return &commodity_->Date;
                                }
                                break;
                                case IDF_TIME:
                                {
                                return &commodity_->ticks_[offset].Time;
                                }
                                break;
                                case IDF_OPEN:
                                {
                                    return &commodity_->ticks_[offset].Price;
                                }
                                break;
                                case IDF_HIGH:
                                {
                                    return &commodity_->ticks_[offset].Price;
                                }
                                break;
                                case IDF_LOW:
                                {
                                    return &commodity_->ticks_[offset].Price;
                                }
                                break;
                                case IDF_CLOSE:
                                {
                                    return &commodity_->ticks_[offset].Price;
                                }
                                break;
                                case IDF_VOLUME:
                                {
                                    return &commodity_->ticks_[offset].Volume;
                                }
                                break;
                                case IDF_AMOUNT:
                                {
		                            static thread_local double _Amount = 0.;
                                    if(commodity_->ticks_[offset].Volume == 0) {
                                        _Amount = 0;
                                    } else {
                                        _Amount = commodity_->ticks_[offset].Volume * commodity_->ticks_[offset].Price;
                                    }
                                    return &_Amount;
                                }
                                break;
                                case IDF_BARGAIN:
                                {
                                    static thread_local uint32_t _Bargain = 1;
                                    return &_Bargain;
                                }
                                break;
                                case IDF_INTEREST:
                                {
                                    return &commodity_->ticks_[offset].OpenInterest;
                                }
                                break;
                                default:
                                {
                                    return 0;
                                }
                                break;
                            }
                        }
                        break;
                    case CYC_5SEC:
                        pdata = &commodity_->rt_kdatas_[period_][0];
                        break;
                    case CYC_ANYSEC:
                        {
                            pdata = &kdatas_[0];
                        }
                        break;
                    case CYC_1MIN:
                        pdata = &commodity_->rt_kdatas_[period_][0];
                        break;
                    case CYC_5MIN:
                    case CYC_15MIN:
                    case CYC_30MIN:
                    case CYC_60MIN:
                    case CYC_ANYMIN:
                        {
                            pdata = &kdatas_[0];
                        }
                        break;
                    case CYC_DAY:
                        {
                            switch(id)
                            {
                                case IDF_DATE:
                                {
                                    return &commodity_->Date;
                                }
                                break;
                                case IDF_TIME:
                                {
                                return &commodity_->Time;
                                }
                                break;
                                case IDF_OPEN:
                                {
                                    return &commodity_->Open;
                                }
                                break;
                                case IDF_HIGH:
                                {
                                    return &commodity_->High;
                                }
                                break;
                                case IDF_LOW:
                                {
                                    return &commodity_->Low;
                                }
                                break;
                                case IDF_CLOSE:
                                {
                                    return &commodity_->Price;
                                }
                                break;
                                case IDF_VOLUME:
                                {
                                    return &commodity_->Volume;
                                }
                                break;
                                case IDF_AMOUNT:
                                {
                                    return &commodity_->Amount;
                                }
                                break;
                                case IDF_BARGAIN:
                                {
                                    return &commodity_->Bargain;
                                }
                                break;
                                case IDF_INTEREST:
                                {
                                    return &commodity_->OpenInterest;
                                }
                                break;
                                default:
                                {
                                    return 0;
                                }
                                break;
                            }
                        }
                        break;
                    case CYC_WEEK:
                    case CYC_MONTH:
                    case CYC_SEASON:
                    case CYC_YEAR:
                    case CYC_ANYDAY:
                        break;
                    default:
                        break;
                    }
                    if(pdata) {
                        switch(id)
                        {
                            case IDF_DATE:
                            {
                                return &pdata[offset].Date;
                            }
                            break;
                            case IDF_TIME:
                            {
                                return &pdata[offset].Time;
                            }
                            break;
                            case IDF_OPEN:
                            {
                                return &pdata[offset].Open;
                            }
                            break;
                            case IDF_HIGH:
                            {
                                return &pdata[offset].High;
                            }
                            break;
                            case IDF_LOW:
                            {
                                return &pdata[offset].Low;
                            }
                            break;
                            case IDF_CLOSE:
                            {
                                return &pdata[offset].Close;
                            }
                            break;
                            case IDF_VOLUME:
                            {
                                return &pdata[offset].Volume;
                            }
                            break;
                            case IDF_AMOUNT:
                            {
                                return &pdata[offset].Amount;
                            }
                            break;
                            case IDF_BARGAIN:
                            {
                                return &pdata[offset].Bargain;
                            }
                            break;
                            case IDF_INTEREST:
                            {
                                return &pdata[offset].OpenInterest;
                            }
                            break;
                            default:
                            {
                                return 0;
                            }
                            break;
                        }
                    }
                    return 0;
                }
                break;
            }
            return Base::GetFieldValue(id, offset);
        }
        // bool GetFieldValueAs(size_t type, void* value, size_t id, size_t offset = 0) 
        // { 
        //     if(Base::GetFieldValueAs(type, value, id, offset)) {
        //         return true;
        //     }
        //     switch(id)
        //     {
        //         case IDF_AMOUNT:
        //         case IDF_BARGAIN:
        //         {
        //             switch (period_)
        //             {
        //             case CYC_TICK:
        //                 {
        //                     switch(id)
        //                     {
        //                         case IDF_AMOUNT:
        //                         {
        //                             double raw_value = commodity_->ticks_[offset].Price*commodity_->ticks_[offset].Volume;
        //                             Convert(FIELD_TYPE_DOUBLE, &raw_value, type, value);
        //                             return true;
        //                         }
        //                         break;
        //                         case IDF_BARGAIN:
        //                         {
        //                             uint32_t raw_value = 1;
        //                             Convert(FIELD_TYPE_INT32, &raw_value, type, value);
        //                             return true;
        //                         }
        //                         break;
        //                         default:
        //                         break;
        //                     }
        //                 }
        //                 break;
        //             default:
        //                 break;
        //             }
        //         }
        //         break;
        //     }
        //     return false; 
        // }

        //更新计算数据
        void Update()
        {
            update_flag_ = true;
        }
    };

   static std::string MDDataSetConv(size_t type, void* val)
   {
       if(!val) {
           return "";
       }
       std::stringstream ss;
       switch(type)
       {
           case FIELD_TYPE_EXCHANGE:
           {
               ExchangeInfo* exchange = (ExchangeInfo*)val;
               ExchangeSetWrapper<IDataSet,ExchangeInfo*,ProductInfo*,CommodityInfo*> exchange_set(exchange);
               DataSetConv conv;
               return conv.to_json(&exchange_set);
           }
           break;
           case FIELD_TYPE_PRODUCT:
           {
               ProductInfo* product = (ProductInfo*)val;
               ProductSetWrapper<IDataSet,ProductInfo*,CommodityInfo*> product_set(product);
               DataSetConv conv;
               return conv.to_json(&product_set);
           }
           break;
           case FIELD_TYPE_COMMODITY:
           {
               CommodityInfo* commodity = (CommodityInfo*)val;
               CommoditySetWrapper<IDataSet,CommodityInfo*> commodity_set(commodity);
               DataSetConv conv;
               return conv.to_json(&commodity_set);
           }
           break;
       }
       return ss.str();
   }

class MDSET_API MarketDataSetEx : public IMarketDataSet
{
public:
    static MarketDataSetEx& Instance();

    static void AddExchange(const char* prefix, const ExchangeInfo* exchange, size_t exchange_count
        , const ProductInfo* product, size_t product_count
        , const CommodityInfo* commodity, size_t commodity_count);
    static void AddExchange(const char* prefix, const ExchangeInfoPtr* exchange, size_t exchange_count
        , const ProductInfoPtr* product, size_t product_count
        , const CommodityInfoPtr* commodity, size_t commodity_count);
    static void RemoveExchange(const ExchangeInfo* exchange, size_t exchange_count);
    static void RemoveExchange(const ExchangeInfoPtr* exchange, size_t exchange_count);
    static void RemoveExchange(const char** exchange, size_t exchange_count);

    static void UpdateMarket(CommodityInfo* commodity, size_t commodity_count = 1);
    static void UpdateStatus(CommodityInfo* commodity, size_t commodity_count = 1);
};

class MDSET_API ExchangeSetEx : public IExchangeSet
{
public:
    virtual ExchangeInfo& get() = 0;
};

class MDSET_API ProductSetEx : public IProductSet
{
public:
    virtual ProductInfo& get() = 0;
};

class MDSET_API CommoditySetEx : public ICommoditySet
{
public:
    virtual CommodityInfo& get() = 0;
};

}

#endif//_H_MDDATASETEX_HPP_
