#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define SERVER_PORT 5100

int main(int argc, char **argv)
{
	int ssock;
	socklen_t clen;
	int n;
	struct sockaddr_in servaddr, cliaddr;
	char mesg[BUFSIZ];

	fd_set readfd;
	int maxfd, client_index, start_index;
	int client_fd[5] = {0};

	if((ssock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket()");
		return -1;
	}

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERVER_PORT);

	if(bind(ssock, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
		perror("bind()");
		return -1;
	}

	if(listen(ssock, 8) < 0) {
		perror("listen()");
		return -1;
	}

	maxfd = ssock;
	client_index = 0;

	do {
		FD_ZERO(&readfd);
		FD_SET(ssock, &readfd);
		for (start_index = 0; start_index < client_index; start_index++) {
			FD_SET(client_fd[start_index], &readfd);
			if(client_fd[start_index] > maxfd)
				maxfd = client_fd[start_index];
		}
		
		maxfd = maxfd + 1;

		select(maxfd, &readfd, NULL, NULL, NULL);
		
		if(FD_ISSET(ssock, &readfd)) {
			clen = sizeof(struct sockaddr_in);
			int csock = accept(ssock, (struct sockaddr*)&cliaddr, &clen);
			if(csock < 0) {
				perror("accept()");
				return -1;
			} else {
				inet_ntop(AF_INET, &cliaddr.sin_addr, mesg, BUFSIZ);
				printf("Client is connected : %s\n", mesg);

				FD_SET(csock, &readfd);
				client_fd[client_index] = csock;
				client_index++;
				continue;
			}

			if (client_index == 5) break;
		}

		for(start_index =0; start_index < client_index; start_index++) {
			if(FD_ISSET(client_fd[start_index], &readfd)) {
				memset(mesg, 0, sizeof(mesg));

				if((n = read(client_fd[start_index], mesg, sizeof(mesg))) > 0) {
					printf("Received data : %s", mesg);
					write(client_fd[start_index], mesg, n);
//					close(client_fd[start_index]);
//					FD_CLR(client_fd[start_index], &readfd);
//					client_index--;
				}
			}
		}
	} while(strncmp(mesg, "q", 1));

	close(ssock);

	return 0;
}
