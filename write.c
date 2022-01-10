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
        printf("Error:"DATA TO WRITE\"\n");
        return -1;
    }
    int fd, res;
    fd = open("/dev/" DEV_NAME, O_WRONLY, 0666);
    if (fd < 0)
    {
        printf("cannot open device %s", DEV_NAME);
        return -1;
    }
    else
    {
        res = write(fd, argv[1], strlen(argv[1]));
        printf("Written %d bytes to the kernel buffer", res);
    }
    close(fd);
    return 0;
}
