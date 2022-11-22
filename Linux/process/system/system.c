#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int system(const char *cmd)
{
	pid_t pid;
	int status;

	if((pid = fork()) < 0) {
		status = -1;
	} else if(pid == 0) {
		execl("/bin/sh", "sh", "-c", cmd, (char*)0);
		_exit(127);
	} else {
		while(waitpid(pid, &status, 0) < 0)
		if(errno != EINTR) {
			status = -1;
			break;
		}
		printf("%d\n", WEXITSTATUS(status));
	}
	return status;
}

int main(int argc, char **argv, char **envp)
{
	while(*envp)
		printf("%s\n", *envp++);

	int status;
	
	status = system("who");
	system("nocommand");
	system("cal");
	system("ls -1");

	printf("status : %d\n", status);
	return 0;
}
 

