#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>

#define BUFF_SIZE 8192

int main(int argc,char *argv[])
{
	int fd,len,ret;
	int fd2,num_bytes;
	char *buf;
	int i;
	if (argc < 2) {
		fprintf(stderr,"Insufficient number of arguments\n");
		exit(EXIT_FAILURE);	
	}

	buf = (char*) malloc(BUFF_SIZE * sizeof(char));
	if (buf == NULL) {
		fprintf(stderr,"Write failed\n");
		exit(EXIT_FAILURE);	
	}
	
	fd = open(argv[1],O_WRONLY);
	printf("Application open fd=%d\n",fd);
	if (fd < 0) {
		perror(":error in opening file\n");
		exit(EXIT_FAILURE);
	}

	fd2 = open("sample_text",O_RDONLY);
	if (fd2 < 0) {
		perror(":error in opening sample_text\n");
		exit(EXIT_FAILURE);
	}

	num_bytes = read(fd2,buf,BUFF_SIZE);
	if (num_bytes < 0) {
		perror(":error in reading file\n");
		exit(EXIT_FAILURE);
	}
	printf("bytes read = %d\n",num_bytes);
	i = 0;	
	while (i < 3) {
		ret = write(fd,buf,BUFF_SIZE);
		if (ret < 0) {	
			fprintf(stderr,"Write failed\n");
			exit(EXIT_FAILURE);	
		}
		printf("number of byte wrtitten %d\n",ret);
		sleep(1);
		i++;
	}
	close(fd);
	close(fd2);
	free(buf);
	printf("writer close fd\n");
	return 0;
}
