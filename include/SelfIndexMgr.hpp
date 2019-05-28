#pragma once
#include "SelfIndex.hpp"

namespace MTP {

template<class SelfIndexPtr, class CommodityPtr>
class SelfIndexMgr
{
protected:
	typedef map<std::string, SelfIndexPtr> Code2SelfIndex;
    Code2SelfIndex selfindexs_;
	typedef std::multimap<CommodityPtr,SelfIndexPtr> Commodity2SelfIndex;
    Commodity2SelfIndex commoditys_;
	std::shared_mutex mutex_;
public:
	void Add(SelfIndexPtr index) 
    {
		std::unique_lock<std::shared_mutex> lock(mutex_);
        Code2SelfIndex[index->Code] = index;
        for(auto it : index.GetCommoditys())
        {
            commoditys_[it.first] = index;
        }
    }
	void Remove(SelfIndexPtr index)
    {
		std::unique_lock<std::shared_mutex> lock(mutex_);
        Code2SelfIndex.erase(index.Code);
        for(auto it : index.GetCommoditys())
        {
			auto pr = commoditys_.equal_range(it.first);
			for(auto it_pr = pr.first; it_pr != pr.second; ++it_pr)
			{
				if(it_pr->second == index)
				{
					it_pr = commoditys_.erase(it_pr);
					break;
				}
			}
        }
    }

	void Calculate(CommodityPtr& commodity)
    {
		std::shared_lock<std::shared_mutex> lock(mutex_);
        auto pr = commoditys_.equal_range(commodity);
        for (auto it = pr.first; it != pr.second; ++it)
        {
            it->second->Calculate(commodity);
        }
    }
};

}