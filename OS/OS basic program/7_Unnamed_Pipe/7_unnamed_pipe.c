#include "stdio.h"
#include "unistd.h"
#include "string.h"
#include "sys/types.h"
#include "sys/wait.h"

int main() {
	int pipefd[2];
	pid_t pid;
	
	if(pipe(pipefd) == -1) {
		perror("pipe failed");
		return 1;
	}
	
	pid = fork();
	
	if(pid < 0) {
		perror("fork failed");
		return 1;
	}
	else if(pid == 0) {
		// Child process
		close(pipefd[1]);
		
		char buffer[100];
		int n = read(pipefd[0], buffer, sizeof(buffer));
		
		if(n > 0) {
			buffer[n] = '\0';
			printf("Child received: %s\n", buffer);
		}
		
		close(pipefd[0]);
	}
	else {
		// Parent process
		close(pipefd[0]);
		
		char message[] = "Hello Child";
		write(pipefd[1], message, strlen(message));
		
		close(pipefd[1]);
		wait(NULL);
	}
	
	return 0;
}
