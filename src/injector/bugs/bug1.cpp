#include "memoryEngine.h"
#include "memoryInjector.h"
#include "memoryInjectorTool.h"


//  BUGS-0001
//
//  #ifdef BUGS         //  this is bug_1 find in 2016-03-24 by gatieme
//      printf("==BUG_1==, 0%lx -=> 0x%lx\n", origData, newData);
//  #endif
int main( )
{
    int iRet;
    static long origData;
    static long newData = 0;
    static long inject_pa = 0x12345;   // 0x80000000e040a1d6
    printf("\n\n\ninject_pa = 0x%lx\n", inject_pa);

    iRet = read_phy_mem(inject_pa, &origData);
	if(iRet == FAIL)
    {
        // modify by gatieme
        printf("Error File %s, Line = %d, iRet = %d\n", __FILE__, __LINE__, iRet);
    }
	newData = ~origData;

    printf("start 0x%lx -=> ox%lx\n", origData, newData);
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

