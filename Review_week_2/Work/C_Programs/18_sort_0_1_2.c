/*
PROGRAM: Sort Array with 0s, 1s, and 2s (Dutch Flag Problem)
QUESTION: Given an array containing only 0, 1, and 2, sort it in-place 
          without using any built-in sort function.
APPROACH: Two-pass counting approach or three-pointer approach
*/

#include <stdio.h>
#include <stdlib.h>

/* Function to sort array with 0, 1, 2 */
void sortColors(int array[], int size)
{
    int i;
    int count0 = 0, count1 = 0, count2 = 0;
    
    /* Count occurrences */
    for (i = 0; i < size; i++)
    {
        if (array[i] == 0)
            count0++;
        else if (array[i] == 1)
            count1++;
        else
            count2++;
    }
    
    /* Place 0s */
    for (i = 0; i < count0; i++)
    {
        array[i] = 0;
    }
    
    /* Place 1s */
    for (i = count0; i < count0 + count1; i++)
    {
        array[i] = 1;
    }
    
    /* Place 2s */
    for (i = count0 + count1; i < size; i++)
    {
        array[i] = 2;
    }
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
    printf("Sort Array with 0s, 1s, and 2s - Runtime Input\n");
    printf("==============================================\n\n");
    
    int size;
    int i;
    
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
    printf("Enter %d numbers (only 0, 1, or 2):\n", size);
    for (i = 0; i < size; i++)
    {
        printf("Number %d: ", i + 1);
        scanf("%d", &array[i]);
        
        /* Validate input */
        if (array[i] < 0 || array[i] > 2)
        {
            printf("Invalid! Please enter only 0, 1, or 2\n");
            i--;
        }
    }
    
    /* Display original array */
    printf("\nBefore sorting: ");
    displayArray(array, size);
    
    /* Sort array */
    sortColors(array, size);
    
    /* Display sorted array */
    printf("After sorting:  ");
    displayArray(array, size);
    
    /* Free allocated memory */
    free(array);
    
    return 0;
}

/*
OUTPUT:
Sort Array with 0s, 1s, and 2s
===============================

--- Test 1 ---
Before: [2, 0, 2, 1, 1, 0]
After:  [0, 0, 1, 1, 2, 2]

--- Test 2 ---
Before: [2, 0, 1]
After:  [0, 1, 2]

--- Test 3 ---
Before: [0, 0, 1, 1, 2, 2]
After:  [0, 0, 1, 1, 2, 2]

--- Test 4 ---
Before: [2, 2, 0, 0, 1]
After:  [0, 0, 1, 2, 2]
*/
