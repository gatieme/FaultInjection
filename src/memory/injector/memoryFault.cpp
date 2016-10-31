#include "memoryFault.h"

ostream& operator<<(ostream &out, const MemoryFault &memoryFault)
{
    out <<"location : ";
    switch(memoryFault.m_location)
    {
        case text_area      :
        {
            out <<"text, ";
            break;

        }
        case data_area      :
        {
            out <<"data, ";
            break;
        }
        case stack_area     :
        {
            out <<"stack, ";
            break;
        }
        case kstack_area    :
        {
            out <<"kernel stack, ";
            break;
        }
        default             :
        {
            out <<"unknown location, ";
            break;
        }
    }

    out <<"mode : ";
    switch(memoryFault.m_addr)
    {
        case -1:
        {
            out <<"random, ";
            break;
        }
        default :
        {
            out <<"physical : " <<memoryFault.m_addr;
        }
    }

    out <<"type : ";
    switch(memoryFault.m_faultType)
    {
        case one_bit_0:
        {
            out <<"one_bit_0, ";
            break;
        }
        case one_bit_1:
        {
            out <<"one_bit_1, ";
            break;
        }
        case one_bit_flip:
        {
            out <<"one_bit_flip, ";
            break;
        }
        case word_0:
        {
            out <<"word_0, ";
            break;
        }
        case word_flip:
        {
            out <<"word_flip, ";
            break;

        }
        case page_0:
        {
            out <<"page_0, ";
            break;
        }
    }
    out <<"time : " <<memoryFault.m_time;
    out <<", timeout : " <<memoryFault.m_timeout;
    return out;
}

bool MemoryFault::SetLocation(std::string location)
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
    else if( location == "kstack" || location == "KSTACK" )
    {
        this->m_location = kstack_area;
    }
    else
    {
        std::cerr << "Error:undefined fault location" <<std::endl;

        return false;
    }
    return true;
}


bool MemoryFault::SetMode(std::string mode)
{
    if( mode == "random" || mode == "RANDOM" )
    {
        this->m_addr = -1;
        return true;
    }
    else
    {
        int iRet = sscanf(mode.c_str(), "%lx", &this->m_addr);

        if( iRet != 1 )
        {
            return false;
        }
        return true;
    }


}

bool MemoryFault::SetFaultType(std::string faulttype)
{
    if( faulttype == "one_bit_0" )
    {
        this->m_faultType = one_bit_0;
    }
    else if( faulttype == "one_bit_1" )
    {
        this->m_faultType = one_bit_1;
    }
    else if( faulttype == "one_bit_flip" )
    {
        this->m_faultType = one_bit_flip;
    }
    else if( faulttype == "word_0" )
    {
        this->m_faultType = word_0;
    }
    else if( faulttype == "word_flip" )
    {
        this->m_faultType = word_flip;
    }
    else if( faulttype == "page_0" )
    {
        this->m_faultType = page_0;
    }
    else
    {
        return false;
    }
    return true;
}
bool MemoryFault::SetTime(std::string timeStr)
{
    int time = atoi(timeStr.c_str( ));
    this->m_time = time;
    return true;
}

bool MemoryFault::SetTimeout(std::string timeoutStr)
{
    int timeout = atoi(timeoutStr.c_str( ));
    this->m_timeout = timeout;
    return true;
}
