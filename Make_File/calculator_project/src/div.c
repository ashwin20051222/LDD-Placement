#include <stdio.h>
#include "calc.h"

float divide(int a, int b)
{
    if (b == 0)
    {
        printf("Division by zero error\n");
        return 0;
    }

    return (float)a / b;
}
