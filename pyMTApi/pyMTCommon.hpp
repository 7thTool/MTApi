#pragma once
#include <boost/python/class.hpp>
#include <boost/python/module.hpp>
#include <boost/python/def.hpp>
#include <boost/python/pure_virtual.hpp>
#include <boost/python/list.hpp>
#include <boost/python/numpy.hpp>
namespace python = boost::python;
namespace numpy = boost::python::numpy;
#include <XUtil/XUtil.hpp>

namespace MTP {

//用于线程安全
class PyLock
{
private:
	PyGILState_STATE gil_state;
 
public:
	//在某个函数方法中创建该对象时，获得GIL锁
	PyLock()
	{
		gil_state = PyGILState_Ensure();
	}
 
	//在某个函数完成后销毁该对象时，解放GIL锁
	~PyLock()
	{
		PyGILState_Release(gil_state);
	}
};

}
