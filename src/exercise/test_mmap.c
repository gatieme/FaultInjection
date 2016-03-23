#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/file.h>
#include <errno.h>

long* mapmem(off_t offset)
{
    int fd;
    long *ret;

    fd = open("/dev/mem", O_RDWR|O_SYNC);

    if (fd == -1) {
        perror("open");
        return NULL;
    }

    printf("offset (pageaddr) is: %ld\n", offset);

    ret = mmap(0, sizeof(long), PROT_READ|PROT_WRITE, MAP_SHARED, fd, offset);
    if (ret == MAP_FAILED) {
        perror("mmap");
        ret = NULL;
    }

    printf("Return address is: %p\n", ret);

    if (close(fd) == -1)
        perror("close");

    return ret;
}

int main(int argc, char *argv[])
{
    long *mem = 0;
    volatile long *_mem = 0, dummy;
    long long int addr, offset, pageaddr;
    char *endpt;

    if (argc != 2) {
        fprintf(stderr, "Usage %s <addr>\n", argv[0]);
        return 1;
    }

    addr = strtoll(argv[1], &endpt, 16);
    offset = addr % sysconf(_SC_PAGE_SIZE);
    pageaddr = addr - offset;

    printf("addr is: %lld, offset: %lld, pageaddr: %lld\n", addr, offset, pageaddr);

    mem = mapmem(pageaddr);

    return 0;
}
