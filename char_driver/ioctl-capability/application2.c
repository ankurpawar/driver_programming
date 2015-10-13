#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<string.h>
#include<sys/ioctl.h>
#include"ioctl_dev.h"

#define BUFF_SIZE 1024

int main(int argc,char *argv[])
{
	int fd,len,ret;
	char buff[BUFF_SIZE];
	int quant_size = 0;	
	char *str = "hello to the world of linux kernel device driver.This is my first kernel driver"
		    ".Hope everything will be fine";
	if (argc < 2) {
		fprintf(stderr,"Insufficient number of arguments\n");
		exit(EXIT_FAILURE);	
	}

	fd = open(argv[1],O_RDONLY);
	printf("Application open fd=%d\n",fd);
	if (fd < 0) {
		perror(":error in opening file");
		exit(EXIT_FAILURE);
	}

	ret = ioctl(fd,SCULL_IOCGQUANTUM,&quant_size);
	if (ret < 0) {
		fprintf(stderr,"ioctl failed\n");
		exit(EXIT_FAILURE);
	}
	printf("qunatum_size = %d \n",quant_size);
	
	quant_size = 16;
	ret = ioctl(fd,SCULL_IOCSQUANTUM,&quant_size);
	if (ret < 0) {
		fprintf(stderr,"ioctl failed\n");
		exit(EXIT_FAILURE);
	}
	printf("qunatum_size = %d ret=%d\n",quant_size,ret);
	
	close(fd);
	
	printf("Application2 close fd\n");
	return 0;
}
