#include "stdio.h"

struct Customer {
    int id;
    char name[50];
    char phone[20];
};

struct Item {
    int code;
    char name[50];
    int quantity;
    double price;
};

struct Order {
    int orderNumber;
    struct Customer customer;
    struct Item item;
};

void print_order(struct Order order)
{
    double total = order.item.quantity * order.item.price;

    printf("Order Number: %d\n", order.orderNumber);
    printf("Customer: %d %s %s\n", order.customer.id, order.customer.name, order.customer.phone);
    printf("Item: %d %s\n", order.item.code, order.item.name);
    printf("Quantity: %d\n", order.item.quantity);
    printf("Unit Price: %.2f\n", order.item.price);
    printf("Total: %.2f\n", total);
}

int main(void)
{
    struct Order order;

    printf("Enter order number: ");
    scanf("%d", &order.orderNumber);

    printf("Enter customer id, name, and phone: ");
    scanf("%d %49s %19s", &order.customer.id, order.customer.name, order.customer.phone);

    printf("Enter item code, name, quantity, and price: ");
    scanf("%d %49s %d %lf", &order.item.code, order.item.name, &order.item.quantity, &order.item.price);

    print_order(order);
    return 0;
}
