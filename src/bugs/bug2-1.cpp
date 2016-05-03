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

class Args
{
public:
    char **m_exe;

    void startExe( )
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
	    execv(m_exe[0], m_exe);
        printf("command ls is not found, error code: %d(%s)", errno, strerror(errno));

    }
};


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
        sleep(1);
        exit(-1);
	}
	else if(pid == 0)	/// child
	{
        printf("child pid = %d\n", getpid( ));
        char **arg = argv + 1;
        Args args;
        args.m_exe = arg;
		args.startExe( );
		_exit( -1 );
    }

    return 0;
}



