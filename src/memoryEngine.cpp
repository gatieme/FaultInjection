/*
*	lib functions
*/
#include "memoryEngine.h"

/*
将内核空间的虚拟地址转换为物理地址
*/
long kern_virt_to_phys(unsigned long va)
{
	int ret;
	int procFile;
	unsigned long physicalAddr = 0;
	char buff[MAX_LINE];

	//send control word
	bzero(buff, sizeof(buff));
	sprintf(buff,"echo %lx > /proc/memoryEngine/virtualAddr",va);
	system(buff);

	bzero(buff, sizeof(buff));
	sprintf(buff,"echo %d > /proc/memoryEngine/ctl",REQUEST_KV2P);
	system(buff);

	//wait for ack signal
	procFile = open("/proc/memoryEngine/signal",O_RDONLY);
	if(procFile == -1)
	{
		perror("Failed to open /proc/memoryEngine/signal");
		return FAIL;
	}

	do {
		bzero(buff, sizeof(buff));
		ret = read(procFile, buff, MAX_LINE);
		if(ret == -1)
		{
			perror("Failed to read /proc/memoryEngine/signal");
			return FAIL;
		}
	} while(atoi(buff) != ACK_KV2P);
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
	return physicalAddr;
}

/*
将用户进程空间的虚拟地址转换为物理地址
*/
long virt_to_phys(int pid, unsigned long va)
{
	int ret;
	int procFile;
	unsigned long physicalAddr = 0;
	char buff[MAX_LINE];

	if(pid <= 0)
	{
		return FAIL;
	}

	// send control word
	bzero(buff, sizeof(buff));
	sprintf(buff, "echo %d > /proc/memoryEngine/pid",pid);
	system(buff);

    bzero(buff, sizeof(buff));
	sprintf(buff,"echo %lx > /proc/memoryEngine/virtualAddr",va);
	system(buff);

    bzero(buff, sizeof(buff));
	sprintf(buff,"echo %d > /proc/memoryEngine/ctl",REQUEST_V2P);
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

	// read physicalAddr
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
	return physicalAddr;
}

/*
获取内核函数虚拟地址
*/
long kern_func_virt_addr(const char *kFuncName)
{
	int ret;
	int procFile;
	unsigned long virtualAddr = FAIL;
	char buff[MAX_LINE];

	//send control word
	bzero(buff, sizeof(buff));
	sprintf(buff,"echo %s > /proc/memoryEngine/kFuncName",kFuncName);
	system(buff);
	bzero(buff, sizeof(buff));
	sprintf(buff,"echo %d > /proc/memoryEngine/ctl",REQUEST_KFUNC_VA);
	system(buff);

	//wait for ack signal
	procFile = open("/proc/memoryEngine/signal",O_RDONLY);
	if(procFile == -1)
	{
		perror("Failed to open /proc/memoryEngine/signal");
		return FAIL;
	}

	do {
		bzero(buff, sizeof(buff));
		ret = read(procFile, buff, MAX_LINE);
		if(ret == -1)
		{
			perror("Failed to read /proc/memoryEngine/signal");
			return FAIL;
		}
	} while(atoi(buff) != ACK_KFUNC_VA);
	close(procFile);

	//read virtualAddr
	procFile = open("/proc/memoryEngine/virtualAddr",O_RDONLY);
	if(procFile == -1)
	{
		perror("Failed to open /proc/memoryEngine/virtualAddr");
		return FAIL;
	}
	bzero(buff, sizeof(buff));
	ret = read(procFile, buff, MAX_LINE);
	if(ret == -1)
	{
		perror("Failed to read /proc/memoryEngine/virtualAddr");
		return FAIL;
	}
	close(procFile);

	if(sscanf(buff,"%lx",&virtualAddr) != 1) { return FAIL; }
	return virtualAddr;
}

/*
*	读取物理内存地址内容
*/

extern int errno;
int read_phy_mem(unsigned long pa,long *data)
{
	int memfd;
	int pageSize;
	int shift;
	int do_mlock;
	void volatile *mapStart;
	void volatile *mapAddr;
	unsigned long pa_base;
	unsigned long pa_offset;

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


	printf("PAGE_SIZE:0x%x\n",PAGE_SIZE);   // 4k = 0x1000
	printf("base:0x%lx\n",pa_base);
	printf("offset:0x%lx\n",pa_offset);
	printf("pa:0x%lx\n",pa);

	mapStart = (void volatile *)mmap(0, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_LOCKED, memfd, pa_base);
	//mapStart = (void volatile *)mmap(0, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, memfd, pa);
	if(mapStart == MAP_FAILED)
	{
		printf("Failed to mmap /dev/mem, errno = %d\n", errno);
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

  //只读一个字节
  memcpy( data, (void *)mapAddr, sizeof(data) );
  //*data = *((char *)mapAddr);

  if(munmap((void *)mapStart, PAGE_SIZE) != 0)
  {
  	perror("Failed to munmap /dev/mem");
  }
	close(memfd);
	return OK;
}

int write_page_0(unsigned long pa)
{
	int memfd;
	int pageSize;
	int shift;
	int do_mlock;
	void volatile *mapStart;
	unsigned long pa_base;

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

	printf("base:0x%lx\n",pa_base);


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

  memset((void *)mapStart, '0', PAGE_SIZE);

  if(munmap((void *)mapStart, PAGE_SIZE) != 0)
  {
  	perror("Failed to munmap /dev/mem");
  }
	close(memfd);
	return OK;
}

/*
*	改写物理内存地址，不会越过一个物理页面
*/
int write_phy_mem(unsigned long pa,void *data,int len)
{
	int memfd;
	int pageSize;
	int shift;
	int size;
	int do_mlock;
	void volatile *mapStart;
	void volatile *mapAddr;
	unsigned long pa_base;
	unsigned long pa_offset;

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

  memcpy((void *)mapAddr, data, size);

  if(munmap((void *)mapStart, PAGE_SIZE) != 0)
  {
  	perror("Failed to munmap /dev/mem");
  }
	close(memfd);
	return OK;
}

/*
*
*/
int getTaskInfo(int pid, pTaskMMInfo taskInfo)
{
	int ret;
	int count;
	int procFile;
	char buff[MAX_LINE];
	char line[MAX_LINE];
	unsigned long iLine;
	int i;

	bzero(buff, sizeof(buff));
	sprintf(buff,"echo %d > /proc/memoryEngine/pid", pid);
	system(buff);

	//send control word
	bzero(buff, sizeof(buff));
	sprintf(buff,"echo %d > /proc/memoryEngine/ctl", REQUEST_TASK_INFO);
	system(buff);

	//wait for ack signal
	procFile = open("/proc/memoryEngine/signal",O_RDONLY);
	if(procFile == -1)
	{
		perror("Failed to open /proc/memoryEngine/signal");
		return FAIL;
	}

	do {
		bzero(buff, sizeof(buff));
		ret = read(procFile, buff, MAX_LINE);
		if(ret == -1)
		{
			perror("Failed to read /proc/memoryEngine/signal");
			return FAIL;
		}
	} while(atoi(buff) != ACK_TASK_INFO);
	close(procFile);

	//read task info
	procFile = open("/proc/memoryEngine/taskInfo",O_RDONLY);
	if(procFile == -1)
	{
		perror("Failed to open /proc/memoryEngine/taskInfo");
		return FAIL;
	}
	bzero(buff, sizeof(buff));
	ret = read(procFile, buff, MAX_LINE);
	if(ret == -1)
	{
		perror("Failed to read /proc/memoryEngine/taskInfo");
		return FAIL;
	}
	close(procFile);

	//printf("%s",buff);

		//fill struct taskMMInfo
		count = 0;
		for(i=0; i<varCount; i++)
		{
			bzero(line,sizeof(line));
			count += ReadLine(buff+count,line);
			sscanf(line,"%lx",&iLine);
			memcpy((void *)((unsigned long)taskInfo + i * sizeof(unsigned long)),&iLine,sizeof(unsigned long));
		}
		/*
		for(i=0; i<varCount; i++)
		{
			printf("0x%lx\n", *(unsigned long *)((unsigned long)taskInfo+i*sizeof(unsigned long)) );
		}
		*/

	return OK;
}

/*
*	Read a line from a string.
*/
int ReadLine(char *input,char *line)
{
	int iLen = 0;
	while(input[iLen] != '\n' && input[iLen] != '\0')
	{
		iLen ++;
	}
	memcpy(line,input,iLen);
	return ++iLen;
}
