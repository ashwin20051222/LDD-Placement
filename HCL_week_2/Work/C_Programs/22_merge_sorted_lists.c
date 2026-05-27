/*
PROGRAM: Merge Two Sorted Linked Lists
QUESTION: Given two sorted linked lists, merge them into a single sorted list.
EXAMPLE:
    List 1: 1 -> 3 -> 5
    List 2: 2 -> 4 -> 6
    Result: 1 -> 2 -> 3 -> 4 -> 5 -> 6
*/

#include <stdio.h>
#include <stdlib.h>

/* Node structure */
typedef struct Node
{
    int data;
    struct Node *next;
} Node;

/* Create new node */
Node *createNode(int value)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->data = value;
    newNode->next = NULL;
    return newNode;
}

/* Insert node at end */
void insertEnd(Node **head, int value)
{
    Node *newNode = createNode(value);
    
    if (*head == NULL)
    {
        *head = newNode;
        return;
    }
    
    Node *current = *head;
    while (current->next != NULL)
    {
        current = current->next;
    }
    
    current->next = newNode;
}

/* Merge two sorted linked lists */
Node *mergeLists(Node *list1, Node *list2)
{
    if (list1 == NULL)
        return list2;
    if (list2 == NULL)
        return list1;
    
    Node *mergedHead = NULL;
    Node *mergedTail = NULL;
    
    while (list1 != NULL && list2 != NULL)
    {
        Node *newNode;
        
        if (list1->data <= list2->data)
        {
            newNode = createNode(list1->data);
            list1 = list1->next;
        }
        else
        {
            newNode = createNode(list2->data);
            list2 = list2->next;
        }
        
        if (mergedHead == NULL)
        {
            mergedHead = newNode;
            mergedTail = newNode;
        }
        else
        {
            mergedTail->next = newNode;
            mergedTail = newNode;
        }
    }
    
    /* Attach remaining nodes */
    if (list1 != NULL)
    {
        mergedTail->next = list1;
    }
    else if (list2 != NULL)
    {
        mergedTail->next = list2;
    }
    
    return mergedHead;
}

/* Display list */
void displayList(Node *head, char *label)
{
    printf("%s: ", label);
    Node *current = head;
    
    while (current != NULL)
    {
        printf("%d -> ", current->data);
        current = current->next;
    }
    
    printf("NULL\n");
}

int main()
{
    printf("Merge Two Sorted Linked Lists - Runtime Input\n");
    printf("============================================\n\n");
    
    int count1, count2;
    int value;
    int i;
    
    /* Create first sorted list */
    Node *list1 = NULL;
    printf("Enter count of elements in List 1: ");
    scanf("%d", &count1);
    
    printf("Enter %d sorted numbers for List 1:\n", count1);
    for (i = 0; i < count1; i++)
    {
        printf("Number %d: ", i + 1);
        scanf("%d", &value);
        insertEnd(&list1, value);
    }
    
    printf("List 1: ");
    displayList(list1, "");
    
    /* Create second sorted list */
    Node *list2 = NULL;
    printf("\nEnter count of elements in List 2: ");
    scanf("%d", &count2);
    
    printf("Enter %d sorted numbers for List 2:\n", count2);
    for (i = 0; i < count2; i++)
    {
        printf("Number %d: ", i + 1);
        scanf("%d", &value);
        insertEnd(&list2, value);
    }
    
    printf("List 2: ");
    displayList(list2, "");
    
    /* Merge lists */
    printf("\nMerging lists...\n");
    Node *mergedList = mergeLists(list1, list2);
    printf("Merged: ");
    displayList(mergedList, "");
    
    return 0;
}

/*
OUTPUT:
Merge Two Sorted Linked Lists
=============================

Creating List 1: List 1: 1 -> 3 -> 5 -> 7 -> NULL
Creating List 2: List 2: 2 -> 4 -> 6 -> 8 -> NULL

Merging lists...
Merged List: 1 -> 2 -> 3 -> 4 -> 5 -> 6 -> 7 -> 8 -> NULL

--- Test Case 2 ---
List 3: 1 -> 2 -> 10 -> NULL
List 4: 3 -> 5 -> NULL
Merged: 1 -> 2 -> 3 -> 5 -> 10 -> NULL
*/
