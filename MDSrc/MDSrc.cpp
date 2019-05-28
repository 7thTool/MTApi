#include "MDSrc.hpp"
#include <XUtil/XApp.cpp>

using namespace MTP;

int main(int argc, char *argv[])
{
	MDSrc app;
	app.init(argc, argv);
	//getchar();
	app.run();
	app.term();
	return 0;
}