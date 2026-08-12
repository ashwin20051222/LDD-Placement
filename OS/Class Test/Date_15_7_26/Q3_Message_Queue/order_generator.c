#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>

typedef struct {
    long messageType;
    int orderId;
    char customerName[50];
    char foodItem[50];
    int quantity;
    char control[10];
} order_message;

int main(void)
{
    key_t key;
    int msgid;
    int choice;
    order_message msg;

    printf("Enter message queue key (integer) : ");
    scanf("%d", &key);

    msgid = msgget(key, IPC_CREAT | 0666);
    if (msgid == -1) {
        perror("msgget failed");
        return 1;
    }

    while (1) {
        printf("\nOrder Generator Menu\n");
        printf("1. Send New Order\n");
        printf("2. Send EXIT Message and Stop\n");
        printf("Enter choice : ");
        scanf("%d", &choice);

        msg.messageType = 1;

        if (choice == 1) {
            printf("Enter Order ID : ");
            scanf("%d", &msg.orderId);

            printf("Enter Customer Name : ");
            scanf(" %49[^\n]", msg.customerName);

            printf("Enter Food Item : ");
            scanf(" %49[^\n]", msg.foodItem);

            printf("Enter Quantity : ");
            scanf("%d", &msg.quantity);

            strcpy(msg.control, "ORDER");

            if (msgsnd(msgid, &msg, sizeof(order_message) - sizeof(long), 0) == -1) {
                perror("msgsnd failed");
                return 1;
            }

            printf("Order sent to kitchen\n");
        } else if (choice == 2) {
            msg.orderId = 0;
            msg.customerName[0] = '\0';
            msg.foodItem[0] = '\0';
            msg.quantity = 0;
            strcpy(msg.control, "EXIT");

            if (msgsnd(msgid, &msg, sizeof(order_message) - sizeof(long), 0) == -1) {
                perror("msgsnd failed");
                return 1;
            }

            printf("EXIT message sent\n");
            break;
        } else {
            printf("Invalid choice\n");
        }
    }

    return 0;
}
