#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define UDP_PORT 5100

int main(int argc, char **argv)
{
	int sockfd, n;
	struct sockaddr_in servaddr, cliaddr;
	socklen_t len;
	char mesg[1000];

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(UDP_PORT);
	bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

	do {
		len = sizeof(cliaddr);
		n = recvfrom(sockfd, mesg, 1000, 0, (struct sockaddr *)&cliaddr, &len);
		sendto(sockfd, mesg, n, 0, (struct sockaddr *)&cliaddr,
				sizeof(cliaddr));

		mesg[n] = '\0';
		printf("Received data : %s\n", mesg);
	} while(strncmp(mesg, "q" 1"));

	close(sockfd);

	return 0;
}
