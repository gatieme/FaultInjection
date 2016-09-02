#include <stdio.h>
#include <string.h>

#define argnum 5
#define MAX_LINE 256

int main(int argc, char **argv)
{
	int i,aim,input,ret;
	char buff[MAX_LINE];
	//printf("%d\n",argc);
	//for(i=0;i<argc;++i)
	//	printf("%d\t%s\n",i,argv[i]);
	if(argc<argnum)
	{
		printf("Not enough args, turn to guide mode.\n");
		printf("Select aim:\n0 ioctl_privcmd_hypercall\n1 xen_pgd_pin\n2 xen_l2_entry_update\n3 remap_pfn_range\n4 xen_l3_entry_update\n5 rw_block_io\ninput:");
		scanf("%d",&input);
		memset(buff,'\0',sizeof(buff));
		sprintf(buff,"echo %d > /proc/jphyper/aim",input);
		ret=system(buff);
		if(ret)
		{
			printf("Can't write the arg to proc, make sure the module's .ko was insmod.\n");
			return 0;
		}
		printf("Select fault:\n");
		aim=input;
		if(aim==0)	
			printf("0 cmd\n1 mfn\n2 linear_addr\ninput:");
		if(aim==1)
			printf("0 cmd\n1 mfn\ninput:");
		if(aim==2)
			printf("0 ptr_address\n1 ptr\n2 val\ninput:");
		if(aim==3)
			printf("0 pgd\n1 vm_flags\ninput:");
		if(aim==4)
			printf("0 ptr_address\n1 ptr\n2 val\ninput:");
		if(aim==5)
			printf("0 id\n1 nr_pages\n2 nr_segments\ninput:");
		scanf("%d",&input);
		memset(buff,'\0',sizeof(buff));
		sprintf(buff,"echo %d > /proc/jphyper/fault",input);
		ret=system(buff);
		if(ret)
		{
			printf("Can't write the arg to proc, make sure the module's .ko was insmod.\n");
			return 0;
		}
		printf("Select time:\n(an integer large than 0)\ninput:");
		scanf("%d",&input);
		memset(buff,'\0',sizeof(buff));
		sprintf(buff,"echo %d > /proc/jphyper/time",input);
		ret=system(buff);
		if(ret)
		{
			printf("Can't write the arg to proc, make sure the module's .ko was insmod.\n");
			return 0;
		}
		printf("Select id:\n(an integer)\ninput:");
		scanf("%d",&input);
		memset(buff,'\0',sizeof(buff));
		sprintf(buff,"echo %d > /proc/jphyper/id",input);
		ret=system(buff);
		if(ret)
		{
			printf("Can't write the arg to proc, make sure the module's .ko was insmod.\n");
			return 0;
		}
		ret=system("echo 1 > /proc/jphyper/signal");
		if(ret)
		{
			printf("Can't write the arg to proc, make sure the module's .ko was insmod.\n");
			return 0;
		}
		printf("FI Arg pass successfully.\n");
		return 0;
	}
	memset(buff,'\0',sizeof(buff));
	sprintf(buff,"echo %s > /proc/jphyper/aim",argv[1]);
	ret=system(buff);
	if(ret)
	{
		printf("Can't write the arg to proc, make sure the module's .ko was insmod.\n");
		return 0;
	}
	memset(buff,'\0',sizeof(buff));
	sprintf(buff,"echo %s > /proc/jphyper/fault",argv[2]);
	ret=system(buff);
	if(ret)
	{
		printf("Can't write the arg to proc, make sure the module's .ko was insmod.\n");
		return 0;
	}
	memset(buff,'\0',sizeof(buff));
	sprintf(buff,"echo %s > /proc/jphyper/time",argv[3]);
	ret=system(buff);
	if(ret)
	{
		printf("Can't write the arg to proc, make sure the module's .ko was insmod.\n");
		return 0;
	}
	memset(buff,'\0',sizeof(buff));
	sprintf(buff,"echo %s > /proc/jphyper/id",argv[4]);
	ret=system(buff);
	if(ret)
	{
		printf("Can't write the arg to proc, make sure the module's .ko was insmod.\n");
		return 0;
	}
	ret=system("echo 1 > /proc/jphyper/signal");
	if(ret)
	{
		printf("Can't write the arg to proc, make sure the module's .ko was insmod.\n");
		return 0;
	}
	printf("FI Arg pass successfully.\n");
	return 0;
}

