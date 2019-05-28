#pragma once

#include "DataDef.h"
#include "IDataSet.h"

#ifdef WIN32
#ifdef MTApi_EXPORTS
#define MTAPI_API __declspec(dllexport)
#else
#define MTAPI_API __declspec(dllimport)
#endif//
#else //LINUX
#define	MTAPI_API 
#endif//

#if defined(__cplusplus)
extern "C" {
#endif//
//
#if defined(__cplusplus)
}

namespace MTP {

/**
 * IMTApi MTApi全局对象，负责处理客户端和MTApi的交互。
 */
class MTAPI_API IMTApi
{
public:
	static IMTApi& Instance();

	virtual bool Start(char* xml, int xmlflag = 0) = 0;
	virtual void Stop() = 0;
    
protected:
	virtual ~IMTApi() {}
};

}

#endif//
