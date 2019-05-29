#pragma once
#ifndef __H_SRC_RPC_IMPL_HPP__
#define __H_SRC_RPC_IMPL_HPP__

#include <IpcBase.hpp>
#include <DataSetConvEx.hpp>

namespace MTP {

template<class Server, class TBase>
class RpcServerImpl : public TBase
{
	typedef RpcServerImpl<Server,TBase> This;
	typedef TBase Base;
protected:
	Server* srv_;
	std::shared_ptr<boost::property_tree::ptree> result_;
public:
	RpcServerImpl(Server* srv):Base(),srv_(srv)
	{
		result_ = std::make_shared<boost::property_tree::ptree>();
	}

	~RpcServerImpl()
	{
	}

	inline std::string build_packet(const std::string& method, IDataSet* dataset)
	{
		std::string str;
		DataSetConvEx conv;
			// boost::property_tree::ptree tmp;
			// conv.to_xml(tmp, dataset + i);
			// CDataSet tmpset;
			// conv.from_xml(tmp, &tmpset);
			// tmp.clear();
			// conv.to_xml(tmp, &tmpset);
			// XUtil::json_to_str(str,tmp);
        build_request(str, method, conv.to_json(dataset));
		return str;
	}

	void post_exchange_update(IDataSet* dataset)
	{
		sendRemoteNotification(0, build_packet("on_exchange_update", dataset));
	}

	inline void post_product_update(IDataSet* dataset)
	{
		sendRemoteNotification(0, build_packet("on_product_update", dataset));
	}

	inline void post_commodity_update(IDataSet* dataset)
	{
		sendRemoteNotification(0, build_packet("on_commodity_update", dataset));
	}

	inline void post_response(size_t peer, size_t id, const std::shared_ptr<boost::property_tree::ptree>& data, bool error = false)
	{
		// std::string params;
		// if(dataset) {
		// 	DataSetConvEx conv;
		// 	params = conv.to_json_ex(dataset);
		// }
		// _sendResponse(peer, id, params, error);
		_sendResponse(peer, id, data, error);
	}

	inline void post_notification(size_t peer, const std::string method, const std::shared_ptr<boost::property_tree::ptree>& params)
	{
		// std::string str;
		// std::string params;
		// if(dataset) {
		// 	DataSetConvEx conv;
		// 	params = conv.to_json_ex(dataset);
		// }
		// build_request(str, method, params);
		// sendRemoteNotification(0, str);
		sendRemoteNotification(peer, method, params);
	}

	void _handleRequest (size_t peer, const std::string& method, const std::shared_ptr<boost::property_tree::ptree>& params, size_t id) {
		srv_->on_request(peer, method, params, id);
	}

	// bool didReceiveCall (size_t peer, const std::string& method, const std::shared_ptr<boost::property_tree::ptree>& params, std::shared_ptr<boost::property_tree::ptree>& result) {
    //     return srv_->didReceiveCall(peer, method, params, result);
    // }
};

}

#endif //__H_SRC_RPC_IMPL_HPP__
