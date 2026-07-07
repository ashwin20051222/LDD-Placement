#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/wait.h"


int main() {
	pid_t pid;
	
	// Creating the child process
	pid = fork();
	
	if(pid < 0) {
		perror("Fork failed\n");
		exit(EXIT_FAILURE);
	}
	
	// Child Process
	else if(pid == 0) {
		printf("Child Process :\n");
		printf("Child PID : %d\n", getpid());
		printf("Executing cal command by child :\n");
		
		execl("/usr/bin/cal", "cal", NULL);
		
		// If execl is fail show the error message
		perror("Exec() is failed in child process.\n");
		exit(EXIT_FAILURE);	
	}
	
	// Parent Process
	else {
		printf("\nParent Process :\n");
		printf("Parent PID : %d\n", getpid());
		printf("\nExecuting date command by parent :\n");
		
		execl("/usr/bin/date", "date", NULL);
		
		// If execl is fail show the error message
		perror("Exec() is failed in parent process.\n");
		exit(EXIT_FAILURE);
	}
	
	return 0;
}

/*
OUTPUT :
(Ashwin S) silicondoc@arch ~/Documents/Class file/LDD-Placement/OS/SystemProgramming/Assignment/ProcessManagement$ gedit 6_parent_date_child_cal_exec.c &
[1] 9893
(Ashwin S) silicondoc@arch ~/Documents/Class file/LDD-Placement/OS/SystemProgramming/Assignment/ProcessManagement$ gcc 6_parent_date_child_cal_exec.c -o 6_Parent_Date_Child_Cal_Exec
[1]+  Done                       gedit 6_parent_date_child_cal_exec.c
(Ashwin S) silicondoc@arch ~/Documents/Class file/LDD-Placement/OS/SystemProgramming/Assignment/ProcessManagement$ ./6_Parent_Date_Child_Cal_Exec

Parent Process :
Parent PID : 10782
Executing date command by parent :
Child Process :
Child PID : 10783
Executing cal command by child :
Tue Jul  7 10:03:41 AM IST 2026
      July 2026     
Su Mo Tu We Th Fr Sa
          1  2  3  4
 5  6  7  8  9 10 11
12 13 14 15 16 17 18
19 20 21 22 23 24 25
26 27 28 29 30 31   
                    
(Ashwin S) silicondoc@arch ~/Documents/Class file/LDD-Placement/OS/SystemProgramming/Assignment/ProcessManagement$ 

*/
