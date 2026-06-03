/*
PROGRAM: Thread Creation and Management with pthreads
QUESTION: Create multiple threads, demonstrate thread creation, 
          execution, and joining.
COMPILE: gcc -o thread_demo thread_demo.c -lpthread
RUN: ./thread_demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

/* Structure to pass data to threads */
typedef struct
{
    int threadID;
    int iterations;
} ThreadArgs;

/* Thread function */
void *threadFunction(void *arg)
{
    ThreadArgs *args = (ThreadArgs *)arg;
    int threadID = args->threadID;
    int iterations = args->iterations;
    int i;
    
    printf("[THREAD %d] Thread started (Thread ID: %lu, PID: %d)\n", 
           threadID, pthread_self(), getpid());
    
    for (i = 1; i <= iterations; i++)
    {
        printf("[THREAD %d] Iteration %d of %d\n", threadID, i, iterations);
        sleep(1);
    }
    
    printf("[THREAD %d] Thread finished\n", threadID);
    
    /* Return thread result */
    int *result = (int *)malloc(sizeof(int));
    *result = threadID * 100;
    
    pthread_exit(result);
}

int main()
{
    pthread_t threads[3];
    ThreadArgs args[3];
    int i;
    int rc;
    
    printf("Thread Creation and Management with pthreads\n");
    printf("===========================================\n\n");
    
    printf("Main process ID: %d\n", getpid());
    printf("Main thread ID: %lu\n\n", pthread_self());
    
    /* Create threads */
    printf("--- Creating Threads ---\n");
    
    for (i = 0; i < 3; i++)
    {
        args[i].threadID = i + 1;
        args[i].iterations = 3;
        
        printf("Creating thread %d...\n", i + 1);
        
        rc = pthread_create(&threads[i], NULL, threadFunction, &args[i]);
        
        if (rc != 0)
        {
            printf("ERROR: Failed to create thread %d (code: %d)\n", i + 1, rc);
            return 1;
        }
        
        printf("Thread %d created successfully\n", i + 1);
    }
    
    printf("\n--- Waiting for Threads to Complete ---\n\n");
    
    /* Join threads */
    for (i = 0; i < 3; i++)
    {
        int *result;
        
        printf("Joining thread %d...\n", i + 1);
        
        rc = pthread_join(threads[i], (void **)&result);
        
        if (rc != 0)
        {
            printf("ERROR: Failed to join thread %d (code: %d)\n", i + 1, rc);
            return 1;
        }
        
        printf("Thread %d joined successfully\n", i + 1);
        printf("Thread %d returned: %d\n\n", i + 1, *result);
        
        free(result);
    }
    
    printf("--- All Threads Completed ---\n");
    printf("Main thread exiting\n");
    
    return 0;
}

/*
OUTPUT:
Thread Creation and Management with pthreads
===========================================

Main process ID: 12345
Main thread ID: 140123456789120

--- Creating Threads ---
Creating thread 1...
Thread 1 created successfully
Creating thread 2...
Thread 2 created successfully
Creating thread 3...
Thread 3 created successfully

--- Waiting for Threads to Complete ---

Joining thread 1...
[THREAD 1] Thread started (Thread ID: 140123456789121, PID: 12345)
[THREAD 1] Iteration 1 of 3
[THREAD 2] Thread started (Thread ID: 140123456789122, PID: 12345)
[THREAD 2] Iteration 1 of 3
[THREAD 3] Thread started (Thread ID: 140123456789123, PID: 12345)
[THREAD 3] Iteration 1 of 3
[THREAD 1] Iteration 2 of 3
[THREAD 2] Iteration 2 of 3
[THREAD 3] Iteration 2 of 3
[THREAD 1] Iteration 3 of 3
[THREAD 2] Iteration 3 of 3
[THREAD 3] Iteration 3 of 3
[THREAD 1] Thread finished
Thread 1 joined successfully
Thread 1 returned: 100
Joining thread 2...
[THREAD 2] Thread finished
Thread 2 joined successfully
Thread 2 returned: 200
Joining thread 3...
[THREAD 3] Thread finished
Thread 3 joined successfully
Thread 3 returned: 300

--- All Threads Completed ---
Main thread exiting
*/
