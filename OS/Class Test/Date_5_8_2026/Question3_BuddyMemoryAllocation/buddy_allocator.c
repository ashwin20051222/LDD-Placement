#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Simple Buddy Allocator Simulation
   Typedef name node; global head and tail used for the block list. */

typedef struct node {
    int size;         /* block size in KB (power of two) */
    int start;        /* starting address in KB */
    int is_free;      /* 1 free, 0 allocated */
    char pid[32];     /* process id if allocated */
    struct node *next;
} node;

node *head = NULL, *tail = NULL;
const int TOTAL = 1024; /* KB */

void initializeMemory()
{
    head = (node *)malloc(sizeof(node));
    head->size = TOTAL;
    head->start = 0;
    head->is_free = 1;
    head->pid[0] = '\0';
    head->next = NULL;
    tail = head;
}

int next_power_of_two(int x)
{
    int p = 1;
    while (p < x) p <<= 1;
    return p;
}

/* split a block into two buddies (replace blk by two halves) */
void splitBlock(node *blk)
{
    if (!blk) return;
    int half = blk->size / 2;
    node *b1 = blk;
    node *b2 = (node *)malloc(sizeof(node));
    b2->size = half;
    b2->start = blk->start + half;
    b2->is_free = 1;
    b2->pid[0] = '\0';
    b2->next = blk->next;

    b1->size = half;
    b1->is_free = 1;
    b1->pid[0] = '\0';
    b1->next = b2;

    if (tail == b1) tail = b2;
}

/* find smallest free block >= reqSize */
node *findBlock(int reqSize)
{
    node *cur = head;
    node *best = NULL;
    while (cur) {
        if (cur->is_free && cur->size >= reqSize) {
            if (!best || cur->size < best->size)
                best = cur;
        }
        cur = cur->next;
    }
    return best;
}

/* allocateMemory by process id and requested KB */
void allocateMemory()
{
    char pid[32];
    int size;
    printf("Enter Process ID: ");
    scanf("%31s", pid);
    printf("Enter Size (KB): ");
    if (scanf("%d", &size) != 1) return;

    int need = next_power_of_two(size);
    if (need > TOTAL) {
        printf("Requested size too large.\n");
        return;
    }

    node *blk = findBlock(need);
    if (!blk) {
        printf("No suitable block available.\n");
        return;
    }

    while (blk->size > need) {
        splitBlock(blk);
        /* after split, blk remains the first half with smaller size */
    }
    blk->is_free = 0;
    strncpy(blk->pid, pid, sizeof(blk->pid)-1);
    blk->pid[sizeof(blk->pid)-1] = '\0';
    printf("Allocated %d KB to %s at address %d\n", blk->size, blk->pid, blk->start);
}

/* helper to remove a node from list */
void removeNode(node *prev, node *cur)
{
    if (!prev) head = cur->next;
    else prev->next = cur->next;
    if (cur == tail) {
        if (prev) tail = prev;
        else tail = NULL;
    }
    free(cur);
}

/* find buddy block (same size, adjacent) */
node *findBuddy(node **prev_out, node *blk)
{
    node *cur = head;
    node *prev = NULL;
    while (cur) {
        if (cur != blk && cur->size == blk->size && cur->is_free) {
            /* buddies if start addresses align and are adjacent pairs */
            int a = blk->start;
            int b = cur->start;
            int s = blk->size;
            if ((a ^ b) == s) {
                if (prev_out) *prev_out = prev;
                return cur;
            }
        }
        prev = cur;
        cur = cur->next;
    }
    return NULL;
}

/* mergeBuddy: after freeing, try merging recursively */
void mergeBuddy(node *blk)
{
    while (1) {
        node *prevBuddy = NULL;
        node *buddy = findBuddy(&prevBuddy, blk);
        if (!buddy) break;
        /* remove buddy and blk from list and create merged block */
        /* determine new start */
        int newStart = blk->start < buddy->start ? blk->start : buddy->start;
        /* remove higher-address node first to preserve prev pointers */
        node *prevBlk = NULL, *cur = head;
        while (cur && cur != blk) { prevBlk = cur; cur = cur->next; }
        /* remove blk and buddy */
        removeNode(prevBlk, blk);
        removeNode(prevBuddy, buddy);
        /* create merged node */
        node *m = (node *)malloc(sizeof(node));
        m->size = blk->size * 2;
        m->start = newStart;
        m->is_free = 1;
        m->pid[0] = '\0';
        /* insert at head for simplicity */
        m->next = head;
        head = m;
        blk = m; /* try merging higher */
    }
}

void freeMemory()
{
    char pid[32];
    printf("Enter Process ID to free: ");
    scanf("%31s", pid);
    node *cur = head;
    while (cur) {
        if (!cur->is_free && strcmp(cur->pid, pid) == 0) {
            cur->is_free = 1;
            cur->pid[0] = '\0';
            printf("Freed block at address %d size %d KB\n", cur->start, cur->size);
            mergeBuddy(cur);
            return;
        }
        cur = cur->next;
    }
    printf("Process ID not found.\n");
}

void displayMemory()
{
    /* sort by start address for display (simple n^2) */
    /* count nodes */
    int n = 0;
    node *cur = head;
    while (cur) { n++; cur = cur->next; }
    node **arr = (node **)malloc(sizeof(node *) * (n ? n : 1));
    cur = head; int i = 0;
    while (cur) { arr[i++] = cur; cur = cur->next; }
    for (int a = 0; a < i; ++a)
        for (int b = a + 1; b < i; ++b)
            if (arr[a]->start > arr[b]->start) {
                node *t = arr[a]; arr[a] = arr[b]; arr[b] = t;
            }

    printf("\nBlock Size\tStart\tEnd\tStatus\tProcess\n");
    for (int j = 0; j < i; ++j) {
        node *p = arr[j];
        printf("%8d KB\t%5d\t%5d\t%s\t%s\n",
               p->size, p->start, p->start + p->size - 1,
               p->is_free ? "Free" : "Allocated",
               p->is_free ? "-" : p->pid);
    }
    free(arr);
}

int main()
{
    initializeMemory();
    int choice;
    while (1) {
        printf("\nMemory = %d KB\n", TOTAL);
        printf("1 Allocate\n2 Free\n3 Display\n4 Exit\nChoose: ");
        if (scanf("%d", &choice) != 1) break;
        switch (choice) {
            case 1: allocateMemory(); break;
            case 2: freeMemory(); break;
            case 3: displayMemory(); break;
            case 4: return 0;
            default: printf("Invalid choice\n");
        }
    }
    return 0;
}
