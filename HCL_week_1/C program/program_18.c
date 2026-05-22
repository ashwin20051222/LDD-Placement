#include "stdio.h"

int main(void)
{
    int first[50];
    int second[50];
    int merged[100];
    int n1;
    int n2;
    int i = 0;
    int j = 0;
    int k = 0;

    printf("Enter size of first sorted array: ");
    scanf("%d", &n1);

    if (n1 < 0 || n1 > 50) {
        printf("Size must be from 0 to 50.\n");
        return 0;
    }

    printf("Enter first sorted array: ");
    for (i = 0; i < n1; i++) {
        scanf("%d", &first[i]);
    }

    printf("Enter size of second sorted array: ");
    scanf("%d", &n2);

    if (n2 < 0 || n2 > 50) {
        printf("Size must be from 0 to 50.\n");
        return 0;
    }

    printf("Enter second sorted array: ");
    for (i = 0; i < n2; i++) {
        scanf("%d", &second[i]);
    }

    i = 0;
    while (i < n1 && j < n2) {
        if (first[i] <= second[j]) {
            merged[k] = first[i];
            i++;
        } else {
            merged[k] = second[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        merged[k] = first[i];
        i++;
        k++;
    }

    while (j < n2) {
        merged[k] = second[j];
        j++;
        k++;
    }

    printf("Merged sorted array: ");
    for (i = 0; i < k; i++) {
        printf("%d ", merged[i]);
    }
    printf("\n");

    return 0;
}
