#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define TCP_PORT 5100

int main(int argc, char **argv)
{
	int ssock;
	socklen_t clen;
	int n;
	struct sockaddr_in servaddr, cliaddr;
	char mesg[BUFSIZ];

	if((ssock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket()");
		return -1;
	}

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(TCP_PORT);

	if(bind(ssock, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
		perror("bind()");
		return -1;
	}

	if(listen(ssock, 8) < 0) {
		perror("listen()");
		return -1;
	}
	
	clen = sizeof(cliaddr);
	do {
		int n, csock = accept(ssock, (struct sockaddr *)&cliaddr, &clen);

		inet_ntop(AF_INET, &cliaddr.sin_addr, mesg, BUFSIZ);
		printf("Client is connected : %s\n", mesg);

		if((n = read(csock, mesg, BUFSIZ)) <= 0)
			perror("read()");

		printf("Received data : %s", mesg);

		if(write(csock, mesg, n) <= 0)
			perror("write()");

		close(csock);

	} while(strncmp(mesg, "q", 1));

	close(ssock);

	return 0;
}
