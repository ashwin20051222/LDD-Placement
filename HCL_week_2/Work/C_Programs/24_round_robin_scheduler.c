/*
PROGRAM: Round Robin CPU Scheduler using Circular Queue
QUESTION: Simulate a round-robin CPU scheduler where each process 
          gets a time slice (quantum) before being placed back in queue.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PROCESSES 10
#define TIME_QUANTUM 2

/* Process structure */
typedef struct
{
    int processID;
    int burstTime;
    int totalTime;
} Process;

/* Circular queue for processes */
typedef struct
{
    Process processes[MAX_PROCESSES];
    int front;
    int rear;
    int count;
} ProcessQueue;

void initQueue(ProcessQueue *queue)
{
    queue->front = 0;
    queue->rear = -1;
    queue->count = 0;
}

int isEmpty(ProcessQueue *queue)
{
    return queue->count == 0;
}

int isFull(ProcessQueue *queue)
{
    return queue->count == MAX_PROCESSES;
}

void enqueue(ProcessQueue *queue, Process process)
{
    if (!isFull(queue))
    {
        queue->rear = (queue->rear + 1) % MAX_PROCESSES;
        queue->processes[queue->rear] = process;
        queue->count++;
    }
}

Process dequeue(ProcessQueue *queue)
{
    Process process = queue->processes[queue->front];
    queue->front = (queue->front + 1) % MAX_PROCESSES;
    queue->count--;
    return process;
}

void scheduleProcesses(ProcessQueue *queue)
{
    int totalTime = 0;
    int processesCompleted = 0;
    int timeSlice;
    
    printf("CPU Scheduler - Round Robin\n");
    printf("===========================\n\n");
    printf("Time Quantum: %d ms\n\n", TIME_QUANTUM);
    printf("Initial Queue:\n");
    
    int i;
    for (i = 0; i < queue->count; i++)
    {
        int idx = (queue->front + i) % MAX_PROCESSES;
        printf("  P%d (Burst: %d ms)\n", 
               queue->processes[idx].processID, 
               queue->processes[idx].burstTime);
    }
    
    printf("\n--- Execution Timeline ---\n\n");
    
    int contextSwitches = 0;
    
    while (!isEmpty(queue))
    {
        Process currentProcess = dequeue(queue);
        
        if (currentProcess.burstTime > TIME_QUANTUM)
        {
            timeSlice = TIME_QUANTUM;
            currentProcess.burstTime -= TIME_QUANTUM;
        }
        else
        {
            timeSlice = currentProcess.burstTime;
            currentProcess.burstTime = 0;
        }
        
        totalTime += timeSlice;
        printf("Time %3d-%3d: Process P%d executing (%d ms)\n", 
               totalTime - timeSlice, totalTime, 
               currentProcess.processID, timeSlice);
        
        if (currentProcess.burstTime > 0)
        {
            printf("            -> P%d remaining: %d ms (back to queue)\n", 
                   currentProcess.processID, currentProcess.burstTime);
            enqueue(queue, currentProcess);
            contextSwitches++;
        }
        else
        {
            printf("            -> P%d completed\n", currentProcess.processID);
            processesCompleted++;
        }
        
        printf("\n");
    }
    
    printf("--- Scheduling Summary ---\n");
    printf("Total execution time: %d ms\n", totalTime);
    printf("Processes completed: %d\n", processesCompleted);
    printf("Context switches: %d\n", contextSwitches);
}

int main()
{
    ProcessQueue queue;
    initQueue(&queue);
    
    printf("Round Robin Scheduler Simulation (Runtime Input)\n");
    printf("================================================\n\n");
    
    int numProcesses;
    int i;
    
    printf("Enter number of processes: ");
    scanf("%d", &numProcesses);
    
    printf("\nEnter burst time for each process:\n");
    for (i = 0; i < numProcesses; i++)
    {
        int burstTime;
        printf("Process P%d burst time (ms): ", i + 1);
        scanf("%d", &burstTime);
        
        Process p = {i + 1, burstTime, burstTime};
        enqueue(&queue, p);
    }
    
    printf("\nInitial Queue:\n");
    ProcessQueue tempQueue = queue;
    while (!isEmpty(&tempQueue))
    {
        Process p = dequeue(&tempQueue);
        printf("  P%d (Burst: %d ms)\n", p.processID, p.burstTime);
    }
    
    scheduleProcesses(&queue);
    
    return 0;
}

/*
OUTPUT:
Round Robin Scheduler Simulation
================================

CPU Scheduler - Round Robin
===========================

Time Quantum: 2 ms

Initial Queue:
  P1 (Burst: 5 ms)
  P2 (Burst: 8 ms)
  P3 (Burst: 3 ms)
  P4 (Burst: 6 ms)

--- Execution Timeline ---

Time   0-  2: Process P1 executing (2 ms)
            -> P1 remaining: 3 ms (back to queue)

Time   2-  4: Process P2 executing (2 ms)
            -> P2 remaining: 6 ms (back to queue)

Time   4-  6: Process P3 executing (2 ms)
            -> P3 remaining: 1 ms (back to queue)

Time   6-  8: Process P4 executing (2 ms)
            -> P4 remaining: 4 ms (back to queue)
*/
