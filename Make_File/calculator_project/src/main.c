#include <stdio.h>
#include "calc.h"

int main()
{
    int a;
    int b;
    printf("A = ");
    scanf("%d", &a);
    printf("\nB = : ");
    scanf("%d", &b);
    printf("Add      : %d\n", add(a, b));
    printf("Subtract : %d\n", sub(a, b));
    printf("Multiply : %d\n", mul(a, b));
    printf("Divide   : %.2f\n", divide(a, b));
    return 0;
}
