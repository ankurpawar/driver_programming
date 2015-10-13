#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<string.h>

#define BUFF_SIZE 1024

int main(int argc,char *argv[])
{
	int fd,len,ret;
	char buff[BUFF_SIZE];
	
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
	printf("app_reader close\n");
	return 0;
}
