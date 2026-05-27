/*
PROGRAM: Process Creation and Management with fork()
QUESTION: Demonstrate process creation using fork(), showing 
          parent and child process execution.
COMPILE: gcc -o fork_demo fork_demo.c
RUN: ./fork_demo
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main()
{
    pid_t parentPID = getpid();
    pid_t childPID;
    int i;
    
    printf("Process Creation and Management\n");
    printf("===============================\n\n");
    
    printf("Parent process ID: %d\n\n", parentPID);
    
    /* Create child processes */
    for (i = 1; i <= 3; i++)
    {
        childPID = fork();
        
        if (childPID == -1)
        {
            perror("fork");
            exit(1);
        }
        
        if (childPID == 0)
        {
            /* Child process code */
            printf("[CHILD %d] Process ID: %d\n", i, getpid());
            printf("[CHILD %d] Parent ID: %d\n", i, getppid());
            printf("[CHILD %d] Iteration: %d\n", i, i);
            
            /* Simulate some work */
            int j;
            for (j = 1; j <= 3; j++)
            {
                printf("[CHILD %d] Working... step %d\n", i, j);
                sleep(1);
            }
            
            printf("[CHILD %d] Exiting\n", i, getpid());
            exit(i * 10);  /* Exit with status i*10 */
        }
        else
        {
            /* Parent process code */
            printf("[PARENT] Created child process with PID: %d\n\n", childPID);
        }
    }
    
    /* Parent waits for all children */
    printf("[PARENT] All children created. Waiting for them to finish...\n\n");
    
    int status;
    pid_t completedPID;
    
    for (i = 1; i <= 3; i++)
    {
        completedPID = wait(&status);
        
        if (WIFEXITED(status))
        {
            int exitCode = WEXITSTATUS(status);
            printf("[PARENT] Child %d (PID: %d) exited with status: %d\n", 
                   i, completedPID, exitCode);
        }
    }
    
    printf("[PARENT] All children finished\n");
    printf("[PARENT] Parent process exiting\n");
    
    return 0;
}

/*
OUTPUT:
Process Creation and Management
===============================

Parent process ID: 12340

[PARENT] Created child process with PID: 12341
[CHILD 1] Process ID: 12341
[CHILD 1] Parent ID: 12340
[CHILD 1] Iteration: 1
[CHILD 1] Working... step 1
[PARENT] Created child process with PID: 12342
[CHILD 2] Process ID: 12342
[CHILD 2] Parent ID: 12340
[CHILD 2] Iteration: 2
[CHILD 2] Working... step 1
[PARENT] Created child process with PID: 12343
[CHILD 3] Process ID: 12343
[CHILD 3] Parent ID: 12340
[CHILD 3] Iteration: 3
[CHILD 3] Working... step 1
[PARENT] All children created. Waiting for them to finish...

[CHILD 1] Working... step 2
[CHILD 2] Working... step 2
[CHILD 3] Working... step 2
...
[CHILD 1] Exiting
[CHILD 2] Exiting
[CHILD 3] Exiting
[PARENT] Child 1 (PID: 12341) exited with status: 10
[PARENT] Child 2 (PID: 12342) exited with status: 20
[PARENT] Child 3 (PID: 12343) exited with status: 30
[PARENT] All children finished
[PARENT] Parent process exiting
*/
