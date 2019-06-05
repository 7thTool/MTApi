#pragma once
#ifndef _H_FIELDDEF_H_
#define _H_FIELDDEF_H_

/** 
 * @brief DATASET字段定义
 */
enum FIELDDEFINE {
    IDF_0 = 0,
    IDF_DATE,
	IDF_TIME,
	IDF_PREV_CLOSE,
	IDF_OPEN, //0-4
	IDF_HIGH,
	IDF_LOW,
	IDF_CLOSE,
	IDF_VOLUME,
	IDF_LAST_VOLUME, //5-9
	IDF_AMOUNT,
	IDF_AVERAGE_PRICE,
	IDF_DELTA,
	IDF_RANGE_PERCENT,
	IDF_DELTA_PERCENT, //10-14
	IDF_WEIBI,
	IDF_BID_ASK_VOLUME_DIFF,
	IDF_VOLUME_RATIO,
	IDF_BID_VOLUME,
	IDF_ASK_VOLUME, //15-19
	IDF_BID_PRICE,
	IDF_ASK_PRICE,
	IDF_SOLD_VOLUME,
	IDF_BOUGHT_VOLUME,
	IDF_RECENT_DELTA_PERCENT, //20-24
	IDF_TURNOVER_RATE,
	IDF_AVERAGE_VOLUME,
	IDF_PRICE_EARNING_RATIO,
	IDF_POSITION,
	IDF_INTEREST, //25-29
	IDF_FULL_PRICE,
	IDF_BARGAIN,
	IDF_AVERAGE_BARGAIN_VOLUME,
	IDF_CEILING_PRICE,
	IDF_FLOOR_PRICE, //30-34
	IDF_TOTAL_VALUE,
	IDF_CURRENT_VALUE,
	IDF_PRICE_BOOK_VALUE_RATIO,
	IDF_PREV_POSITION,
	IDF_PREV_SETTLEMENT, //35-39
	IDF_SETTLEMENT,
	IDF_LAST_DELTA_POSITION,
	IDF_DELTA_POSITION,
	IDF_LAST_OPEN_CLOSE_POSITION,
	IDF_RISE_COUNT, //40-44
	IDF_DROP_COUNT,
	IDF_EQUAL_COUNT, //45-46
	IDF_DIGITS,
	IDF_BASE_MAX,
	//QUOTE
	IDF_ASK_PRICE_10 = IDF_BASE_MAX, // 卖十价		是(元)/是
	IDF_ASK_PRICE_9, // 卖九价		是(元)/是
	IDF_ASK_PRICE_8, // 卖八价		是(元)/是
	IDF_ASK_PRICE_7, // 卖七价		是(元)/是
	IDF_ASK_PRICE_6, // 卖六价		是(元)/是
	IDF_ASK_PRICE_5, // 卖五价		是(元)/是
	IDF_ASK_PRICE_4, // 卖四价		是(元)/是
	IDF_ASK_PRICE_3, // 卖三价		是(元)/是
	IDF_ASK_PRICE_2, // 卖二价		是(元)/是
	IDF_ASK_PRICE_1, // 卖一价，同卖出价IDF_ASK_PRICE
	IDF_BID_PRICE_1, // 买一价，同买入价IDF_BID_PRICE
	IDF_BID_PRICE_2, // 买二价		是(元)/是
	IDF_BID_PRICE_3, // 买三价		是(元)/是
	IDF_BID_PRICE_4, // 买四价		是(元)/是
	IDF_BID_PRICE_5, // 买五价		是(元)/是
	IDF_BID_PRICE_6, // 买六价		是(元)/是
	IDF_BID_PRICE_7, // 买七价		是(元)/是
	IDF_BID_PRICE_8, // 买八价		是(元)/是
	IDF_BID_PRICE_9, // 买九价		是(元)/是
	IDF_BID_PRICE_10, // 买十价		是(元)/是
	IDF_ASK_VOLUME_10, // 卖十量		是(股、张、份)
	IDF_ASK_VOLUME_9, // 卖九量		是(股、张、份)
	IDF_ASK_VOLUME_8, // 卖八量		是(股、张、份)
	IDF_ASK_VOLUME_7, // 卖七量		是(股、张、份)
	IDF_ASK_VOLUME_6, // 卖六量		是(股、张、份)
	IDF_ASK_VOLUME_5, // 卖五量		是(股、张、份)
	IDF_ASK_VOLUME_4, // 卖四量		是(股、张、份)
	IDF_ASK_VOLUME_3, // 卖三量		是(股、张、份)
	IDF_ASK_VOLUME_2, // 卖二量		是(股、张、份)
	IDF_ASK_VOLUME_1, // 卖一量		是(股、张、份)
	IDF_BID_VOLUME_1, // 买一量		是(股、张、份)
	IDF_BID_VOLUME_2, // 买二量		是(股、张、份)
	IDF_BID_VOLUME_3, // 买三量		是(股、张、份)
	IDF_BID_VOLUME_4, // 买四量		是(股、张、份)
	IDF_BID_VOLUME_5, // 买五量		是(股、张、份)
	IDF_BID_VOLUME_6, // 买六量		是(股、张、份)
	IDF_BID_VOLUME_7, // 买七量		是(股、张、份)
	IDF_BID_VOLUME_8, // 买八量		是(股、张、份)
	IDF_BID_VOLUME_9, // 买九量		是(股、张、份)
	IDF_BID_VOLUME_10, // 买十量	IDF_ASK_VOL_DIFF_
	IDF_MAX,
	IDF_EXCHANGEINFO,
	IDF_PRODUCTINFO,
	IDF_COMMODITYINFO,
	IDF_MarketType,
	IDF_ProductType,
	IDF_Exchange,
	IDF_Product,
	IDF_Code,
	IDF_Name,
	IDF_Periods,
	IDF_Multiple,
	IDF_PriceTick,
	IDF_TradingDay,
	IDF_TimeZone,
	IDF_Status,
	IDF_EnterReason,
    //IDF_PreSettlementPrice = IDF_PREV_SETTLEMENT,
    //IDF_SettlementPrice = IDF_SETTLEMENT,
    //IDF_PreClosePrice = IDF_PREV_CLOSE,
    //IDF_UpperLimitPrice = IDF_CEILING_PRICE,
    //IDF_LowerLimitPrice = IDF_FLOOR_PRICE,
    //IDF_OpenInterest = IDF_INTEREST,
    IDF_PreOpenInterest,
    //IDF_LastVolume = IDF_LAST_VOLUME,
    IDF_LastAmount,
    IDF_PreDelta,
    IDF_CurrDelta,
    IDF_BidPrice,
    IDF_AskPrice,
    IDF_BidVolume,
    IDF_AskVolume,
	IDF_BidVolDiff,
    IDF_AskVolDiff,
	IDF_Period,
	IDF_DWType,
	IDF_Indicator,
};

#if defined(__cplusplus)

#include <string>

static const std::string IDFS_DATE = std::to_string(IDF_DATE);
static const std::string IDFS_TIME = std::to_string(IDF_TIME);
static const std::string IDFS_PREV_CLOSE = std::to_string(IDF_PREV_CLOSE);
static const std::string IDFS_OPEN = std::to_string(IDF_OPEN); //0-4
static const std::string IDFS_HIGH = std::to_string(IDF_HIGH);
static const std::string IDFS_LOW = std::to_string(IDF_LOW);
static const std::string IDFS_CLOSE = std::to_string(IDF_CLOSE);
static const std::string IDFS_VOLUME = std::to_string(IDF_VOLUME);
static const std::string IDFS_LAST_VOLUME = std::to_string(IDF_LAST_VOLUME); //5-9
static const std::string IDFS_AMOUNT = std::to_string(IDF_AMOUNT);
static const std::string IDFS_AVERAGE_PRICE = std::to_string(IDF_AVERAGE_PRICE);
static const std::string IDFS_DELTA = std::to_string(IDF_DELTA);
static const std::string IDFS_RANGE_PERCENT = std::to_string(IDF_RANGE_PERCENT);
static const std::string IDFS_DELTA_PERCENT = std::to_string(IDF_DELTA_PERCENT); //10-14
static const std::string IDFS_WEIBI = std::to_string(IDF_WEIBI);
static const std::string IDFS_BID_ASK_VOLUME_DIFF = std::to_string(IDF_BID_ASK_VOLUME_DIFF);
static const std::string IDFS_VOLUME_RATIO = std::to_string(IDF_VOLUME_RATIO);
static const std::string IDFS_BID_VOLUME = std::to_string(IDF_BID_VOLUME);
static const std::string IDFS_ASK_VOLUME = std::to_string(IDF_ASK_VOLUME); //15-19
static const std::string IDFS_BID_PRICE = std::to_string(IDF_BID_PRICE);
static const std::string IDFS_ASK_PRICE = std::to_string(IDF_ASK_PRICE);
static const std::string IDFS_SOLD_VOLUME = std::to_string(IDF_SOLD_VOLUME);
static const std::string IDFS_BOUGHT_VOLUME = std::to_string(IDF_BOUGHT_VOLUME);
static const std::string IDFS_RECENT_DELTA_PERCENT = std::to_string(IDF_RECENT_DELTA_PERCENT); //20-24
static const std::string IDFS_TURNOVER_RATE = std::to_string(IDF_TURNOVER_RATE);
static const std::string IDFS_AVERAGE_VOLUME = std::to_string(IDF_AVERAGE_VOLUME);
static const std::string IDFS_PRICE_EARNING_RATIO = std::to_string(IDF_PRICE_EARNING_RATIO);
static const std::string IDFS_POSITION = std::to_string(IDF_POSITION);
static const std::string IDFS_INTEREST = std::to_string(IDF_INTEREST);//25-29
static const std::string IDFS_FULL_PRICE = std::to_string(IDF_FULL_PRICE);
static const std::string IDFS_BARGAIN = std::to_string(IDF_BARGAIN);
static const std::string IDFS_AVERAGE_BARGAIN_VOLUME = std::to_string(IDF_AVERAGE_BARGAIN_VOLUME);
static const std::string IDFS_CEILING_PRICE = std::to_string(IDF_CEILING_PRICE);
static const std::string IDFS_FLOOR_PRICE = std::to_string(IDF_FLOOR_PRICE); //30-34
static const std::string IDFS_TOTAL_VALUE = std::to_string(IDF_TOTAL_VALUE);
static const std::string IDFS_CURRENT_VALUE = std::to_string(IDF_CURRENT_VALUE);
static const std::string IDFS_PRICE_BOOK_VALUE_RATIO = std::to_string(IDF_PRICE_BOOK_VALUE_RATIO);
static const std::string IDFS_PREV_POSITION = std::to_string(IDF_PREV_POSITION);
static const std::string IDFS_PREV_SETTLEMENT = std::to_string(IDF_PREV_SETTLEMENT); //35-39
static const std::string IDFS_SETTLEMENT = std::to_string(IDF_SETTLEMENT);
static const std::string IDFS_LAST_DELTA_POSITION = std::to_string(IDF_LAST_DELTA_POSITION);
static const std::string IDFS_DELTA_POSITION = std::to_string(IDF_DELTA_POSITION);
static const std::string IDFS_LAST_OPEN_CLOSE_POSITION = std::to_string(IDF_LAST_OPEN_CLOSE_POSITION);
static const std::string IDFS_RISE_COUNT = std::to_string(IDF_RISE_COUNT); //40-44
static const std::string IDFS_DROP_COUNT = std::to_string(IDF_DROP_COUNT);
static const std::string IDFS_EQUAL_COUNT = std::to_string(IDF_EQUAL_COUNT); //45-46
static const std::string IDFS_DIGITS = std::to_string(IDF_DIGITS);
static const std::string IDFS_EXCHANGEINFO = std::to_string(IDF_EXCHANGEINFO);
static const std::string IDFS_PRODUCTINFO = std::to_string(IDF_PRODUCTINFO);
static const std::string IDFS_COMMODITYINFO = std::to_string(IDF_COMMODITYINFO);
static const std::string IDFS_MarketType = std::to_string(IDF_MarketType);
static const std::string IDFS_ProductType = std::to_string(IDF_ProductType);
static const std::string IDFS_Exchange = std::to_string(IDF_Exchange);
static const std::string IDFS_Product = std::to_string(IDF_Product);
static const std::string IDFS_Code = std::to_string(IDF_Code);
static const std::string IDFS_Name = std::to_string(IDF_Name);
static const std::string IDFS_Periods = std::to_string(IDF_Periods);
static const std::string IDFS_Multiple = std::to_string(IDF_Multiple);
static const std::string IDFS_PriceTick = std::to_string(IDF_PriceTick);
static const std::string IDFS_TradingDay = std::to_string(IDF_TradingDay);
static const std::string IDFS_TimeZone = std::to_string(IDF_TimeZone);
static const std::string IDFS_Status = std::to_string(IDF_Status);
static const std::string IDFS_EnterReason = std::to_string(IDF_EnterReason);
    //IDF_PreSettlementPrice = IDF_PREV_SETTLEMENT,
    //IDF_SettlementPrice = IDF_SETTLEMENT,
    //IDF_PreClosePrice = IDF_PREV_CLOSE,
    //IDF_UpperLimitPrice = IDF_CEILING_PRICE,
    //IDF_LowerLimitPrice = IDF_FLOOR_PRICE,
    //IDF_OpenInterest = IDF_INTEREST,
static const std::string IDFS_PreOpenInterest = std::to_string(IDF_PreOpenInterest);
    //IDF_LastVolume = IDF_LAST_VOLUME,
static const std::string IDFS_LastAmount = std::to_string(IDF_LastAmount);
static const std::string IDFS_PreDelta = std::to_string(IDF_PreDelta);
static const std::string IDFS_CurrDelta = std::to_string(IDF_CurrDelta);
static const std::string IDFS_BidPrice = std::to_string(IDF_BidPrice);
static const std::string IDFS_AskPrice = std::to_string(IDF_AskPrice);
static const std::string IDFS_BidVolume = std::to_string(IDF_BidVolume);
static const std::string IDFS_AskVolume = std::to_string(IDF_AskVolume);
static const std::string IDFS_BidVolDiff = std::to_string(IDF_BidVolDiff);
static const std::string IDFS_AskVolDiff = std::to_string(IDF_AskVolDiff);
static const std::string IDFS_Period = std::to_string(IDF_Period);
static const std::string IDFS_DWType = std::to_string(IDF_DWType);
static const std::string IDFS_Indicator = std::to_string(IDF_Indicator);

#endif//

#endif /* _H_FIELDDEF_H_ */
