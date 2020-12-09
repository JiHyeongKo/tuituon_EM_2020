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

#define EXAM_DRIVER_NAME "/dev/exam"

int main(int argc, int **argv)
{
    char data[128] = {33,34,35,36,0,};
    int fd;
    printf("enter test\n");

    // open driver
    fd = open(EXAM_DRIVER_NAME, O_RDWR);

    if(fd < 0)
    {
        printf("open fail\n");
        perror("driver (/dev/exam) open error.\n");
        return 1;
    }

    printf("open success\n");

    write(fd, &data, 4);
    read(fd, data, 4);
    printf("read Value: %c%c%c%c\n", data[0], data[1], data[2], data[3]);

    unsigned int inputCmd;
    unsigned long returnValue;
    
    //char array[20];10
    inputCmd = _IOW(199, 10, int);
    returnValue = ioctl(fd, inputCmd, data);
     printf("1returnValue : %d\n", inputCmd);
    inputCmd = _IOW(199, 11, int);
    int count = 5;
    returnValue = ioctl(fd, inputCmd, &count);
     printf("2returnValue : %d\n", inputCmd);
    close(fd);

    return 0;
}
