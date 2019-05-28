#include "pyMTApi.hpp"
#include <XUtil/XApp.cpp>

namespace MTP {

pyMTApi::pyMTApi():Base(),MTApi(this)
{
    std::cout << "pyMTApi::pyMTApi()" << std::endl;
}

pyMTApi::~pyMTApi()
{
    std::cout << "pyMTApi::~pyMTApi()" << std::endl;
}

void pyMTApi::Start(const char *xml, int xmlflag) 
{
    std::cout << "pyMTApi::Start " << xml << ", " << xmlflag << std::endl;
    if (python::override start = this->get_override("Start")) {
        start(xml, xmlflag); //如果函数进行重载了，则返回重载的
    } else {
        //return Base::f(); //否则返回基类
	    default_Start(xml, xmlflag);
    }
}

void pyMTApi::Stop()
{
    std::cout << "pyMTApi::Stop()" << std::endl;
    if (python::override stop = this->get_override("Stop")) {
        stop(); //如果函数进行重载了，则返回重载的
    } else {
        default_Stop();
    }
}

void pyMTApi::default_Start(const char *xml, int xmlflag) 
{
    std::cout << "pyMTApi::default_Start " << xml << ", " << xmlflag << std::endl;
    MTApi::Start(const_cast<char*>(xml), xmlflag);
    std::vector<ICalculatorSetPtr> all_calculators = MakeAllCalculatorSetPtr();
    for(auto calculator : all_calculators) {
        map_calculators_[calculator->Name()] = calculator;
    }
}

void pyMTApi::default_Stop()
{
    std::cout << "pyMTApi::default_Stop()" << std::endl;
    map_calculators_.clear();
    MTApi::Stop();
}

void pyMTApi::on_exchange_update(IDataSet* dataset, UpdateFlag flag)
{
    PyLock lock;
    try
    {
        //get_override("on_exchange_update")(ptr(&DepthMarketData));
    }
    catch (python::error_already_set const &)
    {
        PyErr_Print();
    }
}

void pyMTApi::on_product_update(IDataSet* dataset, UpdateFlag flag)
{
    PyLock lock;
    try
    {
        //get_override("on_product_update")(ptr(&DepthMarketData));
    }
    catch (python::error_already_set const &)
    {
        PyErr_Print();
    }
}

void pyMTApi::on_commodity_update(IDataSet* dataset, UpdateFlag flag)
{
    PyLock lock;
    try
    {
        //get_override("on_commodity_update")(ptr(&DepthMarketData));
    }
    catch (python::error_already_set const &)
    {
        PyErr_Print();
    }
}

void pyMTApi::on_user_update(IDataSet* dataset, UpdateFlag flag)
{
    PyLock lock;
    try
    {
        //get_override("on_commodity_update")(ptr(&DepthMarketData));
    }
    catch (python::error_already_set const &)
    {
        PyErr_Print();
    }
}

void pyMTApi::on_order_update(IDataSet* dataset, UpdateFlag flag)
{
    PyLock lock;
    try
    {
        //get_override("on_commodity_update")(ptr(&DepthMarketData));
    }
    catch (python::error_already_set const &)
    {
        PyErr_Print();
    }
}

void pyMTApi::on_trade_update(IDataSet* dataset, UpdateFlag flag)
{
    PyLock lock;
    try
    {
        //get_override("on_commodity_update")(ptr(&DepthMarketData));
    }
    catch (python::error_already_set const &)
    {
        PyErr_Print();
    }
}

}
