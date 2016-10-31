/*
*  Author: HIT CS HDMC team.
*  Last modified by zhaozhilong: 2010-1-14
*/
#ifndef _FAULT_H_
#define _FAULT_H_

#include "common.h"


static int myRand();
int randBit();

int _nop();
int _timeout(int pid);
int inject_1_bit_flip( int pid, long pos );
int inject_2_bit_flip( int pid, long pos );
int inject_1_bit_0( int pid, long pos );
int inject_2_bit_0( int pid, long pos );
int inject_1_bit_1( int pid, long pos );
int inject_2_bit_1( int pid, long pos );
int inject_8_low_0( int pid, long pos );
int inject_8_low_1( int pid, long pos );
int inject_8_low_error( int pid, long pos );
int inject_plus_1( int pid, long pos );
int inject_plus_2( int pid, long pos );
int inject_plus_3( int pid, long pos );
int inject_plus_4( int pid, long pos );
int inject_plus_5( int pid, long pos );

INJECTFUNC injectFuncs[] = {
	0,
	0,
	0,
	inject_1_bit_flip,
	inject_2_bit_flip,
	inject_1_bit_0,
	inject_2_bit_0,
	inject_1_bit_1,
	inject_2_bit_1,
	inject_8_low_0,
	inject_8_low_1,
	inject_8_low_error,
	inject_plus_1,
	inject_plus_2,
	inject_plus_3,
	inject_plus_4,
	inject_plus_5,
};

#endif
