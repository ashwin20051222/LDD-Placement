#include "stdio.h"

int main(void)
{
    int number;
    long long n;
    long long reversed = 0;
    int digit;
    int sign = 1;
    int overflow = 0;

    printf("Enter a 32-bit integer: ");
    scanf("%d", &number);

    n = number;
    if (n < 0) {
        sign = -1;
        n = -n;
    }

    while (n > 0) {
        digit = (int)(n % 10);
        if (reversed > (2147483647LL - digit) / 10) {
            overflow = 1;
            break;
        }
        reversed = reversed * 10 + digit;
        n = n / 10;
    }

    if (sign < 0 && reversed > 2147483648LL) {
        overflow = 1;
    }

    if (overflow) {
        printf("Reversing this value crosses the 32-bit int range.\n");
    } else {
        printf("Reversed integer = %lld\n", reversed * sign);
    }

    return 0;
}
