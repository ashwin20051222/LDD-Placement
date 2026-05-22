#include "stdio.h"

int main(void)
{
    int year;
    int leap;

    printf("Enter a year: ");
    scanf("%d", &year);

    leap = (year % 400 == 0) ? 1 :
           (year % 100 == 0) ? 0 :
           (year % 4 == 0) ? 1 : 0;

    if (leap) {
        printf("%d is a leap year.\n", year);
    } else {
        printf("%d is not a leap year.\n", year);
    }

    return 0;
}
