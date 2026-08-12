#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// General tree node typedef as requested
typedef struct node {
    char *name;
    bool is_file;
    struct node *child;    // first child
    struct node *sibling;  // next sibling
} node;

// Globals as requested
node *head = NULL; // root
node *tail = NULL; // last inserted (utility)

node* create_node(const char *name, bool is_file) {
    node *n = (node*)malloc(sizeof(node));
    if (!n) { perror("malloc"); exit(1); }
    n->name = strdup(name);
    n->is_file = is_file;
    n->child = n->sibling = NULL;
    return n;
}

// Helper: find node by absolute path (e.g. /a/b/c)
node* find_node_by_path(node *root, const char *path) {
    if (!root || !path) return NULL;
    if (strcmp(path, "/") == 0) return root;

    // tokenize path skipping leading '/'
    char *copy = strdup(path);
    char *p = copy;
    if (*p == '/') p++;

    node *curr = root;
    char *token = NULL;
    while ((token = strsep(&p, "/")) != NULL) {
        if (token[0] == '\0') continue;
        node *child = curr->child;
        while (child) {
            if (strcmp(child->name, token) == 0) break;
            child = child->sibling;
        }
        if (!child) { free(copy); return NULL; }
        curr = child;
    }
    free(copy);
    return curr;
}

// Insert a file or directory under parent path
void insert_under(const char *parent_path, const char *name, bool is_file) {
    if (!head) {
        // create root
        head = create_node("/", false);
        tail = head;
    }
    node *parent = find_node_by_path(head, parent_path);
    if (!parent) { printf("Parent path not found\n"); return; }
    // ensure parent is a directory
    if (parent->is_file) { printf("Cannot add under a file\n"); return; }

    // check duplicate name among siblings (allowed but keep as separate node)
    node *n = create_node(name, is_file);
    // insert at beginning of child list for simplicity
    n->sibling = parent->child;
    parent->child = n;
    tail = n;
}

// DFS display full tree with indentation
void dfs_display(node *curr, int depth) {
    if (!curr) return;
    for (int i=0;i<depth;i++) printf("  ");
    if (depth==0) printf("%s\n", curr->name);
    else printf("%s%s\n", curr->name, curr->is_file?"":"/");
    // children
    node *c = curr->child;
    while (c) { dfs_display(c, depth+1); c = c->sibling; }
}

// Search file by name and print absolute paths (handle duplicates)
bool dfs_search_print(node *curr, const char *target, char *path, int depth) {
    if (!curr) return false;
    bool found = false;

    // build current path
    char saved[1024];
    if (depth == 0) strcpy(saved, "");
    else {
        snprintf(saved, sizeof(saved), "%s/%s", path, curr->name);
        strcpy(path, saved);
    }

    if (curr->is_file && strcmp(curr->name, target) == 0) {
        printf("Found: %s\n", depth==0?"/":path);
        found = true;
    }

    node *c = curr->child;
    while (c) {
        // prepare child path
        char child_path[1024];
        if (depth==0) snprintf(child_path, sizeof(child_path), "%s", curr->name);
        else snprintf(child_path, sizeof(child_path), "%s", path);
        // recursively search
        if (dfs_search_print(c, target, child_path, depth+1)) found = true;
        c = c->sibling;
    }

    // restore path
    if (depth>0) {
        // remove appended part by finding last '/'
        char *slash = strrchr(path, '/');
        if (slash) *slash = '\0';
        else path[0] = '\0';
    }
    return found;
}

// Count files and directories
void dfs_count(node *curr, int *files, int *dirs) {
    if (!curr) return;
    if (curr->is_file) (*files)++;
    else (*dirs)++;
    node *c = curr->child;
    while (c) { dfs_count(c, files, dirs); c = c->sibling; }
}

// Max depth (root depth 0)
int dfs_max_depth(node *curr) {
    if (!curr) return -1;
    int maxd = 0;
    node *c = curr->child;
    while (c) {
        int d = dfs_max_depth(c) + 1;
        if (d > maxd) maxd = d;
        c = c->sibling;
    }
    return maxd;
}

// Display files with given extension
void dfs_files_with_ext(node *curr, const char *ext, char *path, int depth) {
    if (!curr) return;
    char saved[1024];
    if (depth==0) strcpy(saved, "");
    else {
        snprintf(saved, sizeof(saved), "%s/%s", path, curr->name);
        strcpy(path, saved);
    }

    if (curr->is_file) {
        size_t ln = strlen(curr->name);
        size_t el = strlen(ext);
        if (ln >= el && strcmp(curr->name + ln - el, ext) == 0) {
            printf("%s\n", depth==0?"/":path);
        }
    }

    node *c = curr->child;
    while (c) {
        char child_path[1024];
        if (depth==0) snprintf(child_path, sizeof(child_path), "%s", curr->name);
        else snprintf(child_path, sizeof(child_path), "%s", path);
        dfs_files_with_ext(c, ext, child_path, depth+1);
        c = c->sibling;
    }

    if (depth>0) {
        char *slash = strrchr(path, '/');
        if (slash) *slash = '\0'; else path[0] = '\0';
    }
}

// Free tree
void free_tree(node *curr) {
    if (!curr) return;
    node *c = curr->child;
    while (c) { node *n = c->sibling; free_tree(c); c = n; }
    free(curr->name);
    free(curr);
}

int main() {
    head = create_node("/", false);
    tail = head;

    while (1) {
        printf("\nMenu:\n1. Insert file/directory\n2. Display tree (DFS)\n3. Search file by name\n4. Count files & directories\n5. Max depth\n6. Display files by extension\n7. Exit\
Choose: ");
        int choice;
        if (scanf("%d", &choice) != 1) break;
        int c; while ((c = getchar()) != '\n' && c != EOF) {}

        if (choice == 1) {
            char parent[512], name[256], type[16];
            printf("Parent path (e.g. / or /dir/sub): "); fgets(parent, sizeof(parent), stdin); parent[strcspn(parent, "\n")] = '\0';
            if (strlen(parent) == 0) strcpy(parent, "/");
            printf("Name (file or directory name): "); fgets(name, sizeof(name), stdin); name[strcspn(name, "\n")] = '\0';
            printf("Type (file/dir): "); fgets(type, sizeof(type), stdin); type[strcspn(type, "\n")] = '\0';
            bool is_file = (strcmp(type, "file") == 0);
            insert_under(parent, name, is_file);
        } else if (choice == 2) {
            dfs_display(head, 0);
        } else if (choice == 3) {
            char target[256]; printf("Enter file name to search: "); fgets(target, sizeof(target), stdin); target[strcspn(target, "\n")] = '\0';
            char path[1024] = "";
            if (!dfs_search_print(head, target, path, 0)) printf("Not found\n");
        } else if (choice == 4) {
            int files=0, dirs=0; dfs_count(head, &files, &dirs); printf("Directories: %d, Files: %d\n", dirs, files);
        } else if (choice == 5) {
            int d = dfs_max_depth(head); printf("Max depth: %d\n", d);
        } else if (choice == 6) {
            char ext[64]; printf("Enter extension (include dot, e.g. .txt): "); fgets(ext, sizeof(ext), stdin); ext[strcspn(ext, "\n")] = '\0';
            char path[1024] = ""; dfs_files_with_ext(head, ext, path, 0);
        } else break;
    }

    free_tree(head);
    return 0;
}
