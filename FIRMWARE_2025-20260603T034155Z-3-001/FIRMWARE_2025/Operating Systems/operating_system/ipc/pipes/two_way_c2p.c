/*
   two_way_pipe.c
   Demonstrates Full Duplex Communication using POSIX Pipes
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    int p2c[2];   // Parent to Child
    int c2p[2];   // Child to Parent
    pid_t pid;

    char parent_msg[] = "Hello Child";
    char child_msg[]  = "Hello Parent";

    char buffer[100];

    // Create both pipes
    if (pipe(p2c) == -1 || pipe(c2p) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    else if (pid == 0) {  // Child Process

        close(p2c[1]);  // Close unused write end of p2c
        close(c2p[0]);  // Close unused read end of c2p

        // Read from parent
        read(p2c[0], buffer, sizeof(buffer));
        printf("Child received: %s\n", buffer);

        // Send reply to parent
        write(c2p[1], child_msg, strlen(child_msg) + 1);

        close(p2c[0]);
        close(c2p[1]);
        exit(EXIT_SUCCESS);
    }

    else {  // Parent Process

        close(p2c[0]);  // Close unused read end of p2c
        close(c2p[1]);  // Close unused write end of c2p

        // Send message to child
        write(p2c[1], parent_msg, strlen(parent_msg) + 1);

        // Read reply from child
        read(c2p[0], buffer, sizeof(buffer));
        printf("Parent received: %s\n", buffer);

        close(p2c[1]);
        close(c2p[0]);

        wait(NULL);
    }

    return 0;
}

