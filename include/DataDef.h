#pragma once
#ifndef _H_DATADEF_H_
#define _H_DATADEF_H_

#include <stdint.h>

#define INT64_TO_DOUBLE(x)		(double)((int64_t)(x))
#define UINT64_TO_DOUBLE(x)		(double)((usinged  int64_t)(x))

#define YYMMDD_TO_ULONG(y,m,d)		((y)*10000+m*100+d)
#define ULONG_TO_YYMMDD(x,y,m,d)	((y)=(x)/10000,(m)=(x)/100%100,(d)=(x)%100)

#define HHMMSS_TO_ULONG(h,m,s)		((h)*10000+m*100+s)
#define ULONG_TO_HHMMSS(x,h,m,s)	((h)=(x)/10000,(m)=(x)/100%100,(s)=(x)%100)

#define HHMMSSsss_TO_ULONG(h,m,s,sss)		(HHMMSS_TO_ULONG(h,m,s)*1000+(sss))
#define ULONG_TO_HHMMSSsss(x,h,m,s,sss)		((h)=(x)/(10000000),(m)=(x)/100000%100000,(s)=(x)/1000%1000,(sss)=(x)%1000)

#define MINUTE_TO_HHMM(x)		((x)/60*100 + (x)%60)
#define HHMM_TO_MINUTE(x)		((x)/100*60 + (x)%100)

#define SECOND_TO_HHMM(x)		MINUTE_TO_HHMM((x)/60)
#define HHMM_TO_SECOND(x)		(HHMM_TO_MINUTE(x)*60)

#define HHMMSS_TO_MINUTE(x)		HHMM_TO_MINUTE((x)/100)
#define HHMMSSsss_TO_MINUTE(x)	HHMMSS_TO_MINUTE((x)/1000)
#define MINUTE_TO_HHMMSS(x)		(MINUTE_TO_HHMM(x)*100)
#define MINUTE_TO_HHMMSSsss(x)	(MINUTE_TO_HHMMSS(x)*1000)

#define HHMMSS_TO_SECOND(x)		((x)/10000*60*60 + (x)%10000/100*60 + (x)%10000%100)
#define HHMMSSsss_TO_SECOND(x)	HHMMSS_TO_SECOND((x)/1000)
#define SECOND_TO_HHMMSS(x)		((x)/(60*60)*10000 + (x)%(60*60)/60*100 + (x)%60)
#define SECOND_TO_HHMMSSsss(x)	(SECOND_TO_HHMMSS(x)*1000)

//up_mode=上行模式，即对外提供服务模式
//down_mode=下行模式，即向下接入服务模式
enum UpDownMode
{
    WORK_MODE_RPC 	= 1,
    WORK_MODE_IPC	= 2,
	WORK_MODE_MIX   = 3,
};

//接口回调标志，兼容XML标志
enum
{
	XML_FLAG_JSON_STRING = 0, //默认字符串
	XML_FLAG_JSON_FILE,
	XML_FLAG_XML_STRING,
	XML_FLAG_XML_FILE,
	XML_FLAG_PTREE,
	XML_FLAG_PTREE_PTR,
	XML_FLAG_DATASET,
	XML_FLAG_STR_DATASET,
};

enum UpdateFlag 
{
	Add 			= 0,
	Remove,
	Update,
};

enum FindFlag 
{
	FindOrCreate 	= 0,
	FindOnly,
	CreateOnly,
};

#define		MAX_EXCHANGE_LENGTH		7	//市场代码最大长度
#define		MAX_PRODUCT_LENGTH		7	//产品代码最大长度
#define		MAX_CODE_LENGTH			14	//商品代码最大长度
#define		MAX_NAME_LENGTH			31	//商品名称最大长度

// # Chinese
#define EXCHANGE_CFFEX "CFFEX"         //# China Financial Futures Exchange
#define EXCHANGE_SHFE "SHFE"           //# Shanghai Futures Exchange
#define EXCHANGE_CZCE "CZCE"           //# Zhengzhou Commodity Exchange
#define EXCHANGE_DCE "DCE"             //# Dalian Commodity Exchange
#define EXCHANGE_INE "INE"             //# Shanghai International Energy Exchange
#define EXCHANGE_SSE "SSE"             //# Shanghai Stock Exchange
#define EXCHANGE_SZSE "SZSE"           //# Shenzhen Stock Exchange
//#define EXCHANGE_SGE "SGE"             //# Shanghai Gold Exchange
//#define EXCHANGE_WXE "WXE"             //# Wuxi Steel Exchange

//     # Global
//     SMART = "SMART"         # Smart Router for US stocks
//     NYMEX = "NYMEX"         # New York Mercantile Exchange
//     GLOBEX = "GLOBEX"       # Globex of CME
//     IDEALPRO = "IDEALPRO"   # Forex ECN of Interactive Brokers
//     CME = "CME"             # Chicago Mercantile Exchange
//     ICE = "ICE"             # Intercontinental Exchange
//     SEHK = "SEHK"           # Stock Exchange of Hong Kong
//     HKFE = "HKFE"           # Hong Kong Futures Exchange
//     SGX = "SGX"             # Singapore Global Exchange
//     CBOT = "CBT"            # Chicago Board of Trade
//     DME = "DME"             # Dubai Mercantile Exchange
//     EUREX = "EUX"           # Eurex Exchange
//     APEX = "APEX"           # Asia Pacific Exchange
//     LME = "LME"             # London Metal Exchange
//     BMD = "BMD"             # Bursa Malaysia Derivatives
//     TOCOM = "TOCOM"         # Tokyo Commodity Exchange
//     EUNX = "EUNX"           # Euronext Exchange

//     # CryptoCurrency
//     BITMEX = "BITMEX"
//     OKEX = "OKEX"
//     HUOBI = "HUOBI"
//     BITFINEX = "BITFINEX"
/////////////////////////////////////////////////////////////////////////
///交易状态类型
/////////////////////////////////////////////////////////////////////////
///开盘前
#define XMT_Status_BeforeTrading '0'
///非交易
#define XMT_Status_NoTrading '1'
///连续交易
#define XMT_Status_Continous '2'
///集合竞价报单
#define XMT_Status_AuctionOrdering '3'
///集合竞价价格平衡
#define XMT_Status_AuctionBalance '4'
///集合竞价撮合
#define XMT_Status_AuctionMatch '5'
///收盘
#define XMT_Status_Closed '6'

/////////////////////////////////////////////////////////////////////////
///进入交易状态原因类型
/////////////////////////////////////////////////////////////////////////
///自动切换
#define XMT_Enter_Reason_Automatic '1'
///手动切换
#define XMT_Enter_Reason_Manual '2'
///熔断
#define XMT_Enter_Reason_Fuse '3'

//数据深度
#define XMT_DATA_LEVEL 5


typedef uint16_t period_t[2];

typedef struct tagPeriodsInfo
{
	uint8_t count;
	period_t period[4];
}periods_t;

	enum
	{
		MARKET_DATE_CHANGED = 0X0001,
		/*MARKET_NAMETABLE_CHANGED = 0X0002,
		MARKET_NAMETABLEATTACH_CHANGED = 0X0004,
		MARKET_STATICDATA_CHANGED = 0X0008,
		MARKET_DKPHDATA_CHANGED = 0X0010,*/

		MARKET_TIME_CHANGED = 0X0020,
		MARKET_COMMODITY_PRICE_CHANGED = 0X0040,	//价变，量额不一定变
		MARKET_COMMODITY_VOLUME_CHANGED = 0X0080,	//成交量变，成交额肯定变
		MARKET_COMMODITY_AMOUNT_CHANGED = MARKET_COMMODITY_VOLUME_CHANGED,
		MARKET_OTHER_CHANGED = 0X0100,

		MARKET_COMMODITY_TICK_CHANGED = MARKET_COMMODITY_VOLUME_CHANGED,
		MARKET_COMMODITY_TREND_CHANGED = MARKET_COMMODITY_PRICE_CHANGED | MARKET_COMMODITY_VOLUME_CHANGED,
	};

//Product class.
#define PRODUCT_STOCK 'S' //"股票"
#define PRODUCT_FUTURES 'F'// = "期货"
#define PRODUCT_OPTION 'O'// = "期权"
#define PRODUCT_INDEX 'I'// = "指数"
#define PRODUCT_FOREX 'X'// = "外汇"
#define PRODUCT_SPOT 'P'// = "现货"
#define PRODUCT_ETF 'E'// = "ETF"
#define PRODUCT_BOND 'B'// = "债券"
#define PRODUCT_WARRANT 'W'// = "权证"
#define PRODUCT_SPREAD 'D'// = "价差"
#define PRODUCT_FUND 'U'// = "基金"

enum
{
	PRODUCT_TYPE_DEFAULT	= 0,			// 默认/未知/临时
	PRODUCT_TYPE_SYSTEM,					// 系统板块，不允许用户作修改，一般与服务器保持同步
	PRODUCT_TYPE_USER,						// 用户自定义的板块
	PRODUCT_TYPE_MASK		= 0XC0
};

enum
{
	PRODUCT_INDEX_SELF = 0,	//自选股板块，1-9，值为QSB_SELFBLK+(0-8)
	PRODUCT_INDEX_SELF1, 
	PRODUCT_INDEX_SELF2,
	PRODUCT_INDEX_SELF3,
	PRODUCT_INDEX_SELF4,
	PRODUCT_INDEX_SELF5,
	PRODUCT_INDEX_SELF6,
	PRODUCT_INDEX_SELF7,
	PRODUCT_INDEX_SELF8,
	PRODUCT_INDEX_SELFCOUNT,
	PRODUCT_INDEX_SELFMAX = 16,
};

//周期定义
enum PERIODTYPE
{
	CYC_TICK=0,
	CYC_5SEC, //10秒基数的秒周期
	CYC_ANYSEC,
	CYC_1MIN,
	CYC_5MIN,
	CYC_15MIN,
	CYC_30MIN,
	CYC_60MIN,
	CYC_ANYMIN,
	CYC_DAY,
	CYC_WEEK,
	CYC_MONTH,
	CYC_SEASON,
	CYC_YEAR,
	CYC_ANYDAY,
	CYC_MAX,
};

//除权方式定义
enum DWTYPE
{
	DW_NONE=0,	//不除权
	DW_BACKWARD,//向后	
	DW_FORWARD, //向前
	DW_DAY,		//指定日期
	DW_ALL,		//全程复权
	DW_MAX,
};

/** 
    * @brief 价格类型定义 
    */
enum PRICETYPE
{
	PRICE_CLOSE,		//收盘价格
	PRICE_OPEN,			//开盘价格
	PRICE_HIGH,			//最高价格
	PRICE_LOW,			//最低价格
	PRICE_MEDIAN,		//中间值（高+低）/2
	PRICE_TYPICAL,		//典型价格（高+低+收盘价）/3
	PRICE_WEIGHTED,		//平均价格（高+低+收盘价格+开盘价格）/4
	PRICE_AMOUNT,		//成交额
	PRICE_VOLUME,		//成交量
	PRICE_TICKVOLUME,	//跳动量
	PRICE_AVPRICE,		//平均价（成交额/成交量）
};

/** 
    * @brief 常用周期定义 
    */
#define SECOND_OF_PERIOD_TICK		    0
#define SECOND_OF_PERIOD_1SEC		    1
#define SECOND_OF_PERIOD_5SEC		    5
#define SECOND_OF_PERIOD_1MIN		    60
#define SECOND_OF_PERIOD_5MIN		    60*5
#define SECOND_OF_PERIOD_15MIN		    60*15
#define SECOND_OF_PERIOD_30MIN		    60*30
#define SECOND_OF_PERIOD_HOUR		    60*60
#define SECOND_OF_PERIOD_4HOUR		    60*60*4
#define SECOND_OF_PERIOD_DAY		    60*60*24
#define SECOND_OF_PERIOD_WEEK		    60*60*24*7
#define SECOND_OF_PERIOD_MONTH		    60*60*24*30
#define SECOND_OF_PERIOD_SEASON		    60*60*24*90
#define SECOND_OF_PERIOD_YEAR		    60*60*24*365

//Direction of order/trade/position.
#define DIRECTION_LONG '0'
#define DIRECTION_SHORT '1'
#define DIRECTION_NET '2'

//Offset of order/trade.
#define OFFSET_OPEN '0'
#define OFFSET_CLOSE '1'
#define OFFSET_CLOSETODAY '2'
#define OFFSET_CLOSEYESTERDAY '3'

//Order status.
#define ORDER_STATUS_SUBMITTING '0'
#define ORDER_STATUS_NOTTRADED '1'
#define ORDER_STATUS_PARTTRADED '2'
#define ORDER_STATUS_ALLTRADED '3'
#define ORDER_STATUS_CANCELLED '4'
#define ORDER_STATUS_REJECTED '5'

//Order type.
#define ORDER_LIMIT '0'
#define ORDER_MARKET '1'
#define ORDER_STOP '2'
#define ORDER_FAK '3'
#define ORDER_FOK '4'

//Option type.
#define OPTION_CALL '0'
#define OPTION_PUT '1'

//Currency.
#define CURRENCY_USD '0'
#define CURRENCY_HKD '1'
#define CURRENCY_CNY '2'

#if defined(__cplusplus)

//

#endif//

#endif /* _H_DATADEF_H_ */
