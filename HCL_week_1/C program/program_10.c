#include "stdio.h"

int is_prime(int n)
{
    int i;

    if (n <= 1) {
        return 0;
    }

    if (n == 2) {
        return 1;
    }

    if (n % 2 == 0) {
        return 0;
    }

    for (i = 3; i <= n / i; i = i + 2) {
        if (n % i == 0) {
            return 0;
        }
    }

    return 1;
}

int is_armstrong(int n)
{
    int original = n;
    int temp = n;
    int digits = 0;
    int sum = 0;

    if (n < 0) {
        return 0;
    }

    if (temp == 0) {
        digits = 1;
    }

    while (temp > 0) {
        digits++;
        temp = temp / 10;
    }

    temp = n;
    while (temp > 0) {
        int digit = temp % 10;
        int power = 1;
        int i;

        for (i = 0; i < digits; i++) {
            power = power * digit;
        }

        sum = sum + power;
        temp = temp / 10;
    }

    return sum == original;
}

int is_perfect(int n)
{
    int sum = 1;
    int i;

    if (n <= 1) {
        return 0;
    }

    for (i = 2; i <= n / i; i++) {
        if (n % i == 0) {
            sum = sum + i;
            if (i != n / i) {
                sum = sum + (n / i);
            }
        }
    }

    return sum == n;
}

int main(void)
{
    int number;

    printf("Enter a number: ");
    scanf("%d", &number);

    printf("Prime: %s\n", is_prime(number) ? "yes" : "no");
    printf("Armstrong: %s\n", is_armstrong(number) ? "yes" : "no");
    printf("Perfect: %s\n", is_perfect(number) ? "yes" : "no");

    return 0;
}
