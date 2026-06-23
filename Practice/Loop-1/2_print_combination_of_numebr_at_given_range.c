#include "stdio.h" 

int main() {
    int a, b, c;
    printf("Enter the 3 numbers :");
    scanf("%d %d %d", &a, &b, &c);

    for(int i = a; i <= c; i++) {
        for(int j = a; j <= c; j++) {
            for(int k = a; k <= c; k++) {
                printf("%d%d%d  ", i, j, k);
            }
        }
    }
}