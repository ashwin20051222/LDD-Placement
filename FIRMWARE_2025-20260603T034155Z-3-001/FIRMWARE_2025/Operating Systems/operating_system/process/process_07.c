#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main() {
    pid_t pid;
    int status;

    printf("System Manager: Starting system health check...\n");

    /* Create a child process */
    pid = fork(); // parent will get child process id, child will have 0

    if (pid < 0) {
        /* Fork failed */
        perror("fork failed");
        exit(1);
    }

    else if (pid == 0) {
        /* Child Process */
        printf("Diagnostic Tool (Child): Running diagnostics...\n");

        /* Replace child with external program */
        execl("/bin/ls", "ls", "-l", "/", NULL);

        /* If exec fails */
        perror("exec failed");
        exit(2);
    }

    else {
        /* Parent Process */
        printf("System Manager (Parent): Waiting for diagnostic tool...\n");

        wait(&status);

        if (WIFEXITED(status)) {
            printf("System Manager: Diagnostic completed successfully\n");
            printf("Exit Status: %d\n", WEXITSTATUS(status));
        } else {
            printf("System Manager: Diagnostic terminated abnormally\n");
        }
    }

    printf("System Manager: Health check finished.\n");
    return 0;
}

