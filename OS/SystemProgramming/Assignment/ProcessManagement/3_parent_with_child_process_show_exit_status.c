#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    	int n;

    	printf("Enter the number of elements: ");
    	scanf("%d", &n);

    	int arr[n];

    	printf("Enter %d integer elements:\n", n);

    	for(int i = 0; i < n; i++)
    	{
        	printf("Element %d : ", i + 1);
        	scanf("%d", &arr[i]);
    	}

    	pid_t pid1, pid2, pid3;
    	int status;


    	pid1 = fork();
    	
    	if(pid1 < 0)
    	{
        	printf("First fork failed\n");
        	exit(1);
    	}

    	if(pid1 == 0)
    	{
        	int sum = 0;

        	for(i = 0; i < n; i++) {
            		sum += arr[i];
            	}

            	printf("PID = %d\n", getpid());
            	printf("Sum = %d\n", sum);

            	exit(0);
      	}

    	pid2 = fork();

	if(pid2 < 0)
    	{
        	printf("First fork failed\n");
        	exit(1);
    	}
    	
    	if(pid2 == 0)
    	{
        	int sum = 0;
        	float average;

        	for(i = 0; i < n; i++) {
            		sum += arr[i];
		}
        	average = (float)sum / n;

        	printf("PID = %d\n", getpid());
        	printf("Average = %.2f\n", average);

        	exit(1);
    	}

    	pid3 = fork();

	if (pid3 < 0)
    	{
        	printf("First fork failed\n");
        	exit(1);
    	}
    	
    	if(pid3 == 0)
    	{
        	int max = arr[0];

        	for(i = 1; i < n; i++)
        	{
            	if(arr[i] > max)
               		max = arr[i];
        	}

        	printf("PID = %d\n", getpid());
        	printf("Maximum = %d\n", max);

        	exit(2);
    	}
    	
    	printf("\nParent Process (PID = %d)\n", getpid());
    	printf("Waiting for all child processes\n\n");

    	for(i = 0; i < 3; i++)
    	{
        	pid_t child = wait(&status);

        	if(WIFEXITED(status))
        	{
            		printf("Child Process PID = %d exited with Status = %d\n", child, WEXITSTATUS(status));
        	}
   	}

    	printf("\nAll child processes have completed.\n");

    	return 0;
}
