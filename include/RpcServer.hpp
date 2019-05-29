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
		boost::property_tree::ptree& cfg = theApp.cfg();

    	const size_t io_thread_num = cfg.get<size_t>("rpc_up.io_thread_num", 1);
		Base::start(io_thread_num);
		
    	const size_t port = cfg.get<size_t>("rpc_up.port");
			Base::listen(port);
        return true;
	}

	void stop()
	{
        
		Base::stop();
	}
};

}

