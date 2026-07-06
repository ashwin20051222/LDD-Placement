#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int main()
{
    pid_t child1, child2;
    pid_t grandchild1, grandchild2;

    printf("\nParent Process");
    printf("\nPID  = %d", getpid());
    printf("\nPPID = %d\n\n", getppid());

    // First Child
    child1 = fork();

    if (child1 == 0)
    {
        printf("Child 1");
        printf("\nPID  = %d", getpid());
        printf("\nPPID = %d\n\n", getppid());

        // Grandchild 1
        grandchild1 = fork();

        if (grandchild1 == 0)
        {
            printf("Grandchild 1 -> Child1's Child");
            printf("\nPID  = %d", getpid());
            printf("\nPPID = %d\n\n", getppid());
            exit(0);
        }

        wait(NULL);

        // Grandchild 2
        grandchild2 = fork();

        if (grandchild2 == 0)
        {
            printf("Grandchild 2 -> Child1's Child");
            printf("\nPID  = %d", getpid());
            printf("\nPPID = %d\n\n", getppid());
            exit(0);
        }

        wait(NULL);
        exit(0);
    }

    // Parent waits for Child 1
    wait(NULL);

    // Second Child
    child2 = fork();

    if (child2 == 0)
    {
        printf("Child 2");
        printf("\nPID  = %d", getpid());
        printf("\nPPID = %d\n\n", getppid());

        // Grandchild 3
        grandchild1 = fork();

        if (grandchild1 == 0)
        {
            printf("Grandchild 3 -> Child2's Child");
            printf("\nPID  = %d", getpid());
            printf("\nPPID = %d\n\n", getppid());
            exit(0);
        }

        wait(NULL);

        // Grandchild 4
        grandchild2 = fork();

        if (grandchild2 == 0)
        {
            printf("Grandchild 4 -> Child2's Child");
            printf("\nPID  = %d", getpid());
            printf("\nPPID = %d\n\n", getppid());
            exit(0);
        }

        wait(NULL);
        exit(0);
    }

    // Parent waits for Child 2
    wait(NULL);

    printf("Parent Process Finished\n");

    return 0;
}
