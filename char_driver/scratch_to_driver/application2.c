#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<string.h>

#define BUFF_SIZE 1024

int main(int argc,char *argv[])
{
	int fd,len,ret;
	char buff[BUFF_SIZE];
	
	char *str = "this is another process trying to write on the scull.Will it succeed";
	char *str2 = "These bytes will be inserted in between the scull";
	if (argc < 2) {
		fprintf(stderr,"Insufficient number of arguments\n");
		exit(EXIT_FAILURE);	
	}

	fd = open(argv[1],O_RDWR);
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

	
	ret = write(fd,str2,strlen(str2));
	if (ret < 0) {	
		fprintf(stderr,"Write failed\n");
		exit(EXIT_FAILURE);	
	}
	printf("number of byte wrtitten %d",ret);

	ret = llseek(fd,0,SEEK_SET);
	if (ret < 0) {
		fprintf(stderr,"seek failed\n");
		exit(EXIT_FAILURE);
	}
	printf("new pos %d\n",ret);
	
	memset(buff, 0,sizeof(buff));
	ret = read(fd,buff,20);
	if (ret < 0) {
		fprintf(stderr,"Read failed\n");
		exit(EXIT_FAILURE);
	}	
	printf("number of byte read %d,string=%s\n",ret,buff);

	close(fd);
	printf("Application close fd\n");
	return 0;
}
