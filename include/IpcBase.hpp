#ifndef __H_IPC_BASE_HPP__
#define __H_IPC_BASE_HPP__

#include <XUtil/XIPC.hpp>
#include <RpcBase.hpp>

namespace MTP {
	
	static const char* IPC_NAME = "ipc";
	static const char* IPC_SERVER_NAME = "ipc_server";
	static const char* IPC_CLIENT_NAME = "ipc_client";
	static const size_t IPC_MAX_MSG_NUM = 1024;
	static const size_t IPC_MAX_MSG_SIZE = 10240;
    
struct IPC_TO_SERVER_HEADER
{
    char name[MAX_NAME_LENGTH];
};

template<class T>
class IpcBase
	: public XUtil::XXmlRpc<T>
{
	typedef XUtil::XXmlRpc<T> RpcBase;
protected:
	std::shared_ptr<XUtil::MessageQueue> mq_;
	std::string recv_msg_;
	std::string send_msg_;
public:
	IpcBase()
	{
		recv_msg_.reserve(IPC_MAX_MSG_SIZE);
		send_msg_.reserve(IPC_MAX_MSG_SIZE);
	}

	~IpcBase()
	{
	}

	bool start(const char* name, char* xml, int xmlflag = 0)
	{
        boost::property_tree::ptree& cfg = theApp.cfg();
		auto opt = cfg.get_child_optional(IPC_NAME);
		if (!opt) {
			return false;
		}
		boost::property_tree::ptree mq_cfg = opt.get();
		auto ipc_opt = mq_cfg.get_child_optional(name);
		if (!ipc_opt) {
			return false;
		}
		boost::property_tree::ptree ipc_cfg = ipc_opt.get();

		try
		{
			std::string msg_name = ipc_cfg.get<std::string>("name", name);
			size_t max_msg_num = ipc_cfg.get<size_t>("max_msg_num", IPC_MAX_MSG_NUM);
			size_t max_msg_size = ipc_cfg.get<size_t>("max_msg_size", IPC_MAX_MSG_SIZE);
			mq_ = std::make_shared<XUtil::MessageQueue>(
				boost::interprocess::open_or_create
				, msg_name.c_str() //name
				, max_msg_num
				, max_msg_size
				);
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
		return mq_ != nullptr;
	}

	void stop()
	{
		RpcBase::clear();
        mq_.reset();
	}

	inline bool send(XUtil::MessageQueue* peer, const char* buf, size_t len, unsigned int priority) {
        if(peer) { 
		    peer->send(buf, len, priority);
		    return true;
        }
        return false;
	}
	inline bool try_send(XUtil::MessageQueue* peer, const char* buf, size_t len, unsigned int priority) {
		if(peer) {
		    return peer->try_send(buf, len, priority);
        }
        return false;
	}
	inline bool recv(XUtil::MessageQueue* peer, std::string& msg, unsigned int& priority)
	{
		if(peer) {
			msg.resize(IPC_MAX_MSG_SIZE);
			XUtil::MessageQueue::size_type recvd_size = 0;
			peer->receive((char*)msg.data(), msg.size(), recvd_size, priority);
			msg.resize(recvd_size);
			return recvd_size > 0;
		}
		return false;
	}
	inline bool try_recv(XUtil::MessageQueue* peer, std::string& msg, unsigned int& priority)
	{
		if(peer) {
			msg.resize(MD_SRC_IPC_MAX_MSG_SIZE);
			XUtil::MessageQueue::size_type recvd_size = 0;
			bool rlt = peer->try_receive((char*)msg.data(), msg.size(), recvd_size, priority);
			msg.resize(recvd_size);
			return rlt;
		}
		return false;
	}
	
public:
	//适配Rpc调用
	inline void on_recv(XUtil::MessageQueue* peer, const std::string& msg)
	{
		T* pT = static_cast<T*>(this);
		//调用Rpc
        pT->_handleMessage((size_t)peer, msg);
	}

    void _sendMessage (size_t peer, const std::string& msg) {
        T* pT = static_cast<T*>(this);
		unsigned int priority = 0;
		bool success = pT->try_send((XUtil::MessageQueue*)peer, msg.data(), msg.size(), priority);
		//LOG4E("Ipc sendMessage %s %s.", success?"success":"false", msg.c_str());
    }
};

}

#endif //__H_IPC_BASE_HPP__
