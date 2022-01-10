#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#define DEV_NAME "my_device2"
char buff[50];

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        printf("Error":DATA TO WRITE\"\n");
        return -1;
    }
    int fd, res;
    fd = open("/dev/" DEV_NAME, O_WRONLY, 0666);
    if (fd < 0)
    {
        printf("cannot not open device %s", DEV_NAME);
        return -1;
    }
    else
    {
        read(fd, buff, 50);
        printf("Read from device %s bytes to the kernel buff", buff)
    }
    close(fd);
    return 0;
}

