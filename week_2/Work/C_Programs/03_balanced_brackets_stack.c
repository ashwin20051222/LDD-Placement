/*
PROGRAM: Check Balanced Parentheses/Brackets Using Stack
QUESTION: Given an expression with brackets/parentheses like "({[]})", 
          determine if all brackets are properly balanced using a stack.
EXAMPLE:
    Expression: "{[()]}"
    Output: Balanced

    Expression: "{[(])}"
    Output: Not Balanced
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

/* Initialize stack */
void initStack(Stack *stack)
{
    stack->top = -1;
}

/* Check if stack is empty */
int isEmpty(Stack *stack)
{
    return stack->top == -1;
}

/* Push element to stack */
void push(Stack *stack, char value)
{
    if (stack->top < MAX_SIZE - 1)
    {
        stack->top++;
        stack->items[stack->top] = value;
    }
}

/* Pop element from stack */
char pop(Stack *stack)
{
    if (!isEmpty(stack))
    {
        return stack->items[stack->top--];
    }
    return '\0';
}

/* Get top element without removing */
char peek(Stack *stack)
{
    if (!isEmpty(stack))
    {
        return stack->items[stack->top];
    }
    return '\0';
}

/* Check if brackets match */
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

/* Check if character is opening bracket */
int isOpeningBracket(char ch)
{
    return (ch == '(' || ch == '{' || ch == '[');
}

/* Check if character is closing bracket */
int isClosingBracket(char ch)
{
    return (ch == ')' || ch == '}' || ch == ']');
}

/* Check if expression has balanced brackets */
int isBalanced(char *expression)
{
    Stack stack;
    char ch, topChar;
    int i;
    
    initStack(&stack);
    
    for (i = 0; expression[i] != '\0'; i++)
    {
        ch = expression[i];
        
        if (isOpeningBracket(ch))
        {
            push(&stack, ch);
        }
        else if (isClosingBracket(ch))
        {
            if (isEmpty(&stack))
            {
                return 0;  /* Closing without opening */
            }
            
            topChar = pop(&stack);
            if (!isMatchingPair(topChar, ch))
            {
                return 0;  /* Mismatched pair */
            }
        }
    }
    
    /* Check if all brackets are closed */
    return isEmpty(&stack);
}

int main()
{
    char expression[200];
    
    printf("Checking Balanced Parentheses/Brackets - Runtime Input\n");
    printf("======================================================\n\n");
    printf("Enter expression to check: ");
    fgets(expression, sizeof(expression), stdin);
    
    /* Remove newline */
    if (expression[strlen(expression) - 1] == '\n')
        expression[strlen(expression) - 1] = '\0';
    
    printf("\nExpression: %s\n", expression);
    
    if (isBalanced(expression))
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
Checking Balanced Parentheses/Brackets:
========================================

Expression: {[()]}
Result: BALANCED

Expression: {[(])}
Result: NOT BALANCED

Expression: ({[]})
Result: BALANCED

Expression: ((()))
Result: BALANCED

Expression: {[}]
Result: NOT BALANCED
*/
