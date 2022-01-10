#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>

int main()
{
	int fd;
	char Ubuff[]="character driver";
	char Kbuff[50];

	fd = open("/dev/sample_cdev1", O_RDWR);
	if(fd < 0) 
	{
		printk("Unable to open the device file\n");
		return -1;
	}

	/* Write the data into the device */
	write(fd , Ubuff , strlen(Ubuff) + 1);

	/* Read the data back from the device */
	read(fd , Kbuff , sizeof(Kbuff));
	printf("Data from kernel : %s\n", Kbuff);

	close(fd);	
	return 0;
}

