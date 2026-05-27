/*
PROGRAM: Sliding Window Maximum using Deque
QUESTION: Given an array and a window size k, find the maximum element 
          in each sliding window using a deque.
EXAMPLE:
    Array: [1, 3, 1, 2, 0, 5]
    Window size: 3
    Output: [3, 3, 2, 5]
*/

#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 100

/* Deque node */
typedef struct DequeNode
{
    int data;
    int index;
} DequeNode;

/* Deque structure */
typedef struct
{
    DequeNode items[MAX_SIZE];
    int front;
    int rear;
} Deque;

void initDeque(Deque *deque)
{
    deque->front = 0;
    deque->rear = -1;
}

int isEmpty(Deque *deque)
{
    return deque->rear < deque->front;
}

void insertRear(Deque *deque, int data, int index)
{
    deque->rear++;
    deque->items[deque->rear].data = data;
    deque->items[deque->rear].index = index;
}

void deleteFront(Deque *deque)
{
    if (!isEmpty(deque))
    {
        deque->front++;
    }
}

void deleteRear(Deque *deque)
{
    if (!isEmpty(deque))
    {
        deque->rear--;
    }
}

int getFront(Deque *deque)
{
    if (!isEmpty(deque))
    {
        return deque->items[deque->front].data;
    }
    return -1;
}

int getRearData(Deque *deque)
{
    if (!isEmpty(deque))
    {
        return deque->items[deque->rear].data;
    }
    return -1;
}

int getFrontIndex(Deque *deque)
{
    if (!isEmpty(deque))
    {
        return deque->items[deque->front].index;
    }
    return -1;
}

int getRearIndex(Deque *deque)
{
    if (!isEmpty(deque))
    {
        return deque->items[deque->rear].index;
    }
    return -1;
}

void findMaxInSlidingWindow(int array[], int size, int k)
{
    Deque deque;
    int i;
    int maxCount = size - k + 1;
    int *result = (int *)malloc(maxCount * sizeof(int));
    int resultIndex = 0;
    
    printf("Array: ");
    for (i = 0; i < size; i++)
    {
        printf("%d ", array[i]);
    }
    printf("\nWindow size: %d\n\n", k);
    
    printf("--- Sliding Window Process ---\n\n");
    
    /* Process first window */
    initDeque(&deque);
    
    for (i = 0; i < k; i++)
    {
        /* Remove elements from rear if they are smaller */
        while (!isEmpty(&deque) && getRearData(&deque) < array[i])
        {
            deleteRear(&deque);
        }
        
        insertRear(&deque, array[i], i);
    }
    
    /* Store max of first window */
    result[resultIndex++] = getFront(&deque);
    printf("Window 1 [0-%d]: ", k - 1);
    for (i = 0; i < k; i++)
    {
        printf("%d ", array[i]);
    }
    printf("-> Max: %d\n", result[resultIndex - 1]);
    
    /* Process remaining windows */
    for (i = k; i < size; i++)
    {
        /* Remove element outside window */
        if (!isEmpty(&deque) && getFrontIndex(&deque) <= i - k)
        {
            deleteFront(&deque);
        }
        
        /* Remove elements smaller than current */
        while (!isEmpty(&deque) && getRearData(&deque) < array[i])
        {
            deleteRear(&deque);
        }
        
        insertRear(&deque, array[i], i);
        
        result[resultIndex++] = getFront(&deque);
        printf("Window %d [%d-%d]: ", 
               i - k + 2, i - k + 1, i);
        int j;
        for (j = i - k + 1; j <= i; j++)
        {
            printf("%d ", array[j]);
        }
        printf("-> Max: %d\n", result[resultIndex - 1]);
    }
    
    /* Display results */
    printf("\n--- Maximum Elements in Each Window ---\n");
    printf("Results: [");
    for (i = 0; i < maxCount; i++)
    {
        printf("%d", result[i]);
        if (i < maxCount - 1)
            printf(", ");
    }
    printf("]\n");
    
    free(result);
}

int main()
{
    int size;
    int i;
    int k;
    
    printf("Sliding Window Maximum using Deque - Runtime Input\n");
    printf("==================================================\n\n");
    
    /* Get array size from user */
    printf("Enter array size: ");
    scanf("%d", &size);
    
    int *array = (int *)malloc(size * sizeof(int));
    
    if (array == NULL)
    {
        printf("Memory allocation failed\n");
        return 1;
    }
    
    /* Get array elements from user */
    printf("Enter %d numbers:\n", size);
    for (i = 0; i < size; i++)
    {
        printf("Number %d: ", i + 1);
        scanf("%d", &array[i]);
    }
    
    /* Get window size from user */
    printf("Enter window size (k): ");
    scanf("%d", &k);
    
    if (k > size || k <= 0)
    {
        printf("Invalid window size! Must be between 1 and %d\n", size);
        free(array);
        return 1;
    }
    
    /* Find max in sliding window */
    findMaxInSlidingWindow(array, size, k);
    
    /* Free allocated memory */
    free(array);
    
    return 0;
}

/*
OUTPUT:
Sliding Window Maximum using Deque
==================================

--- Test Case 1 ---
Array: 1 3 1 2 0 5
Window size: 3

--- Sliding Window Process ---

Window 1 [0-2]: 1 3 1 -> Max: 3
Window 2 [1-3]: 3 1 2 -> Max: 3
Window 3 [2-4]: 1 2 0 -> Max: 2
Window 4 [3-5]: 2 0 5 -> Max: 5

--- Maximum Elements in Each Window ---
Results: [3, 3, 2, 5]

--- Test Case 2 ---
Array: 1 3 1 2 0 5 5 6 2
Window size: 3

--- Sliding Window Process ---

Window 1 [0-2]: 1 3 1 -> Max: 3
Window 2 [1-3]: 3 1 2 -> Max: 3
Window 3 [2-4]: 1 2 0 -> Max: 2
Window 4 [3-5]: 2 0 5 -> Max: 5
Window 5 [4-6]: 0 5 5 -> Max: 5
Window 6 [5-7]: 5 5 6 -> Max: 6
Window 7 [6-8]: 5 6 2 -> Max: 6

--- Maximum Elements in Each Window ---
Results: [3, 3, 2, 5, 5, 6, 6]
*/
