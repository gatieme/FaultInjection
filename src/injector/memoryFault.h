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
class MemoryFault
{
public :
    explicit MemoryFault()
    :m_location(data_area),
     m_addr(-1),
     m_faultType(word_0),
     m_time(-1),
     m_timeout(-1)
    {
        this->m_location = data_area
    }

    inline bool SetLocation(std::string location)
    {
        if( location == "text" || location == "TEXT" )
        {
            this->m_location = text_area;
        }
        else if( location == "data" || location == "DATA" )
        {
            this->m_location = data_area;
        }
        else if( location == "stack" || location == "STACK" )
        {
            this->m_location = stack_area;
        }
        else
        {
            std::cerr << "Error:undefined fault location" <<std::endl;

            return false;
        }
        return true;
    }

    inline bool SetAddr(std::string mode)
    {
        if( type == "random" || type == "RANDOM" )
        {
            this->.m_addr = -1;
        }
        else
        {
            int iRet = sscanf(this->m_location.c_str(), "%lx", &this->m_addr);

            if( iRet != 1 )
            {
                return false;
            }
        }
        return true;

    }

    inline bool SetFaultType(std::string faulttype)
    {
        if( strTmp == "one_bit_0" )
        {
            this->m_faultType = one_bit_0;
        }
        else if( strTmp == "one_bit_1" )
        {
            this->m_faultType = one_bit_1;
        }
        else if( strTmp == "one_bit_flip" )
        {
            this->m_faultType = one_bit_flip;
        }
        else if( strTmp == "word_0" )
        {
            this->m_faultType = word_0;
        }
        else if( strTmp == "page_0" )
        {
            this->m_faultType = page_0;
        }
        else
        {
            return false;
        }
        return true;
    }
protected:

    pos     m_location;

    long    m_addr;     //  addr = -1, 时候表示选取随机进行故障注入

    type    m_faultType;

    int     m_time;		//瞬时故障0，永久故障-1，正整数表示故障持续时钟周期

    int     m_timeout;


}

