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
	*p = 1;
    *q = 2;
    int FD;
	FILE* fp = fopen("hello.txt", "a+");
	int i = 0;

    while(1)
	{
		printf("pid = %d, %d, "
               ", hello a's addr = 0x%lx [%d]"
               ", p point addr = 0x%lx q addr = 0x%lx [%d]"
               ", q point addr = 0x%lx q addr = 0x%lx [%d]\n",
                getpid( ), i, &a, a, &p, p, *p, &q, q, *q);
		fprintf(fp, "pid = %d, %d, "
               ", hello a's addr = 0x%lx [%d]"
               ", p point addr = 0x%lx q addr = 0x%lx [%d]"
               ", q point addr = 0x%lx q addr = 0x%lx [%d]\n",
                getpid( ), i, &a, a, &p, p, *p, &q, q, *q);
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
