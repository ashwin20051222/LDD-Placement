/*
PROGRAM: Rotate Array to Right by K Steps
QUESTION: Given an array and an integer k, rotate the array to the right 
          by k steps efficiently.
EXAMPLE:
    Array: [1, 2, 3, 4, 5]
    K: 2
    Output: [4, 5, 1, 2, 3]
*/

#include <stdio.h>
#include <stdlib.h>

/* Reverse array from start to end */
void reverse(int array[], int start, int end)
{
    while (start < end)
    {
        int temp = array[start];
        array[start] = array[end];
        array[end] = temp;
        start++;
        end--;
    }
}

/* Rotate array to right by k steps */
void rotateRight(int array[], int size, int k)
{
    if (size == 0)
        return;
    
    /* Handle k greater than size */
    k = k % size;
    
    if (k == 0)
        return;
    
    /* Reverse entire array */
    reverse(array, 0, size - 1);
    
    /* Reverse first k elements */
    reverse(array, 0, k - 1);
    
    /* Reverse remaining elements */
    reverse(array, k, size - 1);
}

/* Display array */
void displayArray(int array[], int size)
{
    int i;
    printf("[");
    for (i = 0; i < size; i++)
    {
        printf("%d", array[i]);
        if (i < size - 1)
            printf(", ");
    }
    printf("]\n");
}

int main()
{
    int size;
    int i;
    
    printf("Array Rotation - Runtime Input\n");
    printf("==============================\n\n");
    
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
    
    /* Get rotation amount from user */
    int k;
    printf("\nEnter rotation amount (k): ");
    scanf("%d", &k);
    
    /* Display original array */
    printf("\nOriginal array: ");
    displayArray(array, size);
    
    /* Rotate array */
    printf("Rotate right by %d steps\n", k);
    rotateRight(array, size, k);
    
    /* Display rotated array */
    printf("Result: ");
    displayArray(array, size);
    
    /* Free allocated memory */
    free(array);
    
    return 0;
}

/*
OUTPUT:
Rotate Array to Right by K Steps
================================

--- Test 1 ---
Original array: [1, 2, 3, 4, 5]
Rotate right by 2 steps
Result:         [4, 5, 1, 2, 3]

--- Test 2 (k > size) ---
Original array: [1, 2, 3, 4, 5]
Rotate right by 7 steps (k mod size = 2)
Result:         [4, 5, 1, 2, 3]

--- Test 3 ---
Original array: [1, 2, 3, 4, 5, 6, 7]
Rotate right by 3 steps
Result:         [5, 6, 7, 1, 2, 3, 4]

--- Test 4 ---
Original array: [1, 2]
Rotate right by 1 step
Result:         [2, 1]

--- Test 5 (single element) ---
Original array: [1]
Rotate right by 3 steps
Result:         [1]
*/
