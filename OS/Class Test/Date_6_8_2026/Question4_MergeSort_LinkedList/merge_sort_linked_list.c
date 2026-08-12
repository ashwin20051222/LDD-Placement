#include <stdio.h>
#include <stdlib.h>

// Typedef for the singly linked list node
typedef struct Node {
    int data;
    struct Node* next;
} Node;

// Global head and tail pointers
Node* head = NULL;
Node* tail = NULL;

// Function to create a new node
Node* createNode(int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

// Function to insert a node at the end of the list
void insertNode(int data) {
    Node* newNode = createNode(data);
    if (head == NULL) {
        head = newNode;
        tail = newNode;
        return;
    }
    tail->next = newNode;
    tail = newNode;
}

// Function to display the linked list
void displayList() {
    Node* temp = head;
    while (temp != NULL) {
        printf("%d", temp->data);
        if (temp->next != NULL) {
            printf(" -> ");
        }
        temp = temp->next;
    }
    printf("\n");
}

// Function to split the linked list into two halves using slow-fast pointer technique
void splitList(Node* source, Node** front, Node** back) {
    Node* slow = source;
    Node* fast = source->next;

    // Move fast by two and slow by one
    // When fast reaches end, slow will be at middle
    while (fast != NULL) {
        fast = fast->next;
        if (fast != NULL) {
            slow = slow->next;
            fast = fast->next;
        }
    }

    // slow is before the midpoint, split at this point
    *front = source;
    *back = slow->next;
    slow->next = NULL;
}

// Function to merge two sorted linked lists
Node* mergeLists(Node* a, Node* b) {
    Node* result = NULL;

    // Base cases
    if (a == NULL) return b;
    if (b == NULL) return a;

    // Pick either a or b, and recur
    if (a->data <= b->data) {
        result = a;
        result->next = mergeLists(a->next, b);
    } else {
        result = b;
        result->next = mergeLists(a, b->next);
    }
    return result;
}

// Function to perform merge sort on the linked list
void mergeSort(Node** headRef) {
    Node* h = *headRef;
    Node* a;
    Node* b;

    // Base case: 0 or 1 elements
    if (h == NULL || h->next == NULL) {
        return;
    }

    // Split the list into two halves
    splitList(h, &a, &b);

    // Recursively sort the sublists
    mergeSort(&a);
    mergeSort(&b);

    // Merge the two sorted halves
    *headRef = mergeLists(a, b);
}

int main() {
    int n, value;

    // Read number of elements
    printf("Enter number of elements: ");
    scanf("%d", &n);

    // Read elements and create the linked list
    printf("Enter %d elements:\n", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &value);
        insertNode(value);
    }

    // Display original list
    printf("\nOriginal List\n");
    displayList();

    // Sort the list using merge sort
    mergeSort(&head);

    // Display sorted list
    printf("\nSorted List\n");
    displayList();

    return 0;
}
