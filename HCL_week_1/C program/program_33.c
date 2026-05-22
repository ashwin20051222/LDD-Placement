#include "stdio.h"

enum Status {
    PENDING = 1,
    SHIPPED,
    DELIVERED
};

void print_status(enum Status status)
{
    if (status == PENDING) {
        printf("Status: PENDING\n");
    } else if (status == SHIPPED) {
        printf("Status: SHIPPED\n");
    } else if (status == DELIVERED) {
        printf("Status: DELIVERED\n");
    } else {
        printf("Status: UNKNOWN\n");
    }
}

int main(void)
{
    enum Status status = PENDING;
    int choice;

    print_status(status);

    printf("Update status\n");
    printf("1. Pending\n");
    printf("2. Shipped\n");
    printf("3. Delivered\n");
    printf("Choice: ");
    scanf("%d", &choice);

    if (choice >= 1 && choice <= 3) {
        status = (enum Status)choice;
    }

    print_status(status);
    return 0;
}
