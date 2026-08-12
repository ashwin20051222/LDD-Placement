#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Patient {
    int patientID;
    char name[30];
    int severity;
    struct Patient *next;
} node;

node *head = NULL;
node *tail = NULL;

void insertAtEnd(int patientID, char name[], int severity) {
    node *newNode = (node *)malloc(sizeof(node));
    if (newNode == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }

    newNode->patientID = patientID;
    strcpy(newNode->name, name);
    newNode->severity = severity;
    newNode->next = NULL;

    if (head == NULL) {
        head = tail = newNode;
        return;
    }

    tail->next = newNode;
    tail = newNode;
}

void displayList(node *start) {
    node *temp = start;

    while (temp != NULL) {
        printf("%d(%d)", temp->patientID, temp->severity);
        if (temp->next != NULL) {
            printf(" -> ");
        }
        temp = temp->next;
    }
    printf("\n");
}

node *selectionSort(node *start, int *comparisons, int *pointerUpdates) {
    node *sortedHead = NULL;
    node *sortedTail = NULL;
    int pass = 1;

    *comparisons = 0;
    *pointerUpdates = 0;

    while (start != NULL) {
        node *maxPrev = NULL;
        node *maxNode = start;
        node *prev = start;
        node *curr = start->next;

        while (curr != NULL) {
            (*comparisons)++;
            if (curr->severity > maxNode->severity) {
                maxNode = curr;
                maxPrev = prev;
            }
            prev = curr;
            curr = curr->next;
        }

        if (maxPrev != NULL) {
            maxPrev->next = maxNode->next;
            (*pointerUpdates)++;
        } else {
            start = maxNode->next;
        }

        maxNode->next = NULL;
        (*pointerUpdates)++;

        if (sortedHead == NULL) {
            sortedHead = sortedTail = maxNode;
            (*pointerUpdates) += 2;
        } else {
            sortedTail->next = maxNode;
            sortedTail = maxNode;
            (*pointerUpdates) += 2;
        }

        printf("After pass %d: ", pass);
        displayList(sortedHead);
        pass++;
    }

    return sortedHead;
}

int main() {
    int n, patientID, severity;
    char name[30];
    int comparisons, pointerUpdates;

    printf("Enter number of patients: ");
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        printf("Enter patient ID, name, severity: ");
        scanf("%d %29s %d", &patientID, name, &severity);
        insertAtEnd(patientID, name, severity);
    }

    printf("Original list: ");
    displayList(head);

    head = selectionSort(head, &comparisons, &pointerUpdates);

    printf("Sorted list: ");
    displayList(head);
    printf("Comparisons: %d\n", comparisons);
    printf("Pointer updates: %d\n", pointerUpdates);

    return 0;
}
