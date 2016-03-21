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
#include "memoryFault.h"


class Injector
{
public  :

    Injector();
    Injector( int                   &targetPid,          //  故障注入的进程PID
              char                  **&program,          //  故障注入的进程名
              vector<MemoryFault>   &memoryFaultTable);
    Injector( int     &targetPid,
              char    **&program,
              char    *&memoryFaultTablePath);

    ~Injector();


    static Injector * initInjector( int argc, char **argv );
	int initFaultTable( void );
	int startInjection( void );
	int injectFaults( int pid );
	int procMonitor( int pid, int &data );
	char * nameSignal( int signo );
	void startExe();
	void writeResult( int pid, int status, int data );
	static void usage();
	static void cleanup(void);
	static void report(int signo);
	/// static void restore(int signo);
	void timeout(int sec,void(*func)(int));

protected :

	string                 m_memoryFaultTablePath;
	vector <MemoryFault>   m_memoryFaultTable;

    int                    m_targetPid;		//  target is an existing process.
	char                   **m_exeArguments;

};

#endif	/* MEMORY_INJECTOR_H_ */
