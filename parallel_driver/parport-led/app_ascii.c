#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<string.h>

int main(int argc,char *argv[])
{
	int fd,i,ret;
	char c;	
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
	for(i = 0; i < 256 ; i++) {
		c = i;
		ret = write(fd,&c,sizeof(char));
		if (ret < 0) {	
			fprintf(stderr,"Write failed\n");
			exit(EXIT_FAILURE);	
		}
		printf("number of byte wrtitten %d %c\n",ret,c);
		sleep(1);
	}
	close(fd);
	printf("Application close fd\n");
	return 0;
}
