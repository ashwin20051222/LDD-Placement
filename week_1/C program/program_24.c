#include "stdio.h"
#include "stdlib.h"

int text_size(char text[])
{
    int size = 0;

    while (text[size] != '\0' && text[size] != '\n') {
        size++;
    }

    return size;
}

void copy_text(char destination[], char source[])
{
    int i = 0;

    while (source[i] != '\0' && source[i] != '\n') {
        destination[i] = source[i];
        i++;
    }
    destination[i] = '\0';
}

char *make_string(char buffer[])
{
    char *text = malloc((unsigned long)text_size(buffer) + 1);

    if (text != 0) {
        copy_text(text, buffer);
    }

    return text;
}

void swap_pointers(char **first, char **second)
{
    char *temp = *first;
    *first = *second;
    *second = temp;
}

int main(void)
{
    char buffer[100];
    char *first;
    char *second;

    printf("Enter first string: ");
    fgets(buffer, sizeof(buffer), stdin);
    first = make_string(buffer);

    printf("Enter second string: ");
    fgets(buffer, sizeof(buffer), stdin);
    second = make_string(buffer);

    if (first == 0 || second == 0) {
        printf("Memory allocation failed.\n");
        free(first);
        free(second);
        return 0;
    }

    printf("Before swap: %s, %s\n", first, second);
    swap_pointers(&first, &second);
    printf("After swap: %s, %s\n", first, second);

    free(first);
    free(second);
    return 0;
}
