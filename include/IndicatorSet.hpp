#pragma once

#include <IIndicatorSet.h>
#include <DataSet.hpp>

namespace MTP {

class InputSet : public CStrDataSetWrapper<IInputSet>
{
    typedef CStrDataSetWrapper<IInputSet> Base;
public:
    using Base::Base;
    
    /*inline void Set(const char* name, double value) { SetFieldValue(name, value); }
    inline void Set(const char* name, uint64_t value) { SetFieldValue(name, value); }
    inline void Set(const char* name, const char* value) { SetFieldValue(name, value, 1); }
    
	template<typename Ty>
    inline Ty Get(const char* name, const Ty& def()) {
        void* ptr = GetFieldValue(name);
        if(ptr) {
            return *(Ty*)ptr;
        }
        return def;
    }*/
};

}
