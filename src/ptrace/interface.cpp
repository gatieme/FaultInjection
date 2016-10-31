/*
*  Author: HIT CS HDMC team.
*  Last modified by zhaozhilong: 2010-1-14
*/
#include "interface.h"

int main(int argc,char **argv)
{
	if(argc < 5)
	{
		Injector::usage();
		exit ( EXIT_ERR );
	}
	Injector * pInjector = Injector::initInjector(argc, argv);
	if(pInjector == NULL)
	{
		cerr << "Exception, Abort" << endl;
		exit ( EXIT_ERR );
	}
	pInjector->startInjection();
	free(pInjector);
	return 0;
}
