#include "TDSrv.hpp"
#include <XUtil/XApp.cpp>

int main(int argc, char *argv[])
{
	MTP::TDSrv app;
	app.init(argc, argv);
	//getchar();
	app.run();
	app.term();
	return 0;
}