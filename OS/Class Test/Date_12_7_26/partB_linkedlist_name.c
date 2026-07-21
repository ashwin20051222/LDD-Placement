/*
 * Part B: Data Structures
 * Program: Singly Linked List operations on a first name
 * Operations:
 *   1. Read a first name (without spaces) from the user
 *   2. Build a linked list where each node stores one character of the name
 *   3. Display the linked list
 *   4. Reverse the linked list and display
 *   5. Count and display total number of vowels (A, E, I, O, U, both cases)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Node structure: each node holds a single character */
struct Node {
    char ch;
    struct Node *next;
};

/* Function prototypes */
struct Node* buildList(const char *name);
void displayList(struct Node *head);
struct Node* displayReversedName(struct Node *head);
int countVowels(struct Node *head);
void freeList(struct Node *head);

/*
 * buildList: Creates a linked list from the characters of the input name.
 * Each node stores one character in the same order as the name.
 * Returns pointer to the head of the list.
 */
struct Node* buildList(const char *name) {
    struct Node *head = NULL, *tail = NULL;
    int i;

    for (i = 0; name[i] != '\0'; i++) {
        struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
        if (newNode == NULL) {
            printf("Memory allocation failed.\n");
            exit(EXIT_FAILURE);
        }
        newNode->ch = name[i];
        newNode->next = NULL;

        if (head == NULL) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }
    return head;
}

/*
 * displayList: Traverses the list and prints each character in the format
 *   ch -> ch -> ... -> NULL
 */
void displayList(struct Node *head) {
    struct Node *current = head;
    printf("\nLinked List   : ");
    while (current != NULL) {
        printf("%c -> ", current->ch);
        current = current->next;
    }
    printf("NULL\n");
}

/*
 * displayReversedName: Reverses the linked list by reversing the pointers,
 * then prints the resulting string. Returns the new head.
 */
struct Node* displayReversedName(struct Node *head) {
    struct Node *prev = NULL, *current = head, *next = NULL;

    /* Standard iterative linked-list reversal */
    while (current != NULL) {
        next = current->next;   /* save next node */
        current->next = prev;   /* reverse the link  */
        prev = current;         /* move prev forward */
        current = next;         /* move current forward */
    }
    head = prev;                /* new head is the old tail */

    printf("\nReversed Name : ");
    struct Node *p = head;
    while (p != NULL) {
        printf("%c", p->ch);
        p = p->next;
    }
    printf("\n");
    return head;
}

/*
 * countVowels: Traverses the list and counts the number of vowels
 * (A, E, I, O, U in both uppercase and lowercase) in the name.
 */
int countVowels(struct Node *head) {
    struct Node *current = head;
    int count = 0;
    while (current != NULL) {
        char c = current->ch;
        if (c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U' ||
            c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
            count++;
        }
        current = current->next;
    }
    return count;
}

/*
 * freeList: Releases all nodes of the linked list to prevent memory leaks.
 */
void freeList(struct Node *head) {
    struct Node *current = head;
    while (current != NULL) {
        struct Node *temp = current;
        current = current->next;
        free(temp);
    }
}

int main(void) {
    char name[100];
    struct Node *head = NULL;

    printf("Enter your first name: ");
    /* Use scanf with %s since the name is a single word (no spaces) */
    if (scanf("%99s", name) != 1) {
        printf("Invalid input.\n");
        return EXIT_FAILURE;
    }

    /* 1) Build the linked list from the entered name */
    head = buildList(name);

    /* 2) Display the original name */
    printf("\nOriginal Name : %s", name);

    /* 3) Display the linked list representation */
    displayList(head);

    /* 4) Reverse the linked list and display the reversed name */
    head = displayReversedName(head);

    /* 5) Traverse the list and count the vowels */
    int vowels = countVowels(head);
    printf("\nNumber of Vowels : %d\n", vowels);

    /* Clean up */
    freeList(head);
    return EXIT_SUCCESS;
}
