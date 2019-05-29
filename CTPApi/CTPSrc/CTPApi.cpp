#include "CTPApi.hpp"
#include <XUtil/XApp.cpp>

using namespace MTP;

extern "C" {

    static CTPApi api;

MTAPI_API void Start(char* xml, int xmlflag)
{
    api.start(xml, xmlflag);
}

MTAPI_API void Stop()
{
    api.stop();
}

}
