#include "stdio.h"
#include "stdlib.h"

struct Item {
    int id;
    char name[40];
    int quantity;
};

void print_items(struct Item items[], int count)
{
    int i;

    printf("Items:\n");
    for (i = 0; i < count; i++) {
        printf("%d %s %d\n", items[i].id, items[i].name, items[i].quantity);
    }
}

int delete_item(struct Item items[], int *count, int id)
{
    int i;
    int found = -1;

    for (i = 0; i < *count; i++) {
        if (items[i].id == id) {
            found = i;
            break;
        }
    }

    if (found == -1) {
        return 0;
    }

    for (i = found; i < *count - 1; i++) {
        items[i] = items[i + 1];
    }

    *count = *count - 1;
    return 1;
}

int main(void)
{
    struct Item *items;
    int capacity;
    int count = 0;
    int choice;
    int id;

    printf("Enter item array size: ");
    scanf("%d", &capacity);

    if (capacity <= 0) {
        printf("Size must be positive.\n");
        return 0;
    }

    items = malloc((unsigned long)capacity * sizeof(struct Item));
    if (items == 0) {
        printf("Memory allocation failed.\n");
        return 0;
    }

    do {
        printf("1. Add item\n");
        printf("2. Delete item\n");
        printf("3. Display items\n");
        printf("4. Exit\n");
        printf("Choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            if (count == capacity) {
                printf("Array is full.\n");
            } else {
                printf("Enter id, name, and quantity: ");
                scanf("%d %39s %d", &items[count].id, items[count].name, &items[count].quantity);
                count++;
            }
        } else if (choice == 2) {
            printf("Enter id to delete: ");
            scanf("%d", &id);
            if (!delete_item(items, &count, id)) {
                printf("Item not found.\n");
            }
        } else if (choice == 3) {
            print_items(items, count);
        }
    } while (choice != 4);

    free(items);
    return 0;
}
