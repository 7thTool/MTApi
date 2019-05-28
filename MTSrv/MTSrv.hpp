#pragma once

#include <XUtil/XServer.hpp>
#include <XNet/XServer.hpp>
//#include "../Token/Token.hpp"
#include <MTApiMgr.hpp>
#include "MTHandler.hpp"
#include <DataSetConvEx.hpp>

namespace MTP {

class MTSrv
	: public XUtil::XServer<MTSrv>
	, public CMTApiMgr<MTSrv>
	, public XNet::XServerT<MTSrv, MTSrv>
	, public XNet::XServerT<MTSrv, MTSrv>::Listener
{
	typedef XUtil::XServer<MTSrv> Base;
	typedef XNet::XServerT<MTSrv, MTSrv> NetBase;
	typedef CMTApiMgr<MTSrv> MTApi;
	typedef MTHandler<MTSrv> Handler;
  private:
	boost::asio::deadline_timer timer_;
	//std::shared_ptr<Token> token_;
	//MTApi api_;
	std::vector<std::shared_ptr<Handler>> handlers_;
	enum Status { STOP, INIT, RUN };
	Status status_;
	UpDownMode up_mode_;
	UpDownMode down_mode_;
	IExchangeSetPtr exchange_;
	IProductSetPtr product_;
	ICommoditySetPtr commodity_;
	ICalcDataSetPtr calcdata_;
	ICalculatorSetPtr calculator_;
	IBufferSetPtr buffer_;
  public:
	MTSrv() : Base(), MTApi(this), NetBase(this), timer_(service()), status_(Status::STOP), up_mode_(UpDownMode::WORK_MODE_RPC), down_mode_(UpDownMode::WORK_MODE_RPC)
	{
    	
	}

	const char *name()
	{
		return "MTSrv";
	}

	void init(int argc, char *argv[])
	{
		status_ = Status::INIT;

		Base::init(argc, argv);

		boost::property_tree::ptree& cfg = theApp.cfg();
		up_mode_ = (UpDownMode)cfg.get<int>("up_mode");
		down_mode_ = (UpDownMode)cfg.get<int>("down_mode");

		// auto opt = cfg.get_child_optional("token");
		// if (!opt) {
		// 	return;
		// }
		// boost::property_tree::ptree token_cfg = opt.get();
		// token_ = std::make_shared<RegToken>(token_cfg);

		MTApi::Start((char*)&cfg, (int)XUtil::XML_FLAG_PTREE);

		size_t max_thread_count = std::thread::hardware_concurrency();
		
		handlers_.resize(max_thread_count);
		for (size_t i = 0; i < handlers_.size(); i++)
		{
			handlers_[i] = std::make_shared<Handler>(this);
			handlers_[i]->start(
				//token_.get()
				);
		}

		NetBase::start(max_thread_count);
		NetBase::listen(1168);

		register_timer();

		status_ = Status::RUN;
	}

	void term()
	{
		Base::term();
	}

	inline UpDownMode up_mode() { return up_mode_; }
	inline UpDownMode down_mode() { return down_mode_; }

	inline bool is_run() { return status_ == Status::RUN; }
	
	void stop()
	{
		status_ = Status::STOP;

		unregister_timer();

		NetBase::stop();

		Base::stop();

		for (size_t i = 0; i < handlers_.size(); i++)
		{
			handlers_[i]->stop();
		}

		exchange_.reset();
		product_.reset();
		commodity_.reset();
		calcdata_.reset();
		MTApi::Stop();

		handlers_.clear();
	}

  protected:
	void register_timer() 
	{
		timer_.expires_from_now(boost::posix_time::milliseconds(200));
		timer_.async_wait(std::bind(&MTSrv::on_timer, this));
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

		exchange_ = Ref(XFE_CF);
		product_ = Ref(XFE_CF, "IF");
		commodity_ = Ref(XFE_CF, "IF", "IF1906");
		calcdata_ = Ref(XFE_CF, "IF", "IF1906", CYC_5SEC, DW_NONE);
		if(exchange_ && product_ && commodity_ && calcdata_) {
            InputSet input;
            calculator_ = Ref("MA", &input);
            buffer_ = Ref("MACD", &input, calcdata_.get());
			DataSetConv conv;
            std::cout << conv.to_json(exchange_.get()) << std::endl;
        	std::cout << conv.to_json(product_.get()) << std::endl;
        	std::cout << conv.to_json(commodity_.get()) << std::endl;
        	//std::cout << conv.to_json(calcdata_.get()) << std::endl;

			// CStrDataSet userset({{(size_t)"UserID",FIELD_TYPE_STRING},{(size_t)"Password",FIELD_TYPE_STRING},{(size_t)"BrokerID",FIELD_TYPE_STRING}
			// ,{(size_t)"FrontAddress",FIELD_TYPE_STRING},{(size_t)"MdFrontAddress",FIELD_TYPE_STRING}});
			// userset.SetFieldValue("UserID", "113334");
			// userset.SetFieldValue("Password", "861116");
			// userset.SetFieldValue("BrokerID", "9999");
			// const char* FrontAddress[] = {
            //     "tcp://180.168.146.187:10000",
            //     "tcp://180.168.146.187:10001",
            //     "tcp://218.202.237.33:10002"
			// };
			// userset.SetFieldValue("FrontAddress", FrontAddress, 3);
			// const char* MdFrontAddress[] = {
            //     "tcp://180.168.146.187:10010",
            //     "tcp://180.168.146.187:10011",
            //     "tcp://218.202.237.33:10012"
			// };
			// userset.SetFieldValue("MdFrontAddress", MdFrontAddress, 3);
			// DataSetConvEx conv_ex;
			// ITDApi::Instance().AddUser((char*)conv_ex.to_json_ex(&userset).c_str());

			// ITDApi::Instance().AddUser(R"({"UserID": "113334",
            // "Password": "861116",
            // "BrokerID": "9999",
            // "FrontAddress": [
            //     "tcp://180.168.146.187:10000",
            //     "tcp://180.168.146.187:10001",
            //     "tcp://218.202.237.33:10002"
            // ],
            // "MdFrontAddress": [
            //     "tcp://180.168.146.187:10010",
            //     "tcp://180.168.146.187:10011",
            //     "tcp://218.202.237.33:10012"
            // ]
			// })");
			return;
		}

		register_timer();
	}

	inline std::shared_ptr<Handler> select_handler(size_t peer) {
		return handlers_[peer % handlers_.size()];
	}

public:

	virtual void on_exchange_update(IDataSet* dataset, UpdateFlag flag) 
    { 
        DataSetConv conv;
        std::cout << conv.to_json(dataset) << std::endl;
        std::shared_ptr<CDataSet> dataset_ptr = std::make_shared<CDataSet>(*dataset);
        std::cout << conv.to_json(dataset_ptr.get()) << std::endl;

        std::vector<IExchangeSetPtr> exchange_ptrs = Ref_All_Exchange();
        for (auto& exchange_ptr : exchange_ptrs)
        {
        }
    }

	virtual void on_product_update(IDataSet* dataset, UpdateFlag flag) 
    { 
        std::vector<IProductSetPtr> product_ptrs = Ref_All_Product();
        for (auto& product_ptr : product_ptrs)
        {
        }
    }

    virtual void on_commodity_update(IDataSet* dataset, UpdateFlag flag) 
    {
        DataSetConv conv;
        std::cout << conv.to_json(dataset) << std::endl;
    }

	virtual void on_user_update(IDataSet* dataset, UpdateFlag flag) { 
        DataSetConv conv;
        std::cout << conv.to_json_ex(dataset) << std::endl;
	}
	virtual void on_order_update(IDataSet* dataset, UpdateFlag flag) { 
        DataSetConv conv;
        std::cout << conv.to_json_ex(dataset) << std::endl;
	}
	virtual void on_trade_update(IDataSet* dataset, UpdateFlag flag) { 
        DataSetConv conv;
        std::cout << conv.to_json_ex(dataset) << std::endl;
	}
//Handler
	//基础接口
	//inline boost::asio::io_service& service() { return Base::service(); }

	//网络接口
	inline void link_close(size_t peer) { NetBase::close(peer); }
	inline void link_send(size_t peer, const char* buf, size_t len) { 
		size_t peer_type = PEER_TYPE(peer);
		if(peer_type == PEER_TYPE_HTTP || peer_type == PEER_TYPE_HTTPS) {
			auto const http_json_build =
				[](const char* buf, size_t len)
			{
				boost::beast::http::response<boost::beast::http::string_body> res{ boost::beast::http::status::ok, 11 };
				res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
				res.set(boost::beast::http::field::content_type, "application/json");
				res.keep_alive(true);
				res.body().assign(buf,len);
				res.prepare_payload();
				return res;
			};
			link_send(peer, http_json_build(buf,len));
		} else {
			NetBase::post_packet(peer, buf, len); 
		}
	}
	template <bool isRequest, class Body, class Fields>
	inline void link_send(const size_t peer, boost::beast::http::message<isRequest, Body, Fields> &&msg)
	{
		NetBase::post_packet(peer, std::move(msg));
	}

	inline void on_link_open(size_t peer)
	{
		if(!is_run()) {
			return;
		}
		std::cout << "on_link_open: " << peer << std::endl;
		std::shared_ptr<Handler> handler = select_handler(peer);
		service().post(boost::bind(&Handler::on_link_open, handler, peer));
	}

	inline void on_link_close(size_t peer)
	{
		if(!is_run()) {
			return;
		}
		LOG4D("on_link_close: %d", peer);
		std::shared_ptr<Handler> handler = select_handler(peer);
		service().post(boost::bind(&Handler::on_link_close, handler, peer));
	}

	//inline void on_link_read(size_t peer, std::shared_ptr<boost::property_tree::ptree> msg)
	inline void on_link_read(size_t peer, std::string &msg)
	{
		if(!is_run()) {
			return;
		}
		std::shared_ptr<Handler> handler = select_handler(peer);
		service().post(boost::bind(&Handler::on_link_message, handler, peer, msg));
	}
//protected:
//以下是XServer线程回调，运行在网络线程里
#if XSERVER_PROTOTYPE_HTTP || XSERVER_PROTOTYPE_HTTPS
	void on_http_accept(http_ptr peer_ptr)
	{
		if(!is_run()) {
			return;
		}
		on_link_open(peer_ptr->id());
	}

	// Return a reasonable mime type based on the extension of a file.
	boost::beast::string_view
	mime_type(boost::beast::string_view path)
	{
		using boost::beast::iequals;
		auto const ext = [&path]
		{
			auto const pos = path.rfind(".");
			if (pos == boost::beast::string_view::npos)
				return boost::beast::string_view{};
			return path.substr(pos);
		}();
		if (iequals(ext, ".htm"))  return "text/html";
		if (iequals(ext, ".html")) return "text/html";
		if (iequals(ext, ".php"))  return "text/html";
		if (iequals(ext, ".css"))  return "text/css";
		if (iequals(ext, ".txt"))  return "text/plain";
		if (iequals(ext, ".js"))   return "application/javascript";
		if (iequals(ext, ".json")) return "application/json";
		if (iequals(ext, ".xml"))  return "application/xml";
		if (iequals(ext, ".swf"))  return "application/x-shockwave-flash";
		if (iequals(ext, ".flv"))  return "video/x-flv";
		if (iequals(ext, ".png"))  return "image/png";
		if (iequals(ext, ".jpe"))  return "image/jpeg";
		if (iequals(ext, ".jpeg")) return "image/jpeg";
		if (iequals(ext, ".jpg"))  return "image/jpeg";
		if (iequals(ext, ".gif"))  return "image/gif";
		if (iequals(ext, ".bmp"))  return "image/bmp";
		if (iequals(ext, ".ico"))  return "image/vnd.microsoft.icon";
		if (iequals(ext, ".tiff")) return "image/tiff";
		if (iequals(ext, ".tif"))  return "image/tiff";
		if (iequals(ext, ".svg"))  return "image/svg+xml";
		if (iequals(ext, ".svgz")) return "image/svg+xml";
		return "application/text";
	}

	// Append an HTTP rel-path to a local filesystem path.
	// The returned path is normalized for the platform.
	std::string
	path_cat(
		boost::beast::string_view base,
		boost::beast::string_view path)
	{
		if (base.empty())
			return path.to_string();
		std::string result = base.to_string();
	#if BOOST_MSVC
		char constexpr path_separator = '\\';
		if (result.back() == path_separator)
			result.resize(result.size() - 1);
		result.append(path.data(), path.size());
		for (auto& c : result)
			if (c == '/')
				c = path_separator;
	#else
		char constexpr path_separator = '/';
		if (result.back() == path_separator)
			result.resize(result.size() - 1);
		result.append(path.data(), path.size());
	#endif
		return result;
	}

	// This function produces an HTTP response for the given
	// request. The type of the response object depends on the
	// contents of the request, so the interface requires the
	// caller to pass a generic lambda for receiving the response.
	template<
		class Body, class Allocator>
		void
		handle_http_request(http_ptr peer_ptr,
			boost::beast::string_view doc_root,
			boost::beast::http::request<Body, boost::beast::http::basic_fields<Allocator>>&& req)
	{
		// Returns a bad request response
		auto const bad_request =
			[&req](boost::beast::string_view why)
		{
			boost::beast::http::response<boost::beast::http::string_body> res{ boost::beast::http::status::bad_request, req.version() };
			res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
			res.set(boost::beast::http::field::content_type, "text/html");
			res.keep_alive(req.keep_alive());
			res.body() = why.to_string();
			res.prepare_payload();
			return res;
		};

		// Returns a not found response
		auto const not_found =
			[&req](boost::beast::string_view target)
		{
			boost::beast::http::response<boost::beast::http::string_body> res{ boost::beast::http::status::not_found, req.version() };
			res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
			res.set(boost::beast::http::field::content_type, "text/html");
			res.keep_alive(req.keep_alive());
			res.body() = "The resource '" + target.to_string() + "' was not found.";
			res.prepare_payload();
			return res;
		};

		// Returns a server error response
		auto const server_error =
			[&req](boost::beast::string_view what)
		{
			boost::beast::http::response<boost::beast::http::string_body> res{ boost::beast::http::status::internal_server_error, req.version() };
			res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
			res.set(boost::beast::http::field::content_type, "text/html");
			res.keep_alive(req.keep_alive());
			res.body() = "An error occurred: '" + what.to_string() + "'";
			res.prepare_payload();
			return res;
		};

		// Make sure we can handle the method
		if (req.method() != boost::beast::http::verb::get &&
			req.method() != boost::beast::http::verb::head)
			return peer_ptr->do_write(bad_request("Unknown HTTP-method"));

		// Request path must be absolute and not contain "..".
		if (req.target().empty() ||
			req.target()[0] != '/' ||
			req.target().find("..") != boost::beast::string_view::npos)
			return peer_ptr->do_write(bad_request("Illegal request-target"));

		// Build the path to the requested file
		std::string path = path_cat(doc_root, req.target());
		if (req.target().back() == '/')
			path.append("index.html");

		// Attempt to open the file
		boost::beast::error_code ec;
		boost::beast::http::file_body::value_type body;
		body.open(path.c_str(), boost::beast::file_mode::scan, ec);

		// Handle the case where the file doesn't exist
		if (ec == boost::system::errc::no_such_file_or_directory)
			return peer_ptr->do_write(not_found(req.target()));

		// Handle an unknown error
		if (ec)
			return peer_ptr->do_write(server_error(ec.message()));

		// Cache the size since we need it after the move
		auto const size = body.size();

		// Respond to HEAD request
		if (req.method() == boost::beast::http::verb::head)
		{
			boost::beast::http::response<boost::beast::http::empty_body> res{ boost::beast::http::status::ok, req.version() };
			res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
			res.set(boost::beast::http::field::content_type, mime_type(path));
			res.content_length(size);
			res.keep_alive(req.keep_alive());
			return peer_ptr->do_write(std::move(res));
		}

		// Respond to GET request
		boost::beast::http::response<boost::beast::http::file_body> res{
			std::piecewise_construct,
			std::make_tuple(std::move(body)),
			std::make_tuple(boost::beast::http::status::ok, req.version()) };
		res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
		res.set(boost::beast::http::field::content_type, mime_type(path));
		res.content_length(size);
		res.keep_alive(req.keep_alive());
		return peer_ptr->do_write(std::move(res));
	}
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
		std::cout << "on_http_read: " << req << std::endl;
		//handle_http_request(peer_ptr, doc_root, std::move(req));
		std::ostringstream oss;
		oss << req.body();
		std::string body = oss.str();
		on_link_read(peer_ptr->id(), body);
		peer_ptr->do_read();
	}
	void on_http_close(http_t *peer_ptr) 
	{
		if(!is_run()) {
			return;
		}
		on_link_close(peer_ptr->id());
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
		on_link_open(peer_ptr->id());
	}

	void on_ws_accept(ws_ptr peer_ptr)
	{
		if(!is_run()) {
			return;
		}
		on_link_open(peer_ptr->id());
	}

	void on_ws_read(ws_ptr peer_ptr, std::string &buffer)
	{
		if(!is_run()) {
			return;
		}
		size_t peer = peer_ptr->id();
		LOG4D("on_ws_read: %s", buffer.c_str());
		//std::shared_ptr<boost::property_tree::ptree> msg = std::make_shared<boost::property_tree::ptree>();
		//XUtil::xml_from_str(buffer, msg);
		on_link_read(peer_ptr->id(), buffer);
		peer_ptr->do_read();
	}
	
	void on_ws_close(ws_t *peer_ptr) 
	{
		if(!is_run()) {
			return;
		}
		on_link_close(peer_ptr->id());
	}
#endif
};

}
