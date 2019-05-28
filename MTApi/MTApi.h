#pragma once
#ifndef __H_MTAPI_H__
#define __H_MTAPI_H__

#include <IMTApi.h>
#include <XUtil/XServer.hpp>
#include <XUtil/XService.hpp>
#include <XUtil/XXmlRpc.hpp>
#include <RpcClient.hpp>
#include <IpcClient.hpp>
#include <RpcClientImpl.hpp>

namespace MTP {

class CMTApi 
    : public XUtil::XModuleServer<CMTApi>
    , public IMTApi
{
    typedef XUtil::XModuleServer<CMTApi> Base;
    typedef XNet::XServerT<CMTApi, CMTApi> NetBase;
protected:
	//IMTSpi* spi_ = nullptr;
	class ApiRpcClient : public RpcClientImpl<CMTApi,RpcClient<ApiRpcClient>>
	{
		typedef RpcClientImpl<CMTApi,RpcClient<ApiRpcClient>> Base;
	public:
		using Base::Base;
	};
	class ApiIpcClient : public RpcClientImpl<CMTApi,IpcClient<ApiIpcClient>>
	{
		typedef RpcClientImpl<CMTApi,IpcClient<ApiIpcClient>> Base;
	public:
		using Base::Base;
	};
    //typedef IpcClient<CMTApi> ApiIpcClient;
    std::shared_ptr<ApiRpcClient> rpc_client_;
    std::shared_ptr<ApiIpcClient> ipc_client_;
	UpDownMode down_mode_;
public:
    CMTApi();
    virtual ~CMTApi();
    
    virtual bool Start(char* xml, int xmlflag = 0);
    virtual void Stop();

	inline UpDownMode down_mode() { return down_mode_; }

protected:
    //Service 线程内运行
    void on_start();
    void on_stop();

public:
    //行情变化推送
	void on_add_exchange(const std::shared_ptr<boost::property_tree::ptree>& params);
	void on_remove_exchange(const std::shared_ptr<boost::property_tree::ptree>& params);
	void on_commodity_update(const std::shared_ptr<boost::property_tree::ptree>& params);
	void on_commodity_status(const std::shared_ptr<boost::property_tree::ptree>& params);

    //交易变化消息
	void on_add_user(const std::shared_ptr<boost::property_tree::ptree>& params);
	void on_remove_user(const std::shared_ptr<boost::property_tree::ptree>& params);
    void on_update_user(const std::shared_ptr<boost::property_tree::ptree>& params);
	void on_update_order(const std::shared_ptr<boost::property_tree::ptree>& params);
	void on_update_trade(const std::shared_ptr<boost::property_tree::ptree>& params);

	//主动请求
	void on_request(const std::string& method, const std::shared_ptr<boost::property_tree::ptree>& params);
};

extern CMTApi theMTApi;

}

#endif // __H_MTAPI_H__
