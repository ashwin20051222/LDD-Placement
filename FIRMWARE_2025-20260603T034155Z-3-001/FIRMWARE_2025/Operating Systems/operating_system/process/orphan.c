//ps -elf | grep <child_pid>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    pid_t pid = fork();

    if (pid == 0) {
        /* Child */
        sleep(10);
        printf("Child running after parent exit\n");
        exit(0);
    } else {
        /* Parent exits immediately */
        printf("Parent exiting\n");
        exit(0);
    }
}

