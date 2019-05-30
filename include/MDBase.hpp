#pragma once

#include "DataDefEx.h"
#include "CommonEx.hpp"
#include <XUtil/XUtil.hpp>
#include <XUtil/XStr.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/binary_object.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

namespace MTP {

    typedef std::shared_ptr<ExchangeInfo> ExchangeInfoPtr;
	template<class ExchangePtr = ExchangeInfoPtr>
    struct ExchangePtrLess
	{
		bool operator()(const ExchangePtr& x, const ExchangePtr& y) const
		{
			//const char* x_exchange = x->Exchange;
			//const char* y_exchange = y->Exchange;
			return std::strcmp(x->Exchange, y->Exchange) < 0;
		}
	};
	typedef ExchangePtrLess<> ExchangeInfoPtrLess;

    typedef std::shared_ptr<ProductInfo> ProductInfoPtr;
	template<class ProductPtr = ProductInfoPtr>
    struct ProductPtrLess
	{
		bool operator()(const ProductPtr& x, const ProductPtr& y) const
		{
			auto exchange_cmp = std::strcmp(x->Exchange, y->Exchange);
			if (exchange_cmp < 0) {
				return true;
			}
			else if (exchange_cmp == 0) {
				auto product_cmp = std::strcmp(x->Product, y->Product);
				if (product_cmp < 0) {
					return true;
				}
			}
			return false;
		}
	};
	typedef ProductPtrLess<> ProductInfoPtrLess;

    typedef std::shared_ptr<CommodityInfo> CommodityInfoPtr;
	template<class CommodityPtr = CommodityInfoPtr>
    struct CommodityPtrLess
	{
		bool operator()(const CommodityPtr& x, const CommodityPtr& y) const
		{
			int cmp = std::strcmp(x->Exchange, y->Exchange);
			if (cmp < 0) {
				return true;
			}
			else if (cmp == 0) {
				cmp = std::strcmp(x->Product, y->Product);
				if (cmp < 0) {
					return true;
				} else if (cmp == 0) {
					int cmp = std::strcmp(x->Code, y->Code);
					if (cmp < 0) {
						return true;
					}
				}
			}
			return false;
		}
	};
	typedef CommodityPtrLess<> CommodityInfoPtrLess;
	template<class CommodityPtr = CommodityInfoPtr>
	struct CommodityPtrCodeLess
	{
		bool operator()(const CommodityPtr& x, const CommodityPtr& y) const
		{
			int cmp = std::strcmp(x->Code, y->Code);
			if (cmp < 0) {
				return true;
			}
			return false;
		}
	};
	typedef CommodityPtrCodeLess<> CommodityInfoPtrCodeLess;

	template<class T, class Commodity, class Tick, class Tick_vector, class KData, class KData_vector, class Weight, class Weight_vector>
	class CommodityWrapper : public Commodity
	{
	public:
		//using Commodity::Commodity;

		CommodityWrapper()
		{
			memset(this, 0, sizeof(Commodity));
		}

		CommodityWrapper(const Commodity& o)
		{
			memcpy(this, &o, sizeof(o));
		}

		template<class Ty>
		size_t UpdateMarket(const Ty& data)
		{
			T* pT = static_cast<T*>(this);
			if (this->Date == 0)
			{//第一次初始化
				ASSERT(0);
				return 0;
			}
			//这里要比较日期、时间、价格、量、额、其他等变化
			size_t chg = Commodity::UpdateMarket(data);
			if (chg & MARKET_COMMODITY_TICK_CHANGED) {
				pT->AddTickData();
			}
			if (chg & (MARKET_DATE_CHANGED | MARKET_TIME_CHANGED | MARKET_COMMODITY_TREND_CHANGED)) {
				pT->AddRealData();
			}
			return chg;
		}

		uint16_t GetMaxTimePoint(uint16_t pPeriods[][2], uint8_t* pPeriodsCount)
		{
			int i, j;
			uint16_t(*pRealPeriods)[2] = { 0 };
			uint8_t nRealPeriodsCount = 0;
			if (this->PeriodsCount)
			{
				pRealPeriods = this->Periods;
				nRealPeriodsCount = this->PeriodsCount;
			}
			if (pPeriods && pPeriodsCount) {
				for (i = 0; i < nRealPeriodsCount && i<*pPeriodsCount; i++)
				{
					pPeriods[i][0] = pRealPeriods[i][0];
					pPeriods[i][1] = pRealPeriods[i][1];
				}
				*pPeriodsCount = nRealPeriodsCount;
			}
			return GetTimePointCount(pRealPeriods, nRealPeriodsCount);
		}

		uint16_t GetTimePointByHHMMSS(uint32_t HHMMSS)
		{
			uint8_t	PeriodsCount = 4; // 交易时段数量
			uint16_t Periods[4][2] = { 0 }; // 交易时段描述[最多只是4个交易时段，每个时段包括开始、结束时间，精确到分，HH * 60 + MM格式]
			uint16_t MaxTimePoint = GetMaxTimePoint(Periods, &PeriodsCount);
			return MTP::GetTimePointByHHMMSS(HHMMSS, Periods, PeriodsCount);
		}

		uint32_t GetHHMMSSByTimePoint(uint16_t TimePoint)
		{
			uint8_t	PeriodsCount = 4; // 交易时段数量
			uint16_t Periods[4][2] = { 0 }; // 交易时段描述[最多只是4个交易时段，每个时段包括开始、结束时间，精确到分，HH * 60 + MM格式]
			uint16_t MaxTimePoint = GetMaxTimePoint(Periods, &PeriodsCount);
			return MTP::GetHHMMSSByTimePoint(TimePoint, MaxTimePoint, Periods, PeriodsCount);
		}

		uint16_t GetMaxTimePoint(PERIODTYPE Period)
		{
			uint8_t	PeriodsCount = 4; // 交易时段数量
			uint16_t Periods[4][2] = { 0 }; // 交易时段描述[最多只是4个交易时段，每个时段包括开始、结束时间，精确到分，HH * 60 + MM格式]
			uint16_t MaxTimePoint = GetMaxTimePoint(Periods, &PeriodsCount);
			switch (Period)
			{
			case CYC_5SEC:
				MaxTimePoint = MaxTimePoint * 12;
				break;
			case CYC_1MIN:
				break;
			case CYC_5MIN:
				MaxTimePoint = _countxofy(5, MaxTimePoint);
				break;
			case CYC_DAY:
				MaxTimePoint = 1;
				break;
			default:
				ASSERT(0);
				break;
			}
			return MaxTimePoint;
		}

		uint16_t GetTimePointByHHMMSS(uint32_t HHMMSS, PERIODTYPE Period)
		{
			uint16_t TimePoint = GetTimePointByHHMMSS(HHMMSS);
			switch (Period)
			{
			case CYC_5SEC:
				TimePoint = TimePoint * 12 + HHMMSS % 100 / 5;
				break;
			case CYC_1MIN:
				break;
			case CYC_5MIN:
				TimePoint = _countxofy(5, TimePoint);
				break;
			case CYC_DAY:
				TimePoint = 0;
				break;
			default:
				ASSERT(0);
				break;
			}
			return TimePoint;
		}

		uint32_t GetHHMMSSByTimePoint(uint16_t TimePoint, PERIODTYPE Period)
		{
			uint32_t HHMMSS = 0;
			switch (Period)
			{
			case CYC_5SEC:
			{
				uint16_t mTimePoint = TimePoint / 12;
				/*if (mTimePoint > 0) {
				mTimePoint -= 1; //前移一个点
				}*/
				HHMMSS = GetHHMMSSByTimePoint(mTimePoint) + TimePoint % 12 * 5;
			}
			break;
			case CYC_1MIN:
			{
				HHMMSS = GetHHMMSSByTimePoint(TimePoint);
			}
			break;
			case CYC_5MIN:
			{
				HHMMSS = GetHHMMSSByTimePoint(TimePoint * 5);
			}
			break;
			case CYC_DAY:
			{
				HHMMSS = 0;
			}
			break;
			default:
				ASSERT(0);
				break;
			}
			return HHMMSS;
		}
		
		void AddTickData()
		{
			T* pT = static_cast<T*>(this);
			Tick tick = { 0 };
			//tick.Serial = ticks.size();
			tick.Time = this->Time;
			tick.Price = this->Price;
			tick.Volume = this->LastVolume;
			tick.OpenInterest = this->OpenInterest;
			
			if(pT->ticks_.size()>=pT->ticks_.capacity()) {
				//std::copy(pT->ticks_.begin()+1,pT->ticks_.end(),pT->ticks_.begin());
				memmove(&pT->ticks_[0], &pT->ticks_[1], sizeof(Tick)*(pT->ticks_.size()-1));
				pT->ticks_.back() = tick;
			} else {
				pT->ticks_.emplace_back(tick);
			}
		}

		//是否生成MaxTimePoint数据
		//inline bool IsMaxTimePointData() { return true; }

		void AddRealData()
		{
			T* pT = static_cast<T*>(this);
			for (size_t i = CYC_5SEC; i < CYC_MAX; i++)
			{
				if(!(i == CYC_5SEC || i == CYC_1MIN)) {
					continue;
				}
				auto datas = &pT->rt_kdatas_[i];
				uint16_t MaxTimePoint = GetMaxTimePoint((PERIODTYPE)i);
				uint16_t TimePoint = GetTimePointByHHMMSS(this->Time, (PERIODTYPE)i);
				if (TimePoint < 0 || TimePoint >= MaxTimePoint) {
					ASSERT(0);
					break;
				}

				int OldTimePoint = datas->empty() ? -1 :
					GetTimePointByHHMMSS(datas->back().Time, (PERIODTYPE)i);
				//if(!pT->IsMaxTimePointData()) {
					int AddCount = TimePoint - OldTimePoint;//AddCount == 0 时更新当前时间点数据，AddCount > 0 跟新新数据
					if (AddCount >= 0) {
						if (AddCount > 0) {
							datas->resize(datas->size() + 1); //扩展到最新Time大小
						}
						KData& data = datas->back();
						UpdateKData(data, (PERIODTYPE)i);
					}
				//} else {
				//	datas->resize(TimePoint + 1); //扩展到最新Time大小
				//	int AddCount = TimePoint - OldTimePoint;//AddCount == 0 时更新当前时间点数据，AddCount > 0 跟新新数据
				//	if (AddCount >= 0) {
				//		CheckKData((PERIODTYPE)i, &(*datas)[0], OldTimePoint + 1, AddCount);
				//		KData& data = datas->back();
				//		UpdateKData(data, (PERIODTYPE)i);
				//	}
				//}
			}
		}

		inline void CheckKData(PERIODTYPE Period, KData *pData, int Start, int Count)
		{
			T* pT = static_cast<T*>(this);
			for (int i = Start; i < Start + Count; i++)
			{
				if (pData[i].Date == 0) {
					if (i == 0) {
						pData[i].Date = pT->Date;
					}
					else {
						pData[i].Date = pData[i - 1].Date;
					}
				}
				if (pData[i].Time == 0) {
					pData[i].Time = GetHHMMSSByTimePoint(i, Period);
				}

				// 校验价格
				if (IsZeroFloat(pData[i].Close) || IsZeroFloat(pData[i].High) || IsZeroFloat(pData[i].Low) || IsZeroFloat(pData[i].Open)
					|| IsOverFloat(pData[i].Close) || IsOverFloat(pData[i].High) || IsOverFloat(pData[i].Low) || IsOverFloat(pData[i].Open)) {
					if (i == 0) {
						pData[i].Open = pT->PreClosePrice;
						pData[i].High = pData[i].Open;
						pData[i].Low = pData[i].Open;
						pData[i].Close = pData[i].Open;
					}
					else {
						pData[i].Open = pData[i - 1].Close;
						pData[i].High = pData[i].Open;
						pData[i].Low = pData[i].Open;
						pData[i].Close = pData[i].Open;
					}
				}
				ASSERT(!IsZeroFloat(pData[i].Open) && !IsZeroFloat(pData[i].High) && !IsZeroFloat(pData[i].Low) && !IsZeroFloat(pData[i].Close));
			}
		}

		void UpdateKData(KData& data, PERIODTYPE Period)
		{
			T* pT = static_cast<T*>(this);
			data.Date = this->Date;
			data.Time = this->Time;

			double LastPrice = this->Price;
			if (IsZeroFloat(this->Price) || IsOverFloat(this->Price)) {
				LastPrice = this->PreClosePrice;
			}

			if (IsZeroFloat(data.Open) || IsOverFloat(data.Open)) {
				data.Open = LastPrice;
				data.High = LastPrice;
				data.Low = LastPrice;
				data.Close = LastPrice;
			}
			if (data.High < LastPrice) {
				data.High = LastPrice;
			}
			if (data.Low > LastPrice) {
				data.Low = LastPrice;
			}
			data.Close = LastPrice;

			data.Amount += this->LastAmount;
			data.Volume += this->LastVolume;

			data.OpenInterest = this->OpenInterest;
		}

		bool LoadKData(KData_vector& datas, PERIODTYPE Period, uint32_t Date, const boost::filesystem::path& data_path)
		{
			T* pT = static_cast<T*>(this);
			bool ok = false;
			std::string file;
			std::stringstream ss;
			ss << data_path.generic_string() << "/" << this->Exchange;
			if(Period >= CYC_DAY) {
				ss << "/" << this->Code << ".day";
			} else {
				ss << "/" << Date << "/" << this->Code;
				switch (Period)
				{
				case CYC_5SEC:
					ss << ".sec5";
					break;
				case CYC_1MIN:
					ss << ".min1";
					break;
				case CYC_5MIN:
					ss << ".min5";
					break;
				default:
					return false;
					break;
				}
			}
			file = ss.str();
			if (!boost::filesystem::exists(file))
			{
				return false;
			}
			try
			{
				switch (Period)
				{
				case CYC_TICK:
				{
					//
				}
				break;
				default:
				{
					std::ifstream fs(file, std::ofstream::binary);
					boost::archive::binary_iarchive ar(fs);
					ar >> *datas;
					ok = true;
				}
				break;
				}
			}
			catch (...)
			{

			}
			if (!ok) {
				boost::filesystem::remove(file);
				datas->clear();
			}
			return ok;
		}

		bool SaveKData(KData_vector& datas, PERIODTYPE Period, uint32_t Date, const boost::filesystem::path& data_path)
		{
			T* pT = static_cast<T*>(this);
			bool ok = false;
			std::string file;
			std::stringstream ss;
			ss << data_path.generic_string() << "/" << this->Exchange;
			if(Period >= CYC_DAY) {
				ss << "/" << this->Code << ".day";
			} else {
				ss << "/" << Date << "/" << this->Code;
				switch (Period)
				{
				case CYC_5SEC:
					ss << ".sec5";
					break;
				case CYC_1MIN:
					ss << ".min1";
					break;
				case CYC_5MIN:
					ss << ".min5";
					break;
				default:
					return false;
					break;
				}
			}
			file = ss.str();
			try
			{
				switch (Period)
				{
				case CYC_TICK:
				{
					//
				}
				break;
				default:
				{
					std::ofstream fs(file, std::ofstream::trunc | std::ofstream::binary);
					boost::archive::binary_oarchive ar(fs);
					ar << datas;
					ok = true;
				}
				break;
				}
			}
			catch (...)
			{

			}
			if (!ok) {
				boost::filesystem::remove(file);
			}
			return ok;
		}
	};

	template<class T, class ExchangePtr, class ProductPtr, class CommodityPtr>
	class MarketDataWrapper
	{
	public:
		inline void AddExchange(ExchangePtr exchange) 
		{
			T* pT = static_cast<T*>(this);
			pT->exchanges_.emplace_back(exchange);
		}
		
		inline void AddProduct(ProductPtr product) 
		{
			T* pT = static_cast<T*>(this);
			size_t exchange_index = InnerExchange2ExchangeInfo(product->Exchange, nullptr);
			if (exchange_index >= pT->exchanges_.size()) {
				return;
			}
			pT->products_.emplace_back(product);
		}
		
		inline void AddCommodity(CommodityPtr commodity) 
		{
			T* pT = static_cast<T*>(this);
			size_t product_index = InnerProduct2ProductInfo(commodity->Exchange, commodity->Product, nullptr);
			if (product_index >= pT->products_.size()) {
				return;
			}
			pT->commoditys_.emplace_back(commodity);
		}

		void UpdateExchangeInfo()
		{
			T* pT = static_cast<T*>(this);
			size_t i, j;
			for (i = 0, j = pT->exchanges_.size(); i < j; i++)
			{
				UpdateExchangeInfo(i);
			}
			std::sort(pT->exchanges_.begin(), pT->exchanges_.end(), ExchangePtrLess<ExchangePtr>());
		}

		void UpdateProductInfo()
		{
			T* pT = static_cast<T*>(this);
			size_t i, j;
			for (i = 0, j = pT->products_.size(); i < j; i++)
			{
				UpdateProductInfo(i);
			}
			std::sort(pT->products_.begin(), pT->products_.end(), ProductPtrLess<ProductPtr>());
		}
		void UpdateCommodityInfo()
		{
			T* pT = static_cast<T*>(this);
			size_t i, j;
			for (i = 0, j = pT->commoditys_.size(); i < j; i++)
			{
				UpdateCommodityInfo(i);
			}
			std::sort(pT->commoditys_.begin(), pT->commoditys_.end(), CommodityPtrLess<CommodityPtr>());
		}

		void UpdateExchangeInfo(size_t exchange_index)
		{
			T* pT = static_cast<T*>(this);
			ASSERT (exchange_index < pT->exchanges_.size());
			auto& exchange = pT->exchanges_[exchange_index];
			if(!exchange->inited_) {
				exchange->inited_ = true;
				exchange->init();
			}
		}

		void UpdateProductInfo(size_t product_index)
		{
			T* pT = static_cast<T*>(this);
			ASSERT (product_index < pT->products_.size());
			auto& product = pT->products_[product_index];
			if (!product->exchange.lock()) {
				//需要重新更新
				size_t exchange_index = InnerExchange2ExchangeInfo(product->Exchange, nullptr);
				ASSERT (exchange_index < pT->exchanges_.size());
				auto& exchange = pT->exchanges_[exchange_index];
				product->exchange = exchange;
				//product->exchange_index = exchange_index;
				exchange->ProductCount++;
				if (product->PeriodsCount) {
					UpdateExchangePeriod(exchange, product->Periods, product->PeriodsCount);
				} else {
					product->PeriodsCount = exchange->PeriodsCount;
					memcpy(product->Periods, exchange->Periods, sizeof(exchange->Periods));
				}
				product->init();
			}
		}

		void UpdateCommodityInfo(size_t commodity_index)
		{
			T* pT = static_cast<T*>(this);
			ASSERT (commodity_index < pT->commoditys_.size());
			auto& commodity = pT->commoditys_[commodity_index];
			if (!commodity->product.lock()) {
				//需要重新更新
				size_t product_index = InnerProduct2ProductInfo(commodity->Exchange, commodity->Product, nullptr);
				ASSERT(product_index < pT->products_.size());
				auto& product = pT->products_[product_index];
				auto& exchange = product->GetExchangePtr();
				//size_t exchange_index = product->exchange_index;
				//ASSERT (exchange_index < pT->exchanges_.size());
				//auto& exchange = pT->exchanges_[exchange_index];
				commodity->product = product;
				//commodity->product_index = product_index;
				exchange->CommodityCount++;
				product->CommodityCount++;
				if (commodity->PeriodsCount) {
					UpdateProductPeriod(product, commodity->Periods, commodity->PeriodsCount);
				} else {
					commodity->PeriodsCount = product->PeriodsCount;
					memcpy(&commodity->Periods[0][0], &product->Periods[0][0], sizeof(product->Periods));
				}
				commodity->init();
			}
		}

		void UpdateExchangePeriod(ExchangePtr exchange, uint16_t Periods[][2], uint8_t PeriodsCount)
		{
			T* pT = static_cast<T*>(this);
			ASSERT(exchange);
			for (size_t i = 0; i < PeriodsCount; i++)
			{
				bool add = true;
				for (size_t j = 0; j < exchange->PeriodsCount; j++)
				{
					add = !MergePeriod(exchange->Periods[j], Periods[i]);
					if (!add)
					{
						break;
					}
				}
				if (add) {
					exchange->Periods[exchange->PeriodsCount][0] = Periods[i][0];
					exchange->Periods[exchange->PeriodsCount][1] = Periods[i][1];
					exchange->PeriodsCount += 1;
				}
			}
		}

		void UpdateProductPeriod(ProductPtr product, uint16_t Periods[][2], uint8_t PeriodsCount)
		{
			T* pT = static_cast<T*>(this);
			ASSERT(product);
			for (size_t i = 0; i < PeriodsCount; i++)
			{
				bool add = true;
				for (size_t j = 0; j < product->PeriodsCount; j++)
				{
					add = !MergePeriod(product->Periods[j], Periods[i]);
					if (!add)
					{
						break;
					}
				}
				if (add) {
					product->Periods[product->PeriodsCount][0] = Periods[i][0];
					product->Periods[product->PeriodsCount][1] = Periods[i][1];
					product->PeriodsCount += 1;
				}
			}
			auto& exchange = product->GetExchangePtr();
			//size_t exchange_index = product->exchange_index;
			//ASSERT (exchange_index < pT->exchanges_.size());
			//auto& exchange = pT->exchanges_[exchange_index];
			UpdateExchangePeriod(exchange, product->Periods, product->PeriodsCount);
		}

		void RemoveExchange(size_t index)
		{
			T* pT = static_cast<T*>(this);
			if (index >= pT->exchanges_.size()) {
				return;
			}
			{
				auto pr = InnerExchange2CommodityRange(pT->exchanges_[index]->Exchange);
				pT->commoditys_.erase(pr.first, pr.second);
			}
			{
				auto pr = InnerExchange2ProductRange(pT->exchanges_[index]->Exchange);
				pT->products_.erase(pr.first, pr.second);
			}
			pT->exchanges_.erase(pT->exchanges_.begin() + index);
			/*for (size_t i = pT->products_.size(); i > 0; i--)
			{
				if (pT->products_[i - 1]->exchange.lock() == exchange_ptr) {
					RemoveProduct(i - 1);
				}
			}*/
		}

		void RemoveAll()
		{
			T* pT = static_cast<T*>(this);
			pT->commoditys_.clear();
			pT->products_.clear();
			pT->exchanges_.clear();
		}

		// void RemoveProduct(size_t index)
		// {
		// 	T* pT = static_cast<T*>(this);
		// 	if (index >= pT->products_.size()) {
		// 		return;
		// 	}
		// 	auto pr = InnerProduct2CommodityRange(pT->products_[index]->Exchange, pT->products_[index]->Product);
		// 	pT->commoditys_.erase(pr.first, pr.second);
		// 	pT->products_.erase(pT->products_.begin() + index);
		// 	/*for (size_t i = pT->commoditys_.size(); i > 0; i--)
		// 	{
		// 		if (pT->commoditys_[i - 1]->GetProductPtr() == product_ptr) {
		// 			RemoveCommodity(i - 1);
		// 		}
		// 	}*/
		// }

		// void RemoveCommodity(size_t index)
		// {
		// 	T* pT = static_cast<T*>(this);
		// 	if (index >= pT->products_.size()) {
		// 		return;
		// 	}
		// 	pT->commoditys_.erase(pT->commoditys_.begin() + index);
		// }
		
		void CloseExchange(size_t index)
		{
			T* pT = static_cast<T*>(this);
			if (index >= pT->exchanges_.size()) {
				return;
			}
			{
				auto pr = InnerExchange2CommodityRange(pT->exchanges_[index]->Exchange);
				for(auto it = pr.first; it != pr.second; ++it)
				{
					(*it)->close();
				}
			}
			{
				auto pr = InnerExchange2ProductRange(pT->exchanges_[index]->Exchange);
				for(auto it = pr.first; it != pr.second; ++it)
				{
					(*it)->close();
				}
			}
		}

		size_t InnerExchange2ExchangeInfo(const char* exchange, ExchangePtr* pExchange)
		{
			T* pT = static_cast<T*>(this);
			struct ExchangePtrPred
			{
				const char* exchange_;
				ExchangePtrPred(const char* exchange) :exchange_(exchange)
				{}

				bool operator()(const ExchangePtr& x, const ExchangePtr& key) const
				{
					auto cmp = std::strcmp(x->Exchange, exchange_);
					if(cmp < 0) {
						return true;
					}
					return false;
				}
			};
			ExchangePtr key;
			//lower_bound查找小于/低于keyvalue的end，即返回大于等于的it
			auto it = std::lower_bound(pT->exchanges_.begin(), pT->exchanges_.end(), key, ExchangePtrPred(exchange));
			if (it != pT->exchanges_.end()) {
				auto x = *it;
				if(std::strcmp(x->Exchange, exchange) == 0) {
					if (pExchange) {
						*pExchange = *it;
					}
					return (it - pT->exchanges_.begin());
				}
			}
			return size_t(-1);
		}

		size_t InnerProduct2ProductInfo(const char* exchange, const char* product, ProductPtr* pProduct)
		{
			T* pT = static_cast<T*>(this);
			struct ProductPtrPred
			{
				const char* exchange_;
				const char* product_;
				ProductPtrPred(const char* exchange, const char* product) :exchange_(exchange), product_(product)
				{}

				bool operator()(const ProductPtr& x, const ProductPtr& key) const
				{
					auto cmp = std::strcmp(x->Exchange, exchange_);
					if (cmp < 0) {
						return true;
					}
					else if (cmp == 0) {
						cmp = std::strcmp(x->Product, product_);
						if (cmp < 0) {
							return true;
						}
					}
					return false;
				}
			};
			ProductPtr key;
			//lower_bound查找小于/低于keyvalue的end，即返回大于等于的it
			auto it = std::lower_bound(pT->products_.begin(), pT->products_.end(), key, ProductPtrPred(exchange, product));
			if (it != pT->products_.end()) {
				auto x = *it;
				if(std::strcmp(x->Exchange, exchange) == 0 && std::strcmp(x->Product, product) == 0) {
					if (pProduct) {
						*pProduct = *it;
					}
					return (it - pT->products_.begin());
				}
			}
			return size_t(-1);
		}

		size_t InnerCommodity2CommodityInfo(const char* exchange, const char* product, const char* code, CommodityPtr* pCommodity)
		{
			T* pT = static_cast<T*>(this);
			struct CommodityPtrPred
			{
				const char* exchange_;
				const char* product_;
				const char* code_;
				CommodityPtrPred(const char* exchange, const char* product, const char* code) :exchange_(exchange), product_(product), code_(code)
				{}

				bool operator()(const CommodityPtr& x, const CommodityPtr& key) const
				{
					//const char* x_exchange = x->Exchange;
					//const char* x_product = x->Product;
					//const char* x_code = x->Code;
					int cmp = std::strcmp(x->Exchange, exchange_);
					if (cmp < 0) {
						return true;
					}
					else if (cmp == 0) {
						cmp = std::strcmp(x->Product, product_);
						if (cmp < 0) {
							return true;
						}
						else if (cmp == 0) {
							int cmp = std::strcmp(x->Code, code_);
							if (cmp < 0) {
								return true;
							}
						}
					}
					return false;
				}
			};
			CommodityPtr key;
			//lower_bound查找小于/低于keyvalue的end，即返回大于等于的it
			auto it = std::lower_bound(pT->commoditys_.begin(), pT->commoditys_.end(), key, CommodityPtrPred(exchange, product, code));
			if (it != pT->commoditys_.end()) {
				auto x = *it;
				if(std::strcmp(x->Exchange, exchange) == 0 && std::strcmp(x->Product, product) == 0 && std::strcmp(x->Code, code) == 0) {
					if (pCommodity) {
						*pCommodity = *it;
					}
					return (it - pT->commoditys_.begin());
				}
			}
			return size_t(-1);
		}

		auto InnerExchange2ProductRange(const char* exchange)
		{
			T* pT = static_cast<T*>(this);
			struct ProductPtrPred
			{
				const char* exchange_;
				ProductPtrPred(const char* exchange) :exchange_(exchange)
				{}

				bool operator()(const ProductPtr& x, const ProductPtr& y) const
				{
					if (x) {
						return std::strcmp(x->Exchange, exchange_) < 0;
					}
					else {
						return std::strcmp(exchange_, y->Exchange) < 0;
					}
					return false;
				}
			};
			ProductPtr key;
			return std::equal_range(pT->products_.begin(), pT->products_.end(), key, ProductPtrPred(exchange));
		}

		size_t InnerExchange2ProductList(const char* exchange, std::vector<ProductPtr>& product_list)
		{
			auto pr = InnerExchange2ProductRange(exchange);
			product_list.assign(pr.first, pr.second);
			return (pr.second - pr.first);
			return 0;
		}

		auto InnerExchange2CommodityRange(const char* exchange)
		{
			T* pT = static_cast<T*>(this);
			struct CommodityPtrPred
			{
				const char* exchange_;
				CommodityPtrPred(const char* exchange) :exchange_(exchange)
				{}

				bool operator()(const CommodityPtr& x, const CommodityPtr& y) const
				{
					if (x) {
						return std::strcmp(x->Exchange, exchange_) < 0;
					}
					else {
						return std::strcmp(exchange_, y->Exchange) < 0;
					}
					return false;
				}
			};
			CommodityPtr key;
			return std::equal_range(pT->commoditys_.begin(), pT->commoditys_.end(), key, CommodityPtrPred(exchange));
		}
		size_t InnerExchange2CommodityList(const char* exchange, std::vector<CommodityPtr>& commodity_list)
		{
			auto pr = InnerExchange2CommodityRange(exchange);
			commodity_list.assign(pr.first, pr.second);
			return (pr.second - pr.first);
			return 0;
		}

		auto InnerProduct2CommodityRange(const char* exchange, const char* product)
		{
			T* pT = static_cast<T*>(this);
			struct CommodityPtrPred
			{
				const char* exchange_;
				const char* product_;
				CommodityPtrPred(const char* exchange, const char* product) :exchange_(exchange), product_(product)
				{}

				bool operator()(const CommodityPtr& x, const CommodityPtr& y) const
				{
					if (x) {
						int cmp = std::strcmp(x->Exchange, exchange_);
						if (cmp < 0) {
							return true;
						}
						else if (cmp == 0) {
							cmp = std::strcmp(x->Product, product_);
							if (cmp < 0) {
								return true;
							}
						}
					}
					else {
						int cmp = std::strcmp(exchange_, y->Exchange);
						if (cmp < 0) {
							return true;
						}
						else if (cmp == 0) {
							cmp = std::strcmp(product_, y->Product);
							if (cmp < 0) {
								return true;
							}
						}
					}
					return false;
				}
			};
			CommodityPtr key;
			return std::equal_range(pT->commoditys_.begin(), pT->commoditys_.end(), key, CommodityPtrPred(exchange, product));
		}
		size_t InnerProduct2CommodityList(const char* exchange, const char* product, std::vector<CommodityPtr>& commodity_list)
		{
			auto pr = InnerProduct2CommodityRange(exchange,product);
			commodity_list.assign(pr.first, pr.second);
			return (pr.second - pr.first);
			return 0;
		}
	};
}


