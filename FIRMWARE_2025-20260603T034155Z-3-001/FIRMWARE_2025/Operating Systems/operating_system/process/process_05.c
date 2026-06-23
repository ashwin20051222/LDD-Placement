#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    pid_t pid = fork();  // Create child process

    if (pid < 0) {
        perror("Fork failed");
        return 1;
    }
    else if (pid == 0) {
        // Child process
        printf("Child process PID: %d is exiting\n", getpid());
        exit(0);  // Child terminates immediately
    }
    else {
        // Parent process
        printf("Parent process PID: %d\n", getpid());
        printf("Child PID: %d has terminated but not yet waited on\n", pid);

        printf("Check process table using 'ps -l' or 'top' to see zombie\n");
        sleep(30);  // Wait so that you can observe the zombie
        printf("Parent now exiting, child will be cleaned up\n");
    }

    return 0;
}

