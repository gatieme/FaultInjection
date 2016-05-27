#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int mem_fd;


#define KERN_START 0xC0000000
int iskernaddr(unsigned long addr)
{
    /*is address valid?*/
    if(addr < KERN_START)
        return -1;
    else
        return 0 ;
}

/*read from kernel virtual address*/
int read_virt(unsigned long addr, void *buf, unsigned int len)
{
    /*if( iskernaddr(addr)<0 )
        return -1;

    addr = addr - KERN_START;
*/
    lseek(mem_fd, addr, SEEK_SET);

    return read(mem_fd, buf, len);
}


/*write to kernel virtual address*/
int write_virt(unsigned long addr, void *buf, unsigned int len)
{
    /*
    if( iskernaddr(addr)<0 )
        return -1;

    addr = addr - KERN_START;
    */

    lseek(mem_fd, addr, SEEK_SET);

    return write(mem_fd, buf, len);
}

int openmem(off_t offset)
{
    unsigned char * map_base;
    int fd;

    mem_fd = open("/dev/mem", O_RDWR|O_SYNC);
    if (mem_fd == -1)
    {
        return (-1);
    }

    return mem_fd;
}

int main(int argc, char **argv)
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

    char buff[81];
    mem_fd = open("/dev/mem", O_RDWR|O_SYNC);
    if (mem_fd == -1)
    {
        return (-1);
    }
    else
    {
        printf("open success...\n");
    }

    memset(buff, '\0', 81);

    if(read_virt(addr, buff, 81) == -1)
    {
        perror("read error ");
    }
    else
    {
        printf("%s\b", buff);
    }
}
