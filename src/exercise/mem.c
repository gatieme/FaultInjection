#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int mapmem(off_t offset)
{
    unsigned char * map_base;
    int fd;

    fd = open("/dev/mem", O_RDWR|O_SYNC);
    if (fd == -1)
    {
        return (-1);
    }
    lseek()
    write
    return 0;
}


int main(int argc, char *argv[])
{
    //volatile long *_mem = 0, dummy;
    long long int addr, offset, pageaddr;
    char *endpt;

    if (argc > 2)
    {
        fprintf(stderr, "Usage %s <addr>\n", argv[0]);
        return 1;
    }
    else if(argc == 2)
    {
        addr = strtoll(argv[1], &endpt, 16);
    }
    else
    {
        addr = 0x2000;
    }

    offset = addr % sysconf(_SC_PAGE_SIZE);
    pageaddr = addr - offset;

    printf("addr is: 0x%llx, offset: 0x%llx, pageaddr: 0x%llx\n", addr, offset, pageaddr);

    mapmem(pageaddr);

    return 0;
}
