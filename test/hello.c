#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int test()
{
	int a = 10;
	int *p = NULL;
	int *q = NULL;
	p = (int *)malloc(sizeof (int));
	q = (int *)malloc(sizeof (int));
	int FD;
	FILE* fp = fopen("hello.txt", "a+");
	int i = 0;

    while(1)
	{
		printf("pid = %d, %d\t hello a's addr = %lx\tp addr = %lx\tp point addr = %lx\tq addr = %lx\tq point addr = %lx\n",getpid(), i, &a, &p, p, &q, q);
		fprintf(fp, "pid = %d, %d\t hello a's addr = %lx\tp addr = %lx\tp point addr = %lx\tq addr = %lx\tq point addr = %lx\n",getpid(), i, &a, &p, p, &q, q);
		fflush(fp);
		sleep(1);
		i++;
	}
	fclose(fp);
	return 0;
}


int main()
{
	test();
	return 0;
}
