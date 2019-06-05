#pragma once
#ifndef _H_IDATASET_H_
#define _H_IDATASET_H_

#include <stdint.h>

static const size_t DATASET_TYPE_NONE		= 0; //默认无类型，即标准CDataSet
static const size_t DATASET_TYPE_EXCHANGE	= 1;
static const size_t DATASET_TYPE_PRODUCT	= 2;
static const size_t DATASET_TYPE_COMMODITY	= 3;
static const size_t DATASET_TYPE_CALCDATA	= 4;
static const size_t DATASET_TYPE_CALCBUFFER	= 5;
static const size_t DATASET_TYPE_CALCINPUT	= 6;
static const size_t DATASET_TYPE_CALCBUFFERINFO	= 7;
static const size_t DATASET_TYPE_CALCULATOR	= 8;
static const size_t DATASET_TYPE_TRADEUSER	= 9;
static const size_t DATASET_TYPE_ORDER	    = 10;
static const size_t DATASET_TYPE_TRADE	    = 11;
static const size_t DATASET_TYPE_POSITION	= 12;

static const size_t FIELD_TYPE_NONE 		= 0;
static const size_t FIELD_TYPE_CHAR 		= 1;
static const size_t FIELD_TYPE_BYTE 		= 1;
static const size_t FIELD_TYPE_INT8 		= 1;
static const size_t FIELD_TYPE_INT16 	    = 2;
static const size_t FIELD_TYPE_INT32		= 3;
static const size_t FIELD_TYPE_INT64 		= 4;
static const size_t FIELD_TYPE_DOUBLE 		= 5;
static const size_t FIELD_TYPE_STRING 		= 6;
static const size_t FIELD_TYPE_DATASET 		= 7;
static const size_t FIELD_TYPE_EXCHANGE 	= 8;
static const size_t FIELD_TYPE_PRODUCT 		= 9;
static const size_t FIELD_TYPE_COMMODITY 	= 10;
static const size_t FIELD_TYPE_CALCDATASET  = 11;

struct FieldInfo
{
	size_t id; //列ID
	size_t type; //列类型

    FieldInfo():id(0),type(0) {}
    FieldInfo(size_t id, size_t type):id(id),type(type) {}
    FieldInfo(const char* id, size_t type):id((size_t)id),type(type) {}
};

class IDataSet
{
  public:
	virtual ~IDataSet() {}
	
	//类型DATASET_TYPE_xxx
	virtual const size_t Type() const { return DATASET_TYPE_NONE; }

	//比较函数
    virtual bool Less(const IDataSet* r) const
    {
        if(!r) {
            return false;
        }
        const size_t type = Type();
        const size_t r_type = r->Type();
        if (type < r_type)
            return true;
        else if (r_type < type)
            return false;

        return false;
    };
    virtual bool Equal(const IDataSet* r) const
    {
        if(!r) {
            return false;
        }
        return (!Less(r) && !r->Less(this));
    }
    bool operator < (const IDataSet & r) const
    {
			return Less(&r);
	}
    bool operator > (const IDataSet & r) const
    {
			return !Less(&r);
	}
    bool operator == (const IDataSet &r) const
    {
        return Equal(&r);
    }
	
	//取字段数目
    inline size_t GetFieldCount() { return GetFieldInfo(nullptr, 0); }
	//取字段信息
	virtual size_t GetFieldInfo(FieldInfo* info, size_t count) { return 0; }
    virtual size_t GetFieldInfoById(size_t id, FieldInfo* info = nullptr) { return 0; }
    inline size_t GetFieldInfoById(const char* id, FieldInfo* info = nullptr) { return GetFieldInfoById((size_t)id, info); }
	//获取字段值的数量
	virtual size_t GetFieldSize(size_t id) { return 0; }
	//获取字段值地址，如果同一字段既有快照又有序列的话，传-1取快照值地址，否则去序列值地址
	virtual void* GetFieldValue(size_t id, size_t offset = 0) { return nullptr; }
    inline void* GetFieldValue(const char* id, size_t offset = 0) { return GetFieldValue((size_t)id, offset); }
    // template<typename Target = double, typename Type = double>
    // inline Target GetFieldValueAs(size_t id, Target def = Target()) { 
    //     Type* ptr = (Type*)GetFieldValue(id, 0);
    //     if(ptr) {
    //         return (Target)(*ptr);
    //     }
    //     return def; 
    // }
    // template<typename Target = double, typename Type = double>
    // inline Target GetFieldValueAs(size_t id, size_t offset, Target def = Target()) { 
    //     Type* ptr = (Type*)GetFieldValue(id, offset);
    //     if(ptr) {
    //         return (Target)(*ptr);
    //     }
    //     return def; 
    // }
	inline void* GetFieldValueEx(size_t id, size_t offset = 0, FieldInfo* info = nullptr) { 
        if(info) {
            GetFieldInfoById(id, info);
        }
        return GetFieldValue(id, offset);
    }
	inline void* GetFieldValueEx(const char* id, size_t offset = 0, FieldInfo* info = nullptr) { 
        if(info) {
            GetFieldInfoById(id, info);
        }
        return GetFieldValue(id, offset);
    }
    // template<typename Target = double>
    // inline Target GetFieldValueAs(size_t id, const Target& def = Target()) { 
    //     return GetFieldValueAs(id, 0, def);
    // }
    // template<typename Target = double>
    // inline Target GetFieldValueAs(size_t id, size_t offset, const Target& def = Target()) { 
    //     FieldInfo info = {0};
    //     void* ptr = (void*)GetFieldValueEx(id, offset, &info);
    //     if(ptr) {
    //         switch(info.type)
    //         {
    //         case FIELD_TYPE_INT8:
    //         {
    //             return (Target)*(uint8_t *)ptr;
    //         }
    //         break;
    //         case FIELD_TYPE_INT16:
    //         {
    //              return (Target)*(uint16_t *)ptr;
    //         }
    //         break;
    //         case FIELD_TYPE_INT32:
    //         {
    //              return (Target)*(uint32_t *)ptr;
    //         }
    //         break;
    //         case FIELD_TYPE_INT64:
    //         {
    //              return (Target)*(int64_t *)ptr;
    //         }
    //         break;
    //         case FIELD_TYPE_DOUBLE:
    //         {
    //              return (Target)*(double *)ptr;
    //         }
    //         break;
    //         case FIELD_TYPE_STRING:
    //         {
    //             //return XUtil::strto<Target>(std::string((const char*)ptr), def);
    //         }
    //         case FIELD_TYPE_DATASET:
    //         default:
    //         {
                
    //         }
    //         break;
    //         }
    //     }
    //     return def; 
    // }
    // //按type返回字段值给value，改接口比GetFieldValue全面，但是性能上多了一次内存拷贝,而且有些是其他字段合成的临时值，也只能通过这个接口取到该值
    // virtual bool GetFieldValueAs(size_t type, void* value, size_t id, size_t offset = 0) { 
    //     size_t raw_type = 0;
    //     void* raw_val = GetFieldValueEx(id, offset, &raw_type);
    //     if(raw_val) {
    //         return Convert(raw_type,raw_val,type,value);
    //     }
    //     return false; 
    // }
    // inline uint8_t GetFieldValueAsInt8(size_t id, size_t offset = 0) { 
    //     uint8_t value = 0;
    //     GetFieldValueAs(FIELD_TYPE_INT8, &value, id, offset);
    //     return value;
    // }
    // inline uint16_t GetFieldValueAsInt16(size_t id, size_t offset = 0) { 
    //     uint16_t value = 0;
    //     GetFieldValueAs(FIELD_TYPE_INT16, &value, id, offset);
    //     return value;
    // }
    // inline uint32_t GetFieldValueAsInt32(size_t id, size_t offset = 0) { 
    //     uint32_t value = 0;
    //     GetFieldValueAs(FIELD_TYPE_INT32, &value, id, offset);
    //     return value;
    // }
    // inline uint64_t GetFieldValueAsInt64(size_t id, size_t offset = 0) { 
    //     uint64_t value = 0;
    //     GetFieldValueAs(FIELD_TYPE_INT64, &value, id, offset);
    //     return value;
    // }
    // inline double GetFieldValueAsDouble(size_t id, size_t offset = 0) { 
    //     double value = 0;
    //     GetFieldValueAs(FIELD_TYPE_DOUBLE, &value, id, offset);
    //     return value;
    // }
};

#endif //_H_IDATASET_H_
