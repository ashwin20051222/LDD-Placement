/*
PROGRAM: Working with Pipes in Linux - Inter-Process Communication
QUESTION: Create a pipe between parent and child process. 
          Demonstrate reading and writing data through the pipe.
COMPILE: gcc -o pipe_demo pipe_demo.c
RUN: ./pipe_demo
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

int main()
{
    int pipefd[2];  /* File descriptors for pipe */
    pid_t childPID;
    char buffer[100];
    const char *message = "Hello from Parent Process!";
    
    printf("Pipe Demonstration - IPC (Inter-Process Communication)\n");
    printf("=====================================================\n\n");
    
    /* Create pipe */
    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        return 1;
    }
    
    printf("Pipe created successfully\n");
    printf("  Read end (descriptor): %d\n", pipefd[0]);
    printf("  Write end (descriptor): %d\n\n", pipefd[1]);
    
    /* Fork child process */
    childPID = fork();
    
    if (childPID == -1)
    {
        perror("fork");
        return 1;
    }
    
    if (childPID == 0)
    {
        /* Child process */
        printf("[CHILD] Child process created (PID: %d)\n", getpid());
        
        /* Close write end in child */
        close(pipefd[1]);
        
        /* Read from pipe */
        printf("[CHILD] Reading from pipe...\n");
        ssize_t bytesRead = read(pipefd[0], buffer, sizeof(buffer) - 1);
        
        if (bytesRead > 0)
        {
            buffer[bytesRead] = '\0';
            printf("[CHILD] Received message (%ld bytes): %s\n", bytesRead, buffer);
        }
        else
        {
            printf("[CHILD] No data received\n");
        }
        
        /* Close read end */
        close(pipefd[0]);
        
        printf("[CHILD] Child process exiting\n");
        exit(0);
    }
    else
    {
        /* Parent process */
        printf("[PARENT] Parent process created (PID: %d)\n", getpid());
        printf("[PARENT] Child process ID: %d\n\n", childPID);
        
        /* Close read end in parent */
        close(pipefd[0]);
        
        /* Write to pipe */
        printf("[PARENT] Writing message to pipe...\n");
        ssize_t bytesWritten = write(pipefd[1], message, strlen(message));
        printf("[PARENT] Wrote %ld bytes to pipe\n", bytesWritten);
        
        /* Close write end */
        close(pipefd[1]);
        
        /* Wait for child to finish */
        printf("[PARENT] Waiting for child process to complete...\n");
        wait(NULL);
        
        printf("[PARENT] Child process finished\n");
        printf("[PARENT] Parent process exiting\n");
    }
    
    return 0;
}

/*
OUTPUT:
Pipe Demonstration - IPC (Inter-Process Communication)
=====================================================

Pipe created successfully
  Read end (descriptor): 3
  Write end (descriptor): 4

[PARENT] Parent process created (PID: 12345)
[PARENT] Child process ID: 12346

[CHILD] Child process created (PID: 12346)
[CHILD] Reading from pipe...
[PARENT] Writing message to pipe...
[PARENT] Wrote 26 bytes to pipe
[PARENT] Waiting for child process to complete...
[CHILD] Received message (26 bytes): Hello from Parent Process!
[CHILD] Child process exiting
[PARENT] Child process finished
[PARENT] Parent process exiting
*/
