/*
Question : Program that sorts numbers using the QuickSort method.
*/

#include "stdio.h"
#include "stdlib.h"

int compare(const void *a, const void *b) {
	return (*(int*)a - *(int *)b);
}

int main() {

	int n;
	//Get the size of array
	printf("Enter the size of an array :");
	scanf("%d", &n);
	
	int arr[n];
	//Get the array elements
	printf("Enter the array elements :\n");
	for(int i = 0; i < n; i++) {
		printf("Element %d :", i+1);
		scanf("%d", &arr[i]);
	}
	
	qsort(arr, n, sizeof(int), compare);
	printf("Sorted array elements :\n");
	for(int i = 0; i < n; i++) {
		printf("Element %d : %d\n", i+1, arr[i]);
	}
	return 0;
}
