#pragma once
#ifndef _H_ORDER_HPP_
#define _H_ORDER_HPP_

#include "DataDef.h"

namespace MTP {

enum OrderActionType
{
    Cancel = 0,
    Do
};

enum OrderDirectionType
{
    Buy = 0,
    Sell
};

const uint32_t OrderPriceRate = 100; //放大100倍，即保存两位小数

enum OrderPriceType
{
    PriceLimit = 0,
    PriceMarket,
};

enum OrderStatus
{
    OrderUnknown = 0,
    OrderError,
    OrderIn,
    OrderPartionTraded,
    OrderTraded
};

struct Code
{
    char code[MAX_CODE_LENGTH];
    uint64_t price; //放大1000000倍,上市价或昨收价
};

struct Trade
{
    union 
    {
        struct 
        {
            char user_id[MAX_NAME_LENGTH];
            char code[MAX_CODE_LENGTH];
            uint64_t order_id; //原始委托id
            OrderDirectionType direction;
            double price; //成交价格
            uint32_t volume; //成交数量
            uint32_t date; //成交日期
            uint32_t time; //成交时间
        };
        //char reserved[520];
    };
};

//委托
struct Order
{
    union 
    {
        struct 
        {
            char user_id[MAX_NAME_LENGTH];
            char code[MAX_CODE_LENGTH];
            OrderActionType action;
            OrderDirectionType direction;
            OrderPriceType price_type;
            uint64_t price; //放大1000000倍
            uint32_t volume; //委托量
            uint32_t date; //委托日期
            uint32_t time; //委托时间
            uint64_t id; //委托id
            OrderStatus status; //委托状态
            uint64_t traded_id; //最新对手id
            uint64_t traded_price; //最新成交价
            uint32_t traded_volume; //最新成交数量
            uint32_t total_traded_volume; //累计成交数量
        };
        char reserved[260];
    };

		bool operator == (const Order &r) const
		{
			return (id == r.id);
		}

    inline bool cancel() { return action == OrderActionType::Cancel; }
    inline bool buy() { return direction == OrderDirectionType::Buy; }
    inline bool sell() { return direction == OrderDirectionType::Sell; }

    //委托是否可撮合
    inline bool match(uint64_t last_price, Order& order)
    {
        if(price_type==OrderPriceType::PriceLimit && order.price_type ==OrderPriceType::PriceLimit) {
            if(direction == OrderDirectionType::Buy) {
                if(price < order.price) {
                    return false;
                }
                if(time > order.time) { //先买后卖，以买价成交
                    traded_price = price;
                    order.traded_price = price;
                } else if(time < order.time) { //先卖后买，以卖价成交
                    traded_price = order.price;
                    order.traded_price = order.price;
                } else { //同时委托，以中间价成交
                    uint64_t tmp_price = (price + order.price) / 2;
                    traded_price = tmp_price;
                    order.traded_price = tmp_price;
                }
            } else {
                if(price > order.price) {
                    return false;
                }
                if(time > order.time) { //先卖后买，以卖价成交
                    traded_price = price;
                    order.traded_price = price;
                } else if(time < order.time) { //先买后卖，以买价成交
                    traded_price = order.price;
                    order.traded_price = order.price;
                } else { //同时委托，以中间价成交，四舍五入取整
                    uint64_t tmp_price = (((price + order.price) / 2) / OrderPriceRate) * OrderPriceRate;
                    traded_price = tmp_price;
                    order.traded_price = tmp_price;
                }
            }
        } else if(price_type==OrderPriceType::PriceMarket && order.price_type ==OrderPriceType::PriceLimit) {
            traded_price = order.price;
        } else if(price_type==OrderPriceType::PriceLimit && order.price_type ==OrderPriceType::PriceMarket) {
            order.traded_price = price;
        } else {
            traded_price = last_price;
            order.traded_price = last_price;
        }
        traded_volume = std::min<>(volume-total_traded_volume,order.volume-order.total_traded_volume);
        total_traded_volume += traded_volume;
        if(total_traded_volume >= volume) {
            status = OrderStatus::OrderTraded;
        } else {
            status = OrderStatus::OrderPartionTraded;
        }
        order.traded_volume = traded_volume;
        order.total_traded_volume += traded_volume;
        if(order.total_traded_volume >= order.volume) {
            order.status = OrderStatus::OrderTraded;
        } else {
            order.status = OrderStatus::OrderPartionTraded;
        }
        traded_id = order.id;
        order.traded_id = id;
        return true;
    }
};

struct OrderLess {
    bool operator () (const Order& x, const Order& y) const {
       //价格优先
            if(x.price_type < y.price_type) {
                return true;
            } else if(x.price_type > y.price_type) {
                return false;
            } else if(x.price_type==OrderPriceType::PriceLimit) {
                if (x.price < y.price) {
                    return true;
                } else if(x.price > y.price) {
                    return false;
                }
            }
            //时间优先
            if(x.time < y.time) {
				return true;
			}
			return false;
    }
};

struct OrderGreat {
    bool operator () (const Order& x, const Order& y) const {
       //价格优先
            if(x.price_type > y.price_type) {
                return true;
            } else if(x.price_type < y.price_type) {
                return false;
            } else if(x.price_type==OrderPriceType::PriceLimit) {
                if (x.price > y.price) {
                    return true;
                } else if(x.price < y.price) {
                    return false;
                }
            }
            //时间优先
            if(x.time > y.time) {
				return true;
			}
			return false;
    }
};

struct Position
{
    union 
    {
        struct 
        {
            char code[MAX_CODE_LENGTH];
            OrderDirectionType direction;
            double price;
            uint32_t volume;
        };
        char reserved[260];
    };
};

}

#endif//_H_ORDER_HPP_