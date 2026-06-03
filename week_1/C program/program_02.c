#include "stdio.h"

int main(void)
{
    int first;
    int second;

    printf("Enter two integers: ");
    scanf("%d %d", &first, &second);

    printf("Sum = %d\n", first + second);
    printf("Difference = %d\n", first - second);
    printf("Product = %d\n", first * second);

    if (second == 0) {
        printf("Integer quotient is not possible because divisor is zero.\n");
    } else {
        printf("Integer quotient = %d\n", first / second);
    }

    return 0;
}
