/*
*  Author: HIT CS HDMC team.
*  Last modified by zhaozhilong: 2010-1-14
*/
#ifndef _COMMON_H_
#define _COMMON_H_

//C header files
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdarg.h>
#include <time.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <signal.h>


//c++ header files
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
using namespace std;

#define MAX_FAULT 256
#define MAX_LENGTH_ARGUMENT 128
#define MAX_NUM_ARGUMENT 64

//return status
#define RT_OK		0
#define RT_FAIL		-1
#define EXIT_ERR		127
#define EXIT_OK		0

//process status
#define RUN		0
#define STOP	1
#define EXIT	2
#define TERM	3


//fault injection function pointers
//typedef int (*INJECTFUNC)( int, long );

#endif	/* _COMMON_H_ */
