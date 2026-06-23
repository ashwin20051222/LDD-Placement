#include "stdio.h"

int main(void)
{
    char text[200];
    char remove;
    int readIndex = 0;
    int writeIndex = 0;

    printf("Enter a string: ");
    fgets(text, sizeof(text), stdin);

    printf("Enter the character to remove: ");
    scanf(" %c", &remove);

    while (text[readIndex] != '\0') {
        if (text[readIndex] != remove) {
            text[writeIndex] = text[readIndex];
            writeIndex++;
        }
        readIndex++;
    }
    text[writeIndex] = '\0';

    printf("After removing '%c': %s", remove, text);
    return 0;
}
