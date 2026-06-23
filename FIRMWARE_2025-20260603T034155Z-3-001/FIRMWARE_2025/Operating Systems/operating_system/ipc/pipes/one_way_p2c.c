/*
   pipe_example.c
   Demonstrates POSIX Pipe (Parent → Child)
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    int fd[2];
    pid_t pid;
    char write_msg[] = "Hello from Parent";
    char read_msg[100];

    // Create pipe
    if (pipe(fd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    else if (pid == 0) {  // Child
        close(fd[1]);  // Close unused write end

        read(fd[0], read_msg, sizeof(read_msg));
        printf("Child received: %s\n", read_msg);

        close(fd[0]);
        exit(EXIT_SUCCESS);
    }

    else {  // Parent
        close(fd[0]);  // Close unused read end

        write(fd[1], write_msg, strlen(write_msg) + 1);
        close(fd[1]);

        wait(NULL);  // Wait for child
    }

    return 0;
}

