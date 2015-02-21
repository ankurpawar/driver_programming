#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>

#define BUFF_SIZE 512

int main(int argc,char *argv[])
{
	int fd,len,ret;
	int i;
	char *buf;
	if (argc < 2) {
		fprintf(stderr,"Insufficient number of arguments\n");
		exit(EXIT_FAILURE);	
	}

	buf = (char *) malloc(BUFF_SIZE * sizeof(char));
	if (buf == NULL) {
		fprintf(stderr,"error in allocating memory\n");
		exit(EXIT_FAILURE);
	}

	fd = open(argv[1],O_RDONLY);
	printf("Application open fd=%d\n",fd);
	if (fd < 0) {
		perror(":error in opening file");
		exit(EXIT_FAILURE);
	}

	i = 0;
	while (i < 30) {
		memset(buf,0,BUFF_SIZE);
		ret = read(fd,buf,BUFF_SIZE);
		if (ret < 0) {
			fprintf(stderr,"Read failed\n");
			exit(EXIT_FAILURE);
		}
		printf("\nnumber of byte read %d,%s\n",ret,buf);
		sleep(1);
		i++;
	}
	close(fd);
	free(buf);
	printf("reader close fd\n");
	return 0;
}
