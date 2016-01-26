
#include<stdio.h>
#include<unistd.h>
#include<sys/mman.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>



extern int errno;
int main()
{
    unsigned char * map_base;
    FILE *f;
    int n, fd;

    fd = open("/dev/mem", O_RDWR|O_SYNC);
    if (fd == -1)
    {
        printf("open error...!\n");
        return (-1);
    }
    map_base = mmap(NULL, 0x10000, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0x411f50000);

    if (map_base == MAP_FAILED)
    {
        printf("NULL pointer! errono = %d\n");
    }
    else
    {
        printf("Successfull!\n");
    }

    unsigned long addr;
    unsigned char content;

    int i = 0;
    for (;i < 0x10000; ++i)
    {
        addr = (unsigned long)(map_base + i);
        content = map_base[i];
        printf("address: 0x%lx   content 0x%x\t\t", addr, (unsigned int)content);

        map_base[i] = (unsigned char)i;
        content = map_base[i];
        printf("updated address: 0x%lx   content 0x%x\n", addr, (unsigned int)content);
    }

    close(fd);

    munmap(map_base, 0xff);

    return (1);
}

