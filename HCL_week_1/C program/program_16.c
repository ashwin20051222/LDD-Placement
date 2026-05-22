#include "stdio.h"

int find_substring(char text[], char pattern[])
{
    int i;
    int j;

    if (pattern[0] == '\0') {
        return 0;
    }

    for (i = 0; text[i] != '\0'; i++) {
        j = 0;
        while (pattern[j] != '\0' && text[i + j] == pattern[j]) {
            j++;
        }

        if (pattern[j] == '\0') {
            return i;
        }
    }

    return -1;
}

void remove_newline(char text[])
{
    int i = 0;

    while (text[i] != '\0') {
        if (text[i] == '\n') {
            text[i] = '\0';
            return;
        }
        i++;
    }
}

int main(void)
{
    char text[200];
    char pattern[80];
    int index;

    printf("Enter the main string: ");
    fgets(text, sizeof(text), stdin);
    remove_newline(text);

    printf("Enter the substring: ");
    fgets(pattern, sizeof(pattern), stdin);
    remove_newline(pattern);

    index = find_substring(text, pattern);

    if (index >= 0) {
        printf("First occurrence is at index %d.\n", index);
    } else {
        printf("Substring not found.\n");
    }

    return 0;
}
