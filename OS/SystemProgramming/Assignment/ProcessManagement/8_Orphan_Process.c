#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/wait.h"

int main() {
	pid_t orphan_pid, pid;
	
	// Creating the child process
	orphan_pid = fork();
	
	if(orphan_pid < 0) {
		perror("Fork Failed to create.\n");
		exit(EXIT_FAILURE);
	}
	
	if(orphan_pid == 0) {
		printf("Child PID              : %d\n", getpid());
        	printf("Parent PID Before Exit : %d\n", getppid());
        	
        	sleep(5);
        	
        	printf("\nParent has terminated.\n");

        	printf("Child PID              : %d\n", getpid());
        	printf("Parent PID After Exit  : %d\n", getppid());

		// Process table display
       		printf("\nCreating another child to display process table.\n");
       		
       		// Creating child process for display process table 
       		pid = fork();
       		
       		if(pid < 0) {
       			perror("Process creation failed.\n");
       			exit(EXIT_FAILURE);
       		}
       		
       		// Display the process table
       		if (pid == 0)
        	{
            		execl("/usr/bin/sh", "sh", "-c", "ps -o pid,ppid,state,cmd", (char *)NULL);
			perror("execl");
            		exit(EXIT_FAILURE);
        	}
        	
        	waitpid(pid, NULL, 0);

        	printf("\nOrphan Process Completed.\n");
	}
	
	// Parent process
	else {
		printf("Parent PID : %d\n", getpid());
        	printf("Child PID  : %d\n", orphan_pid);
        	printf("\nParent will terminate after 2 seconds.\n");

        	sleep(2);

        	printf("Parent process exit\n");
        	exit(EXIT_SUCCESS);
	}
}

/*
Output :
(Ashwin S) silicondoc@arch ~/Documents/Class file/LDD-Placement/OS/SystemProgramming/Assignment/ProcessManagement$ gedit 8_Orphan_Process.c &
[1] 25537
(Ashwin S) silicondoc@arch ~/Documents/Class file/LDD-Placement/OS/SystemProgramming/Assignment/ProcessManagement$ gcc 8_Orphan_Process.c -o OrphanProcess
[1]+  Done                       gedit 8_Orphan_Process.c
(Ashwin S) silicondoc@arch ~/Documents/Class file/LDD-Placement/OS/SystemProgramming/Assignment/ProcessManagement$ ./OrphanProcess
Parent PID : 26105
Child PID  : 26106

Parent will terminate after 2 seconds.
Child PID              : 26106
Parent PID Before Exit : 26105
Parent process exit
(Ashwin S) silicondoc@arch ~/Documents/Class file/LDD-Placement/OS/SystemProgramming/Assignment/ProcessManagement$ 
Parent has terminated.
Child PID              : 26106
Parent PID After Exit  : 827

Creating another child to display process table.
    PID    PPID S CMD
   8947    8603 S bash
  26106     827 S ./OrphanProcess
  26108   26106 R ps -o pid,ppid,state,cmd

Orphan Process Completed.


*/
