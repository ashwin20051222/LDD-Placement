#include "stdio.h"

void isPrime(int n1, int n2) {
    int i = 0, j = 0, flag = 0;
    printf("Prime numbers between %d and %d are:\n", n1, n2);
    for(int i = n1; i <= n2; i++) {
        if(i == 0 || i == 1) { // 0 and 1 are not prime numbers
            continue; // skip the rest of the loop and move to the next iteration
        }
        flag = 1;
        for(int j = 2; j <= i/2; j++) { // why j = i/2 , because if i is divisible by any number greater than i/2 then it will be divisible by a number less than i/2, so we only need to check up to i/2
            if(i % j == 0) { // if i and j are divisible then i is not prime number
                flag = 0;
                break;
            }
        }
        if(flag == 1) {
            printf("%d ", i);
        }
    }
}

int main() {
    int num1, num2;
    printf("Enter the range from start to end:\n");
    scanf("%d %d", &num1, &num2);
    isPrime(num1, num2);
    return 0;
}