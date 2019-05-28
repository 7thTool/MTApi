#ifndef __H_IPC_CLIENT_HPP__
#define __H_IPC_CLIENT_HPP__

#include <IpcBase.hpp>

namespace MTP {

template<class T>
class IpcClient
	: public IpcBase<T>
	, public XUtil::XService
{
	typedef IpcClient<T> This;
	typedef IpcBase<T> Base;
	typedef XUtil::XService Service;
protected:
	std::shared_ptr<XUtil::MessageQueue> mq_server_;
public:
	IpcClient() : Base()
	{
		
	}

	~IpcClient()
	{
	}

	bool start(char* xml, int xmlflag = 0)
	{
		if(!Base::start(IPC_CLIENT_NAME,xml,xmlflag)) {
			return false;
		}

		try
		{
			boost::property_tree::ptree& cfg = theApp.cfg();
			auto opt = cfg.get_child_optional(IPC_NAME);
			if (!opt) {
				return false;
			}
			boost::property_tree::ptree mq_cfg = opt.get();
			auto ipc_opt = mq_cfg.get_child_optional(IPC_SERVER_NAME);
			if (!ipc_opt) {
				return false;
			}
			boost::property_tree::ptree ipc_cfg = ipc_opt.get();
		
			std::string msg_name = ipc_cfg.get<std::string>("name", IPC_SERVER_NAME);
			size_t max_msg_num = ipc_cfg.get<size_t>("max_msg_num", IPC_MAX_MSG_NUM);
			size_t max_msg_size = ipc_cfg.get<size_t>("max_msg_size", IPC_MAX_MSG_SIZE);
			mq_server_ = std::make_shared<XUtil::MessageQueue>(
				boost::interprocess::open_or_create
				, msg_name.c_str() //name
				, max_msg_num
				, max_msg_size
				);

			Service::start();

			service().post(boost::bind(&This::run, this));

			return true;
		}
		catch (boost::interprocess::interprocess_exception &ex)
		{
			if (boost::interprocess::already_exists_error == ex.get_error_code())
			{
				
			}
		}
		catch (boost::interprocess::bad_alloc &ex)
		{
			
		}
		catch (...)
		{
			
		}
		return false;
	}

	void stop()
	{
		quit();
		Service::stop();
		Base::stop();
		mq_server_.reset();
	}
	
public:
	inline bool send(XUtil::MessageQueue* peer, const char* buf, size_t len, unsigned int priority) {
        if(peer) { 
			// if(peer->get_num_msg() >= peer->get_max_msg()) {
			// 	return false;
			// }
			size_t msg_len = sizeof(IPC_TO_SERVER_HEADER) + len;
			if(msg_len > IPC_MAX_MSG_SIZE) {
				ASSERT(false);
				return false;
			}
			send_msg_.resize(msg_len);
			IPC_TO_SERVER_HEADER* header = (IPC_TO_SERVER_HEADER*)send_msg_.data();
			strcpy(header->name, mq_->get_msg_name().c_str());
			memcpy(header + 1, buf, len);
		    peer->send(send_msg_.data(), send_msg_.size(), priority);
		    return true;
        }
        return false;
	}
	inline bool try_send(XUtil::MessageQueue* peer, const char* buf, size_t len, unsigned int priority) {
		if(peer) {
			size_t msg_len = sizeof(IPC_TO_SERVER_HEADER) + len;
			if(msg_len > IPC_MAX_MSG_SIZE) {
				ASSERT(false);
				return false;
			}
			send_msg_.resize(msg_len);
			IPC_TO_SERVER_HEADER* header = (IPC_TO_SERVER_HEADER*)send_msg_.data();
			strcpy(header->name, mq_->get_msg_name().c_str());
			memcpy(header + 1, buf, len);
		    return peer->try_send(send_msg_.data(), send_msg_.size(), priority);
        }
        return false;
	}

	inline void quit() { 
		if(mq_) {
			char quit = 1;
			mq_->send(&quit, 1, 0);
		}
	}

	void run()
	{
		//握手
		std::shared_ptr<boost::property_tree::ptree> params;
		auto ft = sendRemoteRequest((size_t)mq_server_.get(), "handshake", params);
		
		unsigned int priority;
		while(recv(mq_.get(), recv_msg_, priority)) {
			if(recv_msg_.size() <= 1) {
				break;
			}
			on_recv(mq_.get(), recv_msg_);
		} 
		//service().post(boost::bind(&MDIpcBase::try_to_receive, this));
		//std::this_thread::yield();

		//再见
		//sendRemoteNotification((size_t)mq_server_.get(), "byebye", params);
	}

    void _sendRequest (size_t peer, const std::string& method, std::shared_ptr<boost::property_tree::ptree> params, size_t id) {
        std::string msg;
        boost::property_tree::ptree json;
        build_request(json, method, params?*params:boost::property_tree::ptree(), id);
        json_to_str(json, msg);
        //_sendMessage(peer, msg);
		unsigned int priority = 0;
		bool success = try_send((XUtil::MessageQueue*)peer, msg.data(), msg.size(), priority);
		if(!success) {
			if(mq_) {
        		build_error_response(json, params?*params:boost::property_tree::ptree(), id);
        		json_to_str(json, msg);
				mq_->send(msg.data(), msg.size(), priority);
			}
		}
    }
};

}

#endif //__H_IPC_CLIENT_HPP__
