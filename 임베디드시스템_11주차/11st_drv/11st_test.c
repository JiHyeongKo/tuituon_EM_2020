#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <ctype.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define HELLO_DRIVER_NAME "/dev/hello"

int main(int argc, int **argv)
{
    char data[4] = {0,};
    int fd;
    printf("enter test\n");

    // open driver
    fd = open(HELLO_DRIVER_NAME, O_RDWR);

    if(fd < 0)
    {
        printf("open fail\n");
        perror("driver (/dev/hello) open error.\n");
        return 1;
    }

    printf("open success");

    write(fd, &data, 4);
    read(fd, data, 4);
    printf("read Value: %c%c%c%c\n", data[0], data[1], data[2], data[3]);

    close(fd);

    return 0;
}
