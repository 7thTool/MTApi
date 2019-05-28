#pragma once
#ifndef _H_IINDICATORSET_H_
#define _H_IINDICATORSET_H_

#include <DataDef.h>
#include <IIndicatorApi.h>
#include <IMDDataSet.h>

namespace MTP {

	//class IInputSet;
	//class IBufferInfoSet;
    class IIndicatorSet;
    class IBufferSet;

/**
 * IInputSet输入参数，参数id是字符串,例如：MA指标有参数N，KDJ指标参数K,D,J
 */
//typedef IDataSet IInputSet;
class IInputSet : public IDataSet
{
public:
	virtual const size_t Type() const { return DATASET_TYPE_CALCINPUT; }

    bool Less(const IInputSet* r) const
    {
		if(!r) {
			return false;
		}

		IInputSet* x = const_cast<IInputSet*>(this);
		IInputSet* y = const_cast<IInputSet*>(r);

		int cmp = 0;
		size_t x_field_num = x->GetFieldCount(), y_field_num = y->GetFieldCount();
		if (x_field_num < y_field_num)
		{
			return true;
		}
		else if (x_field_num > y_field_num)
		{
			return false;
		}
		if (x_field_num > 0)
		{
			FieldInfo *x_field_info = (FieldInfo *)alloca(sizeof(FieldInfo) * x_field_num);
			x_field_num = x->GetFieldInfo(x_field_info, x_field_num);
			//FieldInfo* y_field_info = (FieldInfo*)alloca(sizeof(FieldInfo)*y_field_num);
			//y_field_num = x->GetFieldInfo(y_field_info, y_field_num);
			for (size_t i = 0; i < x_field_num; i++)
			{
				const char* x_id = (const char*)x_field_info[i].id;
				//size_t x_size = x->GetFieldSize(x_field_info[i].id);
				void *x_value = x->GetFieldValue((size_t)x_id);
				void *y_value = y->GetFieldValue((size_t)x_id);
				if (!x_value)
				{
					return true;
				}
				else if (!y_value)
				{
					return false;
				}
				switch (x_field_info[i].type)
				{
				//case FIELD_TYPE_CHAR:
				//	break;
				//case FIELD_TYPE_BYTE:
				//	break;
				case FIELD_TYPE_INT8:
					if (*(uint8_t *)x_value < *(uint8_t *)y_value)
					{
						return true;
					}
					else if (*(uint8_t *)x_value > *(uint8_t *)y_value)
					{
						return false;
					}
					break;
				case FIELD_TYPE_INT16:
					if (*(uint16_t *)x_value < *(uint16_t *)y_value)
					{
						return true;
					}
					else if (*(uint16_t *)x_value > *(uint16_t *)y_value)
					{
						return false;
					}
					break;
				case FIELD_TYPE_INT32:
					if (*(uint32_t *)x_value < *(uint32_t *)y_value)
					{
						return true;
					}
					else if (*(uint32_t *)x_value > *(uint32_t *)y_value)
					{
						return false;
					}
					break;
				case FIELD_TYPE_INT64:
					if (*(uint64_t *)x_value < *(uint64_t *)y_value)
					{
						return true;
					}
					else if (*(uint64_t *)x_value > *(uint64_t *)y_value)
					{
						return false;
					}
					break;
				case FIELD_TYPE_DOUBLE:
					if (*(double *)x_value < *(double *)y_value)
					{
						return true;
					}
					else if (*(double *)x_value > *(double *)y_value)
					{
						return false;
					}
					break;
				case FIELD_TYPE_STRING:
					cmp = strcmp((const char*)x_value, (const char*)y_value);
					if (cmp < 0)
					{
						return true;
					}
					else if (cmp > 0)
					{
						return false;
					}
					break;
				default:
					if (x_value < y_value)
					{
						return true;
					}
					else if (x_value > y_value)
					{
						return false;
					}
					break;
				}
			}
		}
		return false;
    }
};

enum BufferType {
	Line,
	Calc,
};

static const size_t IDF_BUFFERINFO_TYPE 	= 0;
static const size_t IDF_BUFFERINFO_SHIFT 	= 1;
static const size_t IDF_BUFFERINFO_BEIGIN 	= 2;
static const size_t IDF_BUFFERINFO_DRAW 	= 3;
static const size_t IDF_BUFFERINFO_LINE 	= 4;
static const size_t IDF_BUFFERINFO_NEXT 	= 5;
static const size_t IDF_BUFFERINFO_DIGITS 	= 6;
static const size_t IDF_BUFFERINFO_WIDTH 	= 7;
static const size_t IDF_BUFFERINFO_COLOR 	= 8;

/**
 * IBufferInfoSet结果参数，参数id是字符串
 */
//typedef IDataSet IBufferInfoSet;
class IBufferInfoSet : public IDataSet
{
public:
	virtual const char* Name() const { return ""; }

	virtual const size_t Type() const { return DATASET_TYPE_CALCBUFFERINFO; }

    bool Less(const IDataSet* r) const
    {
		if(!r) {
			return false;
		}
		const size_t type = Type(), r_type = r->Type();
		if (type < r_type)
            return true;
        else if(r_type < type)
			return false;

		IBufferInfoSet* x = const_cast<IBufferInfoSet*>(dynamic_cast<const IBufferInfoSet*>(this));
		IBufferInfoSet* y = const_cast<IBufferInfoSet*>(dynamic_cast<const IBufferInfoSet*>(r));

		return strcmp(x->Name(),y->Name()) < 0;
    }
	
    virtual bool Equal(const IDataSet* r) const
    {
        if(!r) {
            return false;
        }

		const size_t type = Type(), r_type = r->Type();
		if (type < r_type)
            return true;
        else if(r_type < type)
			return false;

		IBufferInfoSet* x = const_cast<IBufferInfoSet*>(dynamic_cast<const IBufferInfoSet*>(this));
		IBufferInfoSet* y = const_cast<IBufferInfoSet*>(dynamic_cast<const IBufferInfoSet*>(r));

		return strcmp(x->Name(),y->Name()) == 0;
    }
};

/**
 * ICalculatorSet计算器
 */
class INDICATOR_API ICalculatorSet : public IDataSet
{
public:
	static size_t Ref_All_Calculator(ICalculatorSet** calculator, size_t max_count);

    static ICalculatorSet* Ref(const char* name, const IInputSet* input, FindFlag flag = FindFlag::FindOrCreate);
    static void Release(ICalculatorSet* dataset);

	virtual const char* Name() const { return ""; }

	virtual const IInputSet* GetInput() const { return nullptr; }
    inline size_t GetBufferCount() { return GetBufferInfo(nullptr, 0); }
	virtual size_t GetBufferInfo(IBufferInfoSet* buffer, size_t max_count) { return 0; }

	virtual const size_t Type() const { return DATASET_TYPE_CALCULATOR; }

    bool Less(const IDataSet* r) const
    {
		if(!r) {
			return false;
		}
		const size_t type = Type(), r_type = r->Type();
		if (type < r_type)
            return true;
        else if(r_type < type)
			return false;

		const ICalculatorSet* rc = dynamic_cast<const ICalculatorSet*>(r);
		
			int cmp = 0;
			const char *name = Name(), *r_name = rc->Name();
			cmp = strcmp(name, r_name);
			if(cmp < 0) {
				return true;
			} else if(cmp > 0) {
				return false;
			}

			const IInputSet *input = GetInput(), *r_input = rc->GetInput();
			if(*input < *r_input) {
				return true;
			}
			return false;
    };

    bool Equal(const IDataSet* r) const
    {
		if(!r) {
			return false;
		}
		if(Type() != r->Type()) {
			return false;
		}
		
		const ICalculatorSet* rc = dynamic_cast<const ICalculatorSet*>(r);
		
        return (strcmp(Name(),rc->Name()) == 0 && *GetInput() == *rc->GetInput());
    }
};

// class TempCalculatorSetInfo : public ICalculatorSet
// {
// protected:
// 	const char* name_;
// 	const IInputSet* input_;
// public:
// 	TempCalculatorSetInfo(const char* name, const IInputSet* input, size_t input_count, FindFlag flag = FindFlag::FindOrCreate)
// 	:name_(name),input_(input),input_count_(input_count) { }

// 	virtual const char* Name() const { return name_; }
	
// 	virtual IInputSet* Input() { return input_; }
// };

/**
 * IBufferSet计算结果集，参数id是字符串，结果集依赖ICalculatorSet和ICalcDataSet，Buffer数由ICalculatorSet决定
 */
class INDICATOR_API IBufferSet : public IDataSet
{
public:
	static size_t Ref_All_Buffer(IBufferSet** buffer, size_t max_count);

    static IBufferSet* Ref(const char* name, const IInputSet* input, IDataSet* calcdata, FindFlag flag = FindFlag::FindOrCreate);
    static void Release(IBufferSet* dataset);

	//virtual const char* Name() const { return ""; }
	virtual const ICalculatorSet* GetCalculatorSet() const { return nullptr; }
	virtual const ICalcDataSet* GetCalcDataSet() const { return nullptr; }

	virtual const size_t Type() const { return DATASET_TYPE_CALCBUFFER; }

    bool Less(const IDataSet* r) const
    {
		if(!r) {
			return false;
		}
		const size_t type = Type(), r_type = r->Type();
		if (type < r_type)
            return true;
        else if(r_type < type)
			return false;

		IBufferSet* x = const_cast<IBufferSet*>(dynamic_cast<const IBufferSet*>(this));
		IBufferSet* y = const_cast<IBufferSet*>(dynamic_cast<const IBufferSet*>(r));
		const ICalculatorSet* x_calculator = x->GetCalculatorSet();
		if(!x_calculator) {
			return true;
		}
		const ICalculatorSet* y_calculator = y->GetCalculatorSet();
		if(!y_calculator) {
			return false;
		}
		if(*x_calculator < *y_calculator) {
			return true;
		} else if(*x_calculator > *y_calculator) {
			return false;
		}

		const ICalcDataSet* x_calcdata = x->GetCalcDataSet();
		if(!x_calcdata) {
			return true;
		}
		const ICalcDataSet* y_calcdata = y->GetCalcDataSet();
		if(!y_calcdata) {
			return false;
		}
		if(*x_calcdata < *y_calcdata) {
			return true;
		} else if(*x_calcdata > *y_calcdata) {
			return false;
		}
		return false;
    };

    bool Equal(const IDataSet* r) const
    {
		if(!r) {
			return false;
		}
		if(Type() != r->Type()) {
			return false;
		}
		
		IBufferSet* x = const_cast<IBufferSet*>(dynamic_cast<const IBufferSet*>(this));
		IBufferSet* y = const_cast<IBufferSet*>(dynamic_cast<const IBufferSet*>(r));
		
        return (x->GetCalculatorSet() == y->GetCalculatorSet() && x->GetCalcDataSet() == y->GetCalcDataSet());
    }
};

typedef std::shared_ptr<ICalculatorSet> ICalculatorSetPtr;
typedef std::shared_ptr<IBufferSet> IBufferSetPtr;

inline ICalculatorSetPtr MakeCalculatorSetPtr(const char* name, const IInputSet* input, FindFlag flag = FindFlag::FindOrCreate) { 
	return ICalculatorSetPtr(ICalculatorSet::Ref(name,input,flag),&ICalculatorSet::Release);
}
inline std::vector<ICalculatorSetPtr> MakeAllCalculatorSetPtr() { 
    std::vector<ICalculatorSetPtr> calculator_ptrs;
    size_t count = ICalculatorSet::Ref_All_Calculator(nullptr, 0);
	ICalculatorSet** ptrs = (ICalculatorSet**)alloca(sizeof(ICalculatorSet*)*count);
	count = ICalculatorSet::Ref_All_Calculator(ptrs, count);
    calculator_ptrs.resize(count);
    for(size_t i = 0; i < count; i++) 
	{
		calculator_ptrs[i] = ICalculatorSetPtr(ptrs[i],&ICalculatorSet::Release);
	}
	return calculator_ptrs;
}

inline IBufferSetPtr MakeBufferSetPtr(const char* name, const IInputSet* input, IDataSet* calcdata, FindFlag flag = FindFlag::FindOrCreate) { 
	return IBufferSetPtr(IBufferSet::Ref(name, input, calcdata, flag),&IBufferSet::Release);
}

}

#endif//_H_IINDICATORSET_H_
