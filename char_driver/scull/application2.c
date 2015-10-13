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
	
	fd = open(argv[1],O_RDONLY);
	printf("Application open fd=%d\n",fd);
	if (fd < 0) {
		perror(":error in opening file");
		exit(EXIT_FAILURE);
	}

	
	ret = llseek(fd,50,SEEK_SET);
	if (ret < 0) {
		fprintf(stderr,"seek failed\n");
		exit(EXIT_FAILURE);
	}
	printf("new pos %d\n",ret);
	
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
