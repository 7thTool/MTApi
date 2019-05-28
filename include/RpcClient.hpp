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
		Base::start(xml, xmlflag);

		boost::property_tree::ptree &cfg = theApp.cfg();

		const std::string ip = cfg.get<std::string>("rpc.ip", "");
		const unsigned short port = cfg.get<unsigned short>("rpc.port", 8848);
		const size_t timeout = cfg.get<size_t>("rpc.timeout", 3000);
		auto peer_ptr = Base::connect<ws_clt_t>(ip, port, timeout);
        if(peer_ptr) {
            peer_ = peer_ptr->id();
        }
		return true;
	}

	void stop()
	{
		Base::stop();
	}
};

} // namespace MTP
