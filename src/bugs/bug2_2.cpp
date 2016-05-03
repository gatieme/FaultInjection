#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <iostream>
#include <exception>
using namespace std;


void startExe(char **exe);

int main(int argc, char * argv[])
{
	int             pid;
    int             ret;

    if(argc <= 1)
	{
		printf("Useage:./bug2 exe [ args ]\n");
        exit(-1);
    }
	pid = fork();
	if(pid < 0)
    {
        perror("fork");
        exit(-1);
	}
	else if(pid == 0)	/// child
	{
        printf("child pid = %d\n", getpid( ));
        char **args = argv + 1;
        for(int i = 0; i < argc; i++)
        {
            cout <<args[i] <<" ";
        }
        cout <<args[0] <<", address = " <<args <<endl;
        cout <<argc <<endl;
        sleep(10);
        startExe(args);
	//	_exit( -1 );
    }
    return 0;
}



void startExe(char **exe)
{
	int fd;
	if((fd = open("/dev/null", O_RDWR)) == -1)
	{
		perror("open");
		_exit(-1);
	}
	dup2(fd, STDOUT_FILENO);
	dup2(fd, STDERR_FILENO);
	close(fd);
	execv(exe[0], exe);
}
