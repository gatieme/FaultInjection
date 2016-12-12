/*
*  Author: HIT CS HDMC team.
*  Last modified by zhaozhilong: 2010-1-14
*/
#include "ptrace.h"
int ptraceTraceme()
{
	errno = 0;
	ptrace(PTRACE_TRACEME,0,NULL,0);
	if( errno != 0 )
	{
		perror( "ptraceTraceme" );
		return RT_FAIL;
	}
	return RT_OK;
}

int ptraceAttach(int pid)
{
	errno = 0;
	ptrace(PTRACE_ATTACH,pid,NULL,0);
	if( errno != 0 )
	{
		perror( "ptraceAttach" );
		return RT_FAIL;
	}
	return RT_OK;
}

int ptraceCont(int pid)
{
	errno = 0;
	ptrace(PTRACE_CONT,pid,0,0);
	if( errno != 0 )
	{
		perror( "ptraceCont" );
		return RT_FAIL;
	}
	return RT_OK;
}

int ptraceCont(int pid,int sig)
{
	errno = 0;

	ptrace(PTRACE_CONT,pid,1,sig);
	if( errno != 0 )
	{
		perror( "ptraceCont" );
		return RT_FAIL;
	}
	return RT_OK;
}

int ptraceStep(int pid,int sig)
{
	errno = 0;
	ptrace(PTRACE_SINGLESTEP,pid,1,sig);
	if( errno != 0 )
	{
		perror( "ptraceStep" );
		return RT_FAIL;
	}
	return RT_OK;
}

int ptraceGetReg( int pid, long offset, long &data )
{
	if( offset < PT_F32 || offset > PT_PMD || (offset & 0x0007) != 0 )
	{
		cerr << "Error:incorrect register offset" << endl;
		return RT_FAIL;
	}
	//ptrace( PTRACE_POKEUSER, pid, offset, 20);
	data = ptrace( PTRACE_PEEKUSER, pid, offset, 0 );
	if( data == -1 )
	{
		perror("ptrace");
		return RT_FAIL;
	}
	return RT_OK;
}

int ptraceSetReg( int pid, long offset, long data )
{
	int iRet = 0;
	if( offset < PT_F32 || offset > PT_PMD || (offset & 0x0007) != 0 )
	{
		cerr << "Error:incorrect register offset" << endl;
		return RT_FAIL;
	}
	iRet = ptrace( PTRACE_POKEUSER, pid, offset, data );
	if( iRet == -1 )
	{
		perror("ptrace");
		return RT_FAIL;
	}
	return RT_OK;
}

