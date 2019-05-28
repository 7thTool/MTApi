#ifndef _H_CTPSESSIONAPI_H_
#define _H_CTPSESSIONAPI_H_

#include "CTPExport.h"
#include <string>

//////////////////////////////////////////////////////////////////////////
//请求格式：{ "RequestApi":"Api", "RequestID": 0, "Data": { Property:Value } }
//返回格式：{ "ReturnApi":"Api", "ErrorMsg": "成功", "ErrorID": 0, "RequestID": 0, "IsLast": 0, "Data": { Property:Value } }
//其中:
//Property对应CTP数据结构数据，Value对应CTP数据结构数据值
//请求数据里必须有 RequestApi:Api 属性，例如：{ RequestApi:ReqOrderInsert }
//返回数据里必填ReturnApi:Api，例如：{ ReturnApi:RtnOrder }
//ErrorMsg：返回消息内容
//ErrorID：返回错误值，0表示成功，其他表示错误
//RequestID：请求传入的RequestID原样返回
//IsLast：标示是否是最后一个包
//Data：数据内容，是Property:Value组成的对象
//请求出错：
//请求异常服务器直接断开和客户端连接
//请求非异常错误服务器返回ReturnApi=RequestApi
//返回出错：
//返回异常服务器直接断开和客户端连接
//返回非异常错误ReturnApi=CTP返回相关Api
//////////////////////////////////////////////////////////////////////////
//BrokerID统一为：9999
//标准CTP：
//第一组：Trade Front：180.168.146.187:10000，Market Front：180.168.146.187 : 10010；【电信】
//第二组：Trade Front：180.168.146.187 : 10001，Market Front：180.168.146.187 : 10011；【电信】
//第三组：Trade Front：218.202.237.33 : 10002，Market Front：218.202.237.33 : 10012；【移动】
//账号115569 密码 654321
//WEB
//IP:123.57.8.45:5588
//帐号： administrator
//密码 : af(201 - 30326 * &jUYjes)yy
//////////////////////////////////////////////////////////////////////////

class CTPAPI_API CTPSessionSpi
{
public:
	virtual void OnRecv(const std::string& data) {};
	virtual void OnClose() {}
};

class CTPAPI_API CTPSessionApi
{
public:
	static CTPSessionApi* CreateCTPSessionApi();
	static void DestroyCTPSessionApi(CTPSessionApi* p);

	virtual void RegisterSpi(CTPSessionSpi* pSpi) = 0;

	virtual bool IsOpen() = 0;
	virtual bool Open(const std::string& data) = 0;
	virtual void Send(const std::string& data) = 0;
	virtual void Close() = 0;
};


#endif//_H_CTPSESSIONAPI_H_
