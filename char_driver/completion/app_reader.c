#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<string.h>

#define BUFF_SIZE 1024

int main(int argc,char *argv[])
{
	int fd,len,ret;
	char buff[BUFF_SIZE];
	
	char *str ="This is aplication 2 trying to write in the scull along with application 1.Which of the two application would succeed?";
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
	
	memset(buff, 0,sizeof(buff));
	ret = read(fd,buff,349);
	if (ret < 0) {
		fprintf(stderr,"Read failed\n");
		exit(EXIT_FAILURE);
	}	
	printf("number of byte read %d,string=%s\n",ret,buff);
	
	close(fd);
	printf("Application close fd\n");
	return 0;
}
