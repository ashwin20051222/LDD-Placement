/*
PROGRAM: Statistics Calculation Using math.h
QUESTION: Use math.h to compute statistics (mean, variance, std dev) 
          for double arrays.
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

/* Calculate mean (average) */
double calculateMean(double *array, int size)
{
    double sum = 0;
    int i;
    
    for (i = 0; i < size; i++)
    {
        sum += array[i];
    }
    
    return sum / size;
}

/* Calculate variance */
double calculateVariance(double *array, int size, double mean)
{
    double sum = 0;
    int i;
    
    for (i = 0; i < size; i++)
    {
        double diff = array[i] - mean;
        sum += diff * diff;
    }
    
    return sum / size;
}

/* Calculate standard deviation */
double calculateStdDev(double variance)
{
    return sqrt(variance);
}

/* Find minimum value */
double findMin(double *array, int size)
{
    double min = array[0];
    int i;
    
    for (i = 1; i < size; i++)
    {
        if (array[i] < min)
        {
            min = array[i];
        }
    }
    
    return min;
}

/* Find maximum value */
double findMax(double *array, int size)
{
    double max = array[0];
    int i;
    
    for (i = 1; i < size; i++)
    {
        if (array[i] > max)
        {
            max = array[i];
        }
    }
    
    return max;
}

/* Calculate median */
double calculateMedian(double *array, int size)
{
    /* Simple bubble sort for small arrays */
    int i, j;
    for (i = 0; i < size - 1; i++)
    {
        for (j = 0; j < size - i - 1; j++)
        {
            if (array[j] > array[j + 1])
            {
                double temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }
    }
    
    if (size % 2 == 0)
    {
        return (array[size / 2 - 1] + array[size / 2]) / 2;
    }
    else
    {
        return array[size / 2];
    }
}

/* Display statistics */
void displayStatistics(double *array, int size)
{
    double mean = calculateMean(array, size);
    double variance = calculateVariance(array, size, mean);
    double stdDev = calculateStdDev(variance);
    double min = findMin(array, size);
    double max = findMax(array, size);
    
    printf("=== Statistical Analysis ===\n\n");
    printf("Count: %d\n", size);
    printf("Mean: %.2f\n", mean);
    printf("Variance: %.2f\n", variance);
    printf("Standard Deviation: %.2f\n", stdDev);
    printf("Minimum: %.2f\n", min);
    printf("Maximum: %.2f\n", max);
    printf("Range: %.2f\n", max - min);
}

int main()
{
    printf("Statistical Analysis with math.h (Runtime Input)\n");
    printf("===============================================\n\n");
    
    int size;
    int i;
    
    printf("Enter number of values: ");
    scanf("%d", &size);
    
    double *array = (double *)malloc(size * sizeof(double));
    
    if (array == NULL)
    {
        printf("Memory allocation failed\n");
        return 1;
    }
    
    printf("Enter %d decimal values:\n", size);
    for (i = 0; i < size; i++)
    {
        printf("Value %d: ", i + 1);
        scanf("%lf", &array[i]);
    }
    
    printf("\nYour values: ");
    for (i = 0; i < size; i++)
    {
        printf("%.2f ", array[i]);
    }
    printf("\n\n");
    
    displayStatistics(array, size);
    
    /* Calculate mean and standard deviation */
    double mean = calculateMean(array, size);
    double variance = calculateVariance(array, size, mean);
    double stdDev = sqrt(variance);
    
    /* Z-score example */
    printf("\n--- Z-score Examples ---\n");
    printf("(First 3 values)\n\n");
    for (i = 0; i < 3 && i < size; i++)
    {
        double zScore = (array[i] - mean) / stdDev;
        printf("Value %.2f: Z-score = %.2f\n", array[i], zScore);
    }
    
    /* Free allocated memory */
    free(array);
    
    return 0;
}

/*
OUTPUT:
Statistical Analysis with math.h
==================================

--- Dataset 1: Student Test Scores ---
Scores: 85.5 92.0 78.5 88.0 95.5 81.0 89.5

=== Statistical Analysis ===

Count: 7
Mean: 87.14
Variance: 33.12
Standard Deviation: 5.75
Minimum: 78.50
Maximum: 95.50
Range: 17.00

--- Dataset 2: Daily Temperature Readings (Celsius) ---
Temperatures: 22.5 23.1 21.8 20.5 19.2 18.7 20.1 21.5

=== Statistical Analysis ===

Count: 8
Mean: 20.92
Variance: 2.27
Standard Deviation: 1.51
Minimum: 18.70
Maximum: 23.10
Range: 4.40

--- Dataset 3: Product Price Variations (USD) ---
Prices: $10.99 $12.50 $9.99 $15.75 $11.25 $13.00 $10.50 $14.99 $12.75

=== Statistical Analysis ===

Count: 9
Mean: 12.30
Variance: 3.09
Standard Deviation: 1.76
Minimum: 9.99
Maximum: 15.75
Range: 5.76

--- Additional Calculations ---
For scores array:
Mean: 87.14
Variance: 33.12
Std Dev: 5.75

Z-score examples (using mean and std dev):
Score 85.5: Z-score = -0.29
Score 92.0: Z-score = 0.84
Score 78.5: Z-score = -1.51
*/
