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
    double (*operation)(double, double) = 0;

    printf("Enter expression like 10 + 5: ");
    scanf("%lf %c %lf", &first, &operator, &second);

    if (operator == '+') {
        operation = add;
    } else if (operator == '-') {
        operation = subtract;
    } else if (operator == '*') {
        operation = multiply;
    } else if (operator == '/') {
        if (second == 0.0) {
            printf("Division by zero is not allowed.\n");
            return 0;
        }
        operation = divide;
    }

    if (operation == 0) {
        printf("Unknown operator.\n");
    } else {
        printf("Result = %.2f\n", operation(first, second));
    }

    return 0;
}
