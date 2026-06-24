#include "stdio.h"
#include "pthread.h"
#include "semaphore.h"

sem_t semaphore;

void* thread_func(void* arg) {
	int thread_num = *(int*)arg;
	
	sem_wait(&semaphore);
	printf("Thread %d: Accessing shared resource\n", thread_num);
	sem_post(&semaphore);
	
	pthread_exit(NULL);
}

int main() {
	pthread_t thread1, thread2;
	int id1 = 1, id2 = 2;
	
	sem_init(&semaphore, 0, 1);
	
	pthread_create(&thread1, NULL, thread_func, (void*)&id1);
	pthread_create(&thread2, NULL, thread_func, (void*)&id2);
	
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	
	sem_destroy(&semaphore);
	printf("Both threads completed\n");
	return 0;
}
