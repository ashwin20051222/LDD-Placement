#include "stdio.h"

struct Product {
    int id;
    char name[50];
    double price;
};

double discounted_price(struct Product product, double discountPercent)
{
    return product.price - (product.price * discountPercent / 100.0);
}

void update_price(struct Product *product, double newPrice)
{
    product->price = newPrice;
}

int main(void)
{
    struct Product product;
    double discount;
    double newPrice;

    printf("Enter product id, name, and price: ");
    scanf("%d %49s %lf", &product.id, product.name, &product.price);

    printf("Enter discount percentage: ");
    scanf("%lf", &discount);
    printf("Discounted price = %.2f\n", discounted_price(product, discount));

    printf("Enter new price: ");
    scanf("%lf", &newPrice);
    update_price(&product, newPrice);

    printf("Updated product: %d %s %.2f\n", product.id, product.name, product.price);
    return 0;
}
