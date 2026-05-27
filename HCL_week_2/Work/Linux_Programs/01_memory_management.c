/*
PROGRAM: Dynamic Memory Management - malloc, calloc, realloc, free
QUESTION: Demonstrate memory allocation, deallocation, and resizing. 
          Show proper memory management practices.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void displayMemory(int *ptr, int size, char *label)
{
    printf("%s: ", label);
    int i;
    for (i = 0; i < size; i++)
    {
        printf("%d ", ptr[i]);
    }
    printf("\n");
}

int main()
{
    printf("Dynamic Memory Management in Linux\n");
    printf("==================================\n\n");
    
    /* 1. malloc - allocate uninitialized memory */
    printf("--- 1. malloc() - Allocate Uninitialized Memory ---\n");
    int *array1 = (int *)malloc(5 * sizeof(int));
    
    if (array1 == NULL)
    {
        printf("Memory allocation failed\n");
        return 1;
    }
    
    printf("Allocated memory for 5 integers at address: %p\n", array1);
    
    /* Initialize manually */
    int i;
    for (i = 0; i < 5; i++)
    {
        array1[i] = (i + 1) * 10;
    }
    
    displayMemory(array1, 5, "After initialization");
    printf("\n");
    
    /* 2. calloc - allocate initialized memory (zeros) */
    printf("--- 2. calloc() - Allocate Zero-Initialized Memory ---\n");
    int *array2 = (int *)calloc(5, sizeof(int));
    
    if (array2 == NULL)
    {
        printf("Memory allocation failed\n");
        free(array1);
        return 1;
    }
    
    printf("Allocated and initialized memory for 5 integers\n");
    displayMemory(array2, 5, "Initialized by calloc");
    printf("\n");
    
    /* 3. realloc - resize allocated memory */
    printf("--- 3. realloc() - Resize Allocated Memory ---\n");
    printf("Original size: 5 integers\n");
    displayMemory(array2, 5, "Before resize");
    
    int *array2_resized = (int *)realloc(array2, 8 * sizeof(int));
    
    if (array2_resized == NULL)
    {
        printf("Memory reallocation failed\n");
        free(array1);
        free(array2);
        return 1;
    }
    
    array2 = array2_resized;
    printf("Resized to 8 integers\n");
    displayMemory(array2, 8, "After resize");
    printf("\n");
    
    /* 4. Dynamically sized structure */
    printf("--- 4. Dynamic Array with Variable Size ---\n");
    int size;
    printf("Enter size of dynamic array: ");
    scanf("%d", &size);
    
    int *dynamicArray = (int *)malloc(size * sizeof(int));
    
    if (dynamicArray == NULL)
    {
        printf("Memory allocation failed\n");
        free(array1);
        free(array2);
        return 1;
    }
    
    printf("Allocated memory for %d integers\n", size);
    
    for (i = 0; i < size; i++)
    {
        dynamicArray[i] = i * 2;
    }
    
    printf("Array elements: ");
    for (i = 0; i < size; i++)
    {
        printf("%d ", dynamicArray[i]);
    }
    printf("\n\n");
    
    /* 5. String allocation */
    printf("--- 5. Dynamic String Allocation ---\n");
    char *string = (char *)malloc(50 * sizeof(char));
    
    if (string == NULL)
    {
        printf("Memory allocation failed\n");
        free(array1);
        free(array2);
        free(dynamicArray);
        return 1;
    }
    
    strcpy(string, "Hello from dynamic memory!");
    printf("String: %s\n", string);
    printf("Address: %p\n", string);
    printf("Size allocated: 50 bytes\n\n");
    
    /* 6. Memory deallocation */
    printf("--- 6. Freeing Memory ---\n");
    printf("Freeing allocated memory...\n");
    
    free(array1);
    free(array2);
    free(dynamicArray);
    free(string);
    
    printf("All memory freed\n");
    printf("Note: Pointers now contain garbage values, should be set to NULL\n");
    
    /* Best practice: Set to NULL after freeing */
    array1 = NULL;
    array2 = NULL;
    dynamicArray = NULL;
    string = NULL;
    
    printf("Pointers set to NULL for safety\n");
    
    return 0;
}

/*
OUTPUT:
Dynamic Memory Management in Linux
==================================

--- 1. malloc() - Allocate Uninitialized Memory ---
Allocated memory for 5 integers at address: 0x564d0e70
After initialization
[10, 20, 30, 40, 50]

--- 2. calloc() - Allocate Zero-Initialized Memory ---
Allocated and initialized memory for 5 integers
Initialized by calloc: [0, 0, 0, 0, 0]

--- 3. realloc() - Resize Allocated Memory ---
Original size: 5 integers
Before resize: [0, 0, 0, 0, 0]
Resized to 8 integers
After resize: [0, 0, 0, 0, 0, 0, 0, 0]

--- 4. Dynamic Array with Variable Size ---
Enter size of dynamic array: 6
Allocated memory for 6 integers
Array elements: 0 2 4 6 8 10

--- 5. Dynamic String Allocation ---
String: Hello from dynamic memory!
Address: 0x564d0f00
Size allocated: 50 bytes

--- 6. Freeing Memory ---
Freeing allocated memory...
All memory freed
Note: Pointers now contain garbage values, should be set to NULL
Pointers set to NULL for safety
*/
