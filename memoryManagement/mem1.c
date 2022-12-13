#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	char *str;
	str = (char*)malloc(sizeof(char)*5);
	strcpy(str, "Hello World\n");
	printf("%s\n", str);

	return 0;
}


