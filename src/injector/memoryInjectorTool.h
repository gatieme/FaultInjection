/*
*  Author: HIT CS HDMC team.
*  Create: 2010-3-13 9:20:04
*  Last modified: 2010-6-16 14:11:00
*/

#ifndef MEMORY_INJECTORTOOL_H_
#define MEMORY_INJECTORTOOL_H_

/*
*	common header files
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdarg.h>	//变长参数
#include <time.h>
#include <sys/time.h>

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

#include "ptrace.h"
#include "memoryInjector.h"
#include "memoryFault.h"

class InjectorTool
{
public:
	InjectorTool( );
	~InjectorTool( );
    void usage( );

    int initFaultTable( int argc, char **argv );
    int startInjection( );
    Injector* CreateInjector( int argc, char **argv );
    int initFaultTable( );
protected :

    //
    //  故障注入工具
    //
    Injector            *m_injector;             // the memory fault injector object

    int                 m_targetPid;      //  target is an existing process.
    char                **m_exeArguments;


    //
    //  接收到的控制台参数
    //
    int                 m_argc;                  // the count of the arguement
    char                **m_argv;                // the arguement list

    //
    //  使用-c指定故障注入表
    //  使用-l[注入位置] -m[模式, 随机还是地址] -t[]指定故障注入类型
    //
    bool                m_hasFaultTable;         // 如果使用了-c参数就使用注入表,此值为true
    //  when m_hasFaultTable == false
    MemoryFault         m_memoryFault;           //
    //  when m_hasFaultTable == true
    string                  m_memoryFaultTablePath;
	vector <MemoryFault>    m_memoryFaultTable;
};

#endif	/* MEMORY_INJECTORTOOL_H_ */
