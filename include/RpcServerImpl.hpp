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

	inline void post_add_exchange(const std::vector<ExchangeInfoPtr> exchanges)
	{
		std::string str;
		std::stringstream ss;
		ss << R"([)";
		size_t i = 0;
		for(auto exchange : exchanges)
		{
			if(i++ != 0) {
				ss << R"(,)";
			}
			ExchangeSetWrapper<IDataSet,ExchangeInfo*,ProductInfo*,CommodityInfo*> exchange_set(exchange.get());
			DataSetConvEx conv;
			boost::property_tree::ptree tmp;
			conv.to_xml(tmp, &exchange_set);
			CDataSet dataset;
			conv.from_xml(tmp, &dataset);
			tmp.clear();
			conv.to_xml(tmp, &dataset);
			std::string str;
			XUtil::json_to_str(str,tmp);
			ss << conv.to_json(&exchange_set);
		}
		ss << R"(])";
        build_request(str, "on_add_exchange", ss.str());
		sendRemoteNotification(0, str);
	}

	inline void post_remove_exchange(const std::vector<ExchangeInfoPtr> exchanges)
	{
		std::string str;
		std::stringstream ss;
		ss << R"([)";
		size_t i = 0;
		for (auto exchange : exchanges)
		{
			if (i++ != 0) {
				ss << R"(,)";
			}
			ExchangeSetWrapper<IDataSet, ExchangeInfo*, ProductInfo*, CommodityInfo*> exchange_set(exchange.get());
			DataSetConvEx conv;
			ss << conv.to_json(&exchange_set);
		}
		ss << R"(])";
		build_request(str, "on_remove_exchange", ss.str());
		sendRemoteNotification(0, str);
	}

	inline void post_commodity_update(const CommodityInfoPtr& commodity)
	{
		std::string str;
		CommoditySetWrapper<IDataSet, CommodityInfo*> commodity_set(commodity.get());
		DataSetConvEx conv;
		build_request(str, "on_commodity_update", conv.to_json(&commodity_set));
		sendRemoteNotification(0, str);
	}

	inline void post_commodity_status(const CommodityInfoPtr& commodity)
	{
		std::string str;
		CommoditySetWrapper<IDataSet, CommodityInfo*> commodity_set(commodity.get());
		DataSetConvEx conv;
		build_request(str, "on_commodity_status", conv.to_json(&commodity_set));
		sendRemoteNotification(0, str);
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
