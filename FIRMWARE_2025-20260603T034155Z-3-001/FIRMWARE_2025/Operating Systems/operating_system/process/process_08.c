/* send value from child to parent */
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int main() {
    int a = 10, b = 20;
    pid_t pid;
    int status;

    pid = fork();   // Create child process

    if (pid < 0) {
        perror("Fork failed");
        return 1;
    }
    else if (pid == 0) {
        // 🔹 Child Process
        int sum = a + b;

        printf("Child Process:\n");
        printf("Child PID  : %d\n", getpid());
        printf("Parent PID : %d\n", getppid());
        printf("Addition   : %d + %d = %d\n", a, b, sum);

        exit(sum);   // send sum to parent (max 255)
    }
    else {
        // 🔹 Parent Process
        wait(&status);   // wait for child

        if (WIFEXITED(status)) {
            int received_sum = WEXITSTATUS(status);

            printf("Parent Process:\n");
            printf("Parent PID : %d\n", getpid());
            printf("Child PID  : %d\n", pid);
            printf("Received Sum from Child = %d\n", received_sum);
        }
    }

    return 0;
}

