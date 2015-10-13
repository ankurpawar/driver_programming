#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<string.h>

int main(int argc,char *argv[])
{
	int fd,len,ret;
	char c;	
	if (argc < 2) {
		fprintf(stderr,"Insufficient number of arguments\n");
		exit(EXIT_FAILURE);	
	}

	fd = open(argv[1],O_RDONLY);
	printf("reader open fd=%d\n",fd);
	if (fd < 0) {
		perror(":error in opening file");
		exit(EXIT_FAILURE);
	}

	ret = read(fd,&c,sizeof(char));
	if (ret < 0) {	
		fprintf(stderr,"read failed ret=%d\n",ret);
		exit(EXIT_FAILURE);	
	}
	printf("number of byte read %d,%c,%x\n",ret,c,c);
	close(fd);
	printf("reader close fd\n");
	return 0;
}
