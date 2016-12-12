/*
*  Author: HIT CS HDMC team.
*  Last modified by zhaozhilong: 2010-1-14
*/
#ifndef _COMMON_H_
#define _COMMON_H_


#define BUILD_FOR_ARM

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

//fault type
typedef enum type{
	err_ftype,	//identify incorrect fault type
	nop,
	timeout,
	_1_bit_flip,
	_2_bit_flip,
	_1_bit_0,
	_2_bit_0,
	_1_bit_1,
	_2_bit_1,
	_8_low_0,
	_8_low_1,
	_8_low_error,
	_plus_1,
	_plus_2,
	_plus_3,
	_plus_4,
	_plus_5,
} TYPE;



//fault injection function pointers
typedef int (*INJECTFUNC)( int, long );

#endif	/* _COMMON_H_ */
