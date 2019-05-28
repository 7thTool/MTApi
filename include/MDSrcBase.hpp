#pragma once

#include <XUtil/XApi.hpp>
#include <XUtil/XService.hpp>
#include <XUtil/XXmlRpc.hpp>
#include <XUtil/XIPC.hpp>
#include "MDBase.hpp"

namespace MTP
{

	const char* MD_SRC_HANDLER_NAME = "MDSrcHandler";

template<class T, class TExchange, class TProduct, class TCommodity>
class MDSrcBase
	: public XUtil::XApi
	, public XUtil::XService
{
	typedef XUtil::XApi Base;
	typedef XUtil::XService Service;
protected:
	typedef typename std::shared_ptr<TExchange> ExchangePtr;
	typedef typename std::shared_ptr<TProduct> ProductPtr;
	typedef typename std::shared_ptr<TCommodity> CommodityPtr;
	std::string trading_day_;
	std::string old_trading_day_;
	uint32_t date_ = 0;
	uint32_t time_ = 0;
	std::vector<ExchangePtr> exchanges_;   //交易所列表
	std::vector<ProductPtr> products_;	 //产品分类列表
	std::vector<CommodityPtr> commoditys_; //商品列表

  public:
	MDSrcBase() : Base(), Service()
	{

	}
	~MDSrcBase()
	{

	}

	bool start(char* xml, int xmlflag = 0)
	{
		bool ret = Base::init(xml, xmlflag);

		Service::start();

		return ret;
	}

	void stop()
	{
		Service::stop();
		
		Base::term();
	}

	inline boost::asio::io_service &service() { return Service::service(); }

	void on_src_date(const std::string& date)
	{
		LOG4I("on_market_update %s", date.c_str());
		old_trading_day_ = trading_day_;
		trading_day_ = date;
		if (old_trading_day_ != trading_day_) {
			exchanges_.clear();
			products_.clear();
			commoditys_.clear();
		}
	}

	ExchangePtr on_add_exchange(const ExchangePtr& exchange)
	{
		if (exchange)
		{
			int pos = InnerFindExchange(exchange->Exchange, find_or_add);
			exchanges_[pos] = exchange;
			date_ = exchange->Date;
			time_ = exchange->Time;
			return exchanges_[pos];
		}
		return nullptr;
	}

	ProductPtr on_add_product(const ProductPtr& product)
		{
		if (product)
		{
			int pos = InnerFindProduct(product->Exchange, product->Product, find_or_add);
			products_[pos] = product;
			return products_[pos];
		}
		return nullptr;
	}

	CommodityPtr on_add_commodity(const CommodityPtr& commodity)
	{
		if (commodity)
		{
			if(commodity->Date && commodity->Time) {
				date_ = commodity->Date;
				time_ = commodity->Time;
			}
			int pos = InnerFindCommodity(commodity->Exchange, commodity->Product, commodity->Code, find_or_add);
			commoditys_[pos] = commodity;
		}
		return nullptr;
	}

	CommodityPtr on_init_commodity(const CommodityPtr& commodity)
	{
		if (commodity)
		{
			if(commodity->Date && commodity->Time) {
				date_ = commodity->Date;
				time_ = commodity->Time;
			}
			int pos = InnerFindCommodity(commodity->Exchange, commodity->Product, commodity->Code, find_or_add);
			commoditys_[pos]->InitMarket(commodity);
			return commoditys_[pos];
		}
		return nullptr;
	}

	void on_src_init()
	{
		
	}

	CommodityPtr on_commodity_update(const CommodityPtr& commodity)
	{
		if(commodity)
		{
			if(commodity->Date && commodity->Time) {
				date_ = commodity->Date;
				time_ = commodity->Time;
			}
			int pos = InnerFindCommodity(commodity->Exchange, commodity->Product, commodity->Code, find_or_add);
			commoditys_[pos]->UpdateMarket(commodity);
			return commoditys_[pos];
		}
		return nullptr;
	}

	template<class Ty>
	ExchangePtr on_exchange_status(const Ty& commodity)
	{
		if (commodity)
		{
			int pos = InnerFindExchange(commodity->Exchange, find_or_add);
			exchanges_[pos]->UpdateStatus(commodity);
			return exchanges_[pos];
		}
		return nullptr;
	}

	template<class Ty>
	ProductPtr on_product_status(const Ty& commodity)
	{
		if (commodity)
		{
			int pos = InnerFindProduct(commodity->Exchange, commodity->Product, find_or_add);
			products_[pos]->UpdateStatus(commodity);
			return products_[pos];
		}
		return nullptr;
	}

	template<class Ty>
	CommodityPtr on_commodity_status(const Ty& commodity)
	{
		if (commodity)
		{
			int pos = InnerFindCommodity(commodity->Exchange, commodity->Product, commodity->Code, find_or_add);
			commoditys_[pos]->UpdateStatus(commodity);
			return commoditys_[pos];
		}
		return nullptr;
	}

protected:
	enum { find_only = 0, find_or_add = 1 };
	inline int InnerFindExchange(const char *exchange, int flag = find_only)
	{
		struct ExchangePtrPred
		{
			const char* exchange_;
			ExchangePtrPred(const char* exchange) :exchange_(exchange)
			{}

			bool operator()(const ExchangePtr& x, const ExchangePtr& key) const
			{
				auto cmp = std::strcmp(x->Exchange, exchange_);
				if (cmp < 0) {
					return true;
				}
				return false;
			}
		};
		ExchangePtr key;
		//lower_bound查找小于/低于keyvalue的end，即返回大于等于的it
		auto it = std::lower_bound(exchanges_.begin(), exchanges_.end(), key, ExchangePtrPred(exchange));
		if (it != exchanges_.end()) {
			auto x = *it;
			if(strcmp(x->Exchange, exchange) == 0) {
				return (it - exchanges_.begin());
			}
		}
		if(flag == find_or_add) {
			key = std::make_shared<TExchange>(exchange);
			it = exchanges_.insert(it,key);
			return (it - exchanges_.begin());
		}
		return -1;
	}
	inline int InnerFindProduct(const char* exchange, const char* product, int flag = find_only)
	{
		struct ProductPtrPred
		{
			const char* exchange_;
			const char* product_;
			ProductPtrPred(const char* exchange, const char* product) :exchange_(exchange), product_(product)
			{}

			bool operator()(const ProductPtr& x, const ProductPtr& key) const
			{
				auto cmp = std::strcmp(x->Exchange, exchange_);
				if (cmp < 0) {
					return true;
				}
				else if (cmp == 0) {
					cmp = std::strcmp(x->Product, product_);
					if (cmp < 0) {
						return true;
					}
				}
				return false;
			}
		};
		ProductPtr key;
		//lower_bound查找小于/低于keyvalue的end，即返回大于等于的it
		auto it = std::lower_bound(products_.begin(), products_.end(), key, ProductPtrPred(exchange, product));
		if (it != products_.end()) {
			auto x = *it;
			if(strcmp(x->Exchange, exchange) == 0 && strcmp(x->Product, product) == 0) {
				return (it - products_.begin());
			}
		}
		if(flag == find_or_add) {
			key = std::make_shared<TProduct>(exchange, product);
			it = products_.insert(it,key);
			return (it - products_.begin());
		}
		return -1;
	}
	inline int InnerFindCommodity(const char* exchange, const char* product, const char *code, int flag = find_only)
	{
		struct CommodityPtrPred
		{
			const char* exchange_;
			const char* product_;
			const char* code_;
			CommodityPtrPred(const char* exchange, const char* product, const char* code) :exchange_(exchange), product_(product), code_(code)
			{}

			bool operator()(const CommodityPtr& x, const CommodityPtr& key) const
			{
				return std::strcmp(x->Code, code_) < 0;
			}
		};
		CommodityPtr key;
		//lower_bound查找小于/低于keyvalue的end，即返回大于等于的it
		auto it = std::lower_bound(commoditys_.begin(), commoditys_.end(), key, CommodityPtrPred(exchange, product, code));
		if (it != commoditys_.end()) {
			auto x = *it;
			if(//strcmp(x->Exchange, exchange) == 0 && strcmp(x->Product, product) == 0 && 
				strcmp(x->Code, code) == 0) {
				return (it - commoditys_.begin());
			}
		}
		if (flag == find_or_add) {
			key = std::make_shared<TCommodity>(exchange, product, code);
			it = commoditys_.insert(it,key);
			return (it - commoditys_.begin());
		}
		return -1;
	}
};

} // namespace MTP
