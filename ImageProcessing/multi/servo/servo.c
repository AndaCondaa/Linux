#include <wiringPi.h>
#include <softPwm.h>
#include <stdio.h>
#include <stdlib.h>

void servoMotorControl(int gpio)
{
	pinMode(gpio, OUTPUT);
	softPwmCreate(gpio, 0, 200);
	while(1) {
		softPwmWrite(gpio, 26);
		delay(1000);
		softPwmWrite(gpio, 4);
		delay(1000);
	}
	softPwmWrite(gpio, 0);
}

int main(int argc, char** argv)
{
	int gno = atoi(argv[1]);
	wiringPiSetup();
	servoMotorControl(gno);
	return 0;
}
