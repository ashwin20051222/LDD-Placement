#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    int status;

    pid_t pid = fork();

    if(pid == 0)
    {
        printf("Child Running...\n");

        exit(5);
    }
    else
    {
        pid_t child = wait(&status);

        printf("Child PID = %d\n", child);

        printf("Raw Status = %d\n", status);

        printf("Decoded Exit Status = %d\n", WEXITSTATUS(status));
    }

    return 0;
}
