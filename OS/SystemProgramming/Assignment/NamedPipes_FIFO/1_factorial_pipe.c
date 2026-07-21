#include "stdio.h"
#include "unistd.h"
#include "stdlib.h"
#include "sys/types.h"
#include "sys/wait.h"

int main() {
	int p2c[2], c2p[2];
	pid_t pid;
	
	int num;
	long long fact = 1;
	
	// Creating two pipes for full duplex communication
	if(pipe(p2c) == -1 || pipe(c2p) == -1) {
		perror("Pipe creation failed.\n");
		exit(EXIT_FAILURE);
	}
	
	pid = fork();

	// If the process creation failed
    	if (pid < 0)
    	{
        	perror("Fork");
        	exit(EXIT_FAILURE);
    	}
    	
    	// Child Process
    	else if (pid == 0) {
    		close(p2c[1]); // Close the unused write end
    		close(c2p[0]); // Close the unused read end
    	}
    	
    	// Parent Process 
    	else {
    		
    	}
    	
    	return 0;
}

