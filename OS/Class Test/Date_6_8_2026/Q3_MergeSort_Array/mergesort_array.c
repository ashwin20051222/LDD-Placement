#include <stdio.h>
#include <stdlib.h>

/* Merge Sort using arrays. Shows array after every merge. */

typedef int element_t;

void printArray(element_t arr[], int n)
{
    for (int i = 0; i < n; ++i) {
        if (i) putchar(' ');
        printf("%d", arr[i]);
    }
    putchar('\n');
}

void merge(element_t arr[], int l, int m, int r)
{
    int n1 = m - l + 1;
    int n2 = r - m;
    element_t *L = malloc(n1 * sizeof(element_t));
    element_t *R = malloc(n2 * sizeof(element_t));

    for (int i = 0; i < n1; ++i) L[i] = arr[l + i];
    for (int j = 0; j < n2; ++j) R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) arr[k++] = L[i++];
        else arr[k++] = R[j++];
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];

    /* Print merged portion */
    for (int x = l; x <= r; ++x) {
        if (x > l) putchar(' ');
        printf("%d", arr[x]);
    }
    putchar('\n');

    free(L);
    free(R);
}

void mergeSort(element_t arr[], int l, int r)
{
    if (l >= r) return;
    int m = l + (r - l) / 2;
    mergeSort(arr, l, m);
    mergeSort(arr, m + 1, r);
    merge(arr, l, m, r);
}

int main(void)
{
    int N;
    printf("Enter N: ");
    if (scanf("%d", &N) != 1 || N <= 0) return 0;

    element_t *arr = malloc(N * sizeof(element_t));
    printf("Enter %d integers:\n", N);
    for (int i = 0; i < N; ++i) scanf("%d", &arr[i]);

    printf("\nOriginal Array\n\n");
    printArray(arr, N);

    printf("\nAfter Merge\n\n");
    mergeSort(arr, 0, N - 1);

    printf("\nFinal Sorted Array\n\n");
    printArray(arr, N);

    free(arr);
    return 0;
}
