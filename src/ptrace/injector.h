/*
*  Author: HIT CS HDMC team.
*  Last modified by zhaozhilong: 2010-1-14
*/
#ifndef _INJECTOR_H_
#define _INJECTOR_H_

#include "common.h"
#include "ptrace.h"
#include "util.h"


extern INJECTFUNC injectFuncs[];
extern int _nop();
extern int _timeout( int pid );
class Injector
{
public:
	Injector();
	~Injector();

public:
	string faultTablePath;
    vector <FAULT> faultTable;
    FAULT defaultFault;
    FAULT timeoutFault;
	int targetPid;		//target is an existing process.
	char **exeArguments;

public:
    static void report(int signo);
    static void signaltimeout(int sec, void (*func)(int));
	static void cleanup();
    static Injector * initInjector( int argc, char **argv );
	int initFaultTable( void );
	int startInjection( void );
	int injectFaults( int pid, int &status );
	int procMonitor( int pid, int &data );
	static char * nameSignal( int signo );
	void startExe();
	static void usage();
	int runStep(int pid,int steps);
	int runTime(int pid, int msecs);


    static void sigAlrm( int sig );
	static void getTime( int *pSec, int *pUsec );
	static void setTimer( int sec, int usec );
	static void writeResult( int pid, int status, int data );

    static void handleSigchld(int signo);

};

#endif	/* _INJECTOR_H_ */
