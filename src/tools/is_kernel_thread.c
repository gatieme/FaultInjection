/*************************************************************************
    > File Name: is_kernel_thread.c
    > Author: gatieme
    > Created Time: Fri 20 May 2016 03:26:47 PM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



/*
 * get the process ID of the parent of the process which you give
 *
 * ps -p ${pid:-$$} -o ppid=
 *
 * example, ps -p 1 -o ppid=
 * */
int getppid(int pid)
{

}


/*
 * get the command of the process which you give
 *
 * ps -p ${pid:-$$} -o command=
 *
 * example, ps -p 1 -o command=
 * */
int getcommand(int pid, char *result)
{
    char command[81];

    memset(command, 0, 81);
    sprintf(command, "ps -p %d -o command=", pid);

    run_command(command, result);

    printf("result : %s", result);
}



int is_kernel_thread(int pid)
{
    char command[81];
    char result[81];

    memset(command, 0, 81);
    memset(result, 0, 81);

    sprintf(command, "ps -p %d -o command=", pid);
    run_command(command, result);

    printf("result : %s", result);
    if(result[0] == '[')
    {
        printf("process %d is a kernel thread\n", pid);
        return 1;
    }
    else
    {
        printf("process %d is a user process\n", pid);
        return 0;
    }
}

int run_command(char *command, char *result)
{
    FILE *fstream = NULL;
    char buff[81];

    if((fstream = popen(command, "r")) == NULL)
    {
        perror("execute command failed : ");
        return -1;
    }

    memset(buff, 0, sizeof(buff));
    while(fgets(buff, sizeof(buff), fstream) != NULL)
    {
        //printf("%s\n",buff);
        strcat(result, buff);
    }
    pclose(fstream);

    return 0;
}

int main()
{

    is_kernel_thread(2);

}

int test_popen(void)
{
    FILE *fstream = NULL;
    char buff[1024];

    memset(buff, 0, sizeof(buff));

    if((fstream = popen("ls -al","r")) == NULL)
    {
        perror("execute command failed : ");
        return -1;
    }

    while(fgets(buff, sizeof(buff), fstream) != NULL)
    {
        printf("%s\n",buff);
    }

    pclose(fstream);

    return 0;
}
