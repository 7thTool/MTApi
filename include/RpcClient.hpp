#pragma once

#include <RpcBaser.hpp>

namespace MTP
{

template <class T>
class RpcClient
		: public RpcBaser<T>
{
	typedef RpcBaser<T> Base;
protected:
    size_t peer_ = 0;
public:
	RpcClient() : Base()
	{
	}

	bool start(char *xml, int xmlflag = 0)
	{
		boost::property_tree::ptree &cfg = theApp.cfg();
		
		Base::start(1);

		auto opt_rpc_down = cfg.get_child_optional("rpc_down");
		if (opt_rpc_down) {
			const boost::property_tree::ptree &rpc_down = opt_rpc_down.get();
			BOOST_FOREACH (const boost::property_tree::ptree::value_type &one, rpc_down)
			{
				const std::string ip = one.second.get<std::string>("ip");
				const unsigned short port = one.second.get<unsigned short>("port");
				const size_t timeout = one.second.get<size_t>("timeout", 3000);
				auto peer_ptr = Base::connect<ws_clt_t>(ip, port, timeout);
				if(peer_ptr) {
					peer_ = peer_ptr->id();
				}
				break;
			}
		} else {
			ASSERT(0);
		}
		return true;
	}

	void stop()
	{
		Base::stop();
	}
};

} // namespace MTP
