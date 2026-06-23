#include "stdio.h"

typedef struct {
    int day;
    int month;
    int year;
} Date;

int main(void)
{
    Date joiningDate;

    printf("Enter date as day month year: ");
    scanf("%d %d %d", &joiningDate.day, &joiningDate.month, &joiningDate.year);

    printf("Date = %02d-%02d-%04d\n", joiningDate.day, joiningDate.month, joiningDate.year);
    return 0;
}
