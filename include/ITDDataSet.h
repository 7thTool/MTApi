#pragma once
#ifndef _H_ITDDATASET_H_
#define _H_ITDDATASET_H_

#include <DataDef.h>
#include <ITDSet.h>
#include <IDataSet.h>

namespace MTP {

    class ITradeDataSet;
    class ITradeUserSet;
    class IOrderSet;
    class ITradeSet;
    class IPositionSet;
  
class TDSET_API ITradeDataSet : public IDataSet
{
public:
    static ITradeDataSet& Instance();

	static size_t Ref_All_User(ITradeUserSet** userid, size_t max_count);
	static size_t Ref_All_Order(IOrderSet** order, size_t max_count);
	static size_t Ref_All_Trade(ITradeSet** trade, size_t max_count);
	static size_t Ref_All_Position(IPositionSet** position, size_t max_count);

	static void OnAddUser(char* xml, int xmlflag);
	static void OnRemoveUser(char* xml, int xmlflag);
	static void OnUpdateUser(char* xml, int xmlflag);
	static void OnUpdateOrder(char* xml, int xmlflag);
	static void OnUpdateTrade(char* xml, int xmlflag);
	static void OnUpdatePosition(char* xml, int xmlflag);
};

class TDSET_API ITradeUserSet : public IDataSet
{
public:
    static ITradeUserSet* Ref(const char* userid, FindFlag flag = FindFlag::FindOnly);
    static void Release(ITradeUserSet* dataset);

	size_t Ref_All_Order(IOrderSet** order, size_t max_count);
	size_t Ref_All_Trade(ITradeSet** trade, size_t max_count);
	size_t Ref_All_Position(IPositionSet** position, size_t max_count);

	virtual const size_t Type() const { return DATASET_TYPE_TRADEUSER; }

	virtual const char* UserID() const { return ""; }

    bool Less(const IDataSet* r) const
    {
		if(!r) {
			return false;
		}
		const size_t type = Type(), r_type = r->Type();
		if (type < r_type)
            return true;
        else if(type > r_type)
			return false;

		const ITradeUserSet* rc = dynamic_cast<const ITradeUserSet*>(r);
		
			int cmp = 0;
			const char *userid = UserID(), *r_userid = rc->UserID();
			cmp = strcmp(userid, r_userid);
			if(cmp < 0) {
				return true;
			} else if(cmp > 0) {
				return false;
			}
			return false;
    };

    bool Equal(const IDataSet* r) const
    {
		if(!r) {
			return false;
		}
		if(Type() != r->Type()) {
			return false;
		}
		
		const ITradeUserSet* rc = dynamic_cast<const ITradeUserSet*>(r);
		
        return (strcmp(UserID(),rc->UserID()) == 0);
    }
};

class TradeUserSetInfo : public ITradeUserSet
{
protected:
	const std::string userid_;
public:
	TradeUserSetInfo(const char* userid)
	:userid_(userid) { }

	virtual const char* UserID() const { return userid_.c_str(); }
};

class TDSET_API IOrderSet : public IDataSet
{
public:
    static IOrderSet* Ref(const char* userid, const char* orderid, FindFlag flag = FindFlag::FindOnly);
    static void Release(IOrderSet* dataset);

	virtual const size_t Type() const { return DATASET_TYPE_ORDER; }

	virtual const char* UserID() const { return ""; }
	virtual const char* OrderID() const { return ""; }
	
    bool Less(const IDataSet* r) const
    {
		if(!r) {
			return false;
		}
		const size_t type = Type(), r_type = r->Type();
		if (type < r_type)
            return true;
        else if(type > r_type)
			return false;

		const IOrderSet* rc = dynamic_cast<const IOrderSet*>(r);
		
			int cmp = 0;
			const char *userid = UserID(), *r_userid = rc->UserID();
			cmp = strcmp(userid, r_userid);
			if(cmp < 0) {
				return true;
			} else if(cmp > 0) {
				return false;
			}
			const char *orderid = OrderID(), *r_orderid = rc->OrderID();
			cmp = strcmp(orderid, r_orderid);
			if(cmp < 0) {
				return true;
			} else if(cmp > 0) {
				return false;
			}
			return false;
    };

    bool Equal(const IDataSet* r) const
    {
		if(!r) {
			return false;
		}
		if(Type() != r->Type()) {
			return false;
		}
		
		const IOrderSet* rc = dynamic_cast<const IOrderSet*>(r);
		
        return (strcmp(UserID(),rc->UserID()) == 0 && strcmp(OrderID(),rc->OrderID()) == 0);
    }
};

class OrderSetInfo : public IOrderSet
{
protected:
	const std::string userid_;
	const std::string orderid_;
public:
	OrderSetInfo(const char* userid, const char* orderid)
	:userid_(userid),orderid_(orderid) { }

	virtual const char* UserID() const { return userid_.c_str(); }
	virtual const char* OrderID() const { return orderid_.c_str(); }
};

class TDSET_API ITradeSet : public IDataSet
{
public:
    static ITradeSet* Ref(const char* userid, const char* tradeid, FindFlag flag = FindFlag::FindOnly);
    static void Release(ITradeSet* dataset);

	virtual const size_t Type() const { return DATASET_TYPE_TRADE; }

	virtual const char* UserID() const { return ""; }
	virtual const char* TradeID() const { return ""; }

    bool Less(const IDataSet* r) const
    {
		if(!r) {
			return false;
		}
		const size_t type = Type(), r_type = r->Type();
		if (type < r_type)
            return true;
        else if(type > r_type)
			return false;

		const ITradeSet* rc = dynamic_cast<const ITradeSet*>(r);
		
			int cmp = 0;
			const char *userid = UserID(), *r_userid = rc->UserID();
			cmp = strcmp(userid, r_userid);
			if(cmp < 0) {
				return true;
			} else if(cmp > 0) {
				return false;
			}
			const char *tradeid = TradeID(), *r_tradeid = rc->TradeID();
			cmp = strcmp(tradeid, r_tradeid);
			if(cmp < 0) {
				return true;
			} else if(cmp > 0) {
				return false;
			}
			return false;
    };

    bool Equal(const IDataSet* r) const
    {
		if(!r) {
			return false;
		}
		if(Type() != r->Type()) {
			return false;
		}
		
		const ITradeSet* rc = dynamic_cast<const ITradeSet*>(r);
		
        return (strcmp(UserID(),rc->UserID()) == 0 && strcmp(TradeID(),rc->TradeID()) == 0);
    }
};

class TradeSetInfo : public ITradeSet
{
protected:
	const std::string userid_;
	const std::string tradeid_;
public:
	TradeSetInfo(const char* userid, const char* tradeid)
	:userid_(userid),tradeid_(tradeid) { }

	virtual const char* UserID() const { return userid_.c_str(); }
	virtual const char* TradeID() const { return tradeid_.c_str(); }
};

class TDSET_API IPositionSet : public IDataSet
{
public:
    static IPositionSet* Ref(const char* userid, const char* exchange, const char* product, const char* code, FindFlag flag = FindFlag::FindOnly);
    static void Release(IPositionSet* dataset);

public:
	virtual const size_t Type() const { return DATASET_TYPE_POSITION; }

	virtual const char* UserID() const { return ""; }
	virtual const char* Exchange() const { return ""; }
	virtual const char* Product() const { return ""; }
	virtual const char* Code() const { return ""; }

    bool Less(const IDataSet* r) const
    {
		if(!r) {
			return false;
		}
		const size_t type = Type(), r_type = r->Type();
		if (type < r_type)
            return true;
        else if(type > r_type)
			return false;

		const IPositionSet* rc = dynamic_cast<const IPositionSet*>(r);
		
			int cmp = 0;
			const char *userid = UserID(), *r_userid = rc->UserID();
			cmp = strcmp(userid, r_userid);
			if(cmp < 0) {
				return true;
			} else if(cmp > 0) {
				return false;
			}
			const char *exchange = Exchange(), *r_exchange = rc->Exchange();
			cmp = strcmp(exchange, r_exchange);
			if(cmp < 0) {
				return true;
			} else if(cmp > 0) {
				return false;
			}
			const char *product = Product(), *r_product = rc->Product();
			cmp = strcmp(product, r_product);
			if(cmp < 0) {
				return true;
			} else if(cmp > 0) {
				return false;
			}
			const char *code = Code(), *r_code = rc->Code();
			cmp = strcmp(code, r_code);
			if(cmp < 0) {
				return true;
			} else if(cmp > 0) {
				return false;
			}
			return false;
    };

    bool Equal(const IDataSet* r) const
    {
		if(!r) {
			return false;
		}
		if(Type() != r->Type()) {
			return false;
		}
		
		const IPositionSet* rc = dynamic_cast<const IPositionSet*>(r);
		
        return (strcmp(UserID(),rc->UserID()) == 0
		 && strcmp(Exchange(),rc->Exchange()) == 0 && strcmp(Product(),rc->Product()) == 0 && strcmp(Code(),rc->Code()) == 0);
    }
};

class PositionSetInfo : public IPositionSet
{
protected:
	const std::string userid_;
	const std::string exchange_;
	const std::string product_;
	const std::string code_;
public:
	PositionSetInfo(const char* userid, const char* exchange, const char* product, const char* code)
	:userid_(userid),exchange_(exchange),product_(product),code_(code) { }

	virtual const char* UserID() const { return userid_.c_str(); }
	virtual const char* Exchange() const { return exchange_.c_str(); }
	virtual const char* Product() const { return product_.c_str(); }
	virtual const char* Code() const { return code_.c_str(); }
};

typedef std::shared_ptr<ITradeUserSet> ITradeUserSetPtr;
typedef std::shared_ptr<IOrderSet> IOrderSetPtr;
typedef std::shared_ptr<ITradeSet> ITradeSetPtr;
typedef std::shared_ptr<IPositionSet> IPositionSetPtr;

inline ITradeUserSetPtr MakeTradeUserSetPtr(const char* userid, FindFlag flag = FindFlag::FindOnly) { 
	return ITradeUserSetPtr(ITradeUserSet::Ref(userid,flag),&ITradeUserSet::Release);
}
inline std::vector<ITradeUserSetPtr> MakeAllTradeUserSetPtr() { 
    std::vector<ITradeUserSetPtr> user_ptrs;
    size_t count = ITradeDataSet::Ref_All_User(nullptr, 0);
	ITradeUserSet** ptrs = (ITradeUserSet**)alloca(sizeof(ITradeUserSet*)*count);
	count = ITradeDataSet::Ref_All_User(ptrs, count);
    user_ptrs.resize(count);
    for(size_t i = 0; i < count; i++) 
	{
		user_ptrs[i] = ITradeUserSetPtr(ptrs[i],&ITradeUserSet::Release);
	}
	return user_ptrs;
}

inline IOrderSetPtr MakeOrderSetPtr(const char* userid, const char* orderid, FindFlag flag = FindFlag::FindOnly) { 
	return IOrderSetPtr(IOrderSet::Ref(userid,orderid,flag),&IOrderSet::Release);
}
inline std::vector<IOrderSetPtr> MakeAllOrderSetPtr(ITradeUserSetPtr user_ptr) { 
    std::vector<IOrderSetPtr> order_ptrs;
	size_t count = user_ptr->Ref_All_Order(nullptr, 0);
	IOrderSet** ptrs = (IOrderSet**)alloca(sizeof(IOrderSet*) * count);
	count = user_ptr->Ref_All_Order(ptrs, count);
    order_ptrs.resize(count);
    for(size_t i = 0; i < count; i++) 
	{
		order_ptrs[i] = IOrderSetPtr(ptrs[i],&IOrderSet::Release);
	}
	return order_ptrs;
}
inline std::vector<IOrderSetPtr> MakeAllOrderSetPtr() { 
    std::vector<IOrderSetPtr> order_ptrs;
	size_t count = ITradeDataSet::Ref_All_Order(nullptr, 0);
	IOrderSet** ptrs = (IOrderSet**)alloca(sizeof(IOrderSet*) * count);
	count = ITradeDataSet::Ref_All_Order(ptrs, count);
    order_ptrs.resize(count);
    for(size_t i = 0; i < count; i++) 
	{
		order_ptrs[i] = IOrderSetPtr(ptrs[i],&IOrderSet::Release);
	}
	return order_ptrs;
}

inline ITradeSetPtr MakeTradeSetPtr(const char* userid, const char* tradeid, FindFlag flag = FindFlag::FindOnly) { 
	return ITradeSetPtr(ITradeSet::Ref(userid,tradeid,flag),&ITradeSet::Release);
}
inline std::vector<ITradeSetPtr> MakeAllTradeSetPtr() { 
    std::vector<ITradeSetPtr> trade_ptrs;
	size_t count = ITradeDataSet::Ref_All_Trade(nullptr, 0);
	ITradeSet** ptrs = (ITradeSet**)alloca(sizeof(ITradeSet*) * count);
	count = ITradeDataSet::Ref_All_Trade(ptrs, count);
    trade_ptrs.resize(count);
    for(size_t i = 0; i < count; i++) 
	{
		trade_ptrs[i] = ITradeSetPtr(ptrs[i],&ITradeSet::Release);
	}
	return trade_ptrs;
}
inline std::vector<ITradeSetPtr> MakeAllTradeSetPtr(ITradeUserSetPtr user_ptr) { 
    std::vector<ITradeSetPtr> trade_ptrs;
	size_t count = user_ptr->Ref_All_Trade(nullptr, 0);
	ITradeSet** ptrs = (ITradeSet**)alloca(sizeof(ITradeSet*) * count);
	count = user_ptr->Ref_All_Trade(ptrs, count);
    trade_ptrs.resize(count);
    for(size_t i = 0; i < count; i++) 
	{
		trade_ptrs[i] = ITradeSetPtr(ptrs[i],&ITradeSet::Release);
	}
	return trade_ptrs;
}

inline IPositionSetPtr MakePositionSetPtr(const char* userid, const char* exchange, const char* product, const char* code, FindFlag flag = FindFlag::FindOnly) { 
	return IPositionSetPtr(IPositionSet::Ref(userid,exchange,product,code,flag),&IPositionSet::Release);
}
inline std::vector<IPositionSetPtr> MakeAllPositionSetPtr() { 
    std::vector<IPositionSetPtr> trade_ptrs;
	size_t count = ITradeDataSet::Ref_All_Position(nullptr, 0);
	IPositionSet** ptrs = (IPositionSet**)alloca(sizeof(IPositionSet*) * count);
	count = ITradeDataSet::Ref_All_Position(ptrs, count);
    trade_ptrs.resize(count);
    for(size_t i = 0; i < count; i++) 
	{
		trade_ptrs[i] = IPositionSetPtr(ptrs[i],&IPositionSet::Release);
	}
	return trade_ptrs;
}
inline std::vector<IPositionSetPtr> MakeAllPositionSetPtr(ITradeUserSetPtr user_ptr) { 
    std::vector<IPositionSetPtr> trade_ptrs;
	size_t count = user_ptr->Ref_All_Position(nullptr, 0);
	IPositionSet** ptrs = (IPositionSet**)alloca(sizeof(IPositionSet*) * count);
	count = user_ptr->Ref_All_Position(ptrs, count);
    trade_ptrs.resize(count);
    for(size_t i = 0; i < count; i++) 
	{
		trade_ptrs[i] = IPositionSetPtr(ptrs[i],&IPositionSet::Release);
	}
	return trade_ptrs;
}

}

#endif//_H_ITDDATASET_H_
