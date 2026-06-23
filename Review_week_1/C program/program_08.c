#include "stdio.h"

long long fibonacci_recursive(int n)
{
    if (n <= 1) {
        return n;
    }

    return fibonacci_recursive(n - 1) + fibonacci_recursive(n - 2);
}

void print_fibonacci_iterative(int count)
{
    int i;
    long long first = 0;
    long long second = 1;
    long long next;

    for (i = 0; i < count; i++) {
        if (i == 0) {
            printf("%lld ", first);
        } else if (i == 1) {
            printf("%lld ", second);
        } else {
            next = first + second;
            printf("%lld ", next);
            first = second;
            second = next;
        }
    }

    printf("\n");
}

int main(void)
{
    int count;
    int i;

    printf("How many Fibonacci numbers do you want? ");
    scanf("%d", &count);

    if (count <= 0) {
        printf("Please enter a positive count.\n");
        return 0;
    }

    printf("Iterative: ");
    print_fibonacci_iterative(count);

    printf("Recursive: ");
    for (i = 0; i < count; i++) {
        printf("%lld ", fibonacci_recursive(i));
    }
    printf("\n");

    return 0;
}
