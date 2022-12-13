#include <stdlib.h>

char *alloc_two()
{
	char *tmp = malloc(5005);
	return malloc(3300);
}

char *alloc_one()
{
	return malloc(1100);
}

int main()
{
	char *mem1 = alloc_two();
	char *mem2 = alloc_one();

	free(mem1);

	return 0;
}





