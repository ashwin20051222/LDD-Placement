#include "stdio.h"

int copy_bounded(char destination[], int size, char source[])
{
    int i = 0;
    int truncated = 0;

    if (size <= 0) {
        return 1;
    }

    while (source[i] != '\0' && source[i] != '\n' && i < size - 1) {
        destination[i] = source[i];
        i++;
    }

    if (source[i] != '\0' && source[i] != '\n') {
        truncated = 1;
    }

    destination[i] = '\0';
    return truncated;
}

int main(void)
{
    char source[100];
    char copied[20];
    int truncated;

    printf("Enter text to copy: ");
    fgets(source, sizeof(source), stdin);

    truncated = copy_bounded(copied, sizeof(copied), source);

    printf("Copied text: %s\n", copied);
    if (truncated) {
        printf("The source was longer than the destination, so extra characters were skipped.\n");
    }
    printf("A bounded copy prevents writing outside the destination buffer.\n");

    return 0;
}
