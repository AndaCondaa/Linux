#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>

int main (int argc, char **argv)
{
	int i, j;
	time_t rawtime;
	struct tm *tm;
	char buf[BUFSIZ];
	struct timeval mytime;

	time(&rawtime);
	printf("time : %u\n", (unsigned)rawtime);

	gettimeofday(&mytime, NULL);
	printf("gettimeofday : %ld/%d\n", mytime.tv_sec, mytime.tv_usec);

	printf("ctime : %s", ctime(&rawtime));

	putenv("TZ=PST3PDT");
	tzset();
	tm = localtime(&rawtime);
	printf("asctime : %s", asctime(tm));

	strftime(buf, sizeof(buf), "%a %b %e %H:%M:%S %Y",tm);
	printf("strftime : %s\n", buf); 

	return 0;
} 

