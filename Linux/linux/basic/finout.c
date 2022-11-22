#include <stdio.h>

int main()
{
	char n;

	do {
		fread(&n, sizeof(char), 1, stdin);
		fwrite(&n, sizeof(char), 1, stdout);
	} while (n!='\n');

	return 0;
}
