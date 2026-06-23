#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    int a = 10, b = 20;
    pid_t pid;

    pid = fork();   // Create child process

    if (pid < 0) {
        // Fork failed
        perror("Fork failed");
        return 1;
    }
    else if (pid == 0) {
        // Child process
        int sum = a + b;
        printf("Child Process:\n");
        printf("  Child PID  : %d\n", getpid());
        printf("  Parent PID : %d\n", getppid());
        printf("  Addition   : %d + %d = %d\n", a, b, sum);
    }
    else {
        // Parent process
        printf("Parent Process:\n");
        printf("  Parent PID : %d\n", getpid());
        printf("  Child PID  : %d\n", pid);
    }

    return 0;
}

