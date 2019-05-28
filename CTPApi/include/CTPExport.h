#ifndef __H_CTPAPI_EXPORT_H__
#define __H_CTPAPI_EXPORT_H__

#ifdef WIN32
#ifdef CTPApi_EXPORTS
#define CTPAPI_API __declspec(dllexport)
#else
#define CTPAPI_API __declspec(dllimport)
#endif
#else
#define CTPAPI_API
#endif//WIN32

#define CTPAPI_VERSION_MAJOR 1
#define CTPAPI_VERSION_MINOR 1
#define CTPAPI_VERSION "1.1.1"

#endif //__H_CTPAPI_EXPORT_H__
