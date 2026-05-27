/*
Question : Program to find the position of a target value within an array using linear search. 
*/

#include "stdio.h"

//Linear search function
int LinearSearch(int *arr, int n, int key) {
	for(int i = 0; i < n; i++) {
		if(arr[i] == key) {
			printf("The position of the  target elements %d is : %d\n", key, i+1);
			return 0;
		}
	}
	printf("Target Element not found\n");
	return -1;
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
	
	//Get the target elements
	printf("Enter the elements to find :");
	int k;
	scanf("%d", &k);
	
	LinearSearch(arr, n, k);
	return 0;
}
