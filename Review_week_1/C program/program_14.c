#include "stdio.h"

int main(void)
{
    char text[200];
    int i = 0;

    printf("Enter a lowercase string: ");
    fgets(text, sizeof(text), stdin);

    while (text[i] != '\0') {
        if (text[i] >= 'a' && text[i] <= 'z') {
            text[i] = (char)(text[i] - 32);
        }
        i++;
    }

    printf("Uppercase string: %s", text);
    return 0;
}
