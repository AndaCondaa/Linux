#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>

int main(int argc, char *argv[])
{
	int fd;
	int data;

	wiringPiSetup();
	fd = wiringPiI2CSetup(0x6a);
	if(fd == -1) {
		printf("Can't setup the I2C device\n");
		return -1;
	} else {
		for(;;) {
			data = wiringPiI2CRead(fd);
			(data==-1)?printf("No data\n") : printf("data = %d\n", data);
		}
	}
	return 0;
}

