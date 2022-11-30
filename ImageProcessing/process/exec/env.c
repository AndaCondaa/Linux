#include <stdio.h>

extern char** environ;

int main(int argc, char** argv)
{
	while(*environ)
		printf("%s\n", *environ++);

	return 0;
}
