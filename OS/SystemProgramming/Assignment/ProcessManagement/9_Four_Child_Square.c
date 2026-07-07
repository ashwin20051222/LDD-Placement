#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    	int numbers[4];
    	int status;
    	pid_t pid;
    	
    	printf("Enter number with condition , square must be less than 255 :\n");

    	for (int i = 0; i < 4; i++)
    	{
        	printf("Number %d : ", i + 1);
        	scanf("%d", &numbers[i]);
    	}

    	printf("\nParent Process PID : %d\n\n", getpid());

    	// Create child process
    	for (int i = 0; i < 4; i++)
    	{
        	pid = fork();

        	if (pid < 0)
        	{
            		perror("Fork Failed to create .\n");
            		exit(EXIT_FAILURE);
        	}

        	if (pid == 0)
        	{
            		int square = numbers[i] * numbers[i];

            		printf("Child %d\n", i + 1);
            		printf("PID              : %d\n", getpid());
            		printf("Parent PID       : %d\n", getppid());
            		printf("Input Number     : %d\n", numbers[i]);
            		printf("Square           : %d\n", square);

            		if (square > 255)
            		{	
                		printf("Square is greater than 255.\n");
                		printf("Cannot return it using exit().\n");
                		exit(255);
            		}

            		printf("Child Exiting\n\n");
            		exit(square);
        	}
    	}

    	for (int i = 0; i < 4; i++)
    	{
        	pid_t child_pid = wait(&status);

        	printf("Collected Child PID : %d\n", child_pid);

        	if (WIFEXITED(status))
        	{
            		printf("Termination Status  : Normal\n");
            		printf("Returned Square     : %d\n",
                   	WEXITSTATUS(status));
        	}
        	else
        	{
            		printf("Child terminated abnormally.\n");
        	}
    	}

    	printf("All Child Processes Completed.\n");

    return 0;
}

/*
Output :
Ashwin S) silicondoc@arch ~/Documents/Class file/LDD-Placement/OS/SystemProgramming/Assignment/ProcessManagement$ ./FourSquareChild
Enter number with condition , square must be less than 255 :
Number 1 : 1
Number 2 : 5
Number 3 : 6
Number 4 : 9

Parent Process PID : 11151

Child 1
PID              : 11152
Parent PID       : 11151
Input Number     : 1
Square           : 1
Child Exiting

Child 2
PID              : 11153
Parent PID       : 11151
Input Number     : 5
Square           : 25
Child Exiting

Child 3
PID              : 11154
Parent PID       : 11151
Input Number     : 6
Square           : 36
Child Exiting

Collected Child PID : 11152
Child 4
Termination Status  : Normal
PID              : 11155
Returned Square     : 1
Collected Child PID : 11153
Termination Status  : Normal
Returned Square     : 25
Parent PID       : 11151
Input Number     : 9
Square           : 81
Child Exiting

Collected Child PID : 11154
Termination Status  : Normal
Returned Square     : 36
Collected Child PID : 11155
Termination Status  : Normal
Returned Square     : 81
All Child Processes Completed.
(Ashwin S) silicondoc@arch ~/Documents/Class file/LDD-Placement/OS/SystemProgramming/Assignment/ProcessManagement$ 
*/
