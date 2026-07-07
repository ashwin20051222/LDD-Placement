#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include "sys/wait.h"

int main()
{
    	pid_t zombie_pid;
	int status;
	
	// Child Process created
    	zombie_pid = fork();

    	if (zombie_pid < 0){
       		perror("Fork failed.\n");
        	exit(EXIT_FAILURE);
    	}
    	
    	if(zombie_pid == 0) {
    		printf("Child Process :\n");
    		printf("Child Process PID : %d\n", getpid());
    		printf("Parent Process PID : %d\n", getppid());
    		
    		printf("\nChild Intensionally terminated.\n");
    		
    		exit(0);
    	}
    	
    	// Parent Process
    	printf("\n[PARENT]\n");
    	printf("Parent PID : %d\n", getpid());
    	printf("Zombie Child PID : %d\n", zombie_pid);

    	printf("\nParent intentionally does NOT call wait().\n");
    	printf("Sleeping for 3 seconds so child becomes a Zombie.\n");

    	sleep(3);
	
	pid_t pid = fork();
	
	pid = fork();
	
    	if (pid < 0){
        	perror("FOrk Failed.");
        	exit(EXIT_FAILURE);
    	}

    	if (pid == 0){
    		printf("\nProcess Table for Zombie Verification :\n");
    		
    		// Command for the process table dispaly
    		execl("/usr/bin/sh", "sh", "-c", "ps -el | grep Z", (char *)NULL);
    		
    		// If the command not execute show the error message
    		perror("execl");
        	exit(EXIT_FAILURE);
    	}    	
    	
    	waitpid(pid, NULL, 0);
    	
    	printf("\nNow collecting Zombie Child using wait()\n");
    	
    	waitpid(zombie_pid, &status, 0);

    	printf("Zombie Child PID : %d\n", zombie_pid);
	
	// get staus and print
    	if (WIFEXITED(status))
    	{
        	printf("Child Exit Status : %d\n", WEXITSTATUS(status));
    	}

    	printf("\nZombie process has removed from process table.\n");

    	return 0;
}

/*
Output :
(Ashwin S) silicondoc@arch ~/Documents/Class file/LDD-Placement/OS/SystemProgramming/Assignment/ProcessManagement$ ./ZombieProcess

[PARENT]
Parent PID : 23009
Zombie Child PID : 23010

Parent intentionally does NOT call wait().
Sleeping for 3 seconds so child becomes a Zombie.
Child Process :
Child Process PID : 23010
Parent Process PID : 23009

Child Intensionally terminated.

Process Table for Zombie Verification :

Process Table for Zombie Verification :
F S   UID     PID    PPID  C PRI  NI ADDR SZ WCHAN  TTY          TIME CMD
0 Z  1000   16365   16355  0  80   0 -     0 -      ?        00:00:00 sd_festival <defunct>
0 Z  1000   16366   16355  0  80   0 -     0 -      ?        00:00:00 sd_espeak-ng-mb <defunct>
0 Z  1000   16387   16355  0  80   0 -     0 -      ?        00:00:00 sd_openjtalk <defunct>
0 S  1000   23009    8947  0  80   0 -   687 do_wai pts/0    00:00:00 ZombieProcess
1 Z  1000   23010   23009  0  80   0 -     0 -      pts/0    00:00:00 ZombieProcess <defunct>
1 S  1000   23012   23009  0  80   0 -   687 do_wai pts/0    00:00:00 ZombieProcess
F S   UID     PID    PPID  C PRI  NI ADDR SZ WCHAN  TTY          TIME CMD

Now collecting Zombie Child using wait()
Zombie Child PID : 23010
Child Exit Status : 0

Zombie process has removed from process table.
0 Z  1000   16365   16355  0  80   0 -     0 -      ?        00:00:00 sd_festival <defunct>
0 Z  1000   16366   16355  0  80   0 -     0 -      ?        00:00:00 sd_espeak-ng-mb <defunct>
0 Z  1000   16387   16355  0  80   0 -     0 -      ?        00:00:00 sd_openjtalk <defunct>
0 S  1000   23009    8947  0  80   0 -   687 do_wai pts/0    00:00:00 ZombieProcess
1 Z  1000   23010   23009  0  80   0 -     0 -      pts/0    00:00:00 ZombieProcess <defunct>
1 S  1000   23012   23009  0  80   0 -   687 do_wai pts/0    00:00:00 ZombieProcess
0 Z  1000   23017   23014  0  80   0 -     0 -      pts/0    00:00:00 grep <defunct>

Now collecting Zombie Child using wait()
Zombie Child PID : 23010
Child Exit Status : 0

Zombie process has removed from process table.
(Ashwin S) silicondoc@arch ~/Documents/Class file/LDD-Placement/OS/SystemProgramming/Assignment/ProcessManagement$ 


*/

