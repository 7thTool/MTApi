#pragma once
#ifndef _H_DATASET_HPP_
#define _H_DATASET_HPP_

#include <IDataSet.h>
#include <DataDef.h>
#include <FieldDef.h>

namespace MTP {
/*
class DataSet : public IDataSet
{
public:
	virtual const size_t Type() const { return DATASET_TYPE_NONE; }

	virtual size_t GetFieldInfo(FieldInfo* info, size_t count) { return 0; }
	virtual size_t GetFieldSize(size_t id) { return 0; }
	virtual void* GetFieldValue(size_t id, size_t offset = 0) { return nullptr; }
};
*/
#include <map>
#include <vector>
#include <string>
#include <initializer_list>

template<class CDataSet>
struct FieldValueWrapper : public FieldInfo
{
	size_t max_size = 0;
	size_t size = 0;
	union {
		char c;
		uint8_t n8;
		uint16_t n16;
		uint32_t n32;
		uint64_t n64 = 0;
		double f64;
		//std::string str; //std::string*
		void *ptr;
		uint8_t *p8;
		uint16_t *p16;
		uint32_t *p32;
		uint64_t *p64;
		double *pf64;
		std::string *str;
		void **pptr;
	};
	FieldValueWrapper() { }
	FieldValueWrapper(const FieldInfo &info)
	{
		*(FieldInfo *)(this) = info;
	}
	FieldValueWrapper(size_t id, size_t type, void* value, size_t size):FieldInfo(id,type) {
		Set(value, size);
	}
	~FieldValueWrapper()
	{
		Reset();
	}

	inline size_t GetSize() { return size; }

	inline void *GetValue(size_t offset)
	{
		auto &field_value = *this;
		if (offset >= field_value.size)
		{
			return nullptr;
		}
		else if (field_value.size == 0)
		{
			return nullptr;
		}
		else if (field_value.max_size == 1)
		{
			switch (field_value.type)
			{
			//case FIELD_TYPE_CHAR:
			//	break;
			//case FIELD_TYPE_BYTE:
			//	break;
			case FIELD_TYPE_INT8:
				return &field_value.n8;
				break;
			case FIELD_TYPE_INT16:
				return &field_value.n16;
				break;
			case FIELD_TYPE_INT32:
				return &field_value.n32;
				break;
			case FIELD_TYPE_INT64:
				return &field_value.n64;
				break;
			case FIELD_TYPE_DOUBLE:
				return &field_value.f64;
				break;
			case FIELD_TYPE_STRING:
				return (void *)field_value.str->c_str();
				break;
			default:
				return field_value.ptr;
				break;
			}
		}
		else
		{
			switch (field_value.type)
			{
			//case FIELD_TYPE_CHAR:
			//	break;
			//case FIELD_TYPE_BYTE:
			//	break;
			case FIELD_TYPE_INT8:
				return field_value.p8 + offset;
				break;
			case FIELD_TYPE_INT16:
				return field_value.p16 + offset;
				break;
			case FIELD_TYPE_INT32:
				return field_value.p32 + offset;
				break;
			case FIELD_TYPE_INT64:
				return field_value.p64 + offset;
				break;
			case FIELD_TYPE_DOUBLE:
				return field_value.pf64 + offset;
				break;
			case FIELD_TYPE_STRING:
				return (void *)(field_value.str + offset)->c_str();
				break;
			default:
				return field_value.pptr + offset;
				break;
			}
		}
		return nullptr;
	}

	void SetMaxSize(size_t size) {
		auto &field_value = *this;
		if(field_value.size == 0 && field_value.max_size == 0)
			field_value.max_size = size;
	}

	void * SetSize(size_t size) {
		void* ret = nullptr;
		auto &field_value = *this;
		if (size > field_value.max_size)
		{
			Reset();
		}
		if (size > field_value.max_size)
		{
			field_value.max_size = size + field_value.max_size;
		}
		field_value.size = size;
		if (field_value.size == 0)
		{
			;
		}
		else if (field_value.max_size == 1)
		{
			switch (field_value.type)
			{
			//case FIELD_TYPE_CHAR:
			//	break;
			//case FIELD_TYPE_BYTE:
			//	break;
			case FIELD_TYPE_INT8:
				ret = &field_value.n8;
				break;
			case FIELD_TYPE_INT16:
				ret = &field_value.n16;
				break;
			case FIELD_TYPE_INT32:
				ret = &field_value.n32;
				break;
			case FIELD_TYPE_INT64:
				ret = &field_value.n64;
				break;
			case FIELD_TYPE_DOUBLE:
				ret = &field_value.f64;
				break;
			case FIELD_TYPE_STRING:
				if (!field_value.str)
				{
					field_value.str = new std::string();
				}
				ret = (void*)field_value.str->c_str();
				break;
			case FIELD_TYPE_DATASET:
			default:
				ret = field_value.ptr;
				break;
			}
		}
		else
		{
			switch (field_value.type)
			{
			//case FIELD_TYPE_CHAR:
			//	break;
			//case FIELD_TYPE_BYTE:
			//	break;
			case FIELD_TYPE_INT8:
				if (!field_value.p8)
				{
					field_value.p8 = new uint8_t[field_value.max_size];
				}
				ret = field_value.p8;
				break;
			case FIELD_TYPE_INT16:
				if (!field_value.p16)
				{
					field_value.p16 = new uint16_t[field_value.max_size];
				}
				ret = field_value.p16;
				break;
			case FIELD_TYPE_INT32:
				if (!field_value.p32)
				{
					field_value.p32 = new uint32_t[field_value.max_size];
				}
				ret = field_value.p32;
				break;
			case FIELD_TYPE_INT64:
				if (!field_value.p64)
				{
					field_value.p64 = new uint64_t[field_value.max_size];
				}
				ret = field_value.p64;
				break;
			case FIELD_TYPE_DOUBLE:
				if (!field_value.pf64)
				{
					field_value.pf64 = new double[field_value.max_size];
				}
				ret = field_value.pf64;
				break;
			case FIELD_TYPE_STRING:
				if (!field_value.str)
				{
					field_value.str = new std::string[field_value.max_size];
				}
				ret = (void *)(field_value.str + 0)->c_str();
				break;
			case FIELD_TYPE_DATASET:
			default:
				if (!field_value.pptr)
				{
					field_value.pptr = new void *[field_value.max_size];
				}
				ret = field_value.pptr;
				break;
			}
		}
		return ret;
	}
	template <typename Ty>
	inline void *SetValue(const Ty &value)
	{
		return SetValue((void *)&value, 1);
	}
	inline void *SetValue(void *value, size_t size)
	{
		void* ret = nullptr;
		auto &field_value = *this;
		if (!value || size > field_value.max_size)
		{
			Reset();
		}
		SetSize(size);
		if (field_value.size == 0)
		{
			;
		}
		else if (field_value.max_size == 1)
		{
			switch (field_value.type)
			{
			//case FIELD_TYPE_CHAR:
			//	break;
			//case FIELD_TYPE_BYTE:
			//	break;
			case FIELD_TYPE_INT8:
				field_value.n8 = *(uint8_t *)value;
				ret = &field_value.n8;
				break;
			case FIELD_TYPE_INT16:
				field_value.n16 = *(uint16_t *)value;
				ret = &field_value.n16;
				break;
			case FIELD_TYPE_INT32:
				field_value.n32 = *(uint32_t *)value;
				ret = &field_value.n32;
				break;
			case FIELD_TYPE_INT64:
				field_value.n64 = *(uint64_t *)value;
				ret = &field_value.n64;
				break;
			case FIELD_TYPE_DOUBLE:
				field_value.f64 = *(double *)value;
				ret = &field_value.f64;
				break;
			case FIELD_TYPE_STRING:
				field_value.str->assign((char *)value);
				ret = (void *)field_value.str->c_str();
				break;
			case FIELD_TYPE_DATASET:
				if (field_value.ptr)
				{
					delete (CDataSet *)field_value.ptr;
				}
			default:
				field_value.ptr = value;
				ret = field_value.ptr;
				break;
			}
		}
		else
		{
			switch (field_value.type)
			{
			//case FIELD_TYPE_CHAR:
			//	break;
			//case FIELD_TYPE_BYTE:
			//	break;
			case FIELD_TYPE_INT8:
				memcpy(field_value.p8, (uint8_t *)value, size * sizeof(uint8_t));
				ret = field_value.p8;
				break;
			case FIELD_TYPE_INT16:
				memcpy(field_value.p16, (uint16_t *)value, size * sizeof(uint16_t));
				ret = field_value.p16;
				break;
			case FIELD_TYPE_INT32:
				memcpy(field_value.p32, (uint32_t *)value, size * sizeof(uint32_t));
				ret = field_value.p32;
				break;
			case FIELD_TYPE_INT64:
				memcpy(field_value.p64, (uint64_t *)value, size * sizeof(uint64_t));
				ret = field_value.p64;
				break;
			case FIELD_TYPE_DOUBLE:
				memcpy(field_value.pf64, (double *)value, size * sizeof(double));
				ret = field_value.pf64;
				break;
			case FIELD_TYPE_STRING:
				for (size_t i = 0; i < size; i++)
				{
					field_value.str[i] = ((char **)value)[i];
				}
				ret = (void *)(field_value.str + 0)->c_str();
				break;
			case FIELD_TYPE_DATASET:
				if (field_value.pptr)
				{
					for (size_t i = 0; i < field_value.size; i++)
					{
						CDataSet *dataset = (CDataSet *)field_value.pptr[i];
						delete dataset;
					}
				}
			default:
				memcpy(field_value.pptr, (void **)value, size * sizeof(void *));
				ret = field_value.pptr;
				break;
			}
		}
		return ret;
	}

	inline void Reset()
	{
		auto &field_value = *this;
		if (field_value.size == 0)
		{
			;
		}
		else if (field_value.max_size == 1)
		{
			switch (field_value.type)
			{
			//case FIELD_TYPE_CHAR:
			//	break;
			//case FIELD_TYPE_BYTE:
			//	break;
			case FIELD_TYPE_INT8:
				break;
			case FIELD_TYPE_INT16:
				break;
			case FIELD_TYPE_INT32:
				break;
			case FIELD_TYPE_INT64:
				break;
			case FIELD_TYPE_DOUBLE:
				break;
			case FIELD_TYPE_STRING:
				if (field_value.str)
				{
					delete field_value.str;
				}
				break;
			case FIELD_TYPE_DATASET:
				if (field_value.ptr)
				{
					delete (CDataSet *)field_value.ptr;
				}
				break;
			default:
				break;
			}
		}
		else
		{
			switch (field_value.type)
			{
			//case FIELD_TYPE_CHAR:
			//	break;
			//case FIELD_TYPE_BYTE:
			//	break;
			case FIELD_TYPE_INT8:
				if (field_value.p8)
				{
					delete[] field_value.p8;
				}
				break;
			case FIELD_TYPE_INT16:
				if (field_value.p16)
				{
					delete[] field_value.p16;
				}
				break;
			case FIELD_TYPE_INT32:
				if (field_value.p32)
				{
					delete[] field_value.p32;
				}
				break;
			case FIELD_TYPE_INT64:
				if (field_value.p64)
				{
					delete[] field_value.p64;
				}
				break;
			case FIELD_TYPE_DOUBLE:
				if (field_value.pf64)
				{
					delete[] field_value.pf64;
				}
				break;
			case FIELD_TYPE_STRING:
				if (field_value.str)
				{
					delete[] field_value.str;
				}
				break;
			case FIELD_TYPE_DATASET:
				if (field_value.pptr)
				{
					for (size_t i = 0; i < field_value.size; i++)
					{
						CDataSet *dataset = (CDataSet *)field_value.pptr[i];
						delete dataset;
					}
					delete[] field_value.pptr;
				}
				break;
			default:
				if (field_value.pptr)
				{
					delete[] field_value.pptr;
				}
				break;
			}
		}
		field_value.n64 = 0;	  	//
		field_value.size = 0;	 	//
		field_value.max_size = 0; 	//
	}
};

template<class BaseSet = IDataSet>
class CDataSetWrapper : public BaseSet
{
	typedef CDataSetWrapper<BaseSet> This;
	typedef BaseSet Base;
	typedef FieldValueWrapper<This> FieldValue;
	protected:
		std::map<size_t,FieldValue> field_values_;
	public:
		CDataSetWrapper() { }
		CDataSetWrapper(const std::initializer_list<FieldInfo>& list)
		{
			Init(list);
		}
		CDataSetWrapper(FieldInfo* field_info, size_t count)
		{
			Init(field_info, count);
		}
		CDataSetWrapper(IDataSet& dataset)
		{
			size_t field_num = dataset.GetFieldCount();
			FieldInfo* field_info = (FieldInfo*)alloca(sizeof(FieldInfo)*field_num);
			field_num = dataset.GetFieldInfo(field_info, field_num);
			for(size_t i = 0; i < field_num; i++)
			{
				field_values_[field_info[i].id] = field_info[i];
				size_t field_size = dataset.GetFieldSize(field_info[i].id);
				SetFieldValue(field_info[i].id, dataset.GetFieldValue(field_info[i].id,0), field_size);
			}
		}
		CDataSetWrapper(const This& dataset)
		{
			field_values_ = dataset.field_values_;
		}
		CDataSetWrapper(const This&& dataset)
		{
			field_values_ = std::move(dataset.field_values_);
		}
		~CDataSetWrapper()
		{
			for(auto& it : field_values_)
			{
				auto& field_value = it.second;
				field_value.Reset();
			}
		}
		
		inline void Init(const std::initializer_list<FieldInfo>& list)
		{
			for (auto it = list.begin(); it != list.end(); ++it) {
            	field_values_[it->id] = *it;
        	}
		}
		inline void Init(const FieldInfo* field_info, size_t count)
		{
			for (size_t i = 0; i < count; i++) {
            	field_values_[field_info[i].id] = field_info[i];
        	}
		}
		inline void SetFieldInfo(const FieldInfo* field_info, size_t count = 1)
		{
			for (size_t i = 0; i < count; i++) {
            	field_values_[field_info[i].id] = field_info[i];
        	}
		}

	virtual size_t GetFieldInfo(FieldInfo* info, size_t count) { 
		if (!info || !count) {
			return field_values_.size();
		}
		size_t index = 0;
		for(auto& it : field_values_)
		{
			info[index++] = it.second;
		}
		return index;
	}
        virtual size_t GetFieldInfoById(size_t id, FieldInfo* info = nullptr)
		{
			if(!info) {
				return 0;
			}
			auto it = field_values_.find(id);
			if(it != field_values_.end())
			{
				info->id = id;
				info->type = it->second.type;
				return id;
			}
			return 0;
		}
	virtual size_t GetFieldSize(size_t id) { 
		auto it = field_values_.find(id);
		if(it != field_values_.end()) {
			auto& field_value = it->second;
			return field_value.size;
		}
		return 0;
	}
	virtual void * GetFieldValue(size_t id, size_t offset = 0) { 
		auto it = field_values_.find(id);
		if(it != field_values_.end()) {
			auto& field_value = it->second;
			return field_value.GetValue(offset);
		}
		return nullptr; 
	}

	void SetFieldMaxSize(size_t id, void* value, size_t size) { 
		auto it = field_values_.find(id);
		if(it != field_values_.end()) {
			auto& field_value = it->second;
			field_value.SetMaxSize(size);
		}
	}
	void * SetFieldSize(size_t id, size_t size) { 
		auto it = field_values_.find(id);
		if(it != field_values_.end()) {
			auto& field_value = it->second;
			return field_value.SetSize(size);
		}
		return nullptr;
	}
	template<typename Ty>
	inline void * SetFieldValue(size_t id, const Ty& value) { 
		return SetFieldValue(id, (void*)&value, 1);
	}
	void * SetFieldValue(size_t id, void* value, size_t size) { 
		auto it = field_values_.find(id);
		if(it != field_values_.end()) {
			auto& field_value = it->second;
			return field_value.SetValue(value, size);
		}
		return nullptr;
	}
};

typedef CDataSetWrapper<IDataSet> CDataSet;

template<class BaseSet = IDataSet>
class CStrDataSetWrapper : public BaseSet
{
	typedef CStrDataSetWrapper<BaseSet> This;
	typedef BaseSet Base;
	typedef FieldValueWrapper<This> FieldValue;
	protected:
		std::map<std::string,FieldValue> field_values_;
	public:
		CStrDataSetWrapper() { }
		CStrDataSetWrapper(const std::initializer_list<FieldInfo>& list)
		{
			Init(list);
		}
		CStrDataSetWrapper(FieldInfo* field_info, size_t count)
		{
			Init(field_info, count);
		}
		CStrDataSetWrapper(IDataSet& dataset)
		{
			size_t field_num = dataset.GetFieldCount();
			FieldInfo* field_info = (FieldInfo*)alloca(sizeof(FieldInfo)*field_num);
			field_num = dataset.GetFieldInfo(field_info, field_num);
			for(size_t i = 0; i < field_num; i++)
			{
				//field_values_[(const char*)field_info[i].id] = field_info[i];
				auto pr_insert = field_values_.insert(std::make_pair((const char*)field_info[i].id, field_info[i]));
				if(pr_insert.second) {
					pr_insert.first->second.id = (size_t)pr_insert.first->first.c_str();
				}
				size_t field_size = dataset.GetFieldSize(field_info[i].id);
				SetFieldValue(field_info[i].id, dataset.GetFieldValue(field_info[i].id,0), field_size);
			}
		}
		CStrDataSetWrapper(const This& dataset)
		{
			field_values_ = dataset.field_values_;
		}
		CStrDataSetWrapper(const This&& dataset)
		{
			field_values_ = std::move(dataset.field_values_);
		}
		~CStrDataSetWrapper()
		{
			for(auto& it : field_values_)
			{
				auto& field_value = it.second;
				field_value.Reset();
			}
		}
		
		inline void Init(const std::initializer_list<FieldInfo>& list)
		{
			for (auto it = list.begin(); it != list.end(); ++it) {
				auto pr_insert = field_values_.insert(std::make_pair((const char*)it->id, *it));
				if(pr_insert.second) {
					pr_insert.first->second.id = (size_t)pr_insert.first->first.c_str();
				}
        	}
		}
		inline void Init(const FieldInfo* field_info, size_t count)
		{
			for (size_t i = 0; i < count; i++) {
				auto pr_insert = field_values_.insert(std::make_pair((const char*)field_info[i].id, field_info[i]));
				if(pr_insert.second) {
					pr_insert.first->second.id = (size_t)pr_insert.first->first.c_str();
				}
        	}
		}
		inline void SetFieldInfo(const FieldInfo* field_info, size_t count = 1)
		{
			for (size_t i = 0; i < count; i++) {
            	auto pr_insert = field_values_.insert(std::make_pair((const char*)field_info[i].id, field_info[i]));
				if(pr_insert.second) {
					pr_insert.first->second.id = (size_t)pr_insert.first->first.c_str();
				}
        	}
		}

	virtual size_t GetFieldInfo(FieldInfo* info, size_t count) { 
		if (!info || !count) {
			return field_values_.size();
		}
		size_t index = 0;
		for(auto& it : field_values_)
		{
			info[index++] = it.second;
		}
		return index;
	}
        virtual size_t GetFieldInfoById(size_t id, FieldInfo* info = nullptr)
		{
			if(!info) {
				return 0;
			}
			auto it = field_values_.find((const char*)id);
			if(it != field_values_.end()) {
				info->id = id;
				info->type = it->second.type;
				return id;
			}
			return 0;
		}
	virtual size_t GetFieldSize(size_t id) { 
		auto it = field_values_.find((const char*)id);
		if(it != field_values_.end()) {
			auto& field_value = it->second;
			return field_value.size;
		}
		return 0;
	}
	virtual void * GetFieldValue(size_t id, size_t offset = 0) { 
		auto it = field_values_.find((const char*)id);
		if(it != field_values_.end()) {
			auto& field_value = it->second;
			return field_value.GetValue(offset);
		}
		return nullptr; 
	}
	inline size_t GetFieldSize(const char* id) { 
		return GetFieldSize((size_t)id); 
	}
	inline void * GetFieldValue(const char* id, size_t offset = 0) { 
		return GetFieldValue((size_t)id, offset); 
	}

	void SetFieldMaxSize(size_t id, size_t size) { 
		auto it = field_values_.find((const char*)id);
		if(it != field_values_.end()) {
			auto& field_value = it->second;
			field_value.SetMaxSize(size);
		}
	}
	void * SetFieldSize(size_t id, size_t size) { 
		auto it = field_values_.find((const char*)id);
		if(it != field_values_.end()) {
			auto& field_value = it->second;
			return field_value.SetSize(size);
		}
		return nullptr;
	}
	template<typename Ty>
	inline void * SetFieldValue(size_t id, const Ty& value) { 
		return SetFieldValue((const char*)id, (void*)&value, 1);
	}
	void * SetFieldValue(size_t id, void* value, size_t size) { 
		auto it = field_values_.find((const char*)id);
		if(it != field_values_.end()) {
			auto& field_value = it->second;
			return field_value.SetValue(value, size);
		}
		return nullptr;
	}
	inline void * SetFieldMaxSize(const char* id, size_t size) { 
		return SetFieldMaxSize((size_t)id, size);
	}
	inline void * SetFieldSize(const char* id, size_t size) { 
		return SetFieldSize((size_t)id, size);
	}
	template<typename Ty>
	inline void * SetFieldValue(const char* id, const Ty& value) { 
		return SetFieldValue(id, (void*)&value, 1);
	}
	inline void * SetFieldValue(const char* id, void* value, size_t size) { 
		return SetFieldValue((size_t)id, value, size);
	}
};

typedef CStrDataSetWrapper<IDataSet> CStrDataSet;

}

#endif//_H_DATASET_HPP_
