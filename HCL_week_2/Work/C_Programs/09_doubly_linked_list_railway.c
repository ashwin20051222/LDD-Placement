/*
PROGRAM: Doubly Linked List - Railway System Compartments
QUESTION: Manage railway compartments dynamically. Implement:
          1. Add compartment at front or end
          2. Remove compartment from any position
          3. Display compartments forward and backward
          Demonstrate with at least 4 compartments
*/

#include <stdio.h>
#include <stdlib.h>

/* Compartment node structure */
typedef struct Node
{
    int compartmentNumber;
    int seatCount;
    struct Node *prev;
    struct Node *next;
} Node;

/* Railway system structure */
typedef struct
{
    Node *head;
} Railway;

/* Initialize railway */
void initRailway(Railway *railway)
{
    railway->head = NULL;
}

/* Create new compartment node */
Node *createCompartment(int compartmentNumber, int seatCount)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->compartmentNumber = compartmentNumber;
    newNode->seatCount = seatCount;
    newNode->prev = NULL;
    newNode->next = NULL;
    return newNode;
}

/* Add compartment at front */
void addAtFront(Railway *railway, int compartmentNumber, int seatCount)
{
    Node *newNode = createCompartment(compartmentNumber, seatCount);
    
    if (railway->head == NULL)
    {
        railway->head = newNode;
        printf("Added compartment %d at front (Seats: %d)\n", 
               compartmentNumber, seatCount);
    }
    else
    {
        newNode->next = railway->head;
        railway->head->prev = newNode;
        railway->head = newNode;
        printf("Added compartment %d at front (Seats: %d)\n", 
               compartmentNumber, seatCount);
    }
}

/* Add compartment at end */
void addAtEnd(Railway *railway, int compartmentNumber, int seatCount)
{
    Node *newNode = createCompartment(compartmentNumber, seatCount);
    
    if (railway->head == NULL)
    {
        railway->head = newNode;
        printf("Added compartment %d at end (Seats: %d)\n", 
               compartmentNumber, seatCount);
    }
    else
    {
        Node *current = railway->head;
        
        /* Find last node */
        while (current->next != NULL)
        {
            current = current->next;
        }
        
        /* Add new node */
        current->next = newNode;
        newNode->prev = current;
        printf("Added compartment %d at end (Seats: %d)\n", 
               compartmentNumber, seatCount);
    }
}

/* Remove compartment by number */
void removeCompartment(Railway *railway, int compartmentNumber)
{
    if (railway->head == NULL)
    {
        printf("Railway is empty\n");
        return;
    }
    
    Node *current = railway->head;
    
    /* Search for compartment */
    while (current != NULL)
    {
        if (current->compartmentNumber == compartmentNumber)
        {
            /* Compartment found - remove it */
            if (current->prev == NULL && current->next == NULL)
            {
                /* Only one compartment */
                railway->head = NULL;
            }
            else if (current->prev == NULL)
            {
                /* First compartment */
                railway->head = current->next;
                current->next->prev = NULL;
            }
            else if (current->next == NULL)
            {
                /* Last compartment */
                current->prev->next = NULL;
            }
            else
            {
                /* Middle compartment */
                current->prev->next = current->next;
                current->next->prev = current->prev;
            }
            
            printf("Removed compartment %d (Seats: %d)\n", 
                   compartmentNumber, current->seatCount);
            free(current);
            return;
        }
        
        current = current->next;
    }
    
    printf("Compartment %d not found\n", compartmentNumber);
}

/* Display compartments forward */
void displayForward(Railway *railway)
{
    if (railway->head == NULL)
    {
        printf("Railway is empty\n");
        return;
    }
    
    Node *current = railway->head;
    printf("Forward: ");
    
    while (current != NULL)
    {
        printf("[Comp-%d: %d seats] <-> ", 
               current->compartmentNumber, current->seatCount);
        current = current->next;
    }
    printf("NULL\n");
}

/* Display compartments backward */
void displayBackward(Railway *railway)
{
    if (railway->head == NULL)
    {
        printf("Railway is empty\n");
        return;
    }
    
    /* Find last node */
    Node *current = railway->head;
    while (current->next != NULL)
    {
        current = current->next;
    }
    
    printf("Backward: NULL");
    
    while (current != NULL)
    {
        printf(" <-> [Comp-%d: %d seats]", 
               current->compartmentNumber, current->seatCount);
        current = current->prev;
    }
    printf("\n");
}

/* Display all compartments details */
void displayDetails(Railway *railway)
{
    if (railway->head == NULL)
    {
        printf("Railway is empty\n");
        return;
    }
    
    Node *current = railway->head;
    int position = 1;
    
    printf("\nRailway Compartments Details:\n");
    printf("============================\n");
    
    while (current != NULL)
    {
        printf("Position %d: Compartment %d | Seats: %d\n", 
               position, current->compartmentNumber, current->seatCount);
        current = current->next;
        position++;
    }
    printf("\n");
}

int main()
{
    Railway railway;
    initRailway(&railway);
    
    printf("Railway System - Compartment Management (Runtime Input)\n");
    printf("==============================================\n\n");
    
    int choice;
    int compartmentNum;
    int seatCount;
    
    while (1)
    {
        printf("\n--- Menu ---\n");
        printf("1. Add compartment at front\n");
        printf("2. Add compartment at end\n");
        printf("3. Remove compartment by number\n");
        printf("4. Display all compartments (details)\n");
        printf("5. Display forward and backward\n");
        printf("6. Exit\n");
        printf("Choose option: ");
        scanf("%d", &choice);
        
        switch (choice)
        {
            case 1:
                printf("Enter compartment number: ");
                scanf("%d", &compartmentNum);
                printf("Enter seat count: ");
                scanf("%d", &seatCount);
                addAtFront(&railway, compartmentNum, seatCount);
                break;
                
            case 2:
                printf("Enter compartment number: ");
                scanf("%d", &compartmentNum);
                printf("Enter seat count: ");
                scanf("%d", &seatCount);
                addAtEnd(&railway, compartmentNum, seatCount);
                break;
                
            case 3:
                printf("Enter compartment number to remove: ");
                scanf("%d", &compartmentNum);
                removeCompartment(&railway, compartmentNum);
                break;
                
            case 4:
                displayDetails(&railway);
                break;
                
            case 5:
                displayForward(&railway);
                displayBackward(&railway);
                break;
                
            case 6:
                printf("Exiting...\n");
                return 0;
                
            default:
                printf("Invalid option!\n");
        }
    }
    
    return 0;
}

/*
OUTPUT:
Railway System - Compartment Management
======================================

--- Adding Compartments ---
Added compartment 1 at end (Seats: 72)
Added compartment 2 at end (Seats: 72)
Added compartment 3 at end (Seats: 60)
Added compartment 4 at end (Seats: 48)

Railway Compartments Details:
============================
Position 1: Compartment 1 | Seats: 72
Position 2: Compartment 2 | Seats: 72
Position 3: Compartment 3 | Seats: 60
Position 4: Compartment 4 | Seats: 48

--- Display Forward and Backward ---
Forward: [Comp-1: 72 seats] <-> [Comp-2: 72 seats] <-> [Comp-3: 60 seats] <-> [Comp-4: 48 seats] <-> NULL
Backward: NULL <-> [Comp-4: 48 seats] <-> [Comp-3: 60 seats] <-> [Comp-2: 72 seats] <-> [Comp-1: 72 seats]

--- Adding Compartment at Front ---
Added compartment 0 at front (Seats: 48)

Railway Compartments Details:
============================
Position 1: Compartment 0 | Seats: 48
Position 2: Compartment 1 | Seats: 72
Position 3: Compartment 2 | Seats: 72
Position 4: Compartment 3 | Seats: 60
Position 5: Compartment 4 | Seats: 48

--- Removing Compartment 2 ---
Removed compartment 2 (Seats: 72)

Railway Compartments Details:
============================
Position 1: Compartment 0 | Seats: 48
Position 2: Compartment 1 | Seats: 72
Position 3: Compartment 3 | Seats: 60
Position 4: Compartment 4 | Seats: 48

--- Removing First Compartment (0) ---
Removed compartment 0 (Seats: 48)

Railway Compartments Details:
============================
Position 1: Compartment 1 | Seats: 72
Position 2: Compartment 3 | Seats: 60
Position 3: Compartment 4 | Seats: 48
*/
