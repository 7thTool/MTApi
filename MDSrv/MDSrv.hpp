#pragma once

#include <XUtil/XServer.hpp>
#include <XNet/XServer.hpp>
#include "RegHandler.hpp"
#include "RegDBHandler.hpp"
#include "RegRedisHandler.hpp"
#include "../RegToken/RegToken.hpp"


class RegSrv
	: public XUtil::XServer<RegSrv>
	, public XNet::XServerT<RegSrv, RegSrv>
	, public XNet::XServerT<RegSrv, RegSrv>::Listener
	, public RegHandler::Server
	, public RegDBHandler::Server
	, public RegRedisHandler::Server
{
	typedef XUtil::XServer<RegSrv> Base;
	typedef XNet::XServerT<RegSrv, RegSrv> NetBase;
  private:
	boost::asio::deadline_timer timer_;
	std::shared_ptr<RegDBHandler> db_handler_;
	std::shared_ptr<RegRedisHandler> redis_handler_;
	std::shared_ptr<RegToken> token_;
	std::vector<std::shared_ptr<RegHandler>> handlers_;
	enum Status { STOP, INIT, RUN };
	Status status_;
  public:
	RegSrv() : Base(), timer_(service()), status_(Status::STOP)
	{
    	
	}

	const char *name()
	{
		return "RegSrv";
	}

	void init(int argc, char *argv[])
	{
		status_ = Status::INIT;

		Base::init(argc, argv);

		boost::property_tree::ptree& cfg = theApp->cfg();
		auto opt = cfg.get_child_optional("token");
		if (!opt) {
			return;
		}
		boost::property_tree::ptree token_cfg = opt.get();
		token_ = std::make_shared<RegToken>(token_cfg);

		db_handler_ = std::make_shared<RegDBHandler>(this);
		db_handler_->start();

		redis_handler_ = std::make_shared<RegRedisHandler>(this);
		redis_handler_->start();

		handlers_.resize(5);
		for (size_t i = 0; i < handlers_.size(); i++)
		{
			handlers_[i] = std::make_shared<RegHandler>(this);
			handlers_[i]->start(db_handler_.get(), redis_handler_.get(), token_.get());
		}
		
		NetBase::start(5);
		NetBase::listen(8080);

		register_timer();

		status_ = Status::RUN;
	}

	void term()
	{
		Base::term();
	}

	inline bool is_run() { return status_ == Status::RUN; }
	
	void stop()
	{
		status_ = Status::STOP;

		unregister_timer();

		Base::stop();

		for (size_t i = 0; i < handlers_.size(); i++)
		{
			handlers_[i]->stop();
		}
		redis_handler_->stop();
		db_handler_->stop();

		handlers_.clear();
		redis_handler_.reset();
		db_handler_.reset();

		NetBase::stop();
	}

  protected:
	void register_timer() 
	{
		timer_.expires_from_now(boost::posix_time::milliseconds(200));
		timer_.async_wait(std::bind(&RegSrv::on_timer, this));
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

	inline std::shared_ptr<RegHandler> select_handler(size_t peer) {
		return handlers_[peer % handlers_.size()];
	}

public:
//RegHandler::Server
	//基础接口
	virtual boost::asio::io_service& service() { return Base::service(); }

	//网络接口
	virtual void link_close(size_t peer) { NetBase::close(peer); }
	virtual void link_send(size_t peer, const char* buf, size_t len) { NetBase::post_packet(peer, buf, len); }

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
		std::shared_ptr<RegHandler> handler = select_handler(peer);
		service().post(boost::bind(&RegHandler::on_link_open, handler, peer));
	}

	void on_ws_read(ws_ptr peer_ptr, std::string &buffer)
	{
		if(!is_run()) {
			return;
		}
		size_t peer = peer_ptr->id();
		XNet::LOG4D("on_ws_read: %s", buffer.c_str());
		std::shared_ptr<RegHandler> handler = select_handler(peer);
		service().post(boost::bind(&RegHandler::on_link_message, handler, peer, buffer));
		peer_ptr->do_read();
	}
	
	void on_ws_close(ws_t *peer_ptr) 
	{
		if(!is_run()) {
			return;
		}
		size_t peer = peer_ptr->id();
		XNet::LOG4D("on_ws_close: %d", peer);
		std::shared_ptr<RegHandler> handler = select_handler(peer);
		service().post(boost::bind(&RegHandler::on_link_close, handler, peer));
	}
#endif
};

