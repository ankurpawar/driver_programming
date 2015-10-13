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

	fd = open(argv[1],O_WRONLY);
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
	
	ret = write(fd,str,strlen(str));
	if (ret < 0) {	
		fprintf(stderr,"Write failed\n");
		exit(EXIT_FAILURE);	
	}
	printf("number of byte wrtitten %d",ret);

	close(fd);
	
	fd = open(argv[1],O_RDONLY);
	printf("Application open fd=%d\n",fd);
	if (fd < 0) {
		perror(":error in opening file");
		exit(EXIT_FAILURE);
	}

	
	ret = llseek(fd,0,SEEK_SET);
	if (ret < 0) {
		fprintf(stderr,"seek failed\n");
		exit(EXIT_FAILURE);
	}
	printf("new pos %d\n",ret);
	
	memset(buff, 0,sizeof(buff));
	ret = read(fd,buff,500);
	if (ret < 0) {
		fprintf(stderr,"Read failed\n");
		exit(EXIT_FAILURE);
	}	
	printf("number of byte read %d,string=%s\n",ret,buff);

	close(fd);
	printf("Application close fd\n");
	return 0;
}
