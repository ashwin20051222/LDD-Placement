#include "stdio.h"

double add(double a, double b)
{
    return a + b;
}

double subtract(double a, double b)
{
    return a - b;
}

double multiply(double a, double b)
{
    return a * b;
}

double divide(double a, double b)
{
    return a / b;
}

int main(void)
{
    double first;
    double second;
    int choice;
    double (*operations[4])(double, double) = {add, subtract, multiply, divide};

    printf("1. Add\n");
    printf("2. Subtract\n");
    printf("3. Multiply\n");
    printf("4. Divide\n");
    printf("Choose operation: ");
    scanf("%d", &choice);

    if (choice < 1 || choice > 4) {
        printf("Invalid choice.\n");
        return 0;
    }

    printf("Enter two numbers: ");
    scanf("%lf %lf", &first, &second);

    if (choice == 4 && second == 0.0) {
        printf("Division by zero is not allowed.\n");
        return 0;
    }

    printf("Result = %.2f\n", operations[choice - 1](first, second));
    return 0;
}
