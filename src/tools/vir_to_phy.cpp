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

typedef struct procMMInfo
{
	unsigned long total;	//进程地址空间大小
	unsigned long locked;	//被锁住而无法换出的页面数
	unsigned long shared;	//共享内存映射
	unsigned long exec;		//可执行内存映射
	unsigned long stack;	//用户堆栈
	unsigned long reserve;//保留区

	unsigned long def_flags;//
	unsigned long nr_ptes;	//

	unsigned long start_code;	//代码段开始地址
	unsigned long end_code;		//代码段结束地址
	unsigned long start_data;	//数据段开始地址
	unsigned long end_data;		//数据段结束地址
	unsigned long start_brk;	//堆的起始地址
	unsigned long brk;				//堆的当前最后地址
	unsigned long start_stack;//用户堆栈的起始地址
	unsigned long arg_start;	//命令行参数
	unsigned long arg_end;
	unsigned long env_start;	//环境变量
	unsigned long env_end;
} taskMMInfo, *pTaskMMInfo;

#define OK		0
#define FAIL	1

#define PAGE_SIZE 65536
#define MAX_LINE	PAGE_SIZE
#define varCount	19

/*
*	request command
*/
#define REQUEST_TASK_INFO		1		/// get a task's memory map information
#define REQUEST_V2P					2		/// convert a process's linear address to physical address
#define REQUEST_KV2P				3		/// convert kernel virtual address to physical address
#define REQUEST_KFUNC_VA		4		/// get kernel function's addr(kernel virtual address)
#define REQUEST_READ_KFUNC	5		/// 请求读取内核函数起始地址内容
#define REQUEST_WRITE_KFUNC	6		/// 请求改写内核函数起始地址内容
/
/#define REQUEST_WRITE				10 	/// 请求改写指定物理地址内容，改为用户态实现此功能
///#define REQUEST_MEM					11	/// 请求获取全部物理内存信息
///#define REQUEST_ADDR_STOP		12	///

/*
*	ack signals
*/
#define ACK_TASK_INFO			REQUEST_TASK_INFO
#define ACK_V2P						REQUEST_V2P
#define ACK_KV2P					REQUEST_KV2P
#define ACK_KFUNC_VA			REQUEST_KFUNC_VA
#define ACK_READ_KFUNC		REQUEST_READ_KFUNC
#define ACK_WRITE_KFUNC		REQUEST_WRITE_KFUNC
///#define REQUEST_WRITE			REQUEST_WRITE
///#define ACK_MEM						REQUEST_MEM
///#define ACK_ADDR_STOP			REQUEST_ADDR_STOP

/*
*	utility functions
*/
int main(int argc, char *argv[])
{
	int pid;
	unsigned long va;
	int ret;
	int procFile;
	unsigned long physicalAddr = 0;
	char buff[MAX_LINE];

    if(argc != 3)   //  需要3个参数
	{
		printf("Useage:./vir_to_phy pid vir_addr\n");
		return 0;

        //  argv[0] -=>  程序自身名称
        //  argv[1] -=>  待处理进程的pid
        //  argv[2] -=>  待转换的进程内地址
    }
	pid = atoi(argv[1]);            //  待处理进程的pid
	sscanf(argv[2], "%lx", &va);    //  待处理的内存地址
	if(pid <= 0)
	{
		return FAIL;
	}

	// send control word
	bzero(buff, sizeof(buff));
	sprintf(buff, "echo %d > /proc/memoryEngine/pid", pid);
	system(buff);

    bzero(buff, sizeof(buff));
	sprintf(buff, "echo %lx > /proc/memoryEngine/virtualAddr", va);
	system(buff);

    bzero(buff, sizeof(buff));
	sprintf(buff, "echo %d > /proc/memoryEngine/ctl", REQUEST_V2P);
	system(buff);

	//wait for ack signal
	procFile = open("/proc/memoryEngine/signal", O_RDONLY);
	if(procFile == -1)
	{
		perror("Failed to open /proc/memoryEngine/signal");
		return FAIL;
	}

	do
    {
		bzero(buff, sizeof(buff));
		ret = read(procFile, buff, MAX_LINE);
		if(ret == -1)
		{
			perror("Failed to read /proc/memoryEngine/signal");
			return FAIL;
		}
	} while(atoi(buff) != ACK_V2P);
	close(procFile);

	//read physicalAddr
	procFile = open("/proc/memoryEngine/physicalAddr",O_RDONLY);
	if(procFile == -1)
	{
		perror("Failed to open /proc/memoryEngine/physicalAddr");
		return FAIL;
	}
	bzero(buff, sizeof(buff));
	ret = read(procFile, buff, MAX_LINE);
	if(ret == -1)
	{
		perror("Failed to read /proc/memoryEngine/physicalAddr");
		return FAIL;
	}
	close(procFile);

	if(sscanf(buff,"%lx",&physicalAddr) != 1) { return FAIL; }
	printf("%lx\n", physicalAddr);
	return 0;
}
