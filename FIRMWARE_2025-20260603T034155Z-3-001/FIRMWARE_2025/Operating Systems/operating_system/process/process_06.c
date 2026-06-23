#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid;
    int status;

    pid = fork();  // Create child process

    if (pid < 0) {
        perror("Fork failed");
        return 1;
    }
    else if (pid == 0) {
        // Child process
        int result = 5 + 10;  // Child does some work
        printf("Child PID: %d, calculated result: %d\n", getpid(), result);

        // Exit with result as status (limited to 0-255)
        exit(result);
    }
    else {
        // Parent process
        printf("Parent PID: %d, waiting for child PID: %d\n", getpid(), pid);

        wait(&status);  // Wait for child to finish

        if (WIFEXITED(status)) {
            int child_result = WEXITSTATUS(status);  // Get child exit code
            printf("Child PID: %d finished, result received by parent: %d\n", pid, child_result);
        }
        else {
            printf("Child did not terminate normally.\n");
        }
    }

    return 0;
}

