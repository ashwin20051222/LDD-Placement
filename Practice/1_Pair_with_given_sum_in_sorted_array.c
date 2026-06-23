#include "stdio.h"

int main() {
    int n, key;
    printf("Enter the size of the array: ");
    scanf("%d", &n);
    int arr[n];
    printf("Enter the elements of the array in sorted order: ");
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }
    printf("Enter the key value: ");
    scanf("%d", &key);  
    int low = 0, high = n-1;
    while(low < high) {
        if(arr[low] + arr[high] == key) {
            printf("Pair found: %d and %d\n", arr[low], arr[high]);
            low++;
            high--;
            return 0;
        } 
        else if(arr[low] + arr[high] < key) {
            low++;
        } 
        else {
            high--;
        }
    }
    printf("No pair found with the given sum.\n");
    return 0;
}