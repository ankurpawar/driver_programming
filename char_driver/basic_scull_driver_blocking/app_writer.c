#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<string.h>

#define BUFF_SIZE 1024

int main(int argc,char *argv[])
{
	int fd,len,ret;
	char buff[BUFF_SIZE];
	
	char *str = "This data is for app_reader blocked on reading empty scull.This data will be first copied to scull.Then"
		    " the reader will be woken up.";
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
	
	printf("writer_app close fd\n");
	return 0;
}
