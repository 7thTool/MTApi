#pragma once
#include "pyMTCommon.hpp"
#include <MTApiMgr.hpp>
#include <XUtil/XXml.hpp>
#include <XUtil/XStr.hpp>
#include <XUtil/XServer.hpp>
#include <DataSetConv.hpp>
#include <IndicatorSet.hpp>

namespace MTP {

    // struct pyFieldValue : public FieldInfo, public python::wrapper<pyFieldValue>
    // {
    //     pyFieldValue(const FieldInfo &info)
    //     {
    //         *(FieldInfo *)(this) = info;
    //     }
    //     size_t size = 0;
    //     void** pptr = nullptr;

    //     double operator[](int pos)
    //     {
    //         if(pos < 0 || pos >= size) {
    //             return 0;
    //         }
    //         return *(double*)(ptr[pos]);
    //     }
    // };

template<typename _Ty>
struct pyFieldValue
{
    typedef _Ty value_type;
protected:
    std::shared_ptr<IDataSet> dataset_;
    size_t id_;
    _Ty tmp_ = 0;
public:
    pyFieldValue(std::shared_ptr<IDataSet> dataset, size_t id):dataset_(dataset),id_(id) {
    }

    size_t size() { return dataset_?dataset_->GetFieldSize(id_):0; }
    void push_back(const _Ty& val) { }
    void erase(size_t idx) { }
    void clear() { }

    // _Ty& get(int i)
    // {
    //     _Ty* val = (_Ty*)dataset_->GetFieldValue(id_, i);
    //     return val?*val:tmp_;
    // }
    // void set(int i, _Ty const& v)
    // {
        
    // }
	_Ty& operator[](size_t pos) {
        _Ty* val = (_Ty*)dataset_->GetFieldValue(id_, pos);
        return val?*val:tmp_;
    }
};
typedef pyFieldValue<double> py_double_value;
typedef pyFieldValue<uint32_t> py_uint32_value;
typedef pyFieldValue<uint16_t> py_uint16_value;
typedef pyFieldValue<uint8_t> py_uint8_value;
// struct py_double_value : public pyFieldValue<double>
// {
//     typedef pyFieldValue<double> Base;
//     using Base::Base;
// 	double operator[](size_t pos) {
//         return dataset_->GetFieldValueAsDouble(id_, pos);
//     }
// };
// struct py_uint32_value : public pyFieldValue<uint32_t>
// {
//     typedef pyFieldValue<uint32_t> Base;
//     using Base::Base;
// 	uint32_t operator[](size_t pos) {
//         return dataset_->GetFieldValueAsInt32(id_, pos);
//     }
// };
// struct py_uint16_value : public pyFieldValue<uint16_t>
// {
//     typedef pyFieldValue<uint16_t> Base;
//     using Base::Base;
// 	uint16_t operator[](size_t pos) {
//         return dataset_->GetFieldValueAsInt16(id_, pos);
//     }
// };
// struct py_uint8_value : public pyFieldValue<uint8_t>
// {
//     typedef pyFieldValue<uint8_t> Base;
//     using Base::Base;
// 	uint8_t operator[](size_t pos) {
//         return dataset_->GetFieldValueAsInt8(id_, pos);
//     }
// };

//特殊的方法
std::ostream& operator<<(std::ostream& out, py_double_value val)
{
    for(size_t i = 0, j = val.size(); i < j; i++)
    {
        out << val[i];
        if(i != (j - 1)) {
            out << ",";
        }
    }
    return out;
}
std::ostream& operator<<(std::ostream& out, py_uint32_value val)
{
    for(size_t i = 0, j = val.size(); i < j; i++)
    {
        out << val[i];
        if(i != (j - 1)) {
            out << ",";
        }
    }
    return out;
}
std::ostream& operator<<(std::ostream& out, py_uint16_value val)
{
    for(size_t i = 0, j = val.size(); i < j; i++)
    {
        out << val[i];
        if(i != (j - 1)) {
            out << ",";
        }
    }
    return out;
}
std::ostream& operator<<(std::ostream& out, py_uint8_value val)
{
    for(size_t i = 0, j = val.size(); i < j; i++)
    {
        out << val[i];
        if(i != (j - 1)) {
            out << ",";
        }
    }
    return out;
}

struct pyDataSet : public python::wrapper<pyDataSet>
{
protected:
    std::shared_ptr<IDataSet> dataset;
    //std::map<size_t,pyFieldValue> field_values_;
public:
    pyDataSet(std::shared_ptr<IDataSet> s):dataset(s) {
		/*size_t field_num = dataset->GetFieldCount();
		FieldInfo* field_info = (FieldInfo*)alloca(sizeof(FieldInfo)*field_num);
		field_num = dataset->GetFieldInfo(field_info, field_num);
		for(size_t i = 0; i < field_num; i++)
		{
			field_values_[field_info[i].id] = field_info[i];
			field_values_[field_info[i].id].size = dataset->GetFieldSize(field_info[i].id);
            field_values_[field_info[i].id].ptr = dataset->GetFieldValue(field_info[i].id,0);
		}*/
    }
    //pyRspUserLogin(const CThostFtdcRspUserLoginField& o) { *(CThostFtdcRspUserLoginField*)this = o; }

    inline IDataSet* get() const { return dataset.get(); }

	// virtual pyFieldValue GetFieldValue(size_t id) {
    //     pyFieldValue val;
    //     auto it = field_values_.find(id);
    //     if(it != field_values_.end()) {
    //         val = it->second;
    //     }
    //     return val;
    // }
    py_double_value GetFieldAsNumber(size_t id) { return py_double_value(dataset,id); }
    //py_string_value GetFieldAsString(size_t id) { return py_string_value(dataset,id); }

    const char* Exchange() { 
		if (dataset) {
			return (const char*)dataset->GetFieldValue(IDF_Exchange);
		}
		return nullptr;
	}
    const char* Product() { return dataset?(const char*)dataset->GetFieldValue(IDF_Product):nullptr; }
    const char* Code() { return dataset?(const char*)dataset->GetFieldValue(IDF_Code):nullptr; }

    //double UpperPrice() { return dataset?(const char*)dataset->GetFieldValue(IDF_HIGH):nullptr; }
    //double HighestPrice() { return dataset?(const char*)dataset->GetFieldValue(IDF_HIGH):nullptr; }
    //double LowestPrice() { return dataset?(const char*)dataset->GetFieldValue(IDF_HIGH):nullptr; }

    py_uint32_value Date() { return py_uint32_value(dataset,IDF_DATE); }
    py_uint32_value Time() { return py_uint32_value(dataset,IDF_TIME); }
    py_double_value Open() { return py_double_value(dataset,IDF_OPEN); }
    py_double_value High() { return py_double_value(dataset,IDF_HIGH); }
    py_double_value Low() { return py_double_value(dataset,IDF_LOW); }
    py_double_value Close() { return py_double_value(dataset,IDF_CLOSE); }
    py_uint32_value Volume() { return py_uint32_value(dataset,IDF_VOLUME); }
    py_double_value Amount() { return py_double_value(dataset,IDF_AMOUNT); }
    // numpy::ndarray Open() { 
    //     return numpy::from_data((double*)(dataset?dataset->GetFieldValue(IDF_OPEN):nullptr)
    //     , numpy::dtype::get_builtin<double>()
    //     , python::make_tuple(dataset?dataset->GetFieldSize(IDF_OPEN):0)
    //     , python::make_tuple(sizeof(double))
    //     , python::object()
    //     );
    // }

    // numpy::ndarray High() { 
    //     return numpy::from_data((double*)(dataset?dataset->GetFieldValue(IDF_HIGH):nullptr)
    //     , numpy::dtype::get_builtin<double>()
    //     , python::make_tuple(dataset?dataset->GetFieldSize(IDF_HIGH):0)
    //     , python::make_tuple(sizeof(double))
    //     , python::object()
    //     );
    // }

    // numpy::ndarray Low() { 
    //     return numpy::from_data((double*)(dataset?dataset->GetFieldValue(IDF_HIGH):nullptr)
    //     , numpy::dtype::get_builtin<double>()
    //     , python::make_tuple(dataset?dataset->GetFieldSize(IDF_HIGH):0)
    //     , python::make_tuple(sizeof(double))
    //     , python::object()
    //     );
    // }

    // numpy::ndarray Close() { 
    //     return numpy::from_data((double*)(dataset?dataset->GetFieldValue(IDF_CLOSE):nullptr)
    //     , numpy::dtype::get_builtin<double>()
    //     , python::make_tuple(dataset?dataset->GetFieldSize(IDF_CLOSE):0)
    //     , python::make_tuple(sizeof(double))
    //     , python::object()
    //     );
    // }

    // numpy::ndarray Volume() { 
    //     return numpy::from_data((uint32_t*)(dataset?dataset->GetFieldValue(IDF_VOLUME):nullptr)
    //     , numpy::dtype::get_builtin<uint32_t>()
    //     , python::make_tuple(dataset?dataset->GetFieldSize(IDF_VOLUME):0)
    //     , python::make_tuple(sizeof(uint32_t))
    //     , python::object()
    //     );
    // }

    // numpy::ndarray Amount() { 
    //     return numpy::from_data((double*)(dataset?dataset->GetFieldValue(IDF_AMOUNT):nullptr)
    //     , numpy::dtype::get_builtin<double>()
    //     , python::make_tuple(dataset?dataset->GetFieldSize(IDF_AMOUNT):0)
    //     , python::make_tuple(sizeof(double))
    //     , python::object()
    //     );
    // }
};

template<typename _Ty>
struct pyStrFieldValue
{
    typedef _Ty value_type;
protected:
    std::shared_ptr<IDataSet> dataset_;
    std::string id_;
    _Ty tmp_ = 0;
public:
    pyStrFieldValue(std::shared_ptr<IDataSet> dataset, const char* id):dataset_(dataset),id_(id) {
    }

    size_t size() { return dataset_?dataset_->GetFieldSize((size_t)id_.c_str()):0; }
    void push_back(const _Ty& val) { }
    void erase(size_t idx) { }
    void clear() { }

    // _Ty& get(int i)
    // {
    //     _Ty* val = (_Ty*)dataset_->GetFieldValue((size_t)id_.c_str(), i);
    //     return val?*val:tmp_;
    // }
    // void set(int i, _Ty const& v)
    // {
        
    // }
	_Ty& operator[](size_t pos) {
        _Ty* val = (_Ty*)dataset_->GetFieldValue((size_t)id_.c_str(), pos);
        return val?*val:tmp_;
    }
};
typedef pyStrFieldValue<double> py_str_double_value;
typedef pyStrFieldValue<uint32_t> py_str_uint32_value;
typedef pyStrFieldValue<uint16_t> py_str_uint16_value;
typedef pyStrFieldValue<uint8_t> py_str_uint8_value;
struct pyStrFieldString
{
    typedef const char* value_type;
protected:
    std::shared_ptr<IDataSet> dataset_;
    std::string id_;
    const char* tmp_;
public:
    pyStrFieldString(std::shared_ptr<IDataSet> dataset, const char* id):dataset_(dataset),id_(id) {
    }

    size_t size() { return dataset_?dataset_->GetFieldSize((size_t)id_.c_str()):0; }
    void push_back(const char* const val) { }
    void erase(size_t idx) { }
    void clear() { }

    const char* get(int i)
    {
        const char* val = (const char*)dataset_->GetFieldValue((size_t)id_.c_str(), i);
        return val?val:"";
    }
    void set(int i, const char* const v)
    {
        
    }
};
typedef pyStrFieldString py_str_string_value;

std::ostream& operator<<(std::ostream& out, py_str_string_value val)
{
    for(size_t i = 0, j = val.size(); i < j; i++)
    {
        out << val.get(i);
        if(i != (j - 1)) {
            out << ",";
        }
    }
    return out;
}

struct pyStrDataSet : public python::wrapper<pyStrDataSet>
{
protected:
    std::shared_ptr<IDataSet> dataset;
public:
    pyStrDataSet(std::shared_ptr<IDataSet> s):dataset(s) {
		
    }

    inline IDataSet* get() const { return dataset.get(); }

    py_str_double_value GetFieldAsNumber(const char* id) { return py_str_double_value(dataset,id); }
    
    const char* Provider() { return dataset?(const char*)dataset->GetFieldValue((size_t)"Provider"):""; }
    const char* UserID() { 
		if (dataset) {
			return (const char*)dataset->GetFieldValue((size_t)"UserID");
		}
		return "";
	}

    //Order
    py_str_uint32_value Date() { return py_str_uint32_value(dataset,"Date"); }
    py_str_uint32_value Time() { return py_str_uint32_value(dataset,"Time"); }
    py_str_string_value OrderID() { return py_str_string_value(dataset,"OrderID"); }
};

//特殊的方法
std::ostream& operator<<(std::ostream& out, pyDataSet dataset)
{
	if (dataset.get()) {
		DataSetConv conv;
		out << conv.to_json(dataset.get());
	}
	else {
		out << "object is null";
	}
    return out;
}

std::ostream& operator<<(std::ostream& out, pyStrDataSet dataset)
{
	if (dataset.get()) {
		DataSetConv conv;
		out << conv.to_json_ex(dataset.get());
	}
	else {
		out << "object is null";
	}
    return out;
}

struct pyMTApi 
    : public XUtil::XServer<pyMTApi>
	, public CMTApiMgr<pyMTApi>
    , public python::wrapper<pyMTApi>
{
    typedef XUtil::XServer<pyMTApi> Base;
    typedef CMTApiMgr<pyMTApi> MTApi;
    typedef python::wrapper<pyMTApi> pyWrapper;
protected:
    typedef std::map<const char *, ICalculatorSetPtr, XUtil::strless> Calculators;
    Calculators map_calculators_;
public:
    pyMTApi();
    ~pyMTApi();

    void Start(const char *xml = "", int xmlflag = 0);
    void Stop();
    void default_Start(const char *xml = "", int xmlflag = 0);
    void default_Stop();

    // inline std::vector<IExchangeSetPtr> Ref_All_Exchange()
    // {
    //     return MakeAllExchangeSetPtr();
    // }

    // inline std::vector<IProductSetPtr> Ref_All_Product()
    // {
    //     return MakeAllProductSetPtr();
    // }

    // inline std::vector<IProductSetPtr> Ref_All_Product(IExchangeSetPtr exchange_ptr)
    // {
    //     return MakeAllProductSetPtr(exchange_ptr);
    // }

    // inline std::vector<ICommoditySetPtr> Ref_All_Commodity()
    // {
    //     return MakeAllCommoditySetPtr();
    // }

    // inline std::vector<ICommoditySetPtr> Ref_All_Commodity(IProductSetPtr product_ptr)
    // {
    //     return MakeAllCommoditySetPtr(product_ptr);
    // }

    // inline std::vector<ICommoditySetPtr> Ref_All_Commodity(IExchangeSetPtr exchange_ptr)
    // {
    //     return MakeAllCommoditySetPtr(exchange_ptr);
    // }

    // inline std::vector<ICalculatorSetPtr> Ref_All_Calculator()
    // {
    //     return MakeAllCalculatorSetPtr();
    // }

    // inline std::vector<IBufferSetPtr> Ref_All_Buffer()
    // {
    //     return MakeAllBufferSetPtr();
    // }

    inline pyDataSet Ref_Exchange(const char* exchange, FindFlag flag = FindFlag::FindOrCreate)
    {
        return pyDataSet(MakeExchangeSetPtr(exchange, flag));
    }

    inline pyDataSet Ref_Product(const char* exchange, const char* product, FindFlag flag = FindFlag::FindOrCreate)
    {
        return pyDataSet(MakeProductSetPtr(exchange, product, flag));
    }
    
    inline pyDataSet Ref_Commodity(const char* exchange, const char* product, const char* code, FindFlag flag = FindFlag::FindOrCreate)
    {
        return pyDataSet(MakeCommoditySetPtr(exchange, product, code, flag));
    }

    inline pyDataSet Ref_CalcData(const char* exchange, const char* product, const char* code, PERIODTYPE period, DWTYPE dwtype, FindFlag flag = FindFlag::FindOrCreate)
    {
        return pyDataSet(MakeCalcDataSetPtr(exchange, product, code, period, dwtype, flag));
    }

    inline bool pyInput2InputSet(const ICalculatorSetPtr & calculator, const python::object & input, InputSet & input_set)
    {
        python::dict dict_input = python::extract<python::dict>(input.attr("__dict__"));
        if(dict_input) {
            bool set = false;
            IInputSet* def_input_set = const_cast<IInputSet*>(calculator->GetInput());
            size_t field_num = def_input_set->GetFieldCount();
            FieldInfo *field_info = (FieldInfo *)alloca(sizeof(FieldInfo) * field_num);
            field_num = def_input_set->GetFieldInfo(field_info, field_num);
            for (int i = 0; i < field_num; ++i)
            {
                const char* name = (const char*)field_info[i].id;
                python::object val = dict_input[name];
                switch (field_info[i].type)
                {
                case FIELD_TYPE_INT8:
                {
                    set = true;
                    input_set.SetFieldValue(name, python::extract<uint8_t>(val));
                }
                break;
                case FIELD_TYPE_INT16:
                {
                    set = true;
                    input_set.SetFieldValue(name, python::extract<uint16_t>(val));
                }
                break;
                case FIELD_TYPE_INT32:
                {
                    set = true;
                    input_set.SetFieldValue(name, python::extract<uint32_t>(val));
                }
                break;
                case FIELD_TYPE_INT64:
                {
                    set = true;
                    input_set.SetFieldValue(name, python::extract<uint64_t>(val));
                }
                break;
                case FIELD_TYPE_DOUBLE:
                {
                    set = true;
                    input_set.SetFieldValue(name, python::extract<double>(val));
                }
                break;
                case FIELD_TYPE_STRING:
                {
                    set = true;
                    input_set.SetFieldValue(name, python::extract<const char*>(val));
                }
                break;
                case FIELD_TYPE_DATASET:
                default:
                {
                    //
                }
                break;
                }
            }
            return set;
        }
        return false;
    }
    
    inline pyDataSet Ref_Calculator(const char* name, const python::object & input, FindFlag flag = FindFlag::FindOrCreate)
    {
        auto it = map_calculators_.find(name);
        if(it == map_calculators_.end()) {
            return pyDataSet(nullptr);
        }
        InputSet input_set;
        if(pyInput2InputSet(it->second, input, input_set)) {
            return pyDataSet(MakeCalculatorSetPtr(name, &input_set, flag));
        } else {
            return pyDataSet(it->second);
        }
        //return pyDataSet(MakeCalculatorSetPtr(name, &input_set, flag));
    }

    inline pyDataSet Ref_Buffer(const char* name, const python::object & input, const pyDataSet & calcdata, FindFlag flag = FindFlag::FindOrCreate)
    {
        auto it = map_calculators_.find(name);
        if(it == map_calculators_.end()) {
            return pyDataSet(nullptr);
        }
        InputSet input_set;
        if(pyInput2InputSet(it->second, input, input_set)) {
            return pyDataSet(MakeBufferSetPtr(name, &input_set, calcdata.get(), flag));
        }
        return pyDataSet(MakeBufferSetPtr(name, nullptr, calcdata.get(), flag));
    }

	virtual void on_exchange_update(IDataSet* dataset);
	virtual void on_product_update(IDataSet* dataset);
    virtual void on_commodity_update(IDataSet* dataset);

    std::vector<pyStrDataSet> Ref_All_User()
    {
        std::vector<pyStrDataSet> py_all_user;
        auto all_user = MakeAllTradeUserSetPtr();
        for(auto user : all_user)
        {
            py_all_user.push_back(pyStrDataSet(user));
        }
        return py_all_user;
    }

	virtual void on_user_update(IDataSet* dataset, UpdateFlag flag);
	virtual void on_order_update(IDataSet* dataset, UpdateFlag flag);
	virtual void on_trade_update(IDataSet* dataset, UpdateFlag flag);
};

}

using namespace MTP;

template<class T>
struct py_field_item
{
    typedef typename T::value_type V;
    static V& get(T & x, int i)
    {
        if( i<0 ) i+=x.size();
        if( i>=0 && i<x.size() ) return x[i];
        pyIndexError();
    }
    static void set(T & x, int i, V const& v)
    {
        if( i<0 ) i+=x.size();
        if( i>=0 && i<x.size() ) x[i] = v;
        else pyIndexError();
    }
    static void del(T & x, int i)
    {
        if( i<0 ) i+=x.size();
        if( i>=0 && i<x.size() ) x.erase(i);
        else pyIndexError();
    }
    static void add(T & x, V const& v)
    {
        x.push_back(v);
    }
};
template<class T>
struct py_field_string_item
{
    typedef typename T::value_type V;
    static const char* get(T & x, int i)
    {
        if( i<0 ) i+=x.size();
        if( i>=0 && i<x.size() ) return x.get(i);
        pyIndexError();
    }
    static void set(T & x, int i, V const& v)
    {
        if( i<0 ) i+=x.size();
        if( i>=0 && i<x.size() ) x.set(i,v);
        else pyIndexError();
    }
    static void del(T & x, int i)
    {
        if( i<0 ) i+=x.size();
        if( i>=0 && i<x.size() ) x.erase(i);
        else pyIndexError();
    }
    static void add(T & x, V const& v)
    {
        x.push_back(v);
    }
};
void pyIndexError() { PyErr_SetString(PyExc_IndexError, "Index out of range"); }

//https://www.boost.org/doc/libs/1_70_0/libs/python/doc/html/reference/index.html

//BOOST_PYTHON_FUNCTION_OVERLOADS(f_overloads, f, 1, 4); // 参数个数的最小为1，最大为4
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(MTApi_Ref_Exchange_overloads, Ref_Exchange, 1, 2); // 默认参数个数的最小为1，最大为2
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(MTApi_Ref_Product_overloads, Ref_Product, 2, 3); // 默认参数个数的最小为1，最大为2
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(MTApi_Ref_Commodity_overloads, Ref_Commodity, 3, 4); // 默认参数个数的最小为1，最大为2
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(MTApi_Ref_CalcData_overloads, Ref_CalcData, 5, 6); // 默认参数个数的最小为1，最大为2
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(MTApi_Ref_Buffer_overloads, Ref_Buffer, 3, 4); // 默认参数个数的最小为1，最大为2

//BP_REGISTER_SHARED_PTR_TO_PYTHON(IBufferSetPtr);#这个是源码，注释掉，加上下面一句
//python::register_ptr_to_python<IBufferSetPtr>();

//BOOST_PYTHON_OPAQUE_SPECIALIZED_TYPE_ID(Pointee)

BOOST_PYTHON_MODULE(pyMTApi)
{
    using namespace python;

    numpy::initialize();
    PyEval_InitThreads();	//导入时运行，保证先创建GIL

    enum_<PERIODTYPE>("PERIODTYPE")
        .value("TICK", CYC_TICK)
        .value("SEC5", CYC_5SEC)
        .value("SECX", CYC_ANYSEC)
        .value("MIN1", CYC_1MIN)
        .value("MIN5", CYC_5MIN)
        .value("MIN15", CYC_15MIN)
        .value("MIN30", CYC_30MIN)
        .value("HOUR", CYC_60MIN)
        .value("MINX", CYC_ANYMIN)
        .value("DAY", CYC_DAY)
        .value("WEEK", CYC_WEEK)
        .value("MONTH", CYC_MONTH)
        .value("SEASON", CYC_SEASON)
        .value("YEAR", CYC_YEAR)
        .value("DAYX", CYC_ANYDAY)
        ;

    enum_<DWTYPE>("DWTYPE")
        .value("NONE", DW_NONE)
        .value("BACKWARD", DW_BACKWARD)
        .value("FORWARD", DW_FORWARD)
        .value("DAY", DW_DAY)
        .value("ALL", DW_ALL)
        ;

    class_<py_double_value>("py_double_value", no_init)
        .def("__len__", &py_double_value::size)
        .def("clear", &py_double_value::clear)
        .def("append", &py_field_item<py_double_value>::add,
             with_custodian_and_ward<1, 2>()) // to let container keep value
        .def("__getitem__", &py_field_item<py_double_value>::get,
             return_value_policy<copy_non_const_reference>())
        .def("__setitem__", &py_field_item<py_double_value>::set,
             with_custodian_and_ward<1, 2>()) // to let container keep value
        .def("__delitem__", &py_field_item<py_double_value>::del)
        .def(self_ns::str(self_ns::self))  // __str__ for ostream
        ;

    class_<py_uint32_value>("py_uint32_value", no_init)
        .def("__len__", &py_uint32_value::size)
        .def("clear", &py_uint32_value::clear)
        .def("append", &py_field_item<py_uint32_value>::add,
             with_custodian_and_ward<1, 2>()) // to let container keep value
        .def("__getitem__", &py_field_item<py_uint32_value>::get,
             return_value_policy<copy_non_const_reference>())
        .def("__setitem__", &py_field_item<py_uint32_value>::set,
             with_custodian_and_ward<1, 2>()) // to let container keep value
        .def("__delitem__", &py_field_item<py_uint32_value>::del)
        .def(self_ns::str(self_ns::self))  // __str__ for ostream
        ;

    class_<py_uint16_value>("py_uint16_value", no_init)
        .def("__len__", &py_uint16_value::size)
        .def("clear", &py_uint16_value::clear)
        .def("append", &py_field_item<py_uint16_value>::add,
             with_custodian_and_ward<1, 2>()) // to let container keep value
        .def("__getitem__", &py_field_item<py_uint16_value>::get,
             return_value_policy<copy_non_const_reference>())
        .def("__setitem__", &py_field_item<py_uint16_value>::set,
             with_custodian_and_ward<1, 2>()) // to let container keep value
        .def("__delitem__", &py_field_item<py_uint16_value>::del)
        .def(self_ns::str(self_ns::self))  // __str__ for ostream
        ;

    class_<py_uint8_value>("py_uint8_value", no_init)
        .def("__len__", &py_uint8_value::size)
        .def("clear", &py_uint8_value::clear)
        .def("append", &py_field_item<py_uint8_value>::add,
             with_custodian_and_ward<1, 2>()) // to let container keep value
        .def("__getitem__", &py_field_item<py_uint8_value>::get,
             return_value_policy<copy_non_const_reference>())
        .def("__setitem__", &py_field_item<py_uint8_value>::set,
             with_custodian_and_ward<1, 2>()) // to let container keep value
        .def("__delitem__", &py_field_item<py_uint8_value>::del)
        .def(self_ns::str(self_ns::self))  // __str__ for ostream
        ;

    class_<py_str_string_value>("py_str_string_value", no_init)
        .def("__len__", &py_str_string_value::size)
        .def("clear", &py_str_string_value::clear)
        .def("append", &py_field_string_item<py_str_string_value>::add,
             with_custodian_and_ward<1, 2>()) // to let container keep value
        .def("__getitem__", &py_field_string_item<py_str_string_value>::get)
        .def("__setitem__", &py_field_string_item<py_str_string_value>::set,
             with_custodian_and_ward<1, 2>()) // to let container keep value
        .def("__delitem__", &py_field_string_item<py_str_string_value>::del)
        .def(self_ns::str(self_ns::self))  // __str__ for ostream
        ;

    /*class_<IDataSet, no_init>("IDataSet")
        .def("Type", &IDataSet::Type)
        .def("Less", &IDataSet::Less)
        .def("Equal", &IDataSet::Equal)
        .def("GetFieldInfo", &IDataSet::GetFieldInfo)
        .def("GetFieldSize", &IDataSet::GetFieldSize)
        .def("GetFieldValue", &IDataSet::GetFieldValue)
        .def(str(self));  // __str__ for ostream
        ;*/
    
    //class_<pyFieldValue, no_init>("FieldValue")
    //    ;

    class_<pyDataSet>("DataSet", no_init)
        .def("Exchange", &pyDataSet::Exchange)
        .def("Product", &pyDataSet::Product)
        .def("Code", &pyDataSet::Code)
        .def("Date", &pyDataSet::Date)
        .def("Time", &pyDataSet::Time)
        .def("Open", &pyDataSet::Open)
        .def("High", &pyDataSet::High)
        .def("Low", &pyDataSet::Low)
        .def("Close", &pyDataSet::Close)
        .def("Volume", &pyDataSet::Volume)
        .def("Amount", &pyDataSet::Amount)
        .def(self_ns::str(self_ns::self))  // __str__ for ostream
        ;

    class_<pyStrDataSet>("StrDataSet", no_init)
        .def("Provider", &pyStrDataSet::Provider)
        .def("UserID", &pyStrDataSet::UserID)
        .def("Date", &pyStrDataSet::Date)
        .def("Time", &pyStrDataSet::Time)
        .def("OrderID", &pyStrDataSet::OrderID)
        .def(self_ns::str(self_ns::self))  // __str__ for ostream
        ;

    class_<pyMTApi, boost::noncopyable>("MTApi")
        .def("Start", &pyMTApi::Start, &pyMTApi::default_Start)
        .def("Stop", &pyMTApi::Stop, &pyMTApi::default_Stop)
        .def("Ref", &pyMTApi::Ref_Exchange, MTApi_Ref_Exchange_overloads())
        .def("Ref", &pyMTApi::Ref_Product, MTApi_Ref_Product_overloads())
        .def("Ref", &pyMTApi::Ref_Commodity, MTApi_Ref_Commodity_overloads())
        .def("Ref", &pyMTApi::Ref_CalcData, MTApi_Ref_CalcData_overloads())
        .def("Ref", &pyMTApi::Ref_Buffer, MTApi_Ref_Buffer_overloads())
        .def("on_exchange_update", &pyMTApi::on_exchange_update)
        .def("on_product_update", &pyMTApi::on_product_update)
        .def("on_commodity_update", &pyMTApi::on_commodity_update)
        .def("Ref_All_User", &pyMTApi::Ref_All_User)
        ;

    // register_ptr_to_python<IExchangeSetPtr>();
    // register_ptr_to_python<IProductSetPtr>();
    // register_ptr_to_python<ICommoditySetPtr>();
    // register_ptr_to_python<ICalcDataSetPtr>();
    // register_ptr_to_python<IBufferSetPtr>();
    // register_ptr_to_python<IBufferSetPtr>();
}
