#include "CTPMarket.h"
#include <XUtil/XController.h>

XUtil::XApp app;
MTP::CTPMarket<XUtil::XApp> src(&app);

extern "C"
{
	CTPAPI_API void Start(char*, int);
	CTPAPI_API void	Stop();
}

void Start(char* xml, int xmlflag)
{
	app.init(xml, xmlflag);
	theCtl.SetHandler("CTPMarket", (void*)&src);
}

void Stop()
{
	//src.Stop();
	app.term();
}
