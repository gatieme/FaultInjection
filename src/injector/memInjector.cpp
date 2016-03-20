#include "memoryInjector.h"

int main(int argc,char **argv)
{
	if(argc < 5)
	{
		Injector::usage();
		exit(RT_EXIT);
	}
	Injector * pInjector = Injector::initInjector(argc, argv);
	if(pInjector == NULL)
	{
		cout << "initInjector return null" << endl;
		exit(RT_EXIT);
	}
	pInjector->startInjection();
	free(pInjector);
	return 0;
}
