#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>

int main()
{
	umask(0);
	int fd;
	if((fd = creat("test.txt", 0666)) < 0){
		perror("creat()");
		return -1;
	}
	printf("First printf is on the screen.\n");
	dup2(fd,1);
	printf("Second printf is in this file.\n");
	close(fd);
	return 0;
}
