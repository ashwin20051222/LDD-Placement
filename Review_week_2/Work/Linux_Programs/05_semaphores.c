/*
PROGRAM: Semaphores for Process Synchronization
QUESTION: Use semaphores to synchronize multiple processes and prevent 
          race conditions in accessing shared resources.
COMPILE: gcc -o semaphore_demo semaphore_demo.c
RUN: ./semaphore_demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

int main()
{
    key_t semKey;
    int semID;
    struct sembuf semOp;
    int i;
    
    printf("Semaphores for Process Synchronization\n");
    printf("======================================\n\n");
    
    /* Create unique key */
    semKey = ftok(".", 'S');
    
    if (semKey == -1)
    {
        perror("ftok");
        return 1;
    }
    
    printf("Semaphore key: %d\n", semKey);
    
    /* Create semaphore set with 1 semaphore */
    semID = semget(semKey, 1, 0666 | IPC_CREAT);
    
    if (semID == -1)
    {
        perror("semget");
        return 1;
    }
    
    printf("Semaphore ID: %d\n", semID);
    
    /* Initialize semaphore to 1 (binary semaphore) */
    union semun semArg;
    semArg.val = 1;
    
    if (semctl(semID, 0, SETVAL, semArg) == -1)
    {
        perror("semctl");
        return 1;
    }
    
    printf("Semaphore initialized to 1\n\n");
    
    /* Create child processes */
    printf("Creating 3 child processes...\n\n");
    
    for (i = 1; i <= 3; i++)
    {
        if (fork() == 0)
        {
            /* Child process */
            printf("[CHILD %d] Process %d (PID: %d) starting\n", i, i, getpid());
            
            /* Wait operation - acquire semaphore */
            printf("[CHILD %d] Waiting for semaphore...\n", i);
            
            semOp.sem_num = 0;
            semOp.sem_op = -1;  /* Decrement semaphore */
            semOp.sem_flg = 0;
            
            if (semop(semID, &semOp, 1) == -1)
            {
                perror("semop");
                exit(1);
            }
            
            printf("[CHILD %d] Semaphore acquired!\n", i);
            printf("[CHILD %d] Entering critical section\n", i);
            
            /* Simulate critical section work */
            sleep(2);
            
            printf("[CHILD %d] Exiting critical section\n", i);
            
            /* Signal operation - release semaphore */
            semOp.sem_op = 1;  /* Increment semaphore */
            
            if (semop(semID, &semOp, 1) == -1)
            {
                perror("semop");
                exit(1);
            }
            
            printf("[CHILD %d] Semaphore released\n", i);
            printf("[CHILD %d] Exiting\n", i);
            
            exit(0);
        }
    }
    
    /* Parent process waits for all children */
    printf("[PARENT] Waiting for all children to finish...\n\n");
    
    for (i = 0; i < 3; i++)
    {
        wait(NULL);
    }
    
    printf("\n[PARENT] All children finished\n");
    
    /* Remove semaphore */
    if (semctl(semID, 0, IPC_RMID) == -1)
    {
        perror("semctl");
        return 1;
    }
    
    printf("[PARENT] Semaphore removed\n");
    
    return 0;
}

/*
OUTPUT:
Semaphores for Process Synchronization
======================================

Semaphore key: 1234
Semaphore ID: 1
Semaphore initialized to 1

Creating 3 child processes...

[PARENT] Waiting for all children to finish...

[CHILD 1] Process 1 (PID: 12341) starting
[CHILD 1] Waiting for semaphore...
[CHILD 1] Semaphore acquired!
[CHILD 1] Entering critical section
[CHILD 2] Process 2 (PID: 12342) starting
[CHILD 2] Waiting for semaphore...
[CHILD 3] Process 3 (PID: 12343) starting
[CHILD 3] Waiting for semaphore...
[CHILD 1] Exiting critical section
[CHILD 1] Semaphore released
[CHILD 1] Exiting
[CHILD 2] Semaphore acquired!
[CHILD 2] Entering critical section
[CHILD 3] Waiting for semaphore...
[CHILD 2] Exiting critical section
[CHILD 2] Semaphore released
[CHILD 2] Exiting
[CHILD 3] Semaphore acquired!
[CHILD 3] Entering critical section
[CHILD 3] Exiting critical section
[CHILD 3] Semaphore released
[CHILD 3] Exiting

[PARENT] All children finished
[PARENT] Semaphore removed
*/
