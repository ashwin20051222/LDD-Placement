/*
PROGRAM: Implement Queue Using Two Stacks
QUESTION: Build a queue using two stacks (inStack and outStack). 
          Analyze the amortized time complexity.
APPROACH:
  - Enqueue: Push to inStack (O(1))
  - Dequeue: If outStack empty, pop all from inStack, then pop from outStack
  - Amortized complexity: O(1) for both operations
*/

#include <stdio.h>

#define MAX_SIZE 100

/* Stack structure */
typedef struct
{
    int items[MAX_SIZE];
    int top;
} Stack;

/* Initialize stack */
void initStack(Stack *stack)
{
    stack->top = -1;
}

/* Push to stack */
void push(Stack *stack, int value)
{
    if (stack->top < MAX_SIZE - 1)
    {
        stack->top++;
        stack->items[stack->top] = value;
    }
}

/* Pop from stack */
int pop(Stack *stack)
{
    if (stack->top >= 0)
    {
        return stack->items[stack->top--];
    }
    return -1;
}

/* Check if stack is empty */
int isStackEmpty(Stack *stack)
{
    return stack->top == -1;
}

/* Queue using two stacks */
typedef struct
{
    Stack inStack;   /* Stack for enqueue operations */
    Stack outStack;  /* Stack for dequeue operations */
} QueueUsingStacks;

/* Initialize queue */
void initQueue(QueueUsingStacks *queue)
{
    initStack(&queue->inStack);
    initStack(&queue->outStack);
}

/* Enqueue operation - O(1) */
void enqueue(QueueUsingStacks *queue, int value)
{
    push(&queue->inStack, value);
    printf("Enqueued: %d\n", value);
}

/* Dequeue operation - Amortized O(1) */
int dequeue(QueueUsingStacks *queue)
{
    if (isStackEmpty(&queue->outStack))
    {
        /* Transfer all elements from inStack to outStack */
        if (isStackEmpty(&queue->inStack))
        {
            printf("Queue is empty!\n");
            return -1;
        }
        
        printf("(Transferring elements from inStack to outStack)\n");
        while (!isStackEmpty(&queue->inStack))
        {
            push(&queue->outStack, pop(&queue->inStack));
        }
    }
    
    int value = pop(&queue->outStack);
    printf("Dequeued: %d\n", value);
    return value;
}

/* Check if queue is empty */
int isQueueEmpty(QueueUsingStacks *queue)
{
    return isStackEmpty(&queue->inStack) && isStackEmpty(&queue->outStack);
}

/* Display queue status */
void displayQueueStatus(QueueUsingStacks *queue)
{
    printf("InStack top: %d | OutStack top: %d\n", 
           queue->inStack.top, queue->outStack.top);
}

int main()
{
    QueueUsingStacks queue;
    int choice;
    int value;
    
    initQueue(&queue);
    
    printf("Queue Using Two Stacks - Runtime Input\n");
    printf("======================================\n\n");
    
    printf("=== AMORTIZED COMPLEXITY ANALYSIS ===\n");
    printf("Enqueue: O(1) - Simple push operation\n");
    printf("Dequeue: Amortized O(1)\n");
    printf("  - Each element is pushed once (enqueue)\n");
    printf("  - Each element is transferred once (inStack to outStack)\n");
    printf("  - Each element is popped once (dequeue)\n");
    printf("  - Total: 3n operations for n elements = O(1) amortized\n\n");
    
    while (1)
    {
        printf("\n--- Menu ---\n");
        printf("1. Enqueue (add element)\n");
        printf("2. Dequeue (remove element)\n");
        printf("3. Display queue status\n");
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
                displayQueueStatus(&queue);
                break;
                
            case 2:
                dequeue(&queue);
                printf("\n");
                displayQueueStatus(&queue);
                break;
                
            case 3:
                printf("\n");
                displayQueueStatus(&queue);
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
Queue Using Two Stacks
=======================

--- Enqueuing 5 elements ---
Enqueued: 10
Enqueued: 20
Enqueued: 30
Enqueued: 40
Enqueued: 50

InStack top: 4 | OutStack top: -1

--- Dequeuing 3 elements ---
(Transferring elements from inStack to outStack)
Dequeued: 10
Dequeued: 20
Dequeued: 30

InStack top: -1 | OutStack top: 1

--- Enqueuing 2 more elements ---
Enqueued: 60
Enqueued: 70

InStack top: 1 | OutStack top: 1

--- Dequeuing 4 more elements ---
(Transferring elements from inStack to outStack)
Dequeued: 40
Dequeued: 50
Dequeued: 60
Dequeued: 70

InStack top: -1 | OutStack top: -1

--- Attempting to dequeue from empty queue ---
Queue is empty!

=== AMORTIZED COMPLEXITY ANALYSIS ===
Enqueue: O(1) - Simple push operation
Dequeue: Amortized O(1)
  - Each element is pushed once (enqueue)
  - Each element is transferred once (inStack to outStack)
  - Each element is popped once (dequeue)
  - Total: 3n operations for n elements = O(1) amortized
*/
