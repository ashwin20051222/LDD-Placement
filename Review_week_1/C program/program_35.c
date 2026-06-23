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
    char operator;
    char symbols[4] = {'+', '-', '*', '/'};
    double (*operations[4])(double, double) = {add, subtract, multiply, divide};
    int i;
    int found = -1;

    printf("Enter expression like 8 * 3: ");
    scanf("%lf %c %lf", &first, &operator, &second);

    for (i = 0; i < 4; i++) {
        if (operator == symbols[i]) {
            found = i;
            break;
        }
    }

    if (found == -1) {
        printf("Unknown operation.\n");
    } else if (operator == '/' && second == 0.0) {
        printf("Division by zero is not allowed.\n");
    } else {
        printf("Result = %.2f\n", operations[found](first, second));
    }

    return 0;
}
