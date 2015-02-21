#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>

int main(int argc,char *argv[])
{
	int fd,len,ret;
	char c;
	int i;
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
	c = 0;
	i = 0;	
	while (i < 256) {
		ret = write(fd,&c,1);
		if (ret < 0) {	
			fprintf(stderr,"Write failed\n");
			exit(EXIT_FAILURE);	
		}
		printf("number of byte wrtitten %d,%i,%c\n",ret,c,c);
		sleep(1);
		c++;
		i++;
	}
	close(fd);
	
	printf("writer close fd\n");
	return 0;
}
