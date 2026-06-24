#include "stdio.h"
#include "pthread.h"

void* thread_func(void* arg) {
	printf("Hello Thread\n");
	pthread_exit(NULL);
}

int main() {
	pthread_t thread_id;
	
	int ret = pthread_create(&thread_id, NULL, thread_func, NULL);
	
	if(ret != 0) {
		perror("pthread_create failed");
		return 1;
	}
	
	pthread_join(thread_id, NULL);
	
	printf("Main thread completed\n");
	return 0;
}
