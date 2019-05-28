#include "CTPSrc.hpp"
#include <XUtil/XApp.cpp>

using namespace MTP;

int main(int argc, char *argv[])
{
	MDCTPSrc app;
	app.init(argc, argv);
	//getchar();
	app.run();
	app.term();
	return 0;
}