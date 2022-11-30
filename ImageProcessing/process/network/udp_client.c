#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define UDP_PORT 5100

int main(int argc, char **argv)
{
	int sockfd, n;
	struct sockaddr_in servaddr, cliaddr;
	socklen_t clisize;
	char mesg[BUFSIZ];

	if(argc !=2) {
		printf("Usage : %s <IP address>\n", argv[0]); 
		return -1;
	}

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;

	inet_pton(AF_INET, argv[1], &(servaddr.sin_addr.s_addr));
	servaddr.sin_port = htons(UDP_PORT);


	do {
		fgets(mesg, BUFSIZ, stdin);
		sendto(sockfd, mesg,strlen(mesg), 0, (struct sockaddr *)&servaddr,
				sizeof(servaddr));
		clisize = sizeof(cliaddr);

		n = recvfrom(sockfd, mesg, BUFSIZ, 0, (struct sockaddr*)&cliaddr,
				&clisize);
		mesg[n] = '\0';
		fputs(mesg, stdout);
	} while(strncmp(mesg, "q", 1));

	close(sockfd);

	return 0;
}
