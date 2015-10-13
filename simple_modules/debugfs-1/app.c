#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

#define STR "Hello world"
int main()
{
	int ret;
	int fd;
	int len = strlen(STR);
	fd = open("/sys/kernel/debug/a_rookie/filetxt",O_WRONLY);
	if (fd < 0)
	{
		perror("error in opening file\n");
		exit(EXIT_FAILURE);
	}
	ret = write(fd, STR, len);
	printf("ret = %d\n", ret);
	return 0;
}
