#include <stdio.h>

long totalMoves = 0;

void towerOfHanoi(int n, char source, char auxiliary, char destination)
{
    if (n == 0)
        return;

    towerOfHanoi(n - 1, source, destination, auxiliary);
    printf("Move Disk %d from %c to %c\n", n, source, destination);
    totalMoves++;
    towerOfHanoi(n - 1, auxiliary, source, destination);
}

int main()
{
    int N;
    printf("Enter number of disks: ");
    if (scanf("%d", &N) != 1)
        return 1;

    towerOfHanoi(N, 'A', 'B', 'C');

    printf("\nTotal Moves = %ld\n", totalMoves);
    return 0;
}
