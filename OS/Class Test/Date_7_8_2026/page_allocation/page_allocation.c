#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Data structure and globals as required */
typedef struct Node {
    int pid;
    int page_no;
    int frame_no;
    struct Node *next;
} node;

node *head = NULL;
node *tail = NULL;

/* Frame table: stores pid or -1 if free */
int *frames = NULL;
int num_frames = 0;

/* Page tables per process: page_table[pid][page_no] = frame_no or -1 */
int **page_table = NULL;
int num_processes = 0;
int max_pages_per_process = 0;

/* Append mapping node to linked list */
void append_node(int pid, int page_no, int frame_no) {
    node *n = (node*)malloc(sizeof(node));
    n->pid = pid;
    n->page_no = page_no;
    n->frame_no = frame_no;
    n->next = NULL;
    if (!head) {
        head = tail = n;
    } else {
        tail->next = n;
        tail = n;
    }
}

/* Remove mapping node for given pid and page_no; returns 1 if removed */
int remove_mapping(int pid, int page_no) {
    node *cur = head, *prev = NULL;
    while (cur) {
        if (cur->pid == pid && cur->page_no == page_no) {
            if (prev) prev->next = cur->next;
            else head = cur->next;
            if (cur == tail) tail = prev;
            free(cur);
            return 1;
        }
        prev = cur;
        cur = cur->next;
    }
    return 0;
}

/* Find first free frame index or -1 */
int find_free_frame() {
    for (int i = 0; i < num_frames; ++i) {
        if (frames[i] == -1) return i;
    }
    return -1;
}

/* Display frames status */
void display_frames_status() {
    printf("\nFrame allocation status:\n");
    printf("Frame\tStatus\n");
    for (int i = 0; i < num_frames; ++i) {
        if (frames[i] == -1) {
            printf("%d\tFREE\n", i);
        } else {
            /* Find which page occupies this frame to show page number */
            node *cur = head;
            int shown = 0;
            while (cur) {
                if (cur->frame_no == i) {
                    printf("%d\tPID %d - Page %d\n", i, cur->pid, cur->page_no);
                    shown = 1;
                    break;
                }
                cur = cur->next;
            }
            if (!shown) printf("%d\tPID %d (unknown page)\n", i, frames[i]);
        }
    }
    printf("\n");
}

/* Allocate a page for a given process */
void allocate_page() {
    int pid, page_no;
    printf("Enter process id (0 to %d): ", num_processes - 1);
    if (scanf("%d", &pid) != 1) return;
    if (pid < 0 || pid >= num_processes) {
        printf("Invalid PID.\n");
        return;
    }
    printf("Enter page number (0 to %d): ", max_pages_per_process - 1);
    if (scanf("%d", &page_no) != 1) return;
    if (page_no < 0 || page_no >= max_pages_per_process) {
        printf("Invalid page number.\n");
        return;
    }
    if (page_table[pid][page_no] != -1) {
        printf("Page already allocated in frame %d for PID %d.\n", page_table[pid][page_no], pid);
        return;
    }
    int frame = find_free_frame();
    if (frame == -1) {
        printf("Memory is full. No free frame available.\n");
        return;
    }
    /* mark frame with pid for display and update page table */
    frames[frame] = pid;
    page_table[pid][page_no] = frame;
    append_node(pid, page_no, frame);
    printf("Allocated frame %d to PID %d page %d.\n", frame, pid, page_no);
}

/* Deallocate a page for a given process */
void deallocate_page() {
    int pid, page_no;
    printf("Enter process id (0 to %d): ", num_processes - 1);
    if (scanf("%d", &pid) != 1) return;
    if (pid < 0 || pid >= num_processes) {
        printf("Invalid PID.\n");
        return;
    }
    printf("Enter page number (0 to %d): ", max_pages_per_process - 1);
    if (scanf("%d", &page_no) != 1) return;
    if (page_no < 0 || page_no >= max_pages_per_process) {
        printf("Invalid page number.\n");
        return;
    }
    int frame = page_table[pid][page_no];
    if (frame == -1) {
        printf("Specified page is not allocated.\n");
        return;
    }
    /* free frame and remove mapping */
    frames[frame] = -1;
    page_table[pid][page_no] = -1;
    if (remove_mapping(pid, page_no)) {
        printf("Deallocated frame %d from PID %d page %d.\n", frame, pid, page_no);
    } else {
        printf("Deallocated frame %d but mapping not found in list.\n", frame);
    }
}

int main() {
    printf("Simple Page Allocation Simulation\n");
    printf("Enter number of physical frames: ");
    if (scanf("%d", &num_frames) != 1 || num_frames <= 0) return 0;
    printf("Enter number of processes: ");
    if (scanf("%d", &num_processes) != 1 || num_processes <= 0) return 0;
    printf("Enter max pages per process: ");
    if (scanf("%d", &max_pages_per_process) != 1 || max_pages_per_process <= 0) return 0;

    frames = (int*)malloc(sizeof(int) * num_frames);
    for (int i = 0; i < num_frames; ++i) frames[i] = -1;

    page_table = (int**)malloc(sizeof(int*) * num_processes);
    for (int p = 0; p < num_processes; ++p) {
        page_table[p] = (int*)malloc(sizeof(int) * max_pages_per_process);
        for (int j = 0; j < max_pages_per_process; ++j) page_table[p][j] = -1;
    }

    while (1) {
        printf("\nOptions:\n1. Allocate page\n2. Deallocate page\n3. Display frame status\n4. Exit\nChoose: ");
        int choice;
        if (scanf("%d", &choice) != 1) break;
        switch (choice) {
            case 1: allocate_page(); display_frames_status(); break;
            case 2: deallocate_page(); display_frames_status(); break;
            case 3: display_frames_status(); break;
            case 4: goto cleanup;
            default: printf("Invalid choice.\n");
        }
    }

cleanup:
    /* free memory */
    while (head) {
        node *t = head;
        head = head->next;
        free(t);
    }
    for (int p = 0; p < num_processes; ++p) free(page_table[p]);
    free(page_table);
    free(frames);
    printf("Exiting.\n");
    return 0;
}
