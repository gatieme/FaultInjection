/*
*  Author: HIT CS HDMC team.
*  Last modified by zhaozhilong: 2010-1-14
*/
#ifndef _PTRACE_H_
#define _PTRACE_H_

#include <sys/ptrace.h>
#include <asm/ptrace_offsets.h>	//define PT_F32,etc
#include "common.h"

//control
extern int ptraceTraceme();
extern int ptraceAttach( int pid );
extern int ptraceCont( int pid );
extern int ptraceCont( int pid, int sig );
extern int ptraceStep( int pid, int sig );


//access registers
extern int ptraceGetReg( int pid, long offset, long &data );
extern int ptraceSetReg( int pid, long offset, long data );


#endif /* _PTRACE_H_ */
