#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int main()
{
	srand((unsigned int)time(NULL));
	printf("rand : %d\n ", rand());
	printf("rand : %d\n ", rand());
	printf("rand : %d\n ", rand());
	printf("rand : %d\n ", rand());
	printf("rand : %d\n ", rand());
	return 0;
}

