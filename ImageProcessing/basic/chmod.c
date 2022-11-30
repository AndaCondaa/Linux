#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char **argv)
{
	struct stat statbuf;
	
	if (argc < 3) {
		fprintf(stderr, "Usage: %s file1 file2\n", argv[0]);
		return -1;
	}

	if (stat(argv[1], &statbuf) < 0) {
		perror("stat");
		return -1;
	}

	if (chmod(argv[1], (statbuf.st_mode & ~S_IXGRP) | S_ISGID) < 0) {
		perror("chmod");
		return -1;
	}

	if (chmod(argv[2], S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) < 0) {
		perror("chmod");
		return -1;
	}

	return 0;
}

