
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>

int main()
{
	int fd;
	char Ubuff[100];

	memset(Ubuff,0,sizeof(Ubuff));
	
	fd=open("/dev/desd",O_RDWR);

	write(fd,"Hello Kernel..!!\n",20);
	
	read(fd,(char *)&Ubuff,sizeof(Ubuff));
	printf("\n\nData from the Kernel space : %s\n",Ubuff);
	close(fd);
	return 0;
}


