#pragma once
#ifndef _H_TDDATASET_HPP_
#define _H_TDDATASET_HPP_

#include <ITDDataSet.h>
#include <DataSet.hpp>
#include <DataSetConv.hpp>
#include <TDBase.hpp>

namespace MTP {

class TDSET_API TradeDataSetEx : public ITradeDataSet
{
public:
    //static TradeDataSetEx& Instance();
};

class TDSET_API TradeUserSetEx : public ITradeUserSet
{
public:
    //virtual TradeUserInfo& get() = 0;
};

class TDSET_API OrderSetEx : public IOrderSet
{
public:
    //virtual OrderInfo& get() = 0;
};

class TDSET_API TradeSetEx : public ITradeSet
{
public:
    //virtual TradeInfo& get() = 0;
};

class TDSET_API PositionSetEx : public IPositionSet
{
public:
    //virtual PositionInfo& get() = 0;
};

}

#endif//_H_TDDATASET_HPP_