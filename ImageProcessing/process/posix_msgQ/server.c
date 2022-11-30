#include <stdio.h>
#include <unistd.h>
#include <mqueue.h>

int main(int argc, char **argv)
{
	mqd_t mq;
	struct mq_attr attr;
	const char* name = "/posix_msq";
	char buf[BUFSIZ];
	int n;

	attr.mq_flags = 0;
	attr.mq_maxmsg = 10;
	attr.mq_msgsize = BUFSIZ;
	attr.mq_curmsgs = 0;

	mq = mq_open(name, O_CREAT | O_RDONLY, 0644, &attr);

	while(1) {
		n = mq_receive(mq, buf, sizeof(buf), NULL);
		switch (buf[0]) {
			case 'q':
				goto END;
				break;
			default:
				write(1, buf, n);
				break;
		}
	}

END:
	mq_close(mq);
	mq_unlink(name);

	return 0;
}
