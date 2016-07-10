/*************************************************************************
    > File Name: rand.c
    > Author: gatieme
    > Created Time: Sun 10 Jul 2016 03:18:44 PM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>


int main(void)
{
    srand(time(NULL));

    for(int i = 0; i < 10; i++)
    {
        printf("%d\n", rand() % 10 + 1);
    }

}
