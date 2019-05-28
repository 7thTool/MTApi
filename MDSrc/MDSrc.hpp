#pragma once

#include <XUtil/XServer.hpp>
#include <XNet/XServer.hpp>
#include <XUtil/XController.h>
#include <IMDSet.h>
#include <MDSrcRpcHandler.hpp>

namespace MTP {

class MDSrc
	: public XUtil::XServer<MDSrc>
	, public XNet::XServerT<MDSrc, MDSrc>
	, public XNet::XServerT<MDSrc, MDSrc>::Listener
{
	typedef XUtil::XServer<MDSrc> Base;
	typedef XNet::XServerT<MDSrc, MDSrc> NetBase;
	typedef MDSrcRpcHandler<MDSrc> RpcHandler;
  private:
	boost::asio::deadline_timer timer_;
	std::vector<std::shared_ptr<RpcHandler>> net_handlers_;
	enum Status { STOP, INIT, RUN };
	Status status_;
  public:
	MDSrc() : Base(), NetBase(this), timer_(service()), status_(Status::STOP)
	{
    	
	}

	const char *name()
	{
		return "MDSrc";
	}

	void init(int argc, char *argv[])
	{
		status_ = Status::INIT;

		Base::init(argc, argv);

		boost::property_tree::ptree& cfg = theApp.cfg();

		theCtl.start((char*)&cfg, (int)XUtil::XML_FLAG_PTREE);

		IMDSet::Instance().init((char*)&cfg, (int)XUtil::XML_FLAG_PTREE);

		net_handlers_.resize(5);
		for (size_t i = 0; i < net_handlers_.size(); i++)
		{
			net_handlers_[i] = std::make_shared<RpcHandler>(this);
			net_handlers_[i]->start();
		}
		
    	const size_t port = cfg.get<size_t>("server.port");
    	const size_t io_thread_num = cfg.get<size_t>("server.io_thread_num", 5);
		NetBase::start(io_thread_num);
		NetBase::listen(port);

		register_timer();

		status_ = Status::RUN;
	}

	void term()
	{
		IMDSet::Instance().term();
		Base::term();
	}

	inline bool is_run() { return status_ == Status::RUN; }
	
	void stop()
	{
		status_ = Status::STOP;

		unregister_timer();

		Base::stop();

		for (size_t i = 0; i < net_handlers_.size(); i++)
		{
			net_handlers_[i]->stop();
		}
		NetBase::stop();

		theCtl.stop();

		net_handlers_.clear();
	}

  protected:
	void register_timer() 
	{
		timer_.expires_from_now(boost::posix_time::milliseconds(200));
		timer_.async_wait(std::bind(&MDSrc::on_timer, this));
	}

	void unregister_timer()
	{
		boost::system::error_code ec;
		timer_.cancel(ec);
	}

	void on_timer()
	{
		if (!is_run()) {
			return;
		}

		register_timer();
	}

	inline std::shared_ptr<RpcHandler> select_net_handler(size_t peer) {
		return net_handlers_[peer % net_handlers_.size()];
	}

public:
	//基础接口
	inline boost::asio::io_service& service() { return Base::service(); }

	//网络接口
	inline void link_close(size_t peer) { NetBase::close(peer); }
	inline void link_send(size_t peer, const char* buf, size_t len) { NetBase::post_packet(peer, buf, len); }

//protected:
	//以下是XServer线程回调，运行在网络线程里
#if XSERVER_PROTOTYPE_HTTP || XSERVER_PROTOTYPE_HTTPS || XSERVER_PROTOTYPE_WEBSOCKET
	template <class Body, class Allocator>
	void
	on_ws_preaccept(ws_ptr peer_ptr, boost::beast::http::request<Body, boost::beast::http::basic_fields<Allocator>> &&req)
	{
		if(!is_run()) {
			return;
		}
		size_t peer = peer_ptr->id();
		std::cout << "on_ws_preaccept: " << req << std::endl;
		std::shared_ptr<RpcHandler> handler = select_net_handler(peer);
		service().post(boost::bind(&RpcHandler::on_link_open, handler, peer));
	}

	void on_ws_read(ws_ptr peer_ptr, std::string &buffer)
	{
		if(!is_run()) {
			return;
		}
		size_t peer = peer_ptr->id();
		LOG4D("on_ws_read: %s", buffer.c_str());
		std::shared_ptr<RpcHandler> handler = select_net_handler(peer);
		service().post(boost::bind(&RpcHandler::on_link_message, handler, peer, buffer));
		peer_ptr->do_read();
	}
	
	void on_ws_close(ws_t *peer_ptr) 
	{
		if(!is_run()) {
			return;
		}
		size_t peer = peer_ptr->id();
		LOG4D("on_ws_close: %d", peer);
		std::shared_ptr<RpcHandler> handler = select_net_handler(peer);
		service().post(boost::bind(&RpcHandler::on_link_close, handler, peer));
	}
#endif
};

}

