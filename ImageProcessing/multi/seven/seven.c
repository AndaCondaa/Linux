#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>

void sevenControl()
{
	char a; 
	pinMode(8, OUTPUT);
	pinMode(1, OUTPUT);
	pinMode(7, OUTPUT);
	for (;;) {
		printf("input : " );
		scanf("%c%*c", &a);
		printf("output: %c\n", a);
		if (a =='a' ) {
			digitalWrite(8, HIGH);
			digitalWrite(1, HIGH);
			digitalWrite(7, HIGH);
			delay(1000);
			digitalWrite(1, LOW);
			digitalWrite(7, LOW);
			digitalWrite(8, LOW);
		}
	}
}

int main()
{
	wiringPiSetup();
	sevenControl();
	return 0;
}

