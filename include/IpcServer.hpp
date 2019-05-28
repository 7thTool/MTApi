#ifndef __H_IPC_SERVER_HPP__
#define __H_IPC_SERVER_HPP__

#include <IpcBase.hpp>
#include <DataSetConvEx.hpp>

namespace MTP {

template<class T>
class IpcServer
	: public IpcBase<T>
	, public XUtil::XService
{
	typedef IpcServer<T> This;
	typedef IpcBase<T> Base;
	typedef XUtil::XService Service;
protected:
	std::map<const char*,std::shared_ptr<XUtil::MessageQueue>,XUtil::strless> mq_clients_;
public:
	IpcServer() : Base()
	{
	}

	~IpcServer()
	{
	}

	bool start(char* xml, int xmlflag = 0)
	{
		if(!Base::start(IPC_SERVER_NAME,xml,xmlflag)) {
			return false;
		}

		try
		{
			Service::start();

			service().post(boost::bind(&This::run, this));

			return true;
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
		mq_clients_.clear();
	}

public:
	inline bool send(XUtil::MessageQueue* peer, const char* buf, size_t len, unsigned int priority) {
        if(peer) { 
		    peer->send(buf, len, priority);
		    return true;
        } else {
			return broadcast(buf, len, priority);
		}
        return false;
	}
	inline bool try_send(XUtil::MessageQueue* peer, const char* buf, size_t len, unsigned int priority) {
		if(peer) {
		    return peer->try_send(buf, len, priority);
        } else {
			return try_broadcast(buf, len, priority);
		}
        return false;
	}
	inline bool broadcast(const char* buf, size_t len, unsigned int priority) { 
		for(auto it : mq_clients_)
		{
			it.second->send(buf, len, priority);
		}
		return true;
	}
	inline bool try_broadcast(const char* buf, size_t len, unsigned int priority) {
		for(auto it : mq_clients_)
		{
			it.second->try_send(buf, len, priority);
		}
		return true;
	}

	inline void quit() { 
		if(mq_) {
			char quit = 1;
			mq_->send(&quit, 1, 0);
		}
	}

	void run()
	{
		unsigned int priority;
		while(recv(mq_.get(), recv_msg_, priority)) {
			if(recv_msg_.size() < sizeof(IPC_TO_SERVER_HEADER)) {
				break;
			}
			char* msg = (char*)recv_msg_.data();
			size_t msg_len = recv_msg_.size();
			IPC_TO_SERVER_HEADER* header = (IPC_TO_SERVER_HEADER*)msg;
			msg = (char*)(header + 1);
			msg_len -= sizeof(IPC_TO_SERVER_HEADER);
			auto mq = try_open_client(header->name);
			if(mq) {
				on_recv(mq.get(), std::string(msg,msg_len));
			}
		} 
		//service().post(boost::bind(&MDIpcBase::try_to_receive, this));
		//std::this_thread::yield();
	}

	std::shared_ptr<XUtil::MessageQueue> try_open_client(const char* name)
	{
		auto it = mq_clients_.find(name);
		if(it != mq_clients_.end()) {
			return it->second;
		}
		std::shared_ptr<XUtil::MessageQueue> mq;
		try
		{
			mq = std::make_shared<XUtil::MessageQueue>(
					boost::interprocess::open_only
					, name //name
					);
			mq_clients_[mq->get_msg_name().c_str()] = mq;
		}
		catch(...)
		{
			
		}
		return mq;
	}
};

}

#endif //__H_IPC_SERVER_HPP__
