#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

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
    int prepSeconds;
    int removeChoice;
    order_message msg;

    printf("Enter message queue key (integer) : ");
    scanf("%d", &key);

    printf("Enter preparation time per order (seconds) : ");
    scanf("%d", &prepSeconds);

    msgid = msgget(key, IPC_CREAT | 0666);
    if (msgid == -1) {
        perror("msgget failed");
        return 1;
    }

    while (1) {
        if (msgrcv(msgid, &msg, sizeof(order_message) - sizeof(long), 1, 0) == -1) {
            perror("msgrcv failed");
            return 1;
        }

        if (strcmp(msg.control, "EXIT") == 0) {
            printf("EXIT message received. Stopping kitchen process\n");
            break;
        }

        printf("\nReceived Order\n");
        printf("Order ID : %d\n", msg.orderId);
        printf("Customer Name : %s\n", msg.customerName);
        printf("Food Item : %s\n", msg.foodItem);
        printf("Quantity : %d\n", msg.quantity);

        printf("Preparing food...\n");
        sleep(prepSeconds);
        printf("Order Completed\n");
    }

    printf("Remove message queue now? (1-Yes, 0-No) : ");
    scanf("%d", &removeChoice);
    if (removeChoice == 1) {
        if (msgctl(msgid, IPC_RMID, NULL) == -1) {
            perror("msgctl IPC_RMID failed");
            return 1;
        }
        printf("Message queue removed\n");
    }

    return 0;
}
