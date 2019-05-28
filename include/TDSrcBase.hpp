#pragma once

#pragma once

#include <XUtil/XApi.hpp>
#include <XUtil/XService.hpp>
#include <XUtil/XXmlRpc.hpp>
#include <XUtil/XIPC.hpp>
#include "TDBase.hpp"

namespace MTP
{

	const char* TD_SRC_HANDLER_NAME = "TDSrcHandler";

template<class T>
class TDSrcBase
	: public XUtil::XApi
	, public XUtil::XService
{
	typedef XUtil::XApi Base;
	typedef XUtil::XService Service;
protected:
	std::string trading_day_;
	std::string old_trading_day_;

  public:
	TDSrcBase() : Base(), Service()
	{

	}
	~TDSrcBase()
	{

	}

	bool start(char* xml, int xmlflag = 0)
	{
		bool ret = Base::init(xml, xmlflag);

		Service::start();

		return ret;
	}

	void stop()
	{
		Service::stop();
		
		Base::term();
	}

	inline boost::asio::io_service &service() { return Service::service(); }
};

} // namespace MTP
