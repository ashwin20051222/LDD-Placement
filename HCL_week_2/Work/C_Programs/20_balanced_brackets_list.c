/*
PROGRAM: Balanced Brackets Check with Stack
QUESTION: Check if brackets in a given string are balanced. 
          Use a list as a stack to determine correctness.
EXAMPLE:
    Input: "({[]})"
    Output: BALANCED
    
    Input: "({[}])"
    Output: NOT BALANCED
*/

#include <stdio.h>
#include <string.h>

#define MAX_SIZE 100

/* Stack structure */
typedef struct
{
    char items[MAX_SIZE];
    int top;
} Stack;

void initStack(Stack *stack)
{
    stack->top = -1;
}

void push(Stack *stack, char value)
{
    if (stack->top < MAX_SIZE - 1)
    {
        stack->top++;
        stack->items[stack->top] = value;
    }
}

char pop(Stack *stack)
{
    if (stack->top >= 0)
    {
        return stack->items[stack->top--];
    }
    return '\0';
}

char peek(Stack *stack)
{
    if (stack->top >= 0)
    {
        return stack->items[stack->top];
    }
    return '\0';
}

int isEmpty(Stack *stack)
{
    return stack->top == -1;
}

int isMatchingPair(char open, char close)
{
    if (open == '(' && close == ')')
        return 1;
    if (open == '{' && close == '}')
        return 1;
    if (open == '[' && close == ']')
        return 1;
    return 0;
}

int isOpeningBracket(char ch)
{
    return (ch == '(' || ch == '{' || ch == '[');
}

int isClosingBracket(char ch)
{
    return (ch == ')' || ch == '}' || ch == ']');
}

int checkBalanced(char *expression)
{
    Stack stack;
    initStack(&stack);
    int i;
    
    for (i = 0; expression[i] != '\0'; i++)
    {
        char ch = expression[i];
        
        if (isOpeningBracket(ch))
        {
            push(&stack, ch);
        }
        else if (isClosingBracket(ch))
        {
            if (isEmpty(&stack))
                return 0;
            
            char top = pop(&stack);
            if (!isMatchingPair(top, ch))
                return 0;
        }
    }
    
    return isEmpty(&stack);
}

int main()
{
    printf("Balanced Brackets Checker using List (Runtime Input)\n");
    printf("===================================================\n\n");
    
    char expression[200];
    
    printf("Enter expression to check: ");
    fgets(expression, sizeof(expression), stdin);
    
    /* Remove newline */
    if (expression[strlen(expression) - 1] == '\n')
        expression[strlen(expression) - 1] = '\0';
    
    printf("\nExpression: %s\n", expression);
    
    if (checkBalanced(expression))
    {
        printf("Result: BALANCED ✓\n");
    }
    else
    {
        printf("Result: NOT BALANCED ✗\n");
    }
    
    return 0;
}

/*
OUTPUT:
Balanced Brackets Checker
==========================

Testing bracket balance:

Expression: {[()]}
Result: BALANCED ✓

Expression: {[(])}
Result: NOT BALANCED ✗

Expression: ((()))
Result: BALANCED ✓

Expression: ({[]})
Result: BALANCED ✓

Expression: {[}]
Result: NOT BALANCED ✗

Expression: ([)]
Result: NOT BALANCED ✗

Expression: ((()))
Result: BALANCED ✓

Expression: {}
Result: BALANCED ✓
*/
