#include "stdio.h"
#include "pthread.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* thread_func(void* arg) {
	int thread_num = *(int*)arg;
	
	pthread_mutex_lock(&mutex);
	printf("Thread %d: Mutex locked and printing message\n", thread_num);
	pthread_mutex_unlock(&mutex);
	
	pthread_exit(NULL);
}

int main() {
	pthread_t thread1, thread2;
	int id1 = 1, id2 = 2;
	
	pthread_create(&thread1, NULL, thread_func, (void*)&id1);
	pthread_create(&thread2, NULL, thread_func, (void*)&id2);
	
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	
	printf("Both threads completed\n");
	pthread_mutex_destroy(&mutex);
	return 0;
}
