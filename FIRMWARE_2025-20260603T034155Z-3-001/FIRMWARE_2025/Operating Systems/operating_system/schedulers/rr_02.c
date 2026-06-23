/*
    Round Robin Scheduling Algorithm

*/

#include <stdio.h>

int main()
{
    int n, time_quantum;

    printf("Enter number of processes: ");
    scanf("%d", &n);

    int at[n], bt[n], rt[n];
    int wt[n], tat[n];
    int completed = 0, time = 0;

    // Input
    for(int i = 0; i < n; i++)
    {
        printf("\nProcess P%d\n", i+1);

        printf("Arrival Time: ");
        scanf("%d", &at[i]);

        printf("Burst Time: ");
        scanf("%d", &bt[i]);

        rt[i] = bt[i];   // Remaining Time
        wt[i] = 0;
        tat[i] = 0;
    }

    printf("\nEnter Time Quantum: ");
    scanf("%d", &time_quantum);

    printf("\n========== GANTT CHART ==========\n");
    printf("%d", time);

    while(completed < n)
    {
        int executed = 0;

        for(int i = 0; i < n; i++)
        {
            if(rt[i] > 0 && at[i] <= time)
            {
                executed = 1;

                if(rt[i] > time_quantum)
                {
                    time += time_quantum;
                    rt[i] -= time_quantum;
                }
                else
                {
                    time += rt[i];
                    rt[i] = 0;
                    completed++;

                    tat[i] = time - at[i];
                    wt[i] = tat[i] - bt[i];
                }

                printf(" | P%d | %d", i+1, time);
            }
        }

        // If no process executed (CPU Idle)
        if(executed == 0)
        {
            time++;
        }
    }

    printf("\n\n========== PROCESS TABLE ==========\n");
    printf("Process\tAT\tBT\tTAT\tWT\n");

    float total_wt = 0, total_tat = 0;

    for(int i = 0; i < n; i++)
    {
        total_wt += wt[i];
        total_tat += tat[i];

        printf("P%d\t%d\t%d\t%d\t%d\n",
               i+1, at[i], bt[i], tat[i], wt[i]);
    }

    printf("\nAverage Waiting Time = %.2f",
           total_wt / n);

    printf("\nAverage Turnaround Time = %.2f\n",
           total_tat / n);

    return 0;
}

