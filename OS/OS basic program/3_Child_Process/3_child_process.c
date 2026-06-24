#include "stdio.h"
#include "unistd.h"
#include "sys/types.h"

int main() {
	pid_t pid = fork();
	
	if (pid < 0) {
	        perror("Fork failed");
        	return 1;
    	} 
    	else if (pid == 0) {
        	printf("Hello from Child Process, My PID is: %d\n", getpid());
    	} 
    	else {
        	printf("Hello from Parent Process, My PID is: %d\n", getpid());
    	}

    return 0;

}
