#include <stdio.h>
#include <sys/stat.h>

typedef struct {
	int id;
	char name[16];
	char phoneNum[20];
} ClientInfo;

int main ()
{
	ClientInfo pInfo[2]={ {100, "Park", "010-1234-1234"}, 
				{101, "Lee", "010-4332-1541"}};
	FILE *fp = fopen("client.txt", "w+");
	fwrite(pInfo, sizeof(ClientInfo),2,fp);
	fclose(fp);
	return 0;
}

