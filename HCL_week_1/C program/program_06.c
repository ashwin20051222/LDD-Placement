#include "stdio.h"

int main(void)
{
    int first;
    int second;

    printf("Enter two integers: ");
    scanf("%d %d", &first, &second);

    first = first ^ second;
    second = first ^ second;
    first = first ^ second;

    printf("After swapping: %d %d\n", first, second);

    return 0;
}
