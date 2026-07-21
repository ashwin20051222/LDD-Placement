#include "stdio.h"
#include "unistd.h"
#include "sys/types.h"

void terminate(int sig) {
	printf("Child Process : %d ,get terminate successfully.", getpid());
	exit(EXIT_SUCCESS);
}

int main() {
	pid_t pid;
	int status;
	
	// Creating child process
	pid = fork();
	
	// Condition for the child process fail
	if(pid < 0) {
		perror("Child Process failed.\n");
		exit(EXIT_FAILURE);
	}
	
	// Child Process
	if(pid == 0) {
		// Signal sent by the parent to terminate
		signal(, terminate);
		
		printf("\nPrint untill parent terminate .\n");
		while(1) {
			printf("Child Process PID: %d\n", getpid());
			sleep(1);
		}
	}
	else {
		printf("Parent process PID : %d\n", getpid());
		printf("Child Process PID : %d\n", pid);
		
		printf("\nChild Proces run for 5 second.\n");
		sleep(5);
		
		
	}
	
	
}
