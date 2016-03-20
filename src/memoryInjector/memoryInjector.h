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

#define RT_OK		0
#define RT_FAIL -1
#define RT_EXIT 127

//status of a process
#define RUN		0
#define STOP	1
#define EXIT	2
#define TERM	3

#define PAGESIZE ( getpagesize() )

#define STACK_SIZE 0x1000 //define by fenggang

//fault location
typedef enum locations { 
	text_area,
	data_area,
	stack_area, //add by fenggang, 2011-5-10
} pos;

//fault type
typedef enum types {
	one_bit_0,
	one_bit_1,
	one_bit_flip,
	
	two_bit_0,
	two_bit_1,
	two_bit_flip,
	
	low_8_0,
	low_8_1,
	low_8_error,
	
	word_0,
	word_1,
	word_error,
	
	page_0,
	page_1,
	page_error
	
} type;


/// fault
typedef struct memoryFault
{
	pos location;
	long addr;
	type faultType;
	int time;		//瞬时故障0，永久故障-1，正整数表示故障持续时钟周期
	int timeout;
} memFault, *pMemFault;

class Injector
{
public:
	Injector();
	~Injector();
	
public:
	string faultTablePath;
	vector <memFault> faultTable;
	int targetPid;		//target is an existing process.
	char **exeArguments;
public:
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
};

#endif	/* MEMORY_INJECTOR_H_ */
