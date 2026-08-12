#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Stack using linked list. Typedef name: node. Global head and tail pointers per rod. */

typedef struct node {
    int data;
    struct node *next;
} node;

/* Global heads/tails for rods A, B, C */
node *headA = NULL, *tailA = NULL;
node *headB = NULL, *tailB = NULL;
node *headC = NULL, *tailC = NULL;

void createStack()
{
    headA = tailA = NULL;
    headB = tailB = NULL;
    headC = tailC = NULL;
}

void push_node(node **head, node **tail, int disk)
{
    node *newn = (node *)malloc(sizeof(node));
    if (!newn) return;
    newn->data = disk;
    newn->next = *head;
    *head = newn;
    if (*tail == NULL)
        *tail = newn;
}

int pop_node(node **head, node **tail)
{
    if (*head == NULL)
        return -1; /* empty */
    node *temp = *head;
    int val = temp->data;
    *head = temp->next;
    if (*head == NULL)
        *tail = NULL;
    free(temp);
    return val;
}

int peek_node(node *head)
{
    if (head == NULL)
        return -1;
    return head->data;
}

void displayStack(node *head, node *tail)
{
    /* To print from bottom to top (tail .. head) collect into array and print reverse */
    int arr[1000];
    int c = 0;
    node *cur = head;
    while (cur) {
        arr[c++] = cur->data;
        cur = cur->next;
    }
    /* print from tail to head => reverse of collected array */
    for (int i = c - 1; i >= 0; --i) {
        printf("%d ", arr[i]);
    }
}

void displayAll()
{
    printf("\nA : "); displayStack(headA, tailA); printf("\n");
    printf("B : "); displayStack(headB, tailB); printf("\n");
    printf("C : "); displayStack(headC, tailC); printf("\n");
}

/* wrappers that use char to select rod */
void push(int disk, char rod)
{
    if (rod == 'A') push_node(&headA, &tailA, disk);
    else if (rod == 'B') push_node(&headB, &tailB, disk);
    else if (rod == 'C') push_node(&headC, &tailC, disk);
}

int pop_char(char rod)
{
    if (rod == 'A') return pop_node(&headA, &tailA);
    else if (rod == 'B') return pop_node(&headB, &tailB);
    else return pop_node(&headC, &tailC);
}

void moveDisk(char from, char to)
{
    int disk = pop_char(from);
    if (disk == -1) return;
    push(disk, to);
    printf("\nMove Disk %d from %c to %c\n\n", disk, from, to);
    displayAll();
}

void towerOfHanoi(int n, char source, char auxiliary, char destination)
{
    if (n == 0)
        return;
    towerOfHanoi(n - 1, source, destination, auxiliary);
    moveDisk(source, destination);
    towerOfHanoi(n - 1, auxiliary, source, destination);
}

int main()
{
    int N;
    printf("Enter number of disks: ");
    if (scanf("%d", &N) != 1) return 1;

    createStack();
    /* Push disks into A: largest at bottom (N) ... smallest (1) on top */
    for (int i = N; i >= 1; --i)
        push(i, 'A');

    printf("Initial\n\n");
    displayAll();

    towerOfHanoi(N, 'A', 'B', 'C');
    return 0;
}
