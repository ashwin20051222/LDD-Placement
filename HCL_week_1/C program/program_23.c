#include "stdio.h"
#include "stdlib.h"

int text_length(char text[])
{
    int length = 0;

    while (text[length] != '\0' && text[length] != '\n') {
        length++;
    }

    return length;
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

int compare_text(char first[], char second[])
{
    int i = 0;

    while (first[i] != '\0' && second[i] != '\0') {
        if (first[i] != second[i]) {
            return first[i] - second[i];
        }
        i++;
    }

    return first[i] - second[i];
}

int main(void)
{
    char *cities[50];
    char buffer[100];
    int n;
    int i;
    int j;

    printf("How many city names? ");
    scanf("%d", &n);
    getchar();

    if (n <= 0 || n > 50) {
        printf("Enter a count from 1 to 50.\n");
        return 0;
    }

    for (i = 0; i < n; i++) {
        printf("City %d: ", i + 1);
        fgets(buffer, sizeof(buffer), stdin);

        cities[i] = malloc((unsigned long)text_length(buffer) + 1);
        if (cities[i] == 0) {
            printf("Memory allocation failed.\n");
            while (i > 0) {
                i--;
                free(cities[i]);
            }
            return 0;
        }
        copy_text(cities[i], buffer);
    }

    for (i = 0; i < n - 1; i++) {
        for (j = i + 1; j < n; j++) {
            if (compare_text(cities[i], cities[j]) > 0) {
                char *temp = cities[i];
                cities[i] = cities[j];
                cities[j] = temp;
            }
        }
    }

    printf("Cities in alphabetical order:\n");
    for (i = 0; i < n; i++) {
        printf("%s\n", cities[i]);
        free(cities[i]);
    }

    return 0;
}
