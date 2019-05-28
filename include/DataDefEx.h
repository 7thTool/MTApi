#pragma once
#ifndef _H_DATADEF_EX_H_
#define _H_DATADEF_EX_H_

#include "DataDef.h"
#include <XUtil/XUtil.h>

typedef struct tagExchangeInfo
{
	union
	{
		struct
		{
			uint8_t MarketType;
			char Exchange[MAX_EXCHANGE_LENGTH + 1];
			//
			char Name[MAX_NAME_LENGTH + 1];
			uint8_t ProductCount;			// 产品数量
			uint16_t CommodityCount;		// 商品数量[该市场所有商品数量]
			uint8_t PeriodsCount;			// 交易时段数量
			uint16_t Periods[4][2];		// 交易时段描述[最多只是4个交易时段，每个时段包括开始、结束时间，精确到分，HH * 60 + MM格式]
			uint8_t TimeZone;				// 该市场所处时区
			uint32_t TradingDay;			// 当前交易日期[YYYYMMDD格式]
			uint32_t Date;				// 市场日期[YYYYMMDD格式]
			uint32_t Time;				// 市场时间[HHMMSS格式]
			char Status; //市场交易状态
			char EnterReason; //进入交易状态
		};
		char Reserved[260];
	};
}EXCHANGEINFO,*PEXCHANGEINFO;

typedef struct tagProductInfo
{
	union
	{
		struct
		{
			uint8_t MarketType;
			char Exchange[MAX_EXCHANGE_LENGTH + 1];
			//
			uint8_t ProductType;
			char Product[MAX_PRODUCT_LENGTH + 1];
			//
			char Name[MAX_NAME_LENGTH + 1];
			uint16_t CommodityCount;		// 该分类的商品数量
			uint8_t	PeriodsCount;			// 交易时段数量
			uint16_t Periods[4][2];			// 交易时段描述[最多只是4个交易时段，每个时段包括开始、结束时间，精确到分，HH * 60 + MM格式]
			uint8_t	Digits;					// 有效数字位数
			uint8_t	PriceDigits;			// 该分类价格显示小数位数
			char Status; //市场交易状态
			char EnterReason; //进入交易状态
		};
		char Reserved[260];
	};
} PRODUCTINFO,*PPRODUCTINFO;

typedef struct tagCommodityInfo
{
	union
	{
		struct
		{
			uint8_t MarketType;
			char Exchange[MAX_EXCHANGE_LENGTH + 1];
			//
			uint8_t ProductType;
			char Product[MAX_PRODUCT_LENGTH + 1];
			//
			char Code[MAX_CODE_LENGTH + 1];
			//
			char Name[MAX_NAME_LENGTH + 1];
			uint8_t	PeriodsCount;			// 交易时段数量
			uint16_t Periods[4][2];			// 交易时段描述[最多只是4个交易时段，每个时段包括开始、结束时间，精确到分，HH * 60 + MM格式]
			uint8_t	Digits;					// 有效数字位数
			uint8_t	PriceDigits;			// 该分类价格显示小数位数

			 //静态数据
			 union
			 {
			 	struct 
			 	{
			 		double PreSettlementPrice; //上次结算价
			 		double PreClosePrice;//昨收盘			  
			 		double PreOpenInterest; //昨持仓量
			 		double UpperLimitPrice; //最高价
			 		double LowerLimitPrice; //最低价
			 		union
			 		{
			 			struct //期货
			 			{
			 				double	PreDelta;//昨虚实度
			 			};
			 		};
			 	};
			 	char StaticData[1];
			 };

			 //动态数据
			 union
			 {
			 	struct 
			 	{
			 		char Status; //市场交易状态
			 		char EnterReason; //进入交易状态
			 		uint32_t Date; // YYYYMMDD
			 		uint32_t Time; // 交易时间HHMMSS
			 		double Open;
			 		double High;
			 		double Low;
			 		double Price; // 交易价格，当然也是最新价，单位：元
			 		uint32_t LastVolume;
			 		double LastAmount;
			 		uint32_t Volume; // 成交量(现量)，单位：最小单位(股，张等)
			 		double Amount; // 现额
			 		union 
			 		{
			 			struct // 可交易品种
			 			{
			 				union
			 				{
			 					uint32_t Bargain; // 成交笔数(现笔)，沪深证券交易所专用
			 					struct //期货
			 					{
			 						double SettlementPrice; //结算价
			 						uint32_t OpenInterest; // 持仓量，单位：最小单位
			 						double	CurrDelta; //今虚实度
			 					};
			 				};
			 				double BidPrice[XMT_DATA_LEVEL]; ///申买价一
			 				double AskPrice[XMT_DATA_LEVEL]; ///申卖价一
			 				uint32_t BidVolume[XMT_DATA_LEVEL]; ///申买量一
							uint32_t AskVolume[XMT_DATA_LEVEL]; ///申卖量一
#ifdef COMMODITY_EX_FLAG
							int32_t BidVolDiff[XMT_DATA_LEVEL];
							int32_t AskVolDiff[XMT_DATA_LEVEL];
#endif//
			 			};
			 		};
			 	};
			 	char MarketData[1];
			 };
		};
		//char Reserved[520 + XMT_DATA_LEVEL*8*3];
	};
}COMMODITYINFO,*PCOMMODITYINFO;

typedef struct tagCommodityStaticInfo
{
	union
	{
		struct
		{
			//静态数据
			double PreSettlementPrice; //上次结算价
			double PreClosePrice;//昨收盘
			double PreOpenInterest; //昨持仓量
			double UpperLimitPrice; //涨停价
			double LowerLimitPrice; //跌停价
			union
			{
				struct //期货
				{
					double	PreDelta;//昨虚实度
				};
			};
		};
		//char Reserved[520 + XMT_DATA_LEVEL*8*3];
	};
}COMMODITYSTATICINFO,*PCOMMODITYSTATICINFO;

typedef struct tagCommodityMarketInfo
{
	union
	{
		struct
		{
			//动态数据
			char Status; //市场交易状态
			char EnterReason; //进入交易状态
			uint32_t Date; // YYYYMMDD
			uint32_t Time; // 交易时间HHMMSS
			double Open;
			double High;
			double Low;
			double Price; // 交易价格，当然也是最新价，单位：元
			uint32_t LastVolume;
			double LastAmount;
			uint32_t Volume; // 成交量(现量)，单位：最小单位(股，张等)
			double Amount; // 现额
			union 
			{
				struct // 可交易品种
				{
					union
					{
						uint32_t Bargain; // 成交笔数(现笔)，沪深证券交易所专用
						struct //期货
						{
							double SettlementPrice; //结算价
							uint32_t OpenInterest; // 持仓量，单位：最小单位
							double	CurrDelta; //今虚实度
						};
					};
					double BidPrice[XMT_DATA_LEVEL]; ///申买价一
					double AskPrice[XMT_DATA_LEVEL]; ///申卖价一
					uint32_t BidVolume[XMT_DATA_LEVEL]; ///申买量一
					uint32_t AskVolume[XMT_DATA_LEVEL]; ///申卖量一
#ifdef COMMODITY_EX_FLAG
					int32_t BidVolDiff[XMT_DATA_LEVEL];
					int32_t AskVolDiff[XMT_DATA_LEVEL];
#endif//
				};
			};		
		};
		//char Reserved[520 + XMT_DATA_LEVEL*8*3];
	};
}COMMODITYMARKETINFO,*PCOMMODITYMARKETINFO;

//明细结构
typedef struct tagCommodityTick
{
	union
	{
		struct
		{
			uint32_t Serial; // 序号(按个股从0开始编序)，如果为0xFFFFFFFF表示是拼接明细
			uint32_t Time; // 交易时间HH*3600+MM*60+SS
			double Price; // 交易价格，当然也是最新价，单位：元，放大PriceRate倍
			double Volume; // 成交量(现量)，单位：最小单位(股，张等)
			union 
			{
				struct // 指数
				{
					double Amount; // 现额
				};
				struct // 可交易品种
				{
					char BoughtSoldFlag; // 内、外盘标志，0平盘；1外盘；-1内盘
					union 
					{
						struct // 个股
						{
							uint32_t Bargain; // 成交笔数(现笔)，沪深证券交易所专用
						};
						struct // 期货
						{
							double OpenInterest; // 持仓量，单位：最小单位
						};
					};
				};
			};
		};
		char Reserved[32];
	};
} TICK,*PTICK;

typedef struct tagCommodityHistory 
{
	union
	{
		struct
		{
			uint32_t Date;
			uint32_t Time;
			double Open;			// 开盘价，单位：元(针对大盘指数表示开盘指数)，放大PriceRate倍
			double High;			// 最高价，单位：元(针对大盘指数表示最高指数)，放大PriceRate倍
			double Low;				// 最低价，单位：元(针对大盘指数表示最低指数)，放大PriceRate倍
			double Close;			// 收盘价，单位：元(针对大盘指数表示收盘指数)，放大PriceRate倍
			uint32_t Volume;		// 成交量，单位：最小单位(如股、张等)
			double Amount;			// 成交金额，单位：元
			union
			{
				struct // 个股
				{
					uint32_t Bargain; // 成交笔数(现笔)，沪深证券交易所专用
				};
				struct // 期货
				{
					double OpenInterest; // 持仓量，单位：最小单位
				};
			};
		};
		char Reserved[64];
	};
} KDATA,*PKDATA,BAR,*PBAR;

typedef struct tagWeightInfo
{
	union
	{
		struct
		{
			uint32_t Date;
			//unsigned long	Time;
			float			PresentNumber;		// 每10股送股数，
			float			MatchNumber;		// 每10股配股数，
			float			MatchPrice;			// 配股价(元)，
			float			Bonus;				// 每10股红利，
			float			AddedNumber;		// 每10股增发股数
			double			TotalShare;			// 总股本
			double			CurrentShare;		// 流通股本
			uint32_t		SplitConsolidation;	// 拆并股，拆股为正(1→X时为X)；并股为负(Y→1时为-Y)，放大1000倍——港股专用
		};
		char	Reserved[64];
	};
} WEIGHT,*PWEIGHT;

#if defined(__cplusplus)

    class ExchangeInfo : public EXCHANGEINFO
    {
	public:
        ExchangeInfo()
        {
            memset(this, 0, sizeof(ExchangeInfo));
        }

		ExchangeInfo(const char* exchange)
		{
			memset(this, 0, sizeof(ExchangeInfo));
			strncpy(this->Exchange, exchange, sizeof(this->Exchange));
		}

        ExchangeInfo(const EXCHANGEINFO& o)
        {
            memcpy(this, &o, sizeof(o));
        }

        ExchangeInfo(const ExchangeInfo& o)
        {
            memcpy(this, &o, sizeof(o));
        }

		template<class Ty>
		void UpdateStatus(const Ty& data)
		{
			if(!data) {
				return;
			}
			this->Status = data->Status;
			this->EnterReason = data->EnterReason;
		}
    };

	template<class Exchange = ExchangeInfo>
    struct ExchangeLess
    {
        bool operator()(const ExchangeInfo& x, const ExchangeInfo& y) const
        {
            return strcmp(x.Exchange, y.Exchange) < 0;
        }
    };
	typedef ExchangeLess<> ExchangeInfoLess;

    class ProductInfo : public PRODUCTINFO
    {
    public:
        ProductInfo()
        {
            memset(this, 0, sizeof(ProductInfo));
        }

		ProductInfo(const char* exchange, const char* product)
		{
			memset(this, 0, sizeof(ProductInfo));
			strncpy(this->Exchange, exchange, sizeof(this->Exchange));
			strncpy(this->Product, product, sizeof(this->Product));
		}

        ProductInfo(const PRODUCTINFO& o)
        {
            memcpy(this, &o, sizeof(o));
        }

        ProductInfo(const ProductInfo& o)
        {
            memcpy(this, &o, sizeof(o));
        }

		template<class Ty>
		void UpdateStatus(const Ty& data)
		{
			if(!data) {
				return;
			}
			this->Status = data->Status;
			this->EnterReason = data->EnterReason;
		}
    };

	template<class Product = ProductInfo>
    struct ProductLess
	{
		bool operator()(const ProductInfo& x, const ProductInfo& y) const
		{
			auto exchange_cmp = strcmp(x.Exchange, y.Exchange);
			if (exchange_cmp < 0) {
				return true;
			}
			else if (exchange_cmp == 0) {
				auto product_cmp = strcmp(x.Product, y.Product);
				if (product_cmp < 0) {
					return true;
				}
			}
			return false;
		}
	};
	typedef ProductLess<> ProductInfoLess;
	
    class CommodityInfo : public COMMODITYINFO
		//, public COMMODITYSTATICINFO, public COMMODITYMARKETINFO
    {
    public:
        CommodityInfo()
        {
            memset(this, 0, sizeof(CommodityInfo));
        }

		CommodityInfo(const char* exchange, const char* product, const char* code)
		{
			memset(this, 0, sizeof(CommodityInfo));
			strncpy(this->Exchange, exchange, sizeof(this->Exchange));
			strncpy(this->Product, product, sizeof(this->Product));
			strncpy(this->Code, code, sizeof(this->Code));
		}

        CommodityInfo(const CommodityInfo& o)
        {
            memcpy(this, &o, sizeof(o));
        }

		template<class Ty>
		void InitMarket(const Ty& data)
		{
			if(!data) {
				return;
			}
			this->PreSettlementPrice = data->PreSettlementPrice;
			this->PreClosePrice = data->PreClosePrice;
			this->PreOpenInterest = data->PreOpenInterest;
			this->UpperLimitPrice = data->UpperLimitPrice;
			this->LowerLimitPrice = data->LowerLimitPrice;
			this->PreDelta = data->PreDelta;
			if (data->Date) {
				this->Date = data->Date;
				this->Time = data->Time;
			}
			if (data->Status) {
				this->Status = data->Status;
				this->EnterReason = data->EnterReason;
			}
		}

		template<class Ty>
		size_t UpdateMarket(const Ty& data)
		{
			//这里要比较日期、时间、价格、量、额、其他等变化
			size_t chg = 0;
			if (data->Date < this->Date) {
				//过滤慢和无效数据
				return 0;
			}
			else if (data->Date == this->Date) {
				if (data->Time < this->Time) {
					//过滤慢和无效数据
					return 0;
				} else if (data->Time > this->Time) {
					chg |= MARKET_TIME_CHANGED;
				}
			}
			else {
				chg |= MARKET_DATE_CHANGED;
			}
			if (chg) {
				double price = data->Price - this->Price;
				if (!IsZeroFloat(price)) {
					chg |= MARKET_COMMODITY_PRICE_CHANGED;
				}
				uint32_t last_volume = data->Volume - this->Volume;
				if (last_volume != 0) {
					chg |= MARKET_COMMODITY_VOLUME_CHANGED;
				}
				double last_amount = data->Amount - this->Amount;
				if (!IsZeroFloat(last_amount)) {
					chg |= MARKET_COMMODITY_AMOUNT_CHANGED;
				}
				if (!chg) {
					chg = MARKET_OTHER_CHANGED;
				}
				uint32_t last_date = this->Date;
				uint32_t last_time = this->Time;
				this->Date = data->Date;
				this->Time = data->Time;
				this->Open = data->Open;
				this->High = data->High;
				this->Low = data->Low;
				this->Price = data->Price;
				this->LastVolume = last_volume;
				this->LastAmount = last_amount;
				this->Volume = data->Volume;
				this->Amount = data->Amount;
				switch(this->MarketType) {
				case XMT_STOCK: {
					this->Bargain = data->Bargain;
				}
				break;
				case XMT_FUTURES: {
					this->SettlementPrice = data->SettlementPrice;
					this->OpenInterest = data->OpenInterest;
					this->CurrDelta = data->CurrDelta;
				}
				break;
				}
				for(size_t i = 0; i < XMT_DATA_LEVEL; i++)
				{
#ifdef COMMODITY_EX_FLAG
					uint32_t BidVolDiff = data->BidVolume[i] - this->BidVolume[i];
					uint32_t AskVolDiff = data->AskVolume[i] - this->AskVolume[i];
					this->BidVolDiff[i] = BidVolDiff;
					this->AskVolDiff[i] = AskVolDiff;
#endif//
					this->BidPrice[i] = data->BidPrice[i]; ///申买价一
					this->AskPrice[i] = data->AskPrice[i]; ///申卖价一
					this->BidVolume[i] = data->BidVolume[i]; ///申买量一
					this->AskVolume[i] = data->AskVolume[i]; ///申卖量一
				}
			}
			return chg;
		}

		template<class Ty>
		void UpdateStatus(const Ty& data)
		{
			if(!data) {
				return;
			}
			this->Status = data->Status;
			this->EnterReason = data->EnterReason;
		}
    };

	template<class Commodity = CommodityInfo>
    struct CommodityLess
	{
		bool operator()(const Commodity& x, const Commodity& y) const
		{
			int cmp = strcmp(x.Exchange, y.Exchange);
			if (cmp < 0) {
				return true;
			}
			else if (cmp == 0) {
				cmp = strcmp(x.Product, y.Product);
				if (cmp < 0) {
					return true;
				} else if (cmp == 0) {
					int cmp = strcmp(x.Code, y.Code);
					if (cmp < 0) {
						return true;
					}
				}
			}
			return false;
		}
	};
	typedef CommodityLess<> CommodityInfoLess;

	template<class Commodity = CommodityInfo>
	struct CommodityCodeLess
	{
		bool operator()(const Commodity& x, const Commodity& y) const
		{
			int cmp = strcmp(x.Code, y.Code);
			if (cmp < 0) {
				return true;
			}
			return false;
		}
	};
	typedef CommodityCodeLess<> CommodityInfoCodeLess;

#endif//

#endif /* _H_FIELDDEF_EX_H_ */
