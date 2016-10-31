/*
*  Author: HIT CS HDMC team.
*  Last modified by zhaozhilong: 2010-1-14
*/
#include "fault.h"
#include "ptrace.h"
#include "util.h"

int _nop()
{
	cout << "nop" << endl;
	return RT_OK;
}

int _timeout( int pid )
{
	errno = 0;
	ptrace( PTRACE_CONT, pid, 1, SIGKILL );
	if( errno != 0 )
	{
		perror("ptrace");
		return RT_FAIL;
	}
	cout << "timeout" << endl;
	return RT_OK;
}

//return a random 32 bits int
int myRand()
{
	static int init = 0;
	register int x0, x1, x2, x3;
	if ( !init )
	{
		(void) srand( (int)time((time_t *) NULL) );
		init = 1;
	}
	x0 = (rand() & 0x0000ff00) >> 8;
	x1 = (rand() & 0x0000ff00) >> 8;
	x2 = (rand() & 0x0000ff00) >> 8;
	x3 = (rand() & 0x0000ff00) >> 8;
	return ((x0 << 24) | (x1 << 16) | (x2 << 8) | x3);
}

//return (0,63)
int randBit()
{
	return (myRand() & 0x3f);
}

//
int inject_1_bit_flip( int pid, long pos )
{
	int iRet;
	long origData;
	long newData;

	////read
	iRet = ptraceGetReg( pid, pos, origData );
	if( iRet == RT_FAIL ) { return RT_FAIL; }

	//random 1 bit flip
	newData = origData ^ ( 0x0000000000000001L << randBit() );

	////write
	iRet = ptraceSetReg( pid, pos, newData );
	if( iRet == RT_FAIL ) { return RT_FAIL; }

	cout << "Fault on " << offset2name( pos ) << "(0X" << hex << origData << " -> 0X" << newData << ")" << dec << endl;

	return RT_OK;
}

//
int inject_2_bit_flip( int pid, long pos )
{
	int iRet;
	long origData;
	long newData;

	////read
	iRet = ptraceGetReg( pid, pos, origData );
	if( iRet == RT_FAIL ) { return RT_FAIL; }

	//random 2 bit flip
	newData = origData ^ ( 0x0000000000000001L << randBit() );
	newData = newData ^ ( 0x0000000000000001L << randBit() );

	////write
	iRet = ptraceSetReg( pid, pos, newData );
	if( iRet == RT_FAIL ) { return RT_FAIL; }

	cout << "Fault on " << offset2name( pos ) << "(0X" << hex << origData << " -> 0X" << newData << ")" << dec << endl;

	return RT_OK;
}

int inject_1_bit_0( int pid, long pos )
{
	int iRet;
	long origData;
	long newData;

	////read
	iRet = ptraceGetReg( pid, pos, origData );
	if( iRet == RT_FAIL ) { return RT_FAIL; }

	//random 1 bit set 0
	newData = origData & ( ~( 0x0000000000000001L << randBit() ) );

	////write
	iRet = ptraceSetReg( pid, pos, newData );
	if( iRet == RT_FAIL ) { return RT_FAIL; }

	cout << "Fault on " << offset2name( pos ) << "(0X" << hex << origData << " -> 0X" << newData << ")" << dec << endl;

	return RT_OK;
}

int inject_2_bit_0( int pid, long pos )
{
	int iRet;
	long origData;
	long newData;

	////read
	iRet = ptraceGetReg( pid, pos, origData );
	if( iRet == RT_FAIL ) { return RT_FAIL; }

	//random 2 bit set 0
	newData = origData & ( ~( 0x0000000000000001L << randBit() ) );
	newData = newData & ( ~( 0x0000000000000001L << randBit() ) );

	////write
	iRet = ptraceSetReg( pid, pos, newData );
	if( iRet == RT_FAIL ) { return RT_FAIL; }

	cout << "Fault on " << offset2name( pos ) << "(0X" << hex << origData << " -> 0X" << newData << ")" << dec << endl;

	return RT_OK;
}

int inject_1_bit_1( int pid, long pos )
{
	int iRet;
	long origData;
	long newData;

	////read
	iRet = ptraceGetReg( pid, pos, origData );
	if( iRet == RT_FAIL ) { return RT_FAIL; }

	//random 1 bit set 1
	newData = origData | ( 0x0000000000000001L << randBit() );

	////write
	iRet = ptraceSetReg( pid, pos, newData );
	if( iRet == RT_FAIL ) { return RT_FAIL; }

	cout << "Fault on " << offset2name( pos ) << "(0X" << hex << origData << " -> 0X" << newData << ")" << dec << endl;

	return RT_OK;
}

int inject_2_bit_1( int pid, long pos )
{
	int iRet;
	long origData;
	long newData;

	////read
	iRet = ptraceGetReg( pid, pos, origData );
	if( iRet == RT_FAIL ) { return RT_FAIL; }

	//random 2 bit set 1
	newData = origData | ( 0x0000000000000001L << randBit() );
	newData = newData | ( 0x0000000000000001L << randBit() );

	////write
	iRet = ptraceSetReg( pid, pos, newData );
	if( iRet == RT_FAIL ) { return RT_FAIL; }

	cout << "Fault on " << offset2name( pos ) << "(0X" << hex << origData << " -> 0X" << newData << ")" << dec << endl;

	return RT_OK;
}

int inject_8_low_0( int pid, long pos )
{
	int iRet;
	long origData;
	long newData;

	////read
	iRet = ptraceGetReg( pid, pos, origData );
	if( iRet == RT_FAIL ) { return RT_FAIL; }

	//low 8 bits set 0
	newData = origData & 0xffffffffffffff00L;

	////write
	iRet = ptraceSetReg( pid, pos, newData );
	if( iRet == RT_FAIL ) { return RT_FAIL; }

	cout << "Fault on " << offset2name( pos ) << "(0X" << hex << origData << " -> 0X" << newData << ")" << dec << endl;

	return RT_OK;
}

int inject_8_low_1( int pid, long pos )
{
	int iRet;
	long origData;
	long newData;

	////read
	iRet = ptraceGetReg( pid, pos, origData );
	if( iRet == RT_FAIL ) { return RT_FAIL; }

	//low 8 bits set 1
	newData = origData | 0x00000000000000ffL;

	////write
	iRet = ptraceSetReg( pid, pos, newData );
	if( iRet == RT_FAIL ) { return RT_FAIL; }

	cout << "Fault on " << offset2name( pos ) << "(0X" << hex << origData << " -> 0X" << newData << ")" << dec << endl;

	return RT_OK;
}

int inject_8_low_error( int pid, long pos )
{
	int iRet;
	long origData;
	long newData;

	////read
	iRet = ptraceGetReg( pid, pos, origData );
	if( iRet == RT_FAIL ) { return RT_FAIL; }

	//low 8 bits random error
	newData = origData ^ ( myRand() & 0x00000000000000ffL );

	////write
	iRet = ptraceSetReg( pid, pos, newData );
	if( iRet == RT_FAIL ) { return RT_FAIL; }

	cout << "Fault on " << offset2name( pos ) << "(0X" << hex << origData << " -> 0X" << newData << ")" << dec << endl;

	return RT_OK;
}

int inject_plus_1( int pid, long pos )
{
	int iRet;
	long origData;
	long newData;

	////read
	iRet = ptraceGetReg( pid, pos, origData );
	if( iRet == RT_FAIL ) { return RT_FAIL; }

	//plus 1
	newData = origData + 1;

	////write
	iRet = ptraceSetReg( pid, pos, newData );
	if( iRet == RT_FAIL ) { return RT_FAIL; }

	cout << "Fault on " << offset2name( pos ) << "(0X" << hex << origData << " -> 0X" << newData << ")" << dec << endl;

	return RT_OK;
}

int inject_plus_2( int pid, long pos )
{
	int iRet;
	long origData;
	long newData;

	////read
	iRet = ptraceGetReg( pid, pos, origData );
	if( iRet == RT_FAIL ) { return RT_FAIL; }

	//plus 2
	newData = origData + 2;

	////write
	iRet = ptraceSetReg( pid, pos, newData );
	if( iRet == RT_FAIL ) { return RT_FAIL; }

	cout << "Fault on " << offset2name( pos ) << "(0X" << hex << origData << " -> 0X" << newData << ")" << dec << endl;

	return RT_OK;
}

int inject_plus_3( int pid, long pos )
{
	int iRet;
	long origData;
	long newData;

	////read
	iRet = ptraceGetReg( pid, pos, origData );
	if( iRet == RT_FAIL ) { return RT_FAIL; }

	//plus 3
	newData = origData + 3;

	////write
	iRet = ptraceSetReg( pid, pos, newData );
	if( iRet == RT_FAIL ) { return RT_FAIL; }

	cout << "Fault on " << offset2name( pos ) << "(0X" << hex << origData << " -> 0X" << newData << ")" << dec << endl;

	return RT_OK;
}

int inject_plus_4( int pid, long pos )
{
	int iRet;
	long origData;
	long newData;

	////read
	iRet = ptraceGetReg( pid, pos, origData );
	if( iRet == RT_FAIL ) { return RT_FAIL; }

	//plus 4
	newData = origData + 4;

	////write
	iRet = ptraceSetReg( pid, pos, newData );
	if( iRet == RT_FAIL ) { return RT_FAIL; }

	cout << "Fault on " << offset2name( pos ) << "(0X" << hex << origData << " -> 0X" << newData << ")" << dec << endl;

	return RT_OK;
}

int inject_plus_5( int pid, long pos )
{
	int iRet;
	long origData;
	long newData;

	////read
	iRet = ptraceGetReg( pid, pos, origData );
	if( iRet == RT_FAIL ) { return RT_FAIL; }

	//plus 5
	newData = origData + 5;

	////write
	iRet = ptraceSetReg( pid, pos, newData );
	if( iRet == RT_FAIL ) { return RT_FAIL; }

	cout << "Fault on " << offset2name( pos ) << "(0X" << hex << origData << " -> 0X" << newData << ")" << dec << endl;

	return RT_OK;
}
