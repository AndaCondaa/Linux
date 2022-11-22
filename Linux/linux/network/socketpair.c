#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>
#include <sys/socket.h>

int main(int argc, char **argv)
{
	int ret, sock_fd[2];
	int status;
	char buf[]= "Hello World", line[BUFSIZ];
	pid_t pid;

	ret = socketpair(AF_LOCAL, SOCK_STREAM, 0, sock_fd);
	if(ret == -1) {
		perror("socketpair()");
	}

	printf("socket 1 : %d\n", sock_fd[0]);
	printf("socket 2 : %d\n", sock_fd[1]);

	if((pid = fork()) < 0){
		perror("fork()");
	} else if(pid == 0) {
		write(sock_fd[0], buf, strlen(buf) + 1);
		printf("Data send : %s\n", buf);

		close(sock_fd[0]);
	} else {
		wait(&status);

		read(sock_fd[1], line, BUFSIZ);
		printf("Received data : %s\n", line);

		close(sock_fd[1]);
	}

	return 0;
}






