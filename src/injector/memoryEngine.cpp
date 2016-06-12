/*
*	lib functions
*/
#include "memoryEngine.h"
#include "common.h"

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
		perror("Failed to open /proc/memoryEngine/signal ");
		return FAIL;
	}

	do {
		bzero(buff, sizeof(buff));
		ret = read(procFile, buff, MAX_LINE);
		if(ret == -1)
		{
			perror("Failed to read /proc/memoryEngine/signal ");
			return FAIL;
		}
	} while(atoi(buff) != ACK_KV2P);
	close(procFile);

	//read physicalAddr
	procFile = open("/proc/memoryEngine/physicalAddr",O_RDONLY);
	if(procFile == -1)
	{
		perror("Failed to open /proc/memoryEngine/physicalAddr ");
		return FAIL;
	}
	bzero(buff, sizeof(buff));
	ret = read(procFile, buff, MAX_LINE);
	if(ret == -1)
	{
		perror("Failed to read /proc/memoryEngine/physicalAddr ");
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
		perror("Failed to open /proc/memoryEngine/signal ");
		return FAIL;
	}

	do
    {
		bzero(buff, sizeof(buff));

        ret = read(procFile, buff, MAX_LINE);
		if(ret == -1)
		{
			perror("Failed to read /proc/memoryEngine/signal ");
			return FAIL;
		}

	} while(atoi(buff) != ACK_V2P);
	close(procFile);

	// read physicalAddr
	procFile = open("/proc/memoryEngine/physicalAddr",O_RDONLY);
	if(procFile == -1)
	{
		perror("Failed to open /proc/memoryEngine/physicalAddr ");
		return FAIL;
	}
	bzero(buff, sizeof(buff));
	ret = read(procFile, buff, MAX_LINE);
	if(ret == -1)
	{
		perror("Failed to read /proc/memoryEngine/physicalAddr ");
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
		perror("Failed to open /proc/memoryEngine/signal ");
		return FAIL;
	}

	do {
		bzero(buff, sizeof(buff));
		ret = read(procFile, buff, MAX_LINE);
		if(ret == -1)
		{
			perror("Failed to read /proc/memoryEngine/signal ");
			return FAIL;
		}
	} while(atoi(buff) != ACK_KFUNC_VA);
	close(procFile);

	//read virtualAddr
	procFile = open("/proc/memoryEngine/virtualAddr",O_RDONLY);
	if(procFile == -1)
	{
		perror("Failed to open /proc/memoryEngine/virtualAddr ");
		return FAIL;
	}
	bzero(buff, sizeof(buff));
	ret = read(procFile, buff, MAX_LINE);
	if(ret == -1)
	{
		perror("Failed to read /proc/memoryEngine/virtualAddr ");
		return FAIL;
	}
	close(procFile);

	if(sscanf(buff,"%lx",&virtualAddr) != 1)
    {
        return FAIL;
    }

    return virtualAddr;
}

/*
*	读取物理内存地址内容
*/

extern int errno;
int read_phy_mem(unsigned long pa, long *data)
{
	int memfd;
	int do_mlock;
	void volatile *mapStart;
	void volatile *mapAddr;
	unsigned long pa_base;
	unsigned long pa_offset;

	memfd = open("/dev/mem", O_RDWR | O_SYNC);
	if(memfd == -1)
	{
		perror("Failed to open /dev/mem ");
		return FAIL;
	}

//#ifdef DEBUG
//#endif

#ifdef MMAP_INVALID_ARGUMENT /*    invalid argument    when mmap   */
    dprintf("use MMAP_INVALID_ARGUMENT\n");
    pa >>= 32;
#endif
    pa_base = pa & ~PAGE_MASK;
    pa_offset = pa - pa_base;
    dprintf("=====================\n");
	dprintf("in func %s, line %d\n", __func__, __LINE__);
    dprintf("map_size   : 0x%x\n",  PAGE_SIZE);
	dprintf("pa         : 0x%lx\n", pa);
	dprintf("base       : 0x%lx\n", pa_base);
	dprintf("offset     : 0x%lx\n", pa_offset);
	dprintf("=====================\n");

    mapStart = (void volatile *)mmap(0, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_LOCKED, memfd, pa_base);
    if(mapStart == MAP_FAILED)
	{
		dbgprint("Failed to mmap /dev/mem [0x%lx], errno : [%d, %s]\n", pa_base, errno, strerror(errno));
		perror("Failed to mmap /dev/mem ");
		close(memfd);
		return FAIL;
	}
	if(mlock((void *)mapStart, PAGE_SIZE) == -1)
	{
		perror("Failed to mlock mmaped space ");
        do_mlock = 0;
    }
    dbgprint("mlock mmaped space success...\n");

    do_mlock = 1;

    mapAddr = (void volatile *)((unsigned long)mapStart + pa_offset);
    dbgprint("mapstart = 0x%lx, mapAddr = 0x%lx\n", mapStart, mapAddr);

    //只读一个字节
    if(memcpy( data, (void *)mapAddr, sizeof(data) ) == NULL)
    {
        //*data = *((char *)mapAddr);
        perror("Memcpy eroor ");
    }
    dbgprint("read data 0x%lx at 0x%lx success\n", (unsigned long)*data, pa);


    if(munmap((void *)mapStart, PAGE_SIZE) != 0)
    {
  	    perror("Failed to munmap /dev/mem ");
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
	unsigned long pa_offset;

	memfd = open("/dev/mem", O_RDWR | O_SYNC);
	if(memfd == -1)
	{
		perror("Failed to open /dev/mem ");
		return FAIL;
	}


#ifdef MMAP_INVALID_ARGUMENT /*    invalid argument    when mmap   */
    pa >>= 32;
#endif
    pa_base = pa & ~PAGE_MASK;
    pa_offset = pa - pa_base;
    dprintf("=====================\n");
	dprintf("in func %s, line %d\n", __func__, __LINE__);
    dprintf("map_size   : 0x%x\n",  PAGE_SIZE);
	dprintf("pa         : 0x%lx\n", pa);
	dprintf("base       : 0x%lx\n", pa_base);
	dprintf("offset     : 0x%lx\n", pa_offset);
	dprintf("=====================\n");

    mapStart = (void volatile *)mmap(0, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_LOCKED, memfd, pa_base);
    if(mapStart == MAP_FAILED)
	{
		perror("Failed to mmap /dev/mem ");
		dbgprint("Failed to mmap /dev/mem, errno : [%d, %s]\n", errno, strerror(errno));
		close(memfd);
		return FAIL;
	}
	if(mlock((void *)mapStart, PAGE_SIZE) == -1)
	{
		perror("Failed to mlock mmaped space ");
    do_mlock = 0;
  }
  do_mlock = 1;

  memset((void *)mapStart, '0', PAGE_SIZE);

  if(munmap((void *)mapStart, PAGE_SIZE) != 0)
  {
  	perror("Failed to munmap /dev/mem ");
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
	int size;
	int do_mlock;
	void volatile *mapStart;
	void volatile *mapAddr;
	unsigned long pa_base;
	unsigned long pa_offset;

	memfd = open("/dev/mem", O_RDWR | O_SYNC);
	if(memfd == -1)
	{
		perror("Failed to open /dev/mem ");
		return FAIL;
	}



    pa_base = pa & ~PAGE_MASK;
    pa_offset = pa - pa_base;
    dprintf("=====================\n");
	dprintf("in func %s, line %d\n", __func__, __LINE__);
    dprintf("map_size   : 0x%x\n",  PAGE_SIZE);
	dprintf("pa         : 0x%lx\n", pa);
	dprintf("base       : 0x%lx\n", pa_base);
	dprintf("offset     : 0x%lx\n", pa_offset);
	dprintf("=====================\n");

    mapStart = (void volatile *)mmap(0, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_LOCKED, memfd, pa_base);
	if(mapStart == MAP_FAILED)
	{
		perror("Failed to mmap /dev/mem ");
		dbgprint("Failed to mmap /dev/mem, errno : [%d, %s]\n", errno, strerror(errno));
		close(memfd);
		return FAIL;
	}
	if(mlock((void *)mapStart, PAGE_SIZE) == -1)
	{
		perror("Failed to mlock mmaped space ");
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

    dbgprint("write data 0x%lx at 0x%lx success\n", *(unsigned long *)data, pa);

    //unsigned long temp = ~(-1);
    //print_all_byte(data, size);
    memcpy((void *)mapAddr, data, size);

    if(munmap((void *)mapStart, PAGE_SIZE) != 0)
    {
  	    perror("Failed to munmap /dev/mem ");
    }
	close(memfd);
	return OK;
}

/*
*
*/
int getTaskInfo(int pid, pTaskMMInfo taskInfo)
{
	int             ret;
	int             count;
	int             procFile;
	char            buff[MAX_LINE];
	char            line[MAX_LINE];
	unsigned long   iLine;
	int             i;

	bzero(buff, sizeof(buff));
	sprintf(buff,"echo %d > /proc/memoryEngine/pid", pid);
    dbgprint("%s\n",buff);
	system(buff);

	//send control word
	bzero(buff, sizeof(buff));
	sprintf(buff,"echo %d > /proc/memoryEngine/ctl", REQUEST_TASK_INFO);
    dbgprint("%s\n",buff);
	//return 0;
    system(buff);           //  BUG002 error when write CTL

	//wait for ack signal
	procFile = open("/proc/memoryEngine/signal",O_RDONLY);
	if(procFile == -1)
	{
		perror("Failed to open /proc/memoryEngine/signal ");
		return FAIL;
	}
    dbgprint("open proc/memoryEngine/signal success...\n");

    do
    {
		bzero(buff, sizeof(buff));
		ret = read(procFile, buff, MAX_LINE);
		if(ret == -1)
		{
			perror("Failed to read /proc/memoryEngine/signal ");
			return FAIL;
		}
        dbgprint("%s\n", buff);

	} while(atoi(buff) != ACK_TASK_INFO);
	close(procFile);

	//  read task info
	procFile = open("/proc/memoryEngine/taskInfo",O_RDONLY);
	if(procFile == -1)
	{
		perror("Failed to open /proc/memoryEngine/taskInfo ");
		return FAIL;
	}
	bzero(buff, sizeof(buff));
	ret = read(procFile, buff, MAX_LINE);
	if(ret == -1)
	{
		perror("Failed to read /proc/memoryEngine/taskInfo ");
		return FAIL;
	}
	close(procFile);

    dprintf("\n=====================SHOW THE BUFFER START=====================\n");
    dprintf("[%s, %s, %d]\n", __FILE__, __func__, __LINE__);
	dprintf("%s\n", buff);
    dprintf("\n=====================SHOW THE BUFFER ENDED=====================\n");
    dprintf("\n");

    //fill struct taskMMInfo
	count = 0;
	for(i = 0; i < varCount; i++)
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



/*
 * addr  -=> 待打印的变量的首地址
 * size  -=>·待打印的变量的大小
 * return 成功返回打印的字节数
 * */
int print_all_byte(void *addr, int size)
{
    unsigned char *ptr = (unsigned char *)addr;
    int print_bytes = 0;

    if(ptr == NULL)
    {
        return -1;
    }

    while(print_bytes < size)
    {
        printf("%02x", *ptr);

        ptr++;

        print_bytes++;
    }
    printf("\n");

    return print_bytes;
}


/*  modify by gatieme
 *
 * */
int run_command(char *command, char *result)
{
    FILE *fstream = NULL;
    char buff[81];

    if((fstream = popen(command, "r")) == NULL)
    {
        perror("execute command failed ");
        return -1;
    }

    memset(buff, 0, sizeof(buff));
    while(fgets(buff, sizeof(buff), fstream) != NULL)
    {
        //printf("%s\n",buff);
        strcat(result, buff);
    }
    int i = strlen(result) - 1;
    while(i >= 0 && IsSpace(result[i]) == true)
    {
        //printf("buff[%d] = %c, %d\n", i, result[i], result[i]);
        result[i] = '\0';
        i--;
    }
    pclose(fstream);

    return 0;
}

/*  modify by gatieme @ 2016-05-21 16:12
 *
 */
int is_kthread(int pid)
{
    char command[81];
    char pname[81];

    memset(command, 0, 81);
    memset(pname, 0, 81);

    sprintf(command, "ps -p %d -o command=", pid);
    run_command(command, pname);


    printf("process PID : %d, command : %s ", pid, pname);
    if(pname[0] == '[')
    {
        printf("is a kernel thread\n");
        return 1;
    }
    else if(pname[0] != '\0')
    {
        printf("is a user process\n");
        return 0;
    }
    else
    {
        printf("no such process %d\n", pid);
    }
}

