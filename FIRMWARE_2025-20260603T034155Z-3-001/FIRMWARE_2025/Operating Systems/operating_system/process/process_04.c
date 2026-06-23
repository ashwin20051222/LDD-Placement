#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
int main() {
    pid_t pid;

    pid = fork();  // Create a child process

    if (pid < 0) {
        perror("Fork failed");
        return 1;
    }
    else if (pid == 0) {
        // Child process
        printf("Child Process PID: %d | Running ls command\n", getpid());

        // Execute ls using execl
        execl("/bin/ls", "ls", "-l", NULL);

        // If execl returns, it means it failed
        perror("execl failed");
        return 1;
    }
    else {
        // Parent process
        printf("Parent Process PID: %d | Waiting for child to finish\n", getpid());
        wait(NULL);  // Wait for child to finish
        printf("Child finished executing ls\n");
    }

    return 0;
}

