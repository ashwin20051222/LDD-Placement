#include "stdio.h"

int same_word(char first[], char second[])
{
    int i = 0;

    while (first[i] != '\0' && second[i] != '\0') {
        if (first[i] != second[i]) {
            return 0;
        }
        i++;
    }

    return first[i] == second[i];
}

void copy_word(char destination[], char source[])
{
    int i = 0;

    while (source[i] != '\0') {
        destination[i] = source[i];
        i++;
    }
    destination[i] = '\0';
}

char lower_char(char ch)
{
    if (ch >= 'A' && ch <= 'Z') {
        return (char)(ch + 32);
    }

    return ch;
}

int is_word_char(char ch)
{
    return (ch >= 'a' && ch <= 'z') ||
           (ch >= 'A' && ch <= 'Z') ||
           (ch >= '0' && ch <= '9');
}

int main(void)
{
    char paragraph[500];
    char words[100][40];
    int counts[100];
    char current[40];
    int unique = 0;
    int currentLength = 0;
    int i = 0;
    int j;

    printf("Enter a paragraph: ");
    fgets(paragraph, sizeof(paragraph), stdin);

    while (1) {
        if (is_word_char(paragraph[i]) && currentLength < 39) {
            current[currentLength] = lower_char(paragraph[i]);
            currentLength++;
        } else if (currentLength > 0) {
            current[currentLength] = '\0';

            for (j = 0; j < unique; j++) {
                if (same_word(words[j], current)) {
                    counts[j]++;
                    break;
                }
            }

            if (j == unique && unique < 100) {
                copy_word(words[unique], current);
                counts[unique] = 1;
                unique++;
            }

            currentLength = 0;
        }

        if (paragraph[i] == '\0') {
            break;
        }
        i++;
    }

    printf("Word frequencies:\n");
    for (i = 0; i < unique; i++) {
        printf("%s : %d\n", words[i], counts[i]);
    }

    return 0;
}
