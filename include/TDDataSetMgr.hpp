#pragma once

#include <ITDDataSet.h>

namespace MTP
{

template <class Server, class TradeUserSet, class OrderSet, class TradeSet, class PositionSet>
class CTDDataSetMgr
{
protected:
    Server *srv_;
    struct ITradeUserSetPtrLess
    {
        bool operator()(const ITradeUserSet *const &x, const ITradeUserSet *const &y) const
        {
            return *x < *y;
        }
    };
    struct TradeUserSetEx : public TradeUserSet
    {
        using TradeUserSet::TradeUserSet;
        int ref = 0;
    };
    typedef std::map<const ITradeUserSet *, TradeUserSetEx *, ITradeUserSetPtrLess> TradeUserSets;
    TradeUserSets tradeusersets_;
    std::shared_mutex tradeuser_mutex_;
    struct IOrderSetPtrLess
    {
        bool operator()(const IOrderSet *const &x, const IOrderSet *const &y) const
        {
            return *x < *y;
        }
    };
    struct OrderSetEx : public OrderSet
    {
        using OrderSet::OrderSet;
        int ref = 0;
    };
    typedef std::map<const IOrderSet *, OrderSetEx *, IOrderSetPtrLess> OrderSets;
    OrderSets ordersets_;
    std::shared_mutex order_mutex_;
    struct ITradeSetPtrLess
    {
        bool operator()(const ITradeSet *const &x, const ITradeSet *const &y) const
        {
            return *x < *y;
        }
    };
    struct TradeSetEx : public TradeSet
    {
        using TradeSet::TradeSet;
        int ref = 0;
    };
    typedef std::map<const ITradeSet *, TradeSetEx *, ITradeSetPtrLess> TradeSets;
    TradeSets tradesets_;
    std::shared_mutex trade_mutex_;
    struct IPositionSetPtrLess
    {
        bool operator()(const IPositionSet *const &x, const IPositionSet *const &y) const
        {
            return *x < *y;
        }
    };
    struct PositionSetEx : public PositionSet
    {
        using PositionSet::PositionSet;
        int ref = 0;
    };
    typedef std::map<const IPositionSet *, PositionSetEx *, IPositionSetPtrLess> PositionSets;
    PositionSets positionsets_;
    std::shared_mutex position_mutex_;

public:
    CTDDataSetMgr(Server *srv) : srv_(srv) {}
    ~CTDDataSetMgr() {}

    inline void Clear()
    {
        RemoveUser(nullptr);
    }

    inline void RemoveUser(const char* userid)
    {
        RemoveOrder(userid, nullptr);
        RemoveTrade(userid, nullptr);
        RemovePosition(userid, nullptr, nullptr, nullptr);
        std::unique_lock<std::shared_mutex> w_lock(tradeuser_mutex_);
        if(userid && userid[0])
        {
            TradeUserSetInfo datasetinfo(userid);
            auto it = tradeusersets_.find(&datasetinfo);
            if (it != tradeusersets_.end())
            {
                tradeusersets_.erase(it);
            }
        }
        else
        {
            tradeusersets_.clear();
        }
    }

    inline void RemoveOrder(const char* userid, const char* orderid)
    {
        std::unique_lock<std::shared_mutex> w_lock(order_mutex_);
        if(userid && userid[0])
        {
            OrderSetInfo datasetinfo(userid, orderid);
            auto pr = ordersets_.equal_range(&datasetinfo);
            ordersets_.erase(pr.first, pr.second);
        }
        else
        {
            ordersets_.clear();
        }
    }

    inline void RemoveTrade(const char* userid, const char* tradeid)
    {
        std::unique_lock<std::shared_mutex> w_lock(trade_mutex_);
        if(userid && userid[0])
        {
            TradeSetInfo datasetinfo(userid, tradeid);
            auto pr = tradesets_.equal_range(&datasetinfo);
            tradesets_.erase(pr.first, pr.second);
        }
        else
        {
            tradesets_.clear();
        }
    }

    inline void RemovePosition(const char* userid, const char* exchange, const char* product, const char* code)
    {
        std::unique_lock<std::shared_mutex> w_lock(position_mutex_);
        if(userid && userid[0]) {
            PositionSetInfo datasetinfo(userid, exchange, product, code);
             auto pr = positionsets_.equal_range(&datasetinfo);
            positionsets_.erase(pr.first, pr.second);
        }
        else
        {
            positionsets_.clear();
        }
    }

    ITradeUserSet *RefUser(const char *userid, FindFlag flag = FindFlag::FindOrCreate)
    {
        if (flag == FindFlag::FindOrCreate || flag == FindFlag::FindOnly)
        {
            std::shared_lock<std::shared_mutex> lock(tradeuser_mutex_);
            TradeUserSetInfo datasetinfo(userid);
            auto it = tradeusersets_.find(&datasetinfo);
            if (it != tradeusersets_.end())
            {
                it->second->ref++;
                return it->second;
            }
        }
        if (flag == FindFlag::FindOrCreate || flag == FindFlag::CreateOnly)
        {
            std::unique_lock<std::shared_mutex> lock(tradeuser_mutex_);
            auto tradeuser_ptr = srv_->NewUser();
            if (tradeuser_ptr)
            {
                tradeuser_ptr->ref = 1;
                tradeusersets_[tradeuser_ptr] = tradeuser_ptr;
                return tradeuser_ptr;
            }
        }
        return nullptr;
    }

    void Release(ITradeUserSet *dataset)
    {
        return; //不释放
        TradeUserSetEx* tradeuserset = dynamic_cast<TradeUserSetEx*>(dataset);
        if (!tradeuserset)
        {
            return;
        }
        std::unique_lock<std::shared_mutex> lock(tradeuser_mutex_);
        int ref = --tradeuserset->ref;
        auto it = tradeusersets_.find(dataset);
        if (it != tradeusersets_.end())
        {
            if (ref == 0)
            {
                tradeusersets_.erase(it);
            }
        }
        if (ref == 0)
        {
            delete tradeuserset;
        }
    }

    IOrderSet *RefOrder(const char *userid, const char *orderid, FindFlag flag = FindFlag::FindOrCreate)
    {
        if (flag == FindFlag::FindOrCreate || flag == FindFlag::FindOnly)
        {
            std::shared_lock<std::shared_mutex> lock(order_mutex_);
            OrderSetInfo datasetinfo(userid, orderid);
            auto it = ordersets_.find(&datasetinfo);
            if (it != ordersets_.end())
            {
                it->second->ref++;
                return it->second;
            }
        }
        if (flag == FindFlag::FindOrCreate || flag == FindFlag::CreateOnly)
        {
            std::unique_lock<std::shared_mutex> lock(order_mutex_);
            auto order_ptr = srv_->NewOrder();
            if (order_ptr)
            {
                order_ptr->ref = 1;
                ordersets_[order_ptr] = order_ptr;
                return order_ptr;
            }
        }
        return nullptr;
    }

    void Release(IOrderSet *dataset)
    {
        return; //不释放
        OrderSetEx* orderset = dynamic_cast<OrderSetEx*>(dataset);
        if (!orderset)
        {
            return;
        }
        std::unique_lock<std::shared_mutex> lock(order_mutex_);
        int ref = --orderset->ref;
        auto it = ordersets_.find(dataset);
        if (it != ordersets_.end())
        {
            if (ref == 0)
            {
                ordersets_.erase(it);
            }
        }
        if (ref == 0)
        {
            delete orderset;
        }
    }

    ITradeSet *RefTrade(const char *userid, const char *tradeid, FindFlag flag = FindFlag::FindOrCreate)
    {
        if (flag == FindFlag::FindOrCreate || flag == FindFlag::FindOnly)
        {
            std::shared_lock<std::shared_mutex> lock(trade_mutex_);
            TradeSetInfo datasetinfo(userid, tradeid);
            auto it = tradesets_.find(&datasetinfo);
            if (it != tradesets_.end())
            {
                it->second->ref++;
                return it->second;
            }
        }
        if (flag == FindFlag::FindOrCreate || flag == FindFlag::CreateOnly)
        {
            std::unique_lock<std::shared_mutex> lock(trade_mutex_);
            auto trade_ptr = srv_->NewTrade();
            if (trade_ptr)
            {
                trade_ptr->ref = 1;
                tradesets_[trade_ptr] = trade_ptr;
                return trade_ptr;
            }
        }
        return nullptr;
    }

    void Release(ITradeSet *dataset)
    {
        return; //不释放
        TradeSetEx* tradeset = dynamic_cast<TradeSetEx*>(dataset);
        if (!dataset)
        {
            return;
        }
        std::unique_lock<std::shared_mutex> lock(trade_mutex_);
        int ref = --tradeset->ref;
        auto it = tradesets_.find(dataset);
        if (it != tradesets_.end())
        {
            if (ref == 0)
            {
                tradesets_.erase(it);
            }
        }
        if (ref == 0)
        {
            delete tradeset;
        }
    }

    IPositionSet *RefPosition(const char* userid, const char *exchange, const char *product, const char *code, FindFlag flag = FindFlag::FindOrCreate)
    {
        if (flag == FindFlag::FindOrCreate || flag == FindFlag::FindOnly)
        {
            std::shared_lock<std::shared_mutex> lock(position_mutex_);
            PositionSetInfo datasetinfo(userid, exchange, product, code);
            auto it = positionsets_.find(&datasetinfo);
            if (it != positionsets_.end())
            {
                it->second->ref++;
                return it->second;
            }
        }
        if (flag == FindFlag::FindOrCreate || flag == FindFlag::CreateOnly)
        {
            boost::unique_lock<std::shared_mutex> w_lock(position_mutex_);
            auto position_ptr = srv_->NewPosition();
            if (position_ptr)
            {
                position_ptr->ref = 1;
                positionsets_[position_ptr] = position_ptr;
                return position_ptr;
            }
        }
        return nullptr;
    }

    void Release(IPositionSet *dataset)
    {
        return; //不释放
        PositionSetEx* positionset = dynamic_cast<PositionSetEx*>(dataset);
        if (!dataset)
        {
            return;
        }
        std::unique_lock<std::shared_mutex> lock(position_mutex_);
        int ref = --positionset->ref;
        auto it = positionsets_.find(dataset);
        if (it != positionsets_.end())
        {
            if (ref == 0)
            {
                positionsets_.erase(it);
            }
        }
        if (ref == 0)
        {
            delete positionset;
        }
    }
};

} // namespace MTP
