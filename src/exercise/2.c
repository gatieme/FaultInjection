#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
int main(){
    int fd;
    char *start;
    struct stat sb;
    fd = open("/etc/passwd", O_RDONLY); /*打开/etc/passwd */
    fstat(fd, &sb); /* 取得文件大小 */
    start = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

    if(start == MAP_FAILED) /* 判断是否映射成功 */

        return;

    printf("%s", (char *)start);

    munmap(start, sb.st_size); /* 解除映射 */
    close(fd);
}
