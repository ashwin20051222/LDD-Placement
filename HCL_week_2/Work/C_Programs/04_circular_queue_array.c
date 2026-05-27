/*
PROGRAM: Implement Circular Queue Using Array (Ring Buffer)
QUESTION: Build a circular queue using a fixed-size array where elements 
          wrap around. Demonstrate enqueue, dequeue, and wrap-around behavior.
EXAMPLE:
    Queue size: 5
    Enqueue: 10, 20, 30, 40, 50
    Dequeue: 10, 20
    Enqueue: 60, 70
    Output: [30, 40, 50, 60, 70]
*/

#include <stdio.h>

#define QUEUE_SIZE 5

/* Circular Queue structure */
typedef struct
{
    int items[QUEUE_SIZE];
    int front;
    int rear;
    int count;
} CircularQueue;

/* Initialize queue */
void initQueue(CircularQueue *queue)
{
    queue->front = 0;
    queue->rear = -1;
    queue->count = 0;
}

/* Check if queue is empty */
int isEmpty(CircularQueue *queue)
{
    return queue->count == 0;
}

/* Check if queue is full */
int isFull(CircularQueue *queue)
{
    return queue->count == QUEUE_SIZE;
}

/* Enqueue element */
void enqueue(CircularQueue *queue, int value)
{
    if (isFull(queue))
    {
        printf("Queue is full! Cannot enqueue %d\n", value);
        return;
    }
    
    /* Move rear to next position (wrap around if needed) */
    queue->rear = (queue->rear + 1) % QUEUE_SIZE;
    queue->items[queue->rear] = value;
    queue->count++;
    
    printf("Enqueued: %d (Rear at index %d)\n", value, queue->rear);
}

/* Dequeue element */
int dequeue(CircularQueue *queue)
{
    if (isEmpty(queue))
    {
        printf("Queue is empty! Cannot dequeue\n");
        return -1;
    }
    
    int value = queue->items[queue->front];
    printf("Dequeued: %d (Front was at index %d)\n", value, queue->front);
    
    /* Move front to next position (wrap around if needed) */
    queue->front = (queue->front + 1) % QUEUE_SIZE;
    queue->count--;
    
    return value;
}

/* Display queue contents */
void displayQueue(CircularQueue *queue)
{
    int i;
    int index;
    
    if (isEmpty(queue))
    {
        printf("Queue is empty\n");
        return;
    }
    
    printf("Queue contents: [");
    for (i = 0; i < queue->count; i++)
    {
        index = (queue->front + i) % QUEUE_SIZE;
        printf("%d", queue->items[index]);
        if (i < queue->count - 1)
            printf(", ");
    }
    printf("]\n");
}

int main()
{
    CircularQueue queue;
    int choice;
    int value;
    
    initQueue(&queue);
    
    printf("Circular Queue - Runtime Input\n");
    printf("==============================\n\n");
    printf("Queue size: %d elements maximum\n\n", QUEUE_SIZE);
    
    while (1)
    {
        printf("\n--- Menu ---\n");
        printf("1. Enqueue (add element)\n");
        printf("2. Dequeue (remove element)\n");
        printf("3. Display queue\n");
        printf("4. Exit\n");
        printf("Choose option: ");
        scanf("%d", &choice);
        
        switch (choice)
        {
            case 1:
                printf("Enter value to enqueue: ");
                scanf("%d", &value);
                enqueue(&queue, value);
                printf("\n");
                displayQueue(&queue);
                break;
                
            case 2:
                dequeue(&queue);
                printf("\n");
                displayQueue(&queue);
                break;
                
            case 3:
                printf("\n");
                displayQueue(&queue);
                break;
                
            case 4:
                printf("Exiting...\n");
                return 0;
                
            default:
                printf("Invalid option!\n");
        }
    }
    
    return 0;
}

/*
OUTPUT:
Circular Queue Demonstration (Size: 5)
========================================

--- Enqueuing 5 elements ---
Enqueued: 10 (Rear at index 0)
Enqueued: 20 (Rear at index 1)
Enqueued: 30 (Rear at index 2)
Enqueued: 40 (Rear at index 3)
Enqueued: 50 (Rear at index 4)

Queue contents: [10, 20, 30, 40, 50]

--- Dequeuing 2 elements ---
Dequeued: 10 (Front was at index 0)
Dequeued: 20 (Front was at index 1)

Queue contents: [30, 40, 50]

--- Enqueuing 2 more elements (Wrap-around) ---
Enqueued: 60 (Rear at index 0)
Enqueued: 70 (Rear at index 1)

Queue contents: [30, 40, 50, 60, 70]

--- Queue status ---
Queue is FULL
*/
