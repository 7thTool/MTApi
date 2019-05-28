//#include "stdafx.h"
#include <CTPCommon.h>
#include <XUtil/XDateTime.hpp>
#include <boost/property_tree/ptree.hpp>  
#include <boost/property_tree/json_parser.hpp>  
#include <boost/property_tree/xml_parser.hpp>  
#include <boost/typeof/typeof.hpp>  
#include <boost/foreach.hpp>  
#include <boost/locale.hpp>
#include <utility>

#define CTP_API_REQ_LOGIN	"ReqLogin"

bool parseUserInfoFromString(CTPUserLoginInfo& userInfo, const std::string& jsonString)
{
	try
	{
		boost::property_tree::ptree jsonObject;
		if (XUtil::json_from_str(jsonString, jsonObject)) {
			///RequestApi
			std::string RequestApi = jsonObject.get<std::string>("RequestApi");
			if (RequestApi != CTP_API_REQ_LOGIN) {
				return false;
			}
			//获取Data
			const boost::property_tree::ptree & jsonData = jsonObject.get_child("Data");
			///经纪公司代码
			snprintf(userInfo.BrokerID, _countof(userInfo.BrokerID), "%s", jsonData.get<std::string>("BrokerID","").c_str());
			///用户代码
			snprintf(userInfo.UserID, _countof(userInfo.UserID), "%s", jsonData.get<std::string>("UserID","").c_str());
			///密码
			snprintf(userInfo.Password, _countof(userInfo.Password), "%s", jsonData.get<std::string>("Password","").c_str());
			///用户端产品信息
			snprintf(userInfo.UserProductInfo, _countof(userInfo.UserProductInfo), "%s", jsonData.get<std::string>("UserProductInfo", "").c_str());
			///接口端产品信息
			snprintf(userInfo.InterfaceProductInfo, _countof(userInfo.InterfaceProductInfo), "%s", jsonData.get<std::string>("InterfaceProductInfo", "").c_str());
			///协议信息
			snprintf(userInfo.ProtocolInfo, _countof(userInfo.ProtocolInfo), "%s", jsonData.get<std::string>("ProtocolInfo", "").c_str());
			///Mac地址
			snprintf(userInfo.MacAddress, _countof(userInfo.MacAddress), "%s", jsonData.get<std::string>("MacAddress", "").c_str());
			///动态密码
			snprintf(userInfo.OneTimePassword, _countof(userInfo.OneTimePassword), "%s", jsonData.get<std::string>("OneTimePassword", "").c_str());
			///终端IP地址
			snprintf(userInfo.ClientIPAddress, _countof(userInfo.ClientIPAddress), "%s", jsonData.get<std::string>("ClientIPAddress", "").c_str());
			///登录备注
			snprintf(userInfo.LoginRemark, _countof(userInfo.LoginRemark), "%s", jsonData.get<std::string>("LoginRemark", "").c_str());
			///前置机服务器网络地址
			auto opt_front = jsonData.get_child_optional("FrontAddress");
			if (opt_front) {
				BOOST_FOREACH(const boost::property_tree::ptree::value_type &cfg_front, opt_front.get())
				{
					userInfo.FrontAddress.push_back(cfg_front.second.get_value<std::string>());
				}
			}
			///名字服务器网络地址
			auto opt_nsaddr = jsonData.get_child_optional("NsAddress");
			if (opt_nsaddr) {
				BOOST_FOREACH(const boost::property_tree::ptree::value_type &cfg_addr, opt_nsaddr.get())
				{
					userInfo.NsAddress.push_back(cfg_addr.second.get_value<std::string>());
				}
			}
			///登录模式
			userInfo.LoginMode = jsonData.get<TThostFtdcLoginModeType>("LoginMode", (char)0);
			///认证码
			snprintf(userInfo.AuthCode, _countof(userInfo.AuthCode), "%s", jsonData.get<std::string>("AuthCode", "").c_str());
			///行情前置机服务器网络地址
			auto opt_md_front = jsonData.get_child_optional("MdFrontAddress");
			if (opt_md_front) {
				BOOST_FOREACH(const boost::property_tree::ptree::value_type &cfg_md_front, opt_md_front.get())
				{
					userInfo.MdFrontAddress.push_back(cfg_md_front.second.get_value<std::string>());
				}
			}
			///行情名字服务器网络地址
			auto opt_md_nsaddr = jsonData.get_child_optional("MdNsAddress");
			if (opt_nsaddr) {
				BOOST_FOREACH(const boost::property_tree::ptree::value_type &cfg_md_addr, opt_md_nsaddr.get())
				{
					userInfo.MdNsAddress.push_back(cfg_md_addr.second.get_value<std::string>());
				}
			}
			if (userInfo.BrokerID[0] && userInfo.UserID[0] && userInfo.Password[0] && !userInfo.FrontAddress.empty())
			{
				return true;
			}
		}
	}
	catch (...)
	{
	}
	return false;
}


//////////////////////////////////////////////////////////////////////////

// 是否收到成功的响应
bool IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo)
{
	if (pRspInfo)
	{
		// 如果ErrorID != 0, 说明收到了错误的响应
		if (pRspInfo->ErrorID != 0)
		{
			PRINTF("%s\n", pRspInfo->ErrorMsg);
			return true;
		}
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////

unsigned long CTPDate2Date(const TThostFtdcDateType& Date)
{
	//20180302
	return XUtil::strto<unsigned long>(Date);
}

unsigned long CTPTime2Time(const TThostFtdcTimeType& Time)
{
	//13:12:12
	unsigned long HH = 0, MM = 0, SS = 0;
	//Time[2] = Time[5] = 0;
	HH = XUtil::strto<unsigned long>(&Time[0]);
	ASSERT(HH >= 0 && HH <= 23);
	MM = XUtil::strto<unsigned long>(&Time[3]);
	ASSERT(MM >= 0 && MM <= 59);
	SS = XUtil::strto<unsigned long>(&Time[6]);
	ASSERT(SS >= 0 && SS <= 60);
	//Time[2] = Time[5] = ':';
	return XUtil::MakeTime(HH, MM, SS);
}

//////////////////////////////////////////////////////////////////////////

CThostFtdcExchangeFieldEx::CThostFtdcExchangeFieldEx() 
{
	memset(this, 0, sizeof(CThostFtdcExchangeFieldEx));
}

CThostFtdcExchangeFieldEx::CThostFtdcExchangeFieldEx(const CThostFtdcExchangeField& o) 
{
	memcpy(this, &o, sizeof(o));
}

CThostFtdcExchangeFieldEx::CThostFtdcExchangeFieldEx(const CThostFtdcExchangeFieldEx& o) 
{
	memcpy(this, &o, sizeof(o));
}

//////////////////////////////////////////////////////////////////////////

CThostFtdcProductFieldEx::CThostFtdcProductFieldEx() :ExchangeIndex(-1), WareCount(0), DecimalDigits(DefaultDecimal) {
	memset((CThostFtdcProductField*)this, 0, sizeof(CThostFtdcProductField));
}

CThostFtdcProductFieldEx::CThostFtdcProductFieldEx(const CThostFtdcProductField& o, int Index) : ExchangeIndex(Index), WareCount(0), DecimalDigits(DefaultDecimal) {
	memcpy((CThostFtdcProductField*)this, &o, sizeof(CThostFtdcProductField));
	DecimalDigits = GetDecimalDigits(PriceTick);
}
CThostFtdcProductFieldEx::CThostFtdcProductFieldEx(const CThostFtdcProductFieldEx& o) {
	memcpy(this, &o, sizeof(CThostFtdcProductFieldEx));
}

//////////////////////////////////////////////////////////////////////////

CThostFtdcInstrumentFieldEx::CThostFtdcInstrumentFieldEx() :ProductIndex(-1), DecimalDigits(DefaultDecimal) {
	memset((CThostFtdcInstrumentField*)this, 0, sizeof(CThostFtdcInstrumentField));
}

CThostFtdcInstrumentFieldEx::CThostFtdcInstrumentFieldEx(const CThostFtdcInstrumentField& o, int Index) : ProductIndex(Index), DecimalDigits(DefaultDecimal) {
	memcpy((CThostFtdcInstrumentField*)this, &o, sizeof(CThostFtdcInstrumentField));
	DecimalDigits = GetDecimalDigits(PriceTick);
}

CThostFtdcInstrumentFieldEx::CThostFtdcInstrumentFieldEx(const CThostFtdcInstrumentFieldEx& o) {
	memcpy(this, &o, sizeof(CThostFtdcInstrumentFieldEx));
}

//////////////////////////////////////////////////////////////////////////

CThostFtdcDepthMarketDataFieldEx::CThostFtdcDepthMarketDataFieldEx()
	: Date(0), Time(0), LastVolume(0), LastAmount(0.)
	, BidVolDiff1(0)
	, AskVolDiff1(0)
	, BidVolDiff2(0)
	, AskVolDiff2(0)
	, BidVolDiff3(0)
	, AskVolDiff3(0)
	, BidVolDiff4(0)
	, AskVolDiff4(0)
	, BidVolDiff5(0)
	, AskVolDiff5(0) {
	memset((CThostFtdcDepthMarketDataField*)this, 0, sizeof(CThostFtdcDepthMarketDataField));
}

CThostFtdcDepthMarketDataFieldEx::CThostFtdcDepthMarketDataFieldEx(const CThostFtdcDepthMarketDataField& o)
	: Date(0), Time(0), LastVolume(0), LastAmount(0.)
	, BidVolDiff1(0)
	, AskVolDiff1(0)
	, BidVolDiff2(0)
	, AskVolDiff2(0)
	, BidVolDiff3(0)
	, AskVolDiff3(0)
	, BidVolDiff4(0)
	, AskVolDiff4(0)
	, BidVolDiff5(0)
	, AskVolDiff5(0) {
	memcpy((CThostFtdcDepthMarketDataField*)this, &o, sizeof(CThostFtdcDepthMarketDataField));
	Date = CTPDate2Date(TradingDay);
	Time = CTPTime2Time(UpdateTime);
	CHECKDATETIME(Date, Time);
}

CThostFtdcDepthMarketDataFieldEx::CThostFtdcDepthMarketDataFieldEx(const CThostFtdcDepthMarketDataFieldEx& o) {
	memcpy(this, &o, sizeof(CThostFtdcDepthMarketDataFieldEx));
}

void CThostFtdcDepthMarketDataFieldEx::UpdateNewTime(const TThostFtdcTimeType & NewTime) {
	memcpy(UpdateTime, NewTime, sizeof(TThostFtdcTimeType));
	Time = CTPTime2Time(UpdateTime);
}

//////////////////////////////////////////////////////////////////////////

CThostFtdcInstrumentStatusFieldEx::CThostFtdcInstrumentStatusFieldEx() 
{
	memset((CThostFtdcInstrumentStatusField*)this, 0, sizeof(CThostFtdcInstrumentStatusField));
}

CThostFtdcInstrumentStatusFieldEx::CThostFtdcInstrumentStatusFieldEx(const CThostFtdcInstrumentStatusField& o) 
{
	memcpy((CThostFtdcInstrumentStatusField*)this, &o, sizeof(CThostFtdcInstrumentStatusField));
}

CThostFtdcInstrumentStatusFieldEx::CThostFtdcInstrumentStatusFieldEx(const CThostFtdcInstrumentStatusFieldEx& o) 
{
	memcpy((CThostFtdcInstrumentStatusFieldEx*)this, &o, sizeof(CThostFtdcInstrumentStatusFieldEx));
}

//////////////////////////////////////////////////////////////////////////

