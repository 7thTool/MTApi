#pragma once

#include <XNet/XServer.hpp>
#include <RpcBase.hpp>

namespace MTP {

	static const char* RPC_NAME = "ipc";
	static const char* RPC_SERVER_NAME = "ipc_server";
	static const char* RPC_CLIENT_NAME = "ipc_client";

template<class T>
class RpcBaser
	: public XNet::XServerT<RpcBaser<T>, RpcBaser<T>>
	, public XNet::XServerT<RpcBaser<T>, RpcBaser<T>>::Listener
    , public XUtil::XXmlRpc<T>
{
	typedef XNet::XServerT<RpcBaser<T>, RpcBaser<T>> NetBase;
  private:
	//std::atomic<bool> stop_flag_ = true;
  public:
	RpcBaser() : NetBase(this)
	{
    	
	}

public:
	inline void on_recv(size_t peer, const std::string& msg)
	{
		T* pT = static_cast<T*>(this);
		//调用Rpc
        pT->_handleMessage((size_t)peer, msg);
	}

    void _sendMessage (size_t peer, const std::string& msg) {
		if(peer) {
			post_packet(peer, msg.data(), msg.size());
		} else {
			broadcast_packet(msg.data(), msg.size());
		}
    }

//protected:
	//以下是XServer线程回调，运行在网络线程里
#if XSERVER_PROTOTYPE_HTTP || XSERVER_PROTOTYPE_HTTPS
		void on_http_accept(http_ptr peer_ptr) {}
		// This function produces an HTTP response for the given
		// request. The type of the response object depends on the
		// contents of the request, so the interface requires the
		// caller to pass a generic lambda for receiving the response.
		template <class Body, class Allocator>
		void on_http_read(http_ptr peer_ptr,
						boost::beast::string_view doc_root,
						boost::beast::http::request<Body, boost::beast::http::basic_fields<Allocator>> &&req
						) 
		{
			if(!is_run()) {
				return;
			}
			size_t peer = peer_ptr->id();
			auto buffer = req.body();
			LOG4D("on_ws_read: %s", buffer.c_str());
			T* pT = static_cast<T*>(this);
			//调用Rpc
			pT->on_recv((size_t)peer, buffer);
			//std::shared_ptr<RpcHandler> handler = select_rpc_handler(peer);
			//handler->service().post(boost::bind(&RpcHandler::on_link_message, handler, peer, buffer));
			//peer_ptr->do_read();
		}
		void on_http_close(http_t *peer_ptr) 
		{
			//
		}
#endif
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
		//std::shared_ptr<RpcHandler> handler = select_rpc_handler(peer);
		//handler->service().post(boost::bind(&RpcHandler::on_link_open, handler, peer));
	}

	void on_ws_read(ws_ptr peer_ptr, std::string &buffer)
	{
		if(!is_run()) {
			return;
		}
		size_t peer = peer_ptr->id();
		LOG4D("on_ws_read: %s", buffer.c_str());
		T* pT = static_cast<T*>(this);
		//调用Rpc
		pT->on_recv((size_t)peer, buffer);
		//std::shared_ptr<RpcHandler> handler = select_rpc_handler(peer);
		//handler->service().post(boost::bind(&RpcHandler::on_link_message, handler, peer, buffer));
		peer_ptr->do_read();
	}
	
	void on_ws_close(ws_t *peer_ptr) 
	{
		if(!is_run()) {
			return;
		}
		size_t peer = peer_ptr->id();
		LOG4D("on_ws_close: %d", peer);
		//std::shared_ptr<RpcHandler> handler = select_rpc_handler(peer);
		//handler->service().post(boost::bind(&RpcHandler::on_link_close, handler, peer));
	}
	void on_ws_clt_connect(ws_clt_ptr peer_ptr) 
	{
		if(!is_run()) {
			return;
		}
		size_t peer = peer_ptr->id();
		LOG4D("on_ws_clt_connect: %d", peer);
	}
		void on_ws_clt_read(ws_clt_ptr peer_ptr, std::string &buffer) 
		{
			if(!is_run()) {
				return;
			}
			size_t peer = peer_ptr->id();
			LOG4D("on_ws_clt_read: %d", peer);
            T* pT = static_cast<T*>(this);
            //调用Rpc
            pT->on_recv((size_t)peer, buffer);
            peer_ptr->do_read();
		}
		void on_ws_clt_write(ws_clt_ptr peer_ptr, std::string &buffer) 
		{
            if(!is_run()) {
				return;
			}
			size_t peer = peer_ptr->id();
			LOG4D("on_ws_clt_write: %d", peer);
		}
		void on_ws_clt_close(ws_clt_t *peer_ptr) 
		{
            if(!is_run()) {
				return;
			}
			size_t peer = peer_ptr->id();
			LOG4D("on_ws_clt_close: %d", peer);
		}
#endif
};

}

