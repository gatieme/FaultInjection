#include "memoryEngine.h"
#include "memoryInjector.h"
#include "memoryInjectorTool.h"

void test( );

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

    test( );
    /// version 1.0.1
/*
    InjectorTool injectorTool;

    injectorTool.initInjector(argc, argv);
    injectorTool.startInjection();
*/

    return 0;
}


void test( )
{
    int iRet;
    long origData;
    long newData = 0;
    long inject_pa = 0x12345;
    printf("\n\n\ninject_pa = 0x%lx\n", inject_pa);

    iRet = read_phy_mem(inject_pa, &origData);
	if(iRet == FAIL)
    {
        // modify by gatieme
        printf("Error File %s, Line = %d, iRet = %d\n", __FILE__, __LINE__, iRet);
    }
	newData = 0;

    iRet = write_phy_mem(inject_pa, &newData, sizeof(newData));
    if(iRet == FAIL)
    {
        // modify by gatieme
        printf("Error File %s, Line = %d, iRet = %d\n", __FILE__, __LINE__, iRet);
    }

    iRet = read_phy_mem(inject_pa, &newData);
	if(iRet == FAIL)
    {
        // modify by gatieme
        printf("Error File %s, Line = %d, iRet = %d\n", __FILE__, __LINE__, iRet);
    }

    printf("word 0(0x%lx -> 0x%lx)\n", origData, newData);

}
