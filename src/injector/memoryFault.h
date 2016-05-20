#ifndef __MEMORY_FAULT_H__
#define __MEMORY_FAULT_H__

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
typedef enum locations
{
	text_area,
	data_area,
	stack_area, //add by fenggang, 2011-5-10
} pos;

//fault type
typedef enum types
{
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
class MemoryFault
{
public :
    friend ostream& operator<<(ostream &out, const MemoryFault &memoryFault);

public :
    MemoryFault(pos location, long addr, type faulttype, int time, int timeout)
    :m_location(location),
     m_addr(addr),
     m_faultType(faulttype),
     m_time(time),
     m_timeout(timeout)
    {
        this->m_location = data_area;
    }

    MemoryFault()
    :m_location(data_area),
     m_addr(-1),
     m_faultType(word_0),
     m_time(-1),
     m_timeout(-1)
    {
    }

    bool SetLocation(std::string location);

    bool SetMode(std::string mode);

    bool SetFaultType(std::string faulttype);

    bool SetTime(std::string faulttype);

    bool SetTimeout(std::string faulttype);

//protected:

    pos     m_location;

    long    m_addr;     //  addr = -1, 时候表示选取随机进行故障注入

    type    m_faultType;

    int     m_time;		//瞬时故障0，永久故障-1，正整数表示故障持续时钟周期

    int     m_timeout;


};


#endif  //#define MEMORY_FAULT_H_
