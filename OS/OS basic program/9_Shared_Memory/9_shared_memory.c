#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "sys/ipc.h"
#include "sys/shm.h"

#define SHM_SIZE 1024

int main() {
	key_t key = ftok("/tmp", 'R');
	
	if(key == -1) {
		perror("ftok failed");
		return 1;
	}
	
	int shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666);
	
	if(shmid == -1) {
		perror("shmget failed");
		return 1;
	}
	
	char* shared_memory = (char*)shmat(shmid, NULL, 0);
	
	if(shared_memory == (char*)-1) {
		perror("shmat failed");
		return 1;
	}
	
	// Write to shared memory
	strcpy(shared_memory, "Hello Shared Memory");
	
	printf("Message stored in shared memory: %s\n", shared_memory);
	printf("Shared memory segment ID: %d\n", shmid);
	
	// Detach from shared memory
	shmdt(shared_memory);
	
	// Mark for deletion
	shmctl(shmid, IPC_RMID, NULL);
	
	return 0;
}
