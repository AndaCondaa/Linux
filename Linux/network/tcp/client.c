#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define TCP_PORT 5100

int main(int argc, char **argv)
{
	int ssock;
	struct sockaddr_in servaddr;
	char mesg[BUFSIZ];

	if(argc < 2) {
		printf("Usage : %s IP_ADRESS\n", argv[0]);
		return -1;
	}

	if((ssock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket()");
		return -1;
	}

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;

	inet_pton(AF_INET, argv[1], &(servaddr.sin_addr.s_addr));
	servaddr.sin_port = htons(TCP_PORT);

	if(connect(ssock, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
		perror("connect()");
		return -1;
	}

	fgets(mesg, BUFSIZ, stdin);
	if(send(ssock, mesg, BUFSIZ, MSG_DONTWAIT) <= 0) {
		perror("send()");
		return -1;
	}

	shutdown(ssock, SHUT_WR);

	memset(mesg, 0, BUFSIZ);
	if(recv(ssock, mesg, BUFSIZ, 0) <= 0) {
		perror("recv()");
		return -1;
	}

	shutdown(ssock, SHUT_RD);

	printf("Client Receive  : %s ", mesg);

	close(ssock);

	return 0;
}

