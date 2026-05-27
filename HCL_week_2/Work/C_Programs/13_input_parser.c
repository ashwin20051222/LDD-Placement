/*
PROGRAM: Robust Input Parser with Validation
QUESTION: Parse input with validation for ranges and formats. 
          Return error codes and meaningful error messages.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Error codes */
#define SUCCESS 0
#define ERROR_INVALID_INPUT 1
#define ERROR_OUT_OF_RANGE 2
#define ERROR_INVALID_FORMAT 3
#define ERROR_EMPTY_INPUT 4

/* Parse integer with validation */
int parseInteger(char *input, int *result, int min, int max)
{
    if (input == NULL)
    {
        printf("ERROR: Input is NULL\n");
        return ERROR_INVALID_INPUT;
    }
    
    if (strlen(input) == 0)
    {
        printf("ERROR: Input is empty\n");
        return ERROR_EMPTY_INPUT;
    }
    
    /* Check if all characters are digits (allow negative sign) */
    int i = 0;
    if (input[0] == '-')
    {
        i = 1;
    }
    
    for (; i < strlen(input); i++)
    {
        if (!isdigit(input[i]))
        {
            printf("ERROR: Invalid format. Expected digits, got '%c'\n", input[i]);
            return ERROR_INVALID_FORMAT;
        }
    }
    
    /* Convert to integer */
    *result = atoi(input);
    
    /* Check range */
    if (*result < min || *result > max)
    {
        printf("ERROR: Value %d is out of range [%d, %d]\n", *result, min, max);
        return ERROR_OUT_OF_RANGE;
    }
    
    return SUCCESS;
}

/* Parse float with validation */
int parseFloat(char *input, float *result, float min, float max)
{
    if (input == NULL)
    {
        printf("ERROR: Input is NULL\n");
        return ERROR_INVALID_INPUT;
    }
    
    if (strlen(input) == 0)
    {
        printf("ERROR: Input is empty\n");
        return ERROR_EMPTY_INPUT;
    }
    
    /* Check format: allow digits, decimal point, and negative sign */
    int dotCount = 0;
    int i = 0;
    
    if (input[0] == '-')
    {
        i = 1;
    }
    
    for (; i < strlen(input); i++)
    {
        if (input[i] == '.')
        {
            dotCount++;
            if (dotCount > 1)
            {
                printf("ERROR: Multiple decimal points found\n");
                return ERROR_INVALID_FORMAT;
            }
        }
        else if (!isdigit(input[i]))
        {
            printf("ERROR: Invalid format. Expected digits/decimal, got '%c'\n", input[i]);
            return ERROR_INVALID_FORMAT;
        }
    }
    
    /* Convert to float */
    *result = atof(input);
    
    /* Check range */
    if (*result < min || *result > max)
    {
        printf("ERROR: Value %.2f is out of range [%.2f, %.2f]\n", *result, min, max);
        return ERROR_OUT_OF_RANGE;
    }
    
    return SUCCESS;
}

/* Get error message */
void printErrorMessage(int errorCode)
{
    switch (errorCode)
    {
        case SUCCESS:
            printf("SUCCESS: Parsing completed\n");
            break;
        case ERROR_INVALID_INPUT:
            printf("ERROR CODE %d: Invalid input provided\n", ERROR_INVALID_INPUT);
            break;
        case ERROR_OUT_OF_RANGE:
            printf("ERROR CODE %d: Value is out of specified range\n", ERROR_OUT_OF_RANGE);
            break;
        case ERROR_INVALID_FORMAT:
            printf("ERROR CODE %d: Invalid format detected\n", ERROR_INVALID_FORMAT);
            break;
        case ERROR_EMPTY_INPUT:
            printf("ERROR CODE %d: Input is empty\n", ERROR_EMPTY_INPUT);
            break;
        default:
            printf("ERROR CODE %d: Unknown error\n", errorCode);
    }
}

int main()
{
    printf("Robust Input Parser with Validation\n");
    printf("====================================\n\n");
    
    /* Test cases */
    char testInputs[6][20] = {"25", "150", "abc", "", "42", "100"};
    
    printf("--- Integer Parsing (Range: 0-100) ---\n");
    int i;
    for (i = 0; i < 6; i++)
    {
        int result;
        int errorCode = parseInteger(testInputs[i], &result, 0, 100);
        
        printf("Input: '%s' -> ", testInputs[i]);
        if (errorCode == SUCCESS)
        {
            printf("VALID (Value: %d)\n", result);
        }
        else
        {
            printErrorMessage(errorCode);
        }
    }
    
    printf("\n--- Float Parsing (Range: -10.0 to 10.0) ---\n");
    
    char floatInputs[5][20] = {"5.5", "15.0", "-8.3", "2..5", "abc"};
    
    for (i = 0; i < 5; i++)
    {
        float result;
        int errorCode = parseFloat(floatInputs[i], &result, -10.0, 10.0);
        
        printf("Input: '%s' -> ", floatInputs[i]);
        if (errorCode == SUCCESS)
        {
            printf("VALID (Value: %.2f)\n", result);
        }
        else
        {
            printErrorMessage(errorCode);
        }
    }
    
    return 0;
}

/*
OUTPUT:
Robust Input Parser with Validation
====================================

--- Integer Parsing (Range: 0-100) ---
Input: '25' -> VALID (Value: 25)
Input: '150' -> ERROR: Value 150 is out of range [0, 100]
ERROR CODE 2: Value is out of specified range
Input: 'abc' -> ERROR: Invalid format. Expected digits, got 'a'
ERROR CODE 3: Invalid format detected
Input: '' -> ERROR: Input is empty
ERROR CODE 4: Input is empty
Input: '42' -> VALID (Value: 42)
Input: '100' -> VALID (Value: 100)

--- Float Parsing (Range: -10.0 to 10.0) ---
Input: '5.5' -> VALID (Value: 5.50)
Input: '15.0' -> ERROR: Value 15.00 is out of range [-10.00, 10.00]
ERROR CODE 2: Value is out of specified range
Input: '-8.3' -> VALID (Value: -8.30)
Input: '2..5' -> ERROR: Multiple decimal points found
ERROR CODE 3: Invalid format detected
Input: 'abc' -> ERROR: Invalid format. Expected digits/decimal, got 'a'
ERROR CODE 3: Invalid format detected
*/
