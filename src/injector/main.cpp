#include "memoryInjector.h"
#include "memoryInjectorTool.h"

int main(int argc,char **argv)
{
	if(argc < 5)
	{
		Injector::usage();
		exit(RT_EXIT);
	}

    /// version 1.0.0

    Injector * pInjector = Injector::initInjector(argc, argv);

	if(pInjector == NULL)
	{
		cout << "initInjector return null" << endl;
		exit(RT_EXIT);
	}

    pInjector->startInjection();

    free(pInjector);


    /// version 1.0.1
/*
    InjectorTool injectorTool;

    injectorTool.initInjector(argc, argv);
    injectorTool.startInjection();
*/

    return 0;
}
