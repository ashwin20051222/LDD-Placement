#include "stdio.h"

void sort_numbers(int numbers[], int count)
{
    int i;
    int j;

    for (i = 0; i < count - 1; i++) {
        for (j = i + 1; j < count; j++) {
            if (numbers[i] > numbers[j]) {
                int temp = numbers[i];
                numbers[i] = numbers[j];
                numbers[j] = temp;
            }
        }
    }
}

int main(void)
{
    char inputFile[100];
    char outputFile[100];
    int numbers[200];
    int count = 0;
    int i;
    FILE *input;
    FILE *output;

    printf("Enter input file name: ");
    scanf("%99s", inputFile);

    printf("Enter output file name: ");
    scanf("%99s", outputFile);

    input = fopen(inputFile, "r");
    if (input == 0) {
        printf("Could not open input file.\n");
        return 0;
    }

    while (count < 200 && fscanf(input, "%d", &numbers[count]) == 1) {
        count++;
    }
    fclose(input);

    sort_numbers(numbers, count);

    output = fopen(outputFile, "w");
    if (output == 0) {
        printf("Could not open output file.\n");
        return 0;
    }

    for (i = 0; i < count; i++) {
        fprintf(output, "%d\n", numbers[i]);
    }
    fclose(output);

    printf("%d integers were sorted and written to %s.\n", count, outputFile);
    return 0;
}
