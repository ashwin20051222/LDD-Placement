#include "stdio.h"

int contains_digits_1_to_9(int values[])
{
    int seen[10] = {0};
    int i;

    for (i = 0; i < 9; i++) {
        if (values[i] < 1 || values[i] > 9) {
            return 0;
        }

        if (seen[values[i]]) {
            return 0;
        }

        seen[values[i]] = 1;
    }

    return 1;
}

int rows_and_columns_are_valid(int grid[9][9])
{
    int row;
    int col;
    int line[9];

    for (row = 0; row < 9; row++) {
        for (col = 0; col < 9; col++) {
            line[col] = grid[row][col];
        }
        if (!contains_digits_1_to_9(line)) {
            return 0;
        }
    }

    for (col = 0; col < 9; col++) {
        for (row = 0; row < 9; row++) {
            line[row] = grid[row][col];
        }
        if (!contains_digits_1_to_9(line)) {
            return 0;
        }
    }

    return 1;
}

int main(void)
{
    int grid[9][9];
    int row;
    int col;

    printf("Enter 9x9 grid values:\n");
    for (row = 0; row < 9; row++) {
        for (col = 0; col < 9; col++) {
            scanf("%d", &grid[row][col]);
        }
    }

    if (rows_and_columns_are_valid(grid)) {
        printf("Every row and column contains unique numbers from 1 to 9.\n");
    } else {
        printf("At least one row or column is invalid.\n");
    }

    return 0;
}
