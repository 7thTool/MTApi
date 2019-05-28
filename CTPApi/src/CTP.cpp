// CTP.cpp : implementation file
//

//#include "stdafx.h"
#include <CTP.h>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/format.hpp>
#include <sstream>

CTPMdApi::CTPMdApi(const CTPUserLoginInfo& userInfo, CThostFtdcMdSpi* pSpi) :m_UserInfo(userInfo), m_pApi(NULL), m_pSpi(pSpi ? pSpi : this)
{
	std::string fullpath = boost::filesystem::initial_path<boost::filesystem::path>().string();
	std::ostringstream ss;
	ss << fullpath << (char)boost::filesystem::path::preferred_separator << userInfo.BrokerID << (char)boost::filesystem::path::preferred_separator << userInfo.UserID;
	boost::filesystem::path flowpath(ss.str());
	boost::filesystem::create_directories(flowpath);
	boost::filesystem::path currentpath = boost::filesystem::current_path();
	boost::filesystem::current_path(flowpath);
	m_pApi = CThostFtdcMdApi::CreateFtdcMdApi();
	boost::filesystem::current_path(currentpath);
	for(size_t i=0; i<m_UserInfo.MdFrontAddress.size(); i++)
	{
		m_pApi->RegisterFront((char*)m_UserInfo.MdFrontAddress[i].c_str());
	}
	for(size_t i=0; i<m_UserInfo.MdNsAddress.size(); i++)
	{
		m_pApi->RegisterNameServer((char*)m_UserInfo.MdNsAddress[i].c_str());
	}

	m_pApi->RegisterSpi(this);

	// 使客户端开始与行情发布服务器建立连接
	m_pApi->Init();
}

CTPMdApi::~CTPMdApi()
{
	if (m_pApi) {
		CThostFtdcMdApi * pApi = m_pApi;
		m_pApi = NULL;
		if (pApi) {
			pApi->Release();
			pApi = NULL;
		}
	}
}

void CTPMdApi::OnFrontConnected()
{
	if (m_pApi) {
		m_pApi->ReqUserLogin(&m_UserInfo, 0);

		//if (m_pSpi)
		//	m_pSpi->OnFrontConnected();
	}
}

void CTPMdApi::OnFrontDisconnected(int nReason)
{
	//if (m_pSpi)
	//	m_pSpi->OnFrontDisconnected(nReason);
}


CThostFtdcMdApi* CTPMdApi::getApi()
{
	return m_pApi;
}

CTPTraderApi::CTPTraderApi(const CTPUserLoginInfo& userInfo, CThostFtdcTraderSpi* pSpi):m_UserInfo(userInfo),m_pApi(NULL),m_pSpi(pSpi?pSpi:this)
{
	std::string fullpath = boost::filesystem::initial_path<boost::filesystem::path>().string();
	std::ostringstream ss;
	ss << fullpath << (char)boost::filesystem::path::preferred_separator << userInfo.BrokerID << (char)boost::filesystem::path::preferred_separator << userInfo.UserID;
	boost::filesystem::path flowpath(ss.str());
	boost::filesystem::create_directories(flowpath);
	boost::filesystem::path currentpath = boost::filesystem::current_path();
	boost::filesystem::current_path(flowpath);
	m_pApi = CThostFtdcTraderApi::CreateFtdcTraderApi();
	boost::filesystem::current_path(currentpath);
	for(size_t i=0; i<m_UserInfo.FrontAddress.size(); i++)
	{
		m_pApi->RegisterFront((char*)m_UserInfo.FrontAddress[i].c_str());
	}
	for(size_t i=0; i<m_UserInfo.NsAddress.size(); i++)
	{
		m_pApi->RegisterNameServer((char*)m_UserInfo.NsAddress[i].c_str());
	}

	m_pApi->SubscribePublicTopic(THOST_TERT_QUICK); //这样可以更新到最新的合约状态
	m_pApi->SubscribePrivateTopic(THOST_TERT_QUICK); //交易委托、成交、持仓通过请求+推送更新到最新

	m_pApi->RegisterSpi(this);

	// 使客户端开始与行情发布服务器建立连接
	m_pApi->Init();
}

CTPTraderApi::~CTPTraderApi()
{
	if (m_pApi) {
		m_pApi->Release();
		m_pApi = NULL;
	}
}

CThostFtdcTraderApi* CTPTraderApi::getApi()
{
	return m_pApi;
}

void CTPTraderApi::OnFrontConnected()
{
	if (m_pSpi)
		m_pSpi->OnFrontConnected();

	m_pApi->ReqUserLogin(&m_UserInfo, 0);
}

void CTPTraderApi::OnFrontDisconnected(int nReason)
{
	if (m_pSpi)
		m_pSpi->OnFrontDisconnected(nReason);
}


