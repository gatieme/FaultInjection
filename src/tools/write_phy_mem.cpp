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
///#define REQUEST_WRITE				10 	/// 请求改写指定物理地址内容，改为用户态实现此功能
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
int main(int argc, char * argv[])
{
	unsigned long pa;
	unsigned long data;
	int len;
	int memfd;
	int pageSize;
	int shift;
	int size;
	int do_mlock;
	void volatile *mapStart;
	void volatile *mapAddr;
	unsigned long pa_base;
	unsigned long pa_offset;
	if(argc != 3)
	{
		printf("Usage:./write_phy_mem addr data\n");
		return 0;
	}
	sscanf(argv[1], "%lx", &pa);
	len = sizeof(long);
	sscanf(argv[2], "%lx", &data);
	memfd = open("/dev/mem", O_RDWR | O_SYNC);
	if(memfd == -1)
	{
		perror("Failed to open /dev/mem");
		return FAIL;
	}

	shift = 0;
	pageSize = PAGE_SIZE;
	while(pageSize > 0)
	{
		pageSize = pageSize >> 1;
		shift ++;
	}
	shift --;
	pa_base = (pa >> shift) << shift;
	pa_offset = pa - pa_base;

	mapStart = (void volatile *)mmap(0, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_LOCKED, memfd, pa_base);
	if(mapStart == MAP_FAILED)
	{
		perror("Failed to mmap /dev/mem");
		close(memfd);
		return FAIL;
	}
	if(mlock((void *)mapStart, PAGE_SIZE) == -1)
	{
		perror("Failed to mlock mmaped space");
        do_mlock = 0;
    }
    do_mlock = 1;

    mapAddr = (void volatile *)((unsigned long)mapStart + pa_offset);

	/// 不越过物理页面
    if(len + pa_offset > PAGE_SIZE)
    {
  	    size = PAGE_SIZE - pa_offset;
    }
    else
    {
  	    size = len;
    }

    memcpy((void *)mapAddr, &data, size);

    if(munmap((void *)mapStart, PAGE_SIZE) != 0)
    {
  	    perror("Failed to munmap /dev/mem");
    }
	close(memfd);
	return OK;
}
