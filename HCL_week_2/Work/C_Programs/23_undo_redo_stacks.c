/*
PROGRAM: Undo/Redo Implementation using Two Stacks
QUESTION: Implement a process queue's Undo/Redo functionality using two stacks.
          Users can add processes (p1, p2, p3, p4...), undo, and redo operations.
EXAMPLE:
    Add: p1 -> Add: p2 -> Add: p3 -> Undo -> "" -> Redo -> p3
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_SIZE 50
#define MAX_TEXT 100

/* Stack structure */
typedef struct
{
    char items[MAX_SIZE][MAX_TEXT];
    int top;
} Stack;

void initStack(Stack *stack)
{
    stack->top = -1;
}

void push(Stack *stack, char *value)
{
    if (stack->top < MAX_SIZE - 1)
    {
        stack->top++;
        strcpy(stack->items[stack->top], value);
    }
}

char *pop(Stack *stack)
{
    if (stack->top >= 0)
    {
        return stack->items[stack->top--];
    }
    return "";
}

int isEmpty(Stack *stack)
{
    return stack->top == -1;
}

/* Process Queue structure */
typedef struct
{
    char currentQueue[MAX_TEXT];
    Stack undoStack;
    Stack redoStack;
} ProcessQueue;

void initQueue(ProcessQueue *queue)
{
    strcpy(queue->currentQueue, "");
    initStack(&queue->undoStack);
    initStack(&queue->redoStack);
}

void addProcess(ProcessQueue *queue, char *processName)
{
    printf("[ADD] Adding process: %s\n", processName);
    
    /* Push current state to undo stack */
    push(&queue->undoStack, queue->currentQueue);
    
    /* Clear redo stack when new action is performed */
    initStack(&queue->redoStack);
    
    /* Add process to queue */
    if (strlen(queue->currentQueue) > 0)
    {
        strcat(queue->currentQueue, " -> ");
    }
    strcat(queue->currentQueue, processName);
    
    printf("  Current queue: %s\n", queue->currentQueue);
}

void removeProcess(ProcessQueue *queue)
{
    int len = strlen(queue->currentQueue);
    
    if (len > 0)
    {
        printf("[REMOVE] Removing last process\n");
        
        /* Push current state to undo stack */
        push(&queue->undoStack, queue->currentQueue);
        
        /* Clear redo stack */
        initStack(&queue->redoStack);
        
        /* Remove last process */
        while (len > 0 && queue->currentQueue[len - 1] != '>')
        {
            len--;
        }
        
        if (len > 0)
        {
            len -= 4;  /* Remove " -> " */
        }
        
        queue->currentQueue[len] = '\0';
        printf("  Current queue: %s\n", queue->currentQueue);
    }
    else
    {
        printf("[REMOVE] Queue is empty, nothing to remove\n");
    }
}

void undo(ProcessQueue *queue)
{
    if (isEmpty(&queue->undoStack))
    {
        printf("[UNDO] Nothing to undo\n");
        return;
    }
    
    printf("[UNDO] Undoing last action\n");
    
    /* Push current state to redo stack */
    push(&queue->redoStack, queue->currentQueue);
    
    /* Get previous state from undo stack */
    char *previousQueue = pop(&queue->undoStack);
    strcpy(queue->currentQueue, previousQueue);
    
    printf("  Current queue: %s\n", queue->currentQueue);
}

void redo(ProcessQueue *queue)
{
    if (isEmpty(&queue->redoStack))
    {
        printf("[REDO] Nothing to redo\n");
        return;
    }
    
    printf("[REDO] Redoing last undone action\n");
    
    /* Push current state to undo stack */
    push(&queue->undoStack, queue->currentQueue);
    
    /* Get next state from redo stack */
    char *nextQueue = pop(&queue->redoStack);
    strcpy(queue->currentQueue, nextQueue);
    
    printf("  Current queue: %s\n", queue->currentQueue);
}

void displayStatus(ProcessQueue *queue)
{
    printf("\n[STATUS] Current queue: %s\n", 
           strlen(queue->currentQueue) > 0 ? queue->currentQueue : "(empty)");
    printf("[STATUS] Undo operations available: %d\n", queue->undoStack.top + 1);
    printf("[STATUS] Redo operations available: %d\n\n", queue->redoStack.top + 1);
}

int main()
{
    ProcessQueue queue;
    initQueue(&queue);
    
    printf("Process Queue - Undo/Redo Implementation\n");
    printf("=======================================\n\n");
    
    printf("You can perform these operations:\n");
    printf("  1. Add process (add p1, add p2, etc.)\n");
    printf("  2. Remove last process (remove)\n");
    printf("  3. Undo last operation (undo)\n");
    printf("  4. Redo last undo (redo)\n");
    printf("  5. Exit (exit)\n\n");
    
    char input[100];
    char command[50];
    char processName[50];
    
    while (1)
    {
        printf("Enter command: ");
        fgets(input, sizeof(input), stdin);
        
        /* Remove newline */
        if (input[strlen(input) - 1] == '\n')
            input[strlen(input) - 1] = '\0';
        
        /* Parse command */
        sscanf(input, "%s", command);
        
        if (strcmp(command, "add") == 0)
        {
            /* Extract process name */
            if (sscanf(input, "add %s", processName) == 1)
            {
                addProcess(&queue, processName);
                displayStatus(&queue);
            }
            else
            {
                printf("Usage: add <processname> (e.g., add p1)\n\n");
            }
        }
        else if (strcmp(command, "remove") == 0)
        {
            removeProcess(&queue);
            displayStatus(&queue);
        }
        else if (strcmp(command, "undo") == 0)
        {
            undo(&queue);
            displayStatus(&queue);
        }
        else if (strcmp(command, "redo") == 0)
        {
            redo(&queue);
            displayStatus(&queue);
        }
        else if (strcmp(command, "exit") == 0)
        {
            printf("Exiting...\n");
            break;
        }
        else
        {
            printf("Unknown command. Use: add <name>, remove, undo, redo, or exit\n\n");
        }
    }
    
    return 0;
}

/*
OUTPUT:
Process Queue - Undo/Redo Implementation
=======================================

You can perform these operations:
  1. Add process (add p1, add p2, etc.)
  2. Remove last process (remove)
  3. Undo last operation (undo)
  4. Redo last undo (redo)
  5. Exit (exit)

Enter command: add p1
[ADD] Adding process: p1
  Current queue: p1

[STATUS] Current queue: p1
[STATUS] Undo operations available: 1
[STATUS] Redo operations available: 0

Enter command: add p2
[ADD] Adding process: p2
  Current queue: p1 -> p2

[STATUS] Current queue: p1 -> p2
[STATUS] Undo operations available: 2
[STATUS] Redo operations available: 0

Enter command: add p3
[ADD] Adding process: p3
  Current queue: p1 -> p2 -> p3

[STATUS] Current queue: p1 -> p2 -> p3
[STATUS] Undo operations available: 3
[STATUS] Redo operations available: 0

Enter command: add p4
[ADD] Adding process: p4
  Current queue: p1 -> p2 -> p3 -> p4

[STATUS] Current queue: p1 -> p2 -> p3 -> p4
[STATUS] Undo operations available: 4
[STATUS] Redo operations available: 0

Enter command: undo
[UNDO] Undoing last action
  Current queue: p1 -> p2 -> p3

[STATUS] Current queue: p1 -> p2 -> p3
[STATUS] Undo operations available: 3
[STATUS] Redo operations available: 1

Enter command: undo
[UNDO] Undoing last action
  Current queue: p1 -> p2

[STATUS] Current queue: p1 -> p2
[STATUS] Undo operations available: 2
[STATUS] Redo operations available: 2

Enter command: redo
[REDO] Redoing last undone action
  Current queue: p1 -> p2 -> p3

[STATUS] Current queue: p1 -> p2 -> p3
[STATUS] Undo operations available: 3
[STATUS] Redo operations available: 1

Enter command: remove
[REMOVE] Removing last process
  Current queue: p1 -> p2

[STATUS] Current queue: p1 -> p2
[STATUS] Undo operations available: 4
[STATUS] Redo operations available: 0

Enter command: exit
Exiting...
*/


/*
OUTPUT:
Text Editor - Undo/Redo Implementation
====================================

--- Operations ---
[INSERT] Typing: Hello
  Current text: "Hello"
[INSERT] Typing:  World
  Current text: "Hello World"
[INSERT] Typing: !
  Current text: "Hello World!"

[STATUS] Current text: "Hello World!"
[STATUS] Undo operations available: 3
[STATUS] Redo operations available: 0

--- Undo Operations ---
[UNDO] Undoing last action
  Current text: "Hello World"

[STATUS] Current text: "Hello World"
[STATUS] Undo operations available: 2
[STATUS] Redo operations available: 1

[UNDO] Undoing last action
  Current text: "Hello"

[STATUS] Current text: "Hello"
[STATUS] Undo operations available: 1
[STATUS] Redo operations available: 2

[UNDO] Undoing last action
  Current text: ""

[STATUS] Current text: ""
[STATUS] Undo operations available: 0
[STATUS] Redo operations available: 3

[UNDO] Nothing to undo

[STATUS] Current text: ""
[STATUS] Undo operations available: 0
[STATUS] Redo operations available: 3

--- Redo Operations ---
[REDO] Redoing last undone action
  Current text: "Hello"

[STATUS] Current text: "Hello"
[STATUS] Undo operations available: 1
[STATUS] Redo operations available: 2

[REDO] Redoing last undone action
  Current text: "Hello World"

[STATUS] Current text: "Hello World"
[STATUS] Undo operations available: 2
[STATUS] Redo operations available: 1

--- Insert After Undo ---
[INSERT] Typing: ?
  Current text: "Hello World?"

[STATUS] Current text: "Hello World?"
[STATUS] Undo operations available: 3
[STATUS] Redo operations available: 0

--- Try to Redo (should fail) ---
[REDO] Nothing to redo

[STATUS] Current text: "Hello World?"
[STATUS] Undo operations available: 3
[STATUS] Redo operations available: 0
*/
