/*******************************************************************************
 *  The BYTE UNIX Benchmarks - Release 1
 *          Module: looper.c   SID: 1.4 5/15/91 19:30:22
 *          
 *******************************************************************************
 * Bug reports, patches, comments, suggestions should be sent to:
 *
 *	Ben Smith or Tom Yager at BYTE Magazine
 *	ben@bytepb.byte.com   tyager@bytepb.byte.com
 *
 *******************************************************************************
 *  Modification Log:
 *  
 *  February 25, 1991 -- created (Ben S.)
 *
 ******************************************************************************/
char SCCSid[] = "@(#) @(#)looper.c:1.4 -- 5/15/91 19:30:22";
/*
 *  Shell Process creation
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "timeit.c"

unsigned long iter;
char *cmd_argv[28];
int  cmd_argc;

report()
{
	fprintf(stderr,"%ld loops\n", iter);
	exit(0);
}

main(argc, argv, env)
int	argc;
char	*argv[];
char    *env[];
{
int	slave, count, count1, duration;
int	status;

if (argc < 2) 
	{
	printf("Usage: %s duration command [args..]\n", argv[0]);
	printf("  duration in seconds\n");
	exit(1);
	}

if((duration = atoi(argv[1])) < 1)
	{
	printf("Usage: %s duration command [arg..]\n", argv[0]);
	printf("  duration in seconds\n");
	exit(1);
	}

/* get command  */
cmd_argc=argc-2;
for( count=2;count < argc; ++count)
	cmd_argv[count-2]=argv[count];
#ifdef DEBUG
printf("<<%s>>",cmd_argv[0]);
for(count=1;count < cmd_argc; ++count)
	printf(" <%s>", cmd_argv[count]);
putchar('\n');
exit(0);
#endif

iter = 0;
wake_me(duration, report);

while (1) 
	{
	if ((slave = fork()) == 0) 
		{ /* execute command */
		execvp(cmd_argv[0],cmd_argv);
		exit(0);
		} 
	else if (slave < 0) 
		{
		/* woops ... */
		printf("Fork failed at iteration %d\n", iter);
		perror("Reason");
		exit(2);
		} 
	else
		/* master */
		wait(&status);
	if (status != 0) 
		{
		printf("Bad wait status: 0x%x\n", status);
		exit(2);
		}
	iter++;
	}
}
