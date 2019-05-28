#pragma once

#include "MDBase.hpp"
#include <FieldDef.h>
#include <DataSet.hpp>
#include <DataSetConvEx.hpp>
#include "RpcDef.hpp"
#include <XUtil/XXmlRpc.hpp>

namespace MTP {

//     inline bool xml_to_exchangeset(const boost::property_tree::ptree& xml, CDataSet& dataset)
//     {
//         size_t i = 0;
//         dataset.SetFieldValue(IDF_MarketType, xml.get<uint8_t>(IDFS_MarketType));
// 		dataset.SetFieldValue(IDF_Exchange, xml.get<std::string>(IDFS_Exchange));
// 		dataset.SetFieldValue(IDF_Name, xml.get<std::string>(IDFS_Name));
//         i = 0;
//         uint16_t Periods[4][2] = {0};
//         BOOST_FOREACH (const boost::property_tree::ptree::value_type &one, xml.get_child(IDFS_Periods))
//         {
// 		    Periods[i/2][i%2] = one.second.get_value<uint16_t>();		// 交易时段描述[最多只是4个交易时段，每个时段包括开始、结束时间，精确到分，HH * 60 + MM格式]
//             i++;
//         }
//         dataset.SetFieldValue(IDF_Periods, Periods, i);
// 		dataset.SetFieldValue(IDF_TimeZone, xml.get<uint8_t>(IDFS_TimeZone));				// 该市场所处时区
// 		dataset.SetFieldValue(IDF_TradingDay, xml.get<uint32_t>(IDFS_TradingDay));			// 当前交易日期[YYYYMMDD格式]
// 		dataset.SetFieldValue(IDF_DATE, xml.get<uint32_t>(IDFS_DATE));				// 市场日期[YYYYMMDD格式]
// 		dataset.SetFieldValue(IDF_TIME, xml.get<uint32_t>(IDFS_TIME));				// 市场时间[HHMMSS格式]
// 		dataset.SetFieldValue(IDF_Status, xml.get<char>(IDFS_TIME)); //市场交易状态
// 		dataset.SetFieldValue(IDF_EnterReason, xml.get<char>(IDFS_TIME)); //进入交易状态
//         return true;
//     }

//     inline bool xml_to_exchange(const boost::property_tree::ptree& xml, ExchangeInfo& exchange)
//     {
//         size_t i = 0;
//         exchange.MarketType = xml.get<uint8_t>(IDFS_MarketType);
// 		strncpy(exchange.Exchange, xml.get<std::string>(IDFS_Exchange).c_str(), MAX_EXCHANGE_LENGTH);
// 		strncpy(exchange.Name, xml.get<std::string>(IDFS_Name).c_str(), MAX_NAME_LENGTH);
// 		exchange.ProductCount = 0;			// 产品数量
// 		exchange.CommodityCount = 0;		// 商品数量[该市场所有商品数量]
// 		exchange.PeriodsCount = 0;			// 交易时段数量
//         i = 0;
//         BOOST_FOREACH (const boost::property_tree::ptree::value_type &one, xml.get_child(IDFS_Periods))
//         {
// 		    exchange.Periods[i/2][i%2] = one.second.get_value<uint16_t>();		// 交易时段描述[最多只是4个交易时段，每个时段包括开始、结束时间，精确到分，HH * 60 + MM格式]
//             i++;
//         }
//         exchange.PeriodsCount = i/2;
// 		exchange.TimeZone = xml.get<uint8_t>(IDFS_TimeZone);				// 该市场所处时区
// 		exchange.TradingDay = xml.get<uint32_t>(IDFS_TradingDay);			// 当前交易日期[YYYYMMDD格式]
// 		exchange.Date = xml.get<uint32_t>(IDFS_DATE);				// 市场日期[YYYYMMDD格式]
// 		exchange.Time = xml.get<uint32_t>(IDFS_TIME);				// 市场时间[HHMMSS格式]
// 		exchange.Status = xml.get<char>(IDFS_TIME); //市场交易状态
// 		exchange.EnterReason = xml.get<char>(IDFS_TIME); //进入交易状态
//         return true;
//     }

//     inline bool xml_from_exchange(boost::property_tree::ptree& xml, const ExchangeInfo& exchange)
//     {
//         size_t i = 0;
//         xml.put(IDFS_MarketType, exchange.MarketType);
//         xml.put(IDFS_Exchange, exchange.Exchange);
//         xml.put(IDFS_Name, exchange.Name);
//         boost::property_tree::ptree periods;
//         for(i = 0; i < exchange.PeriodsCount; i++)
//         {
//             boost::property_tree::ptree from, to;
//             from.put("", exchange.Periods[i][0]);
//             to.put("", exchange.Periods[i][1]);
//             periods.push_back(std::make_pair("", from));
//             periods.push_back(std::make_pair("", to));
//         }
//         xml.push_back(std::make_pair(IDFS_Periods, periods));
//         xml.put(IDFS_TimeZone, exchange.TimeZone);
//         xml.put(IDFS_TradingDay, exchange.TradingDay);
//         xml.put(IDFS_DATE, exchange.Date);
//         xml.put(IDFS_TIME, exchange.Time);
//         xml.put(IDFS_Status, exchange.Status);
//         xml.put(IDFS_EnterReason, exchange.EnterReason);
//         return true;
//     }

//     inline bool xml_to_commodity(const boost::property_tree::ptree& xml, CommodityInfo& commodity)
//     {
// //         size_t i = 0;
// //         commodity.MarketType = xml.get<uint8_t>(IDFS_MarketType);
// // 		strncpy(commodity.Exchange, xml.get<std::string>(IDFS_Exchange).c_str(), MAX_EXCHANGE_LENGTH);
// // 			//
// // 		commodity.ProductType = xml.get<uint8_t>(IDFS_ProductType);
// // 		strncpy(commodity.Product, xml.get<std::string>(IDFS_Product).c_str(), MAX_PRODUCT_LENGTH);
// // 			//
// // 		strncpy(commodity.Code, xml.get<std::string>(IDFS_Code).c_str(), MAX_CODE_LENGTH);
// // 			//
// // 		strncpy(commodity.Name, xml.get<std::string>(IDFS_Code).c_str(), MAX_NAME_LENGTH);
// // 		commodity.PeriodsCount = 0;			// 交易时段数量
// // 		i = 0;
// //         BOOST_FOREACH (const boost::property_tree::ptree::value_type &one, xml.get_child(IDFS_Periods))
// //         {
// // 		    commodity.Periods[i/2][i%2] = one.second.get_value<uint16_t>();		// 交易时段描述[最多只是4个交易时段，每个时段包括开始、结束时间，精确到分，HH * 60 + MM格式]
// //             i++;
// //         }
// //         commodity.PeriodsCount = i/2;
// // 		commodity.Digits = xml.get<uint8_t>(IDFS_Digits);					// 有效数字位数
// // 		commodity.PriceDigits = xml.get<uint8_t>(IDFS_PriceDigits);			// 该分类价格显示小数位数
// //         commodity.PreSettlementPrice = xml.get<uint8_t>(IDFS_PreSettlementPrice);//上次结算价
// // 		commodity.PreClosePrice = xml.get<uint8_t>(IDFS_PreClosePrice);//昨收盘			  
// // 		commodity.PreOpenInterest = xml.get<uint8_t>(IDFS_PreOpenInterest); //昨持仓量
// // 		commodity.UpperLimitPrice = xml.get<uint8_t>(IDFS_UpperLimitPrice); //最高价
// // 		commodity.LowerLimitPrice = xml.get<uint8_t>(IDFS_LowerLimitPrice); //最低价
// // 		commodity.PreDelta = xml.get<uint8_t>(IDFS_PreDelta);//昨虚实度
// // 		commodity.Status = xml.get<uint8_t>(IDFS_Status); //市场交易状态
// // 		commodity.EnterReason = xml.get<uint8_t>(IDFS_EnterReason); //进入交易状态
// // 		commodity.Date = xml.get<uint8_t>(IDFS_DATE); // YYYYMMDD
// // 		commodity.Time = xml.get<uint8_t>(IDFS_TIME); // 交易时间HHMMSS
// // 		commodity.Open = xml.get<uint8_t>(IDFS_OPEN);
// // 		commodity.High = xml.get<uint8_t>(IDFS_HIGH);
// // 		commodity.Low = xml.get<uint8_t>(IDFS_LOW);
// // 		commodity.Price = xml.get<uint8_t>(IDFS_CLOSE); // 交易价格，当然也是最新价，单位：元
// // 		commodity.LastVolume = xml.get<uint8_t>(IDFS_LAST_VOLUME);
// // 		commodity.LastAmount = xml.get<uint8_t>(IDFS_LastAmount);
// // 		commodity.Volume = xml.get<uint8_t>(IDFS_VOLUME); // 成交量(现量)，单位：最小单位(股，张等)
// // 		commodity.Amount = xml.get<uint8_t>(IDFS_AMOUNT); // 现额
// // 		commodity.Bargain = xml.get<uint8_t>(IDFS_BARGAIN); // 成交笔数(现笔)，沪深证券交易所专用
// // 		commodity.SettlementPrice = xml.get<uint8_t>(IDFS_SETTLEMENT); //结算价
// // 		commodity.OpenInterest = xml.get<uint8_t>(IDFS_INTEREST); // 持仓量，单位：最小单位
// // 		commodity.CurrDelta = xml.get<uint8_t>(IDFS_CurrDelta); //今虚实度
// //         i = 0;
// //         BOOST_FOREACH (const boost::property_tree::ptree::value_type &one, xml.get_child(IDFS_BidPrice))
// //         {
// // 		    commodity.Periods[i/2][i%2] = one.second.get_value<uint16_t>();		// 交易时段描述[最多只是4个交易时段，每个时段包括开始、结束时间，精确到分，HH * 60 + MM格式]
// //             i++;
// //         }
// // 		commodity.BidPrice[XMT_DATA_LEVEL]; ///申买价一
// // 		commodity.AskPrice[XMT_DATA_LEVEL]; ///申卖价一
// // 		commodity.BidVolume[XMT_DATA_LEVEL]; ///申买量一
// // 		commodity.AskVolume[XMT_DATA_LEVEL]; ///申卖量一
// // #ifdef COMMODITY_EX_FLAG
// // 		commodity.BidVolDiff[XMT_DATA_LEVEL];
// // 		commodity.AskVolDiff[XMT_DATA_LEVEL];
// // #endif//
//         return true;
//     }

//     inline bool xml_from_commodity(boost::property_tree::ptree& xml, const CommodityInfo& commodity)
//     {
//         return true;
//     }

//     inline bool xml_to_commodity_update(const boost::property_tree::ptree& xml, CommodityInfo& commodity)
//     {
//         return true;
//     }

//     inline bool xml_from_commodity_update(boost::property_tree::ptree& xml, const CommodityInfo& commodity)
//     {
//         return true;
//     }

//     inline bool xml_to_commodity_status(const boost::property_tree::ptree& xml, CommodityInfo& commodity)
//     {
//         return true;
//     }

//     inline bool xml_from_commodity_status(boost::property_tree::ptree& xml, const CommodityInfo& commodity)
//     {
//         return true;
//     }

}

