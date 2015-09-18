#include <stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<string.h>

#define MAGIC_NUM "196"

int main()
{
	int ret;
	int fd;
	int len = strlen(ID);
	fd = open("/dev/misc_drv",O_WRONLY);
	if (fd < 0)
	{
		perror("error in opening file\n");
		exit(EXIT_FAILURE);
	}
	ret = write(fd, ID, len);
	printf("ret = %d\n", ret);
	return 0;
}
