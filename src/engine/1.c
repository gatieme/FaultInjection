/*************************************************************************
    > File Name: 1.c
    > Author: gatieme
    > Created Time: 2016年05月25日 星期三 16时08分44秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#define NAME 13131
#define NAME_TO_STR(name)  (#name)

int main()
{
    printf(NAME_TO_STR(NAME));
}
