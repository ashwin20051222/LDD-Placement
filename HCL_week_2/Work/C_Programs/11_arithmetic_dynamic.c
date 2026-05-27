/*
PROGRAM: Arithmetic Operations on Dynamic Number of Integers
QUESTION: Perform arithmetic operations (add, subtract, multiply) 
          on a dynamic number of integers read at runtime.
*/

#include <stdio.h>
#include <stdlib.h>

int main()
{
    int count;
    int i;
    int *numbers;
    int sum = 0;
    int product = 1;
    
    printf("Arithmetic Operations on Dynamic Integers\n");
    printf("=========================================\n\n");
    
    /* Get number of integers */
    printf("Enter how many integers you want to process: ");
    scanf("%d", &count);
    
    if (count <= 0)
    {
        printf("Number must be greater than 0\n");
        return 1;
    }
    
    /* Allocate memory for integers */
    numbers = (int *)malloc(count * sizeof(int));
    
    if (numbers == NULL)
    {
        printf("Memory allocation failed!\n");
        return 1;
    }
    
    /* Read integers */
    printf("\nEnter %d integers:\n", count);
    for (i = 0; i < count; i++)
    {
        printf("Enter integer %d: ", i + 1);
        scanf("%d", &numbers[i]);
    }
    
    /* Calculate sum */
    printf("\n--- Calculation Results ---\n\n");
    printf("Numbers entered: ");
    for (i = 0; i < count; i++)
    {
        printf("%d ", numbers[i]);
        sum += numbers[i];
    }
    printf("\n\n");
    
    printf("Sum: %d\n", sum);
    
    /* Calculate average */
    double average = (double)sum / count;
    printf("Average: %.2f\n", average);
    
    /* Calculate product */
    for (i = 0; i < count; i++)
    {
        product *= numbers[i];
    }
    printf("Product: %d\n", product);
    
    /* Find max and min */
    int max = numbers[0];
    int min = numbers[0];
    
    for (i = 1; i < count; i++)
    {
        if (numbers[i] > max)
            max = numbers[i];
        if (numbers[i] < min)
            min = numbers[i];
    }
    
    printf("Maximum: %d\n", max);
    printf("Minimum: %d\n", min);
    
    /* Free allocated memory */
    free(numbers);
    
    printf("\nMemory freed successfully\n");
    
    return 0;
}

/*
OUTPUT:
Arithmetic Operations on Dynamic Integers
=========================================

Enter how many integers you want to process: 5

Enter 5 integers:
Enter integer 1: 10
Enter integer 2: 20
Enter integer 3: 30
Enter integer 4: 40
Enter integer 5: 50

--- Calculation Results ---

Numbers entered: 10 20 30 40 50

Sum: 150
Average: 30.00
Product: 12000000
Maximum: 50
Minimum: 10

Memory freed successfully
*/
