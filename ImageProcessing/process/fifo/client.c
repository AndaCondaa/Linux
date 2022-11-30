#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define FIFOFILE "fifo"

int main(int argc, char **argv)
{
	int n, fd;
	char buf[BUFSIZ];


	if((fd = open(FIFOFILE, O_WRONLY)) < 0) {
		perror("open()");
		return -1;
	}

	while((n = read(0, buf, sizeof(buf))) > 0)
		write(fd, buf, n);
	close(fd);
	return 0;
}

