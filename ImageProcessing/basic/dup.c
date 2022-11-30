#include <unistd.h>

int main()
{
	char n;
	do {
		read(0, &n, 1);
		write(1, &n, 1);
	} while (n != '\n');

	return 0;
}
