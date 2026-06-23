/*
   simple_write_pipe.c
   Parent writes to pipe
   Child reads from pipe
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    int fd[2]; // file desciptor
    pid_t pid; // process id
    char buffer[100]; // values
//	int pipe_ret = pipe(fd);
    // Create pipe
    if (pipe(fd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
	//create a child process
    pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    else if (pid == 0) {  // Child process

        close(fd[1]);  // Close write end

        read(fd[0], buffer, sizeof(buffer));
        printf("Child received: %s\n", buffer);

        close(fd[0]);
        exit(EXIT_SUCCESS);
    }

    else {  // Parent process

        close(fd[0]);  // Close read end

        char message[] = "Hello from Parent";
        write(fd[1], message, strlen(message) + 1);

        close(fd[1]);  // Important: send EOF
        wait(NULL);
    }

    return 0;
}

