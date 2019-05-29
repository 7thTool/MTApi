#pragma once
#ifndef __H_MTAPI_H__
#define __H_MTAPI_H__

#include <IMTApi.h>
#include <IMDApi.h>
#include <ITDApi.h>
#include <XUtil/XServer.hpp>
#include <XUtil/XService.hpp>
#include <XUtil/XXmlRpc.hpp>

#include <RpcServer.hpp>
#include <IpcServer.hpp>
#include <RpcServerImpl.hpp>

#include <RpcClient.hpp>
#include <IpcClient.hpp>
#include <RpcClientImpl.hpp>

namespace MTP {

class CMTApi 
    : public XUtil::XModuleServer<CMTApi>
    , public IMTApi
	, public IMDSpi
	, public ITDSpi
{
    typedef XUtil::XModuleServer<CMTApi> Base;
    typedef XNet::XServerT<CMTApi, CMTApi> NetBase;
protected:
	class SrcRpcServer : public RpcServerImpl<CMTApi,RpcServer<SrcRpcServer>>
	{
		typedef RpcServerImpl<CMTApi,RpcServer<SrcRpcServer>> Base;
	public:
		using Base::Base;
	};
	class SrcIpcServer : public RpcServerImpl<CMTApi,IpcServer<SrcIpcServer>>
	{
		typedef RpcServerImpl<CMTApi,IpcServer<SrcIpcServer>> Base;
	public:
		using Base::Base;
	};
	std::shared_ptr<SrcRpcServer> rpc_server_;
	std::shared_ptr<SrcIpcServer> ipc_server_;
	UpDownMode up_mode_;
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
	IMDApi* md_api_ = nullptr;
    ITDApi* td_api_ = nullptr;
	UpDownMode down_mode_;
public:
    CMTApi();
    virtual ~CMTApi();
    
    virtual bool Start(char* xml, int xmlflag = 0);
    virtual void Stop();

	inline UpDownMode up_mode() { return up_mode_; }
	inline UpDownMode down_mode() { return down_mode_; }

protected:
    //Service 线程内运行
    void on_start();
    void on_stop();

public:
	//down_mode业务处理

	//IMDSpi 和 ITDSpi 回调
    //virtual void on_state_update(IDataSet* dataset) { }
	virtual void on_exchange_update(IDataSet* dataset);
	virtual void on_product_update(IDataSet* dataset);
    virtual void on_commodity_update(IDataSet* dataset);

	//IPC or RPC消息

    //行情变化推送
	void on_exchange_update(const std::shared_ptr<boost::property_tree::ptree>& params);
	void on_product_update(const std::shared_ptr<boost::property_tree::ptree>& params);
	void on_commodity_update(const std::shared_ptr<boost::property_tree::ptree>& params);

    //交易变化消息
	void on_add_user(const std::shared_ptr<boost::property_tree::ptree>& params);
	void on_remove_user(const std::shared_ptr<boost::property_tree::ptree>& params);
    void on_update_user(const std::shared_ptr<boost::property_tree::ptree>& params);
	void on_update_order(const std::shared_ptr<boost::property_tree::ptree>& params);
	void on_update_trade(const std::shared_ptr<boost::property_tree::ptree>& params);

public:
	//up_mode业务处理

	//IMDApi 和 ITDApi 调用
	void td_request (char* xml, int xmlflag);

	//主动请求
	void on_request (size_t peer, const std::string& method, const std::shared_ptr<boost::property_tree::ptree>& params, size_t id);
};

extern CMTApi theMTApi;

}

#endif // __H_MTAPI_H__
