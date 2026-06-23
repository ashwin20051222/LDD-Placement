#include "stdio.h"

union Data {
    int number;
    float decimal;
    char text[30];
};

struct DataRecord {
    int number;
    float decimal;
    char text[30];
};

void copy_text(char destination[], char source[])
{
    int i = 0;

    while (source[i] != '\0' && source[i] != '\n') {
        destination[i] = source[i];
        i++;
    }
    destination[i] = '\0';
}

int main(void)
{
    union Data data;
    char buffer[30];
    int choice;

    printf("1. Store integer\n");
    printf("2. Store float\n");
    printf("3. Store string\n");
    printf("Choice: ");
    scanf("%d", &choice);
    getchar();

    if (choice == 1) {
        printf("Enter integer: ");
        scanf("%d", &data.number);
        printf("Union value = %d\n", data.number);
    } else if (choice == 2) {
        printf("Enter float: ");
        scanf("%f", &data.decimal);
        printf("Union value = %.2f\n", data.decimal);
    } else if (choice == 3) {
        printf("Enter string: ");
        fgets(buffer, sizeof(buffer), stdin);
        copy_text(data.text, buffer);
        printf("Union value = %s\n", data.text);
    } else {
        printf("Invalid choice.\n");
    }

    printf("Size of union = %lu bytes\n", (unsigned long)sizeof(union Data));
    printf("Size of structure = %lu bytes\n", (unsigned long)sizeof(struct DataRecord));

    return 0;
}
