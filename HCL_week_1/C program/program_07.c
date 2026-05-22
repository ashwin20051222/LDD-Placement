#include "stdio.h"

int main(void)
{
    int n;
    int i;
    unsigned long long factorial = 1;

    printf("Enter n: ");
    scanf("%d", &n);

    if (n < 0) {
        printf("Factorial is not defined for negative numbers.\n");
        return 0;
    }

    for (i = 2; i <= n; i++) {
        factorial = factorial * (unsigned long long)i;
    }

    printf("%d! = %llu\n", n, factorial);
    return 0;
}
