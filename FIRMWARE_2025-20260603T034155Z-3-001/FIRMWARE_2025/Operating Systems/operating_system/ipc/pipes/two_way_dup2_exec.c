/*
   two_way_dup2_exec.c
   Demonstrates two-way communication using
   pipe(), dup2(), and exec()
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    int p2c[2], c2p[2];
    pid_t pid;
    char buffer[100];

    // Create pipes
    if (pipe(p2c) == -1 || pipe(c2p) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    else if (pid == 0) {  // Child

        // Redirect stdin and stdout
        dup2(p2c[0], STDIN_FILENO);
        dup2(c2p[1], STDOUT_FILENO);

        // Close unused descriptors
        close(p2c[1]);
        close(c2p[0]);
        close(p2c[0]);
        close(c2p[1]);

        // Replace child with "tr" command
        execl("/usr/bin/tr", "tr", "a-z", "A-Z", (char *)NULL);

        perror("execl");
        exit(EXIT_FAILURE);
    }

    else {  // Parent

        close(p2c[0]);  // Close unused read end
        close(c2p[1]);  // Close unused write end

        char msg[] = "hello posix pipe\n";

        // Send data to child
        write(p2c[1], msg, strlen(msg));
        close(p2c[1]);   // Important: send EOF

        // Read processed output
        read(c2p[0], buffer, sizeof(buffer));
        printf("Parent received: %s", buffer);

        close(c2p[0]);
        wait(NULL);
    }

    return 0;
}

