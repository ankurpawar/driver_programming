#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<string.h>

#define BUFF_SIZE 1024

int main(int argc,char *argv[])
{
	int fd,len,ret;
	char buff[BUFF_SIZE];
	
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

	ret = write(fd,str,strlen(str));
	if (ret < 0) {	
		fprintf(stderr,"Write failed\n");
		exit(EXIT_FAILURE);	
	}
	printf("number of byte wrtitten %d",ret);

	close(fd);
	printf("write close fd\n");
	return 0;
}
