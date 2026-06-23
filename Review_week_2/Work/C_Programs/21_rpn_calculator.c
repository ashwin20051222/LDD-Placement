/*
PROGRAM: Reverse Polish Notation (RPN) Calculator using Stack
QUESTION: Given tokens of an RPN expression (e.g., ["2","1","+","3","*"]), 
          use a stack to compute the result.
EXAMPLE:
    Input: "2 1 + 3 *"
    Stack operations:
      Push 2, Push 1, Pop 1 and 2, Push 3, 
      Pop 3 and 3, Push 9
    Output: 9
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_SIZE 100

/* Stack structure */
typedef struct
{
    double items[MAX_SIZE];
    int top;
} Stack;

void initStack(Stack *stack)
{
    stack->top = -1;
}

void push(Stack *stack, double value)
{
    if (stack->top < MAX_SIZE - 1)
    {
        stack->top++;
        stack->items[stack->top] = value;
    }
}

double pop(Stack *stack)
{
    if (stack->top >= 0)
    {
        return stack->items[stack->top--];
    }
    return 0;
}

int isEmpty(Stack *stack)
{
    return stack->top == -1;
}

int isOperator(char *token)
{
    return (strlen(token) == 1 && 
            (token[0] == '+' || token[0] == '-' || 
             token[0] == '*' || token[0] == '/'));
}

int isNumber(char *token)
{
    if (token[0] == '-' || isdigit(token[0]))
    {
        int i = 1;
        while (token[i])
        {
            if (!isdigit(token[i]) && token[i] != '.')
                return 0;
            i++;
        }
        return 1;
    }
    return 0;
}

double performOperation(double operand1, double operand2, char operator)
{
    switch (operator)
    {
        case '+':
            return operand1 + operand2;
        case '-':
            return operand1 - operand2;
        case '*':
            return operand1 * operand2;
        case '/':
            if (operand2 != 0)
                return operand1 / operand2;
            else
                printf("Error: Division by zero\n");
            return 0;
        default:
            return 0;
    }
}

double evaluateRPN(char *expression)
{
    Stack stack;
    initStack(&stack);
    
    /* Make a copy of expression */
    char *copy = (char *)malloc(strlen(expression) + 1);
    strcpy(copy, expression);
    
    char *token = strtok(copy, " ");
    
    while (token != NULL)
    {
        if (isNumber(token))
        {
            push(&stack, atof(token));
        }
        else if (isOperator(token))
        {
            if (stack.top < 1)
            {
                printf("Error: Invalid RPN expression\n");
                free(copy);
                return 0;
            }
            
            double operand2 = pop(&stack);
            double operand1 = pop(&stack);
            double result = performOperation(operand1, operand2, token[0]);
            
            push(&stack, result);
        }
        
        token = strtok(NULL, " ");
    }
    
    free(copy);
    
    if (stack.top != 0)
    {
        printf("Error: Invalid RPN expression\n");
        return 0;
    }
    
    return stack.items[0];
}

int main()
{
    printf("Reverse Polish Notation (RPN) Calculator - Runtime Input\n");
    printf("====================================================\n\n");
    
    char expression[200];
    
    printf("Enter RPN expression (space-separated tokens)\n");
    printf("Examples:\n");
    printf("  2 1 +        (result: 3)\n");
    printf("  2 1 + 3 *    (result: 9)\n");
    printf("  5 3 -        (result: 2)\n");
    printf("  10 2 /       (result: 5)\n\n");
    
    printf("Enter expression: ");
    fgets(expression, sizeof(expression), stdin);
    
    /* Remove newline */
    if (expression[strlen(expression) - 1] == '\n')
        expression[strlen(expression) - 1] = '\0';
    
    if (strlen(expression) == 0)
    {
        printf("Error: Empty expression\n");
        return 1;
    }
    
    printf("\nExpression: %s\n", expression);
    
    double result = evaluateRPN(expression);
    printf("Result: %.2f\n", result);
    
    return 0;
}

/*
OUTPUT:
Reverse Polish Notation (RPN) Calculator
=======================================

Evaluating RPN expressions:

Expression: 2 1 +
Result: 3.00

Expression: 2 1 + 3 *
Result: 9.00

Expression: 15 7 1 1 + - / 3 * 2 1 1 + + -
Result: 5.00

Expression: 5 3 -
Result: 2.00

Expression: 10 2 /
Result: 5.00

Expression: 3 4 + 2 * 7 /
Result: 2.00
*/
