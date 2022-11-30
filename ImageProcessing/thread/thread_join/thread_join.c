#include <pthread.h>
#include <stdio.h>

void* thread_routine(void* arg)
{
	printf("[%s:%s] : %d\n", __FILE__, __FUNCTION__, __LINE__);
	return arg;
}


int main(int argc, char** argv)
{
	pthread_t thread_id;
	void *thread_result;
	int status;
	status = pthread_create(&thread_id, NULL, thread_routine, (void*)(23));
	if (status != 0) printf("Create thread : %x\n", status);
	status = pthread_join(thread_id, &thread_result);
	if(status != 0) printf("Thread Join : %x\n", status);
	return (thread_result == NULL)?0:1;
}
