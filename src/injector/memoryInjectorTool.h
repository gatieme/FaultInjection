/*
*  Author: HIT CS HDMC team.
*  Create: 2010-3-13 9:20:04
*  Last modified: 2010-6-16 14:11:00
*/

#ifndef MEMORY_INJECTOR_H_
#define MEMORY_INJECTOR_H_

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


class InjectorTool
{
public:
	InjectorTool();
	~InjectorTool();

	string faultTablePath;
	vector <memFault> faultTable;

	int initFaultTable( void );
	int startInjection( void );
	int injectFaults( int pid );
	int procMonitor( int pid, int &data );

protected :
    Injector    *m_injector;             // the memory fault injector object
    int         m_argc;                 // the count of the arguement
    char        **m_argv;               // the arguement list
};

#endif	/* MEMORY_INJECTOR_H_ */
