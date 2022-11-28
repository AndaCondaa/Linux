#include <stdio.h>

void printBit(int n)
{
	int b = 0b0001;
	for(int i=3; i>=0; i--){
		printf("%d", n>>i&1);
	}
	putchar('\n');
}

int main()
{
	int n1 = 0b0101;
	int n2 = 0b0001;
	int n3 = 0b0010;

	printBit(n1);
	printBit(n2);
	printBit(n3);
	printBit(~n1);
	printBit(n1&n2);
	printBit(n1|n3);
	printf("%d, %d\n", n1&n2, n1|n3);

	return 0;
}
