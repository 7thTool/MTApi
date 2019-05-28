#pragma once

#include <string>
#include <vector>
#include <memory>
#include <vector>
#include <atomic>
#include <string>
#include <sstream>
#include <ThostFtdcTraderApi.h>
#include <ThostFtdcMdApi.h>
#include <boost/serialization/vector.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <XUtil/XStr.hpp>
#include <XUtil/XXml.hpp>
#include <XUtil/XLogger.hpp>

#include "CTPExport.h"

//////////////////////////////////////////////////////////////////////////
//请求格式：{ "RequestApi":"Api", "RequestID": 0, "Data": { Property:Value } }
//返回格式：{ "ReturnApi":"Api", "ErrorMsg": "成功", "ErrorID": 0, "RequestID": 0, "IsLast": 0, "Data": { Property:Value } }
//其中:
//Property对应CTP数据结构数据，Value对应CTP数据结构数据值
//请求数据里必须有 RequestApi:Api 属性，例如：{ RequestApi:ReqOrderInsert }
//返回数据里必填ReturnApi:Api，例如：{ ReturnApi:RtnOrder }
//ErrorMsg：返回消息内容
//ErrorID：返回错误值，0表示成功，其他表示错误
//RequestID：请求传入的RequestID原样返回
//IsLast：标示是否是最后一个包
//Data：数据内容，是Property:Value组成的对象
//请求出错：
//请求异常服务器直接断开和客户端连接
//请求非异常错误服务器返回ReturnApi=RequestApi
//返回出错：
//返回异常服务器直接断开和客户端连接
//返回非异常错误ReturnApi=CTP返回相关Api
//////////////////////////////////////////////////////////////////////////
//BrokerID统一为：9999
//标准CTP：
//第一组：Trade Front：180.168.146.187:10000，Market Front：180.168.146.187 : 10010；【电信】
//第二组：Trade Front：180.168.146.187 : 10001，Market Front：180.168.146.187 : 10011；【电信】
//第三组：Trade Front：218.202.237.33 : 10002，Market Front：218.202.237.33 : 10012；【移动】
//账号115569 密码 654321
//WEB
//IP:123.57.8.45:5588
//帐号： administrator
//密码 : af(201 - 30326 * &jUYjes)yy
//////////////////////////////////////////////////////////////////////////

typedef char TThostFtdcUriAddressType[2048];

class CTPUserLoginInfo : public CThostFtdcReqUserLoginField
{
public:
	CTPUserLoginInfo()
	{
		memset((CThostFtdcReqUserLoginField*)this, 0, sizeof(CThostFtdcReqUserLoginField));
		//FrontAddress;
		//NsAddress;
		memset(&LoginMode, 0, sizeof(LoginMode));
		memset(&ProductInfo, 0, sizeof(ProductInfo));
		memset(&AuthCode, 0, sizeof(AuthCode));
		//MdFrontAddress;
		//MdNsAddress;
	}

	///前置机服务器网络地址
	//TThostFtdcUriAddressType FrontAddress;
	std::vector<std::string> FrontAddress;
	///名字服务器网络地址
	//TThostFtdcUriAddressType NsAddress;
	std::vector<std::string> NsAddress;
	///登录模式
	TThostFtdcLoginModeType	LoginMode;
	///用户端产品信息
	TThostFtdcProductInfoType ProductInfo; 
	///认证码
	TThostFtdcAuthCodeType	AuthCode;

	///行情前置机服务器网络地址
	//TThostFtdcUriAddressType MdFrontAddress;
	std::vector<std::string> MdFrontAddress;
	///行情名字服务器网络地址
	//TThostFtdcUriAddressType MdNsAddress;
	std::vector<std::string> MdNsAddress;
};

bool parseUserInfoFromString(CTPUserLoginInfo& userInfo, const std::string& jsonString);

//////////////////////////////////////////////////////////////////////////

#define DefaultDecimal		(3)		//默认小数位数
#define AmountDecimal		(2)		//额小数位数
#define AmountRatioDecimal	(2)		//额比例位数
#define VolumeDecimal		(0)		//量小数位数

unsigned long CTPDate2Date(const TThostFtdcDateType& Date);
unsigned long CTPTime2Time(const TThostFtdcTimeType& Time);

#define CHECKDATETIME(d,t)

//////////////////////////////////////////////////////////////////////////

//-1，表示网络连接失败；
//-2，表示未处理请求超过许可数；
//-3，表示每秒发送请求数超过许可数。

///CTPTrade内部使用的发送请求宏
#define DoRequest(func,req,reqId) \
	do \
	{ \
		rlt = func(&req, reqId); \
		if (rlt==-2 || rlt==-3) \
		{ \
			boost::this_thread::yield(); \
		} \
		else \
		{ \
			ASSERT(rlt>=0); \
			break; \
		} \
	} while(true)

// 是否收到成功的响应
bool IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo);

//////////////////////////////////////////////////////////////////////////

enum
{
	CTP_STAT_DISCONNECT					= 0,
	CTP_STAT_CONNECTING					= 1,
	CTP_STAT_CONNECT					= 2,
	CTP_STAT_AUTHENTICATE				= 3,
	CTP_STAT_LOGIN						= 4,
	CTP_STAT_INVESTORINFO				= 5,
	CTP_STAT_TRADINGACCOUNT				= 6,
	CTP_STAT_TRADINGCODE				= 7,
	CTP_STAT_NOTICE						= 8,
	CTP_STAT_CFMMC						= 9,
	CTP_STAT_QRYSETTLEMENTINFOCONFIRM	= 10,
	CTP_STAT_QRYSETTLEMENTINFO			= 11,
	CTP_STAT_SETTLEMENTINFOCONFIRM		= 12,
	CTP_STAT_ACCOUNTREGISTER			= 13,
	CTP_STAT_BANK						= 14,
	CTP_STAT_EXCHANGE					= 15,
	CTP_STAT_PRODUCT					= 16,
	CTP_STAT_INSTRUMENT					= 17,
	CTP_STAT_DEPTHMARKETDATA			= 18,
	CTP_STAT_INSTRUMENTSTATUS			= 19,
	CTP_STAT_PARKEDORDER				= 20,
	CTP_STAT_ORDER						= 21,
	CTP_STAT_TRADE						= 22,
	CTP_STAT_POSITION					= 23,
	CTP_STAT_POSITIONDETAIL				= 24,
	CTP_STAT_MAX,
};

//typedef CThostFtdcExchangeField CThostFtdcExchangeFieldEx;
struct CThostFtdcExchangeFieldEx : public CThostFtdcExchangeField
{
	TThostFtdcDateType Date;
	TThostFtdcTimeType Time;

	CThostFtdcExchangeFieldEx();
	CThostFtdcExchangeFieldEx(const CThostFtdcExchangeField& o);
	CThostFtdcExchangeFieldEx(const CThostFtdcExchangeFieldEx& o);

	template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & ExchangeID;
        ar & ExchangeName;
        ar & ExchangeProperty;
    }
};
struct CThostFtdcExchangeFieldExLess
{
	bool operator()(const CThostFtdcExchangeFieldEx& x, const CThostFtdcExchangeFieldEx& y) const
	{
		return stricmp(x.ExchangeID, y.ExchangeID) < 0;
	}
};

//////////////////////////////////////////////////////////////////////////

struct  CThostFtdcProductFieldEx : public CThostFtdcProductField
{
	int ExchangeIndex;
	int WareCount;
	int DecimalDigits;

	CThostFtdcProductFieldEx();
	CThostFtdcProductFieldEx(const CThostFtdcProductField& o, int Index);
	CThostFtdcProductFieldEx(const CThostFtdcProductFieldEx& o);

	template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & ProductID;
        ar & ProductName;
        ar & ExchangeID;
        ar & ProductClass;
        ar & VolumeMultiple;
        ar & PriceTick;
        ar & MaxMarketOrderVolume;
        ar & MinMarketOrderVolume;
        ar & MaxLimitOrderVolume;
        ar & MinLimitOrderVolume;
        ar & PositionType;
        ar & PositionDateType;
        ar & CloseDealType;
        ar & TradeCurrencyID;
        ar & MortgageFundUseRange;
        ar & ExchangeProductID;
        ar & UnderlyingMultiple;
        ar & ExchangeIndex;
        ar & WareCount;
        ar & DecimalDigits;
    }
};
struct CThostFtdcProductFieldExLess
{
	bool operator()(const CThostFtdcProductFieldEx& x, const CThostFtdcProductFieldEx& y) const
	{
		bool ret = false;
		int cmp = stricmp(x.ExchangeID, y.ExchangeID);
		if (cmp < 0) {
			ret = true;
		}
		else if (cmp == 0) {
			cmp = stricmp(x.ProductID, y.ProductID);
			if (cmp < 0) {
				ret = true;
			}
		}
		return ret;
	}
};

//////////////////////////////////////////////////////////////////////////

struct  CThostFtdcInstrumentFieldEx : public CThostFtdcInstrumentField
{
	int ProductIndex;
	int DecimalDigits;

	CThostFtdcInstrumentFieldEx();
	CThostFtdcInstrumentFieldEx(const CThostFtdcInstrumentField& o, int Index);
	CThostFtdcInstrumentFieldEx(const CThostFtdcInstrumentFieldEx& o);

	template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & InstrumentID;
        ar & ExchangeID;
        ar & InstrumentName;
        ar & ExchangeInstID;
        ar & ProductID;
        ar & ProductClass;
        ar & DeliveryYear;
        ar & DeliveryMonth;
        ar & MaxMarketOrderVolume;
        ar & MinMarketOrderVolume;
        ar & MaxLimitOrderVolume;
        ar & MinLimitOrderVolume;
        ar & VolumeMultiple;
        ar & PriceTick;
        ar & CreateDate;
        ar & OpenDate;
        ar & ExpireDate;
        ar & StartDelivDate;
        ar & EndDelivDate;
        ar & InstLifePhase;
	    ar & IsTrading;
	    ar & PositionType;
	    ar & PositionDateType;
	    ar & LongMarginRatio;
	    ar & ShortMarginRatio;
	    ar & MaxMarginSideAlgorithm;
	    ar & UnderlyingInstrID;
	    ar & StrikePrice;
	    ar & OptionsType;
	    ar & UnderlyingMultiple;
	    ar & CombinationType;
	    ar & MinBuyVolume;
	    ar & MinSellVolume;
	    ar & InstrumentCode;
    }
};
struct CThostFtdcInstrumentFieldExLess
{
	bool operator()(const CThostFtdcInstrumentFieldEx& x, const CThostFtdcInstrumentFieldEx& y) const
	{
		bool ret = false;
		int cmp = stricmp(x.ExchangeID, y.ExchangeID);
		if (cmp < 0) {
			ret = true;
		}
		else if (cmp == 0) {
			cmp = stricmp(x.ProductID, y.ProductID);
			if (cmp < 0) {
				ret = true;
			}
			else if (cmp == 0) {
				cmp = stricmp(x.InstrumentID, y.InstrumentID);
				if (cmp < 0) {
					ret = true;
				}
			}
		}
		return ret;
	}
};

//////////////////////////////////////////////////////////////////////////

struct  CThostFtdcDepthMarketDataFieldEx : public CThostFtdcDepthMarketDataField
{
	unsigned long Date;
	unsigned long Time;
	TThostFtdcVolumeType LastVolume;
	TThostFtdcMoneyType LastAmount;
	TThostFtdcVolumeType BidVolDiff1;
	TThostFtdcVolumeType AskVolDiff1;
	TThostFtdcVolumeType BidVolDiff2;
	TThostFtdcVolumeType AskVolDiff2;
	TThostFtdcVolumeType BidVolDiff3;
	TThostFtdcVolumeType AskVolDiff3;
	TThostFtdcVolumeType BidVolDiff4;
	TThostFtdcVolumeType AskVolDiff4;
	TThostFtdcVolumeType BidVolDiff5;
	TThostFtdcVolumeType AskVolDiff5;

	CThostFtdcDepthMarketDataFieldEx();
	CThostFtdcDepthMarketDataFieldEx(const CThostFtdcDepthMarketDataField& o);
	CThostFtdcDepthMarketDataFieldEx(const CThostFtdcDepthMarketDataFieldEx& o);

	void UpdateNewTime(const TThostFtdcTimeType & NewTime);

	template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        
    }
};

//typedef CThostFtdcInstrumentStatusField CThostFtdcInstrumentStatusFieldEx;
struct CThostFtdcInstrumentStatusFieldEx : public CThostFtdcInstrumentStatusField
{
	CThostFtdcInstrumentStatusFieldEx();
	CThostFtdcInstrumentStatusFieldEx(const CThostFtdcInstrumentStatusField& o);
	CThostFtdcInstrumentStatusFieldEx(const CThostFtdcInstrumentStatusFieldEx& o);

	template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        
    }
};
