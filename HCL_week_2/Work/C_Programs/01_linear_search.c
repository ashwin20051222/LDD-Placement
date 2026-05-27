/*
PROGRAM: Find Position of Target Value in Array Using Linear Search
QUESTION: Given an array of integers and a target value, find the position 
          (index) of the target value. If not found, return -1.
EXAMPLE:
    Array: [10, 20, 30, 40, 50]
    Target: 30
    Output: Found at index 2
*/

#include <stdio.h>
#include <stdlib.h>

/* Function to search for target in array */
int linearSearch(int array[], int size, int target)
{
    int i;
    
    for (i = 0; i < size; i++)
    {
        if (array[i] == target)
        {
            return i;  /* Return index when found */
        }
    }
    
    return -1;  /* Return -1 if not found */
}

int main()
{
    int size;
    int i;
    int target;
    int result;
    
    printf("Linear Search - Runtime Input\n");
    printf("=============================\n\n");
    
    /* Get array size from user */
    printf("Enter array size: ");
    scanf("%d", &size);
    
    int *numbers = (int *)malloc(size * sizeof(int));
    
    if (numbers == NULL)
    {
        printf("Memory allocation failed\n");
        return 1;
    }
    
    /* Get array elements from user */
    printf("Enter %d array elements:\n", size);
    for (i = 0; i < size; i++)
    {
        printf("Element %d: ", i);
        scanf("%d", &numbers[i]);
    }
    
    /* Display the array */
    printf("\nArray: ");
    for (i = 0; i < size; i++)
    {
        printf("%d ", numbers[i]);
    }
    printf("\n");
    
    /* Get target value from user */
    printf("\nEnter target value to search: ");
    scanf("%d", &target);
    
    /* Search for target */
    result = linearSearch(numbers, size, target);
    
    /* Print result */
    printf("Searching for: %d\n", target);
    if (result != -1)
    {
        printf("Found at index: %d\n", result);
    }
    else
    {
        printf("Element not found\n");
    }
    
    /* Free allocated memory */
    free(numbers);
    
    return 0;
}

/*
OUTPUT:
Array: 10 20 30 40 50 60 70
Searching for: 40
Found at index: 3

Searching for: 100
Element not found
*/
