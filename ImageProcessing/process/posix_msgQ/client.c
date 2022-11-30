#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <mqueue.h>

int main(int argc, char **argv)
{
	mqd_t mq;
	const char* name = "/posix_msq";
	char buf[BUFSIZ];

	mq = mq_open(name, O_WRONLY);

	strcpy(buf, "Hello, World!\n");
	mq_send(mq, buf, strlen(buf), 0);

	strcpy(buf, "q");
	mq_send(mq, buf, strlen(buf), 0);

	mq_close(mq);

	return 0;
} 
