#include <stdio.h>
#include <stdlib.h>

/* Data structure and globals as required */
typedef struct Node {
    int page;
    struct Node *prev;
    struct Node *next;
} node;

node *head = NULL; /* Most recently used */
node *tail = NULL; /* Least recently used */

/* Helper DLL operations */
void move_to_head(node *n) {
    if (!n || n == head) return;
    /* detach */
    if (n->prev) n->prev->next = n->next;
    if (n->next) n->next->prev = n->prev;
    if (n == tail) tail = n->prev;
    /* attach at head */
    n->prev = NULL;
    n->next = head;
    if (head) head->prev = n;
    head = n;
    if (!tail) tail = head;
}

node* create_node(int page) {
    node *n = (node*)malloc(sizeof(node));
    n->page = page;
    n->prev = n->next = NULL;
    return n;
}

void insert_at_head(node *n) {
    if (!n) return;
    n->prev = NULL;
    n->next = head;
    if (head) head->prev = n;
    head = n;
    if (!tail) tail = head;
}

void remove_tail() {
    if (!tail) return;
    node *t = tail;
    if (t->prev) {
        tail = t->prev;
        tail->next = NULL;
    } else {
        head = tail = NULL;
    }
    free(t);
}

/* Display frames from head (MRU) to tail (LRU) */
void display_frames() {
    node *cur = head;
    printf("[ ");
    while (cur) {
        printf("%d", cur->page);
        cur = cur->next;
        if (cur) printf(" ");
    }
    printf(" ]\n");
}

int main() {
    printf("LRU Page Replacement Simulation\n");
    int frame_size;
    printf("Enter number of frames: ");
    if (scanf("%d", &frame_size) != 1 || frame_size <= 0) return 0;

    int n;
    printf("Enter length of page reference string: ");
    if (scanf("%d", &n) != 1 || n <= 0) return 0;

    int *refs = (int*)malloc(sizeof(int) * n);
    printf("Enter %d page references separated by spaces or newlines:\n", n);
    int max_page = 0;
    for (int i = 0; i < n; ++i) {
        if (scanf("%d", &refs[i]) != 1) { free(refs); return 0; }
        if (refs[i] > max_page) max_page = refs[i];
    }

    /* Create hash table sized by max_page to map page -> node pointer */
    int hash_size = max_page + 1;
    node **hash = (node**)malloc(sizeof(node*) * hash_size);
    for (int i = 0; i < hash_size; ++i) hash[i] = NULL;

    int current_frames = 0;
    int hits = 0, faults = 0;

    for (int i = 0; i < n; ++i) {
        int page = refs[i];
        printf("Accessing page %d: ", page);
        if (page < 0 || page >= hash_size) {
            printf("Invalid page number (out of detected range).\n");
            continue;
        }
        if (hash[page] != NULL) {
            /* Hit: move node to head */
            hits++;
            move_to_head(hash[page]);
            printf("HIT -> ");
            display_frames();
            continue;
        }
        /* Fault */
        faults++;
        if (current_frames < frame_size) {
            /* empty frame available: insert */
            node *nnode = create_node(page);
            insert_at_head(nnode);
            hash[page] = head;
            current_frames++;
            printf("FAULT (loaded into free frame) -> ");
            display_frames();
        } else {
            /* evict LRU (tail) */
            if (!tail) {
                printf("Error: unexpected empty tail.\n");
            } else {
                int victim = tail->page;
                /* remove tail node but keep pointer to free its memory after updating hash */
                node *old_tail = tail;
                if (old_tail->prev) {
                    tail = old_tail->prev;
                    tail->next = NULL;
                } else {
                    head = tail = NULL;
                }
                hash[victim] = NULL; /* evicted */
                free(old_tail);
                /* insert new page at head */
                node *nnode = create_node(page);
                insert_at_head(nnode);
                hash[page] = head;
                printf("FAULT (evicted page %d) -> ", victim);
                display_frames();
            }
        }
    }

    double fault_ratio = (double)faults / (double)n;
    printf("\nTotal references: %d\n", n);
    printf("Page hits: %d\n", hits);
    printf("Page faults: %d\n", faults);
    printf("Page fault ratio: %.4f\n", fault_ratio);

    /* cleanup */
    while (head) {
        node *t = head;
        head = head->next;
        free(t);
    }
    free(hash);
    free(refs);
    return 0;
}
