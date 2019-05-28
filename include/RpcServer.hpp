#pragma once

#include <RpcBaser.hpp>

namespace MTP {

template<class T>
class RpcServer
	: public RpcBaser<T>
{
	typedef RpcBaser<T> Base;
  public:
	RpcServer() : Base()
	{
    	
	}

	bool start(char* xml, int xmlflag = 0)
	{
        Base::start(xml, xmlflag);

		boost::property_tree::ptree& cfg = theApp.cfg();

    	const size_t port = cfg.get<size_t>("rpc.port");
		Base::listen(port);
        return true;
	}

	void stop()
	{
        
		Base::stop();
	}
};

}

