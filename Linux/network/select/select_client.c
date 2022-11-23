#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define TCP_PORT 5100

int main(int argc, char **argv)
{
	struct sockaddr_in servaddr;
	fd_set read_fd;
	int ssock;
	char mesg[BUFSIZ];

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
	
	do {
		FD_ZERO(&read_fd);
		FD_SET(0, &read_fd);
		FD_SET(ssock, &read_fd);

		select(ssock+1, &read_fd, NULL, NULL, NULL);
		memset(mesg, 0, BUFSIZ);

		if(FD_ISSET(ssock, &read_fd) != 0) {
			int n = read(ssock, mesg, BUFSIZ);
			if(n <= 0) break;
			printf("FROM SERVER : %s\n", mesg);	
//			FD_CLR(ssock, &read_fd);
		} 

		if(FD_ISSET(0, &read_fd) != 0) {
			read(0, mesg, BUFSIZ);
			int n = strlen(mesg); 
			write(ssock, mesg, BUFSIZ);
//			FD_CLR(0, &read_fd);
		}
	} while(strcmp(mesg, "q"));

	close(ssock);

	return 0;
}

