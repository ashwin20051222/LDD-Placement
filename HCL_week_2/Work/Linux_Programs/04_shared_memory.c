/*
PROGRAM: Shared Memory - Inter-Process Communication
QUESTION: Create and use shared memory between processes. 
          Demonstrate read/write operations on shared memory.
COMPILE: gcc -o shared_memory shared_memory.c
RUN: ./shared_memory
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>

struct SharedData
{
    int counter;
    char message[100];
};

int main()
{
    key_t sharedKey;
    int shmID;
    struct SharedData *sharedMemory;
    pid_t childPID;
    
    printf("Shared Memory - Inter-Process Communication\n");
    printf("==========================================\n\n");
    
    /* Create unique key for shared memory */
    sharedKey = ftok(".", 'R');
    
    if (sharedKey == -1)
    {
        perror("ftok");
        return 1;
    }
    
    printf("Shared memory key: %d\n", sharedKey);
    
    /* Create shared memory segment */
    shmID = shmget(sharedKey, sizeof(struct SharedData), 0666 | IPC_CREAT);
    
    if (shmID == -1)
    {
        perror("shmget");
        return 1;
    }
    
    printf("Shared memory ID: %d\n", shmID);
    printf("Shared memory size: %lu bytes\n\n", sizeof(struct SharedData));
    
    /* Attach shared memory */
    sharedMemory = (struct SharedData *)shmat(shmID, NULL, 0);
    
    if (sharedMemory == (void *)-1)
    {
        perror("shmat");
        return 1;
    }
    
    printf("Shared memory attached at address: %p\n\n", sharedMemory);
    
    /* Initialize shared memory */
    sharedMemory->counter = 0;
    strcpy(sharedMemory->message, "Shared Memory Demo");
    
    printf("Initial values:\n");
    printf("  Counter: %d\n", sharedMemory->counter);
    printf("  Message: %s\n\n", sharedMemory->message);
    
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
        printf("[CHILD] Child process (PID: %d) accessing shared memory\n", getpid());
        
        sleep(1);  /* Let parent write first */
        
        printf("[CHILD] Current counter: %d\n", sharedMemory->counter);
        printf("[CHILD] Current message: %s\n", sharedMemory->message);
        
        /* Modify shared memory */
        sharedMemory->counter += 5;
        strcpy(sharedMemory->message, "Modified by child");
        
        printf("[CHILD] Updated counter to: %d\n", sharedMemory->counter);
        printf("[CHILD] Updated message to: %s\n", sharedMemory->message);
        
        /* Detach shared memory */
        if (shmdt(sharedMemory) == -1)
        {
            perror("shmdt");
            return 1;
        }
        
        printf("[CHILD] Detached shared memory\n");
        exit(0);
    }
    else
    {
        /* Parent process */
        printf("[PARENT] Parent process (PID: %d) writing to shared memory\n", 
               getpid());
        
        sharedMemory->counter = 10;
        strcpy(sharedMemory->message, "Hello from Parent");
        
        printf("[PARENT] Wrote counter: %d\n", sharedMemory->counter);
        printf("[PARENT] Wrote message: %s\n", sharedMemory->message);
        
        /* Wait for child */
        wait(NULL);
        
        printf("\n[PARENT] Child process finished\n");
        printf("[PARENT] Final counter: %d\n", sharedMemory->counter);
        printf("[PARENT] Final message: %s\n", sharedMemory->message);
        
        /* Detach shared memory */
        if (shmdt(sharedMemory) == -1)
        {
            perror("shmdt");
            return 1;
        }
        
        /* Delete shared memory */
        if (shmctl(shmID, IPC_RMID, NULL) == -1)
        {
            perror("shmctl");
            return 1;
        }
        
        printf("\n[PARENT] Shared memory detached and removed\n");
    }
    
    return 0;
}

/*
OUTPUT:
Shared Memory - Inter-Process Communication
==========================================

Shared memory key: 1234
Shared memory ID: 0
Shared memory size: 104 bytes

Shared memory attached at address: 0x7f1234567000

Initial values:
  Counter: 0
  Message: Shared Memory Demo

[PARENT] Parent process (PID: 12340) writing to shared memory
[PARENT] Wrote counter: 10
[PARENT] Wrote message: Hello from Parent
[CHILD] Child process (PID: 12341) accessing shared memory
[CHILD] Current counter: 10
[CHILD] Current message: Hello from Parent
[CHILD] Updated counter to: 15
[CHILD] Updated message to: Modified by child
[CHILD] Detached shared memory

[PARENT] Child process finished
[PARENT] Final counter: 15
[PARENT] Final message: Modified by child

[PARENT] Shared memory detached and removed
*/
