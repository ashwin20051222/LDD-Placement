/*
PROGRAM: Sort Numbers Using QuickSort Method
QUESTION: Given an array of integers, sort them in ascending order using 
          the QuickSort algorithm.
EXAMPLE:
    Array: [50, 30, 80, 20, 10]
    Output: [10, 20, 30, 50, 80]
*/

#include <stdio.h>
#include <stdlib.h>

/* Function to swap two integers */
void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

/* Function to partition array and return pivot index */
int partition(int array[], int low, int high)
{
    int pivot = array[high];  /* Choose last element as pivot */
    int i = low - 1;
    int j;
    
    for (j = low; j < high; j++)
    {
        if (array[j] < pivot)
        {
            i++;
            swap(&array[i], &array[j]);
        }
    }
    
    swap(&array[i + 1], &array[high]);
    return i + 1;
}

/* QuickSort recursive function */
void quickSort(int array[], int low, int high)
{
    if (low < high)
    {
        int pivot_index = partition(array, low, high);
        
        /* Sort left side */
        quickSort(array, low, pivot_index - 1);
        
        /* Sort right side */
        quickSort(array, pivot_index + 1, high);
    }
}

/* Function to display array */
void displayArray(int array[], int size)
{
    int i;
    printf("Array: ");
    for (i = 0; i < size; i++)
    {
        printf("%d ", array[i]);
    }
    printf("\n");
}

int main()
{
    int size;
    int i, j;
    
    printf("QuickSort - Runtime Input\n");
    printf("=========================\n\n");
    
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
    printf("Enter %d numbers to sort:\n", size);
    for (i = 0; i < size; i++)
    {
        printf("Number %d: ", i + 1);
        scanf("%d", &numbers[i]);
    }
    
    /* Display original array */
    printf("\nBefore sorting:\n");
    displayArray(numbers, size);
    
    /* Sort array using quicksort */
    quickSort(numbers, 0, size - 1);
    
    /* Display sorted array */
    printf("\nAfter sorting:\n");
    displayArray(numbers, size);
    
    /* Free allocated memory */
    free(numbers);
    
    return 0;
}

/*
OUTPUT:
Before sorting:
Array: 50 30 80 20 10 90 40

After sorting:
Array: 10 20 30 40 50 80 90
*/
