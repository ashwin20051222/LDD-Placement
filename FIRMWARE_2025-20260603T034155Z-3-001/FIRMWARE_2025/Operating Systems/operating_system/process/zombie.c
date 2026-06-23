//ps -elf | grep Z

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    pid_t pid = fork();

    if (pid == 0) {
        /* Child */
        printf("Child exiting...\n");
        exit(5);
    } else {
        /* Parent does NOT call wait() */
        printf("Parent sleeping... Child becomes zombie\n");
        sleep(30);
    }
    return 0;
}

