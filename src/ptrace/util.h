/*
*  Author: HIT CS HDMC team.
*  Last modified by zhaozhilong: 2010-1-14
*/
#ifndef _UTIL_H_
#define _UTIL_H_

#include "common.h"

#define NULL_POS		-1
#define ERR_POS		-2


long name2offset(const char* name);
char * offset2name(const long offset);
TYPE fault2type(string strTmp);
string type2fault(TYPE faultType);



//fault struct
typedef class FAULT
{
public :
	union{
		int microsec;
		int step;
	};
	char unit;
	long faultPos;
	TYPE faultType;

    FAULT(int step = 3000000, char unit = 'm', long faultPos = NULL_POS, TYPE faultType = timeout)
    {
        this->step = step;
        this->unit = unit;
        this->faultPos = faultPos;
        this->faultType = faultType;
    }

    FAULT(int step, char unit, char *faultPos, char *faultType)
    {
        this->step = step;
        this->unit = unit;
        this->faultPos = name2offset(faultPos);
        this->faultType = fault2type(faultType);
    }
    FAULT(const FAULT &fault)
    {
        this->step = fault.step;
        this->unit = fault.unit;
        this->faultPos = fault.faultPos;
        this->faultType = fault.faultType;
    }

    void Show()
    {
        cout <<"step|misrosec = " <<step <<", ";
        cout <<"unit = " <<unit <<", ";
        cout <<"position = " <<offset2name(faultPos) <<"(" <<faultPos <<"), ";
        cout <<"type = " <<type2fault(faultType) <<"(" <<faultType <<")" <<endl;
    }
};


#endif /* _UTIL_H_ */
