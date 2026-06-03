#include "stdio.h"

int main(void)
{
    int values[50];
    int n;
    int *p;
    int *end;
    int sum = 0;

    printf("How many numbers? ");
    scanf("%d", &n);

    if (n <= 0 || n > 50) {
        printf("Enter a size from 1 to 50.\n");
        return 0;
    }

    printf("Enter %d numbers: ", n);
    for (p = values; p < values + n; p++) {
        scanf("%d", p);
    }

    end = values + n;
    for (p = values; p < end; p++) {
        sum = sum + *p;
    }

    printf("Average = %.2f\n", (double)sum / n);
    return 0;
}
