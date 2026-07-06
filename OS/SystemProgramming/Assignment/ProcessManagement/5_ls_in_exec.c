#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/wait.h"

int main() {
	pid_t pid;
	
	// Child Peocess
	pid = fork();
	
	if(pid < 0) {
		printf("Fork creation is failed.\n");
		exit(1);
	}
	
	// Child Process
	else if(pid == 0){
		printf("Child Process :\n");
		printf("Child PID : %d\n", getpid());
		
		// Replace child with ls -l command 
		execl("/usr/bin/ls", "ls", "-l", NULL);
		
		// Replace child 
		perror("execl failed");
		exit(1);
	}
	
	// Parent Process
	else {
		printf("Parent process waiting .\n");
		
		// wait untill child process exit
		wait(NULL);
		
		printf("CHild execution completed .\n");
		printf("Parent PID = %d\n", getpid());
	}
	
	return 0;
}

/*
OUTPUT :

(Ashwin S) silicondoc@arch ~/Documents/Class file/LDD-Placement/OS/SystemProgramming/Assignment/ProcessManagement$ gedit 5_ls_in_exec.c &
[1] 26174
(Ashwin S) silicondoc@arch ~/Documents/Class file/LDD-Placement/OS/SystemProgramming/Assignment/ProcessManagement$ gcc 5_ls_in_exec.c -o 5Exec
[1]+  Done                       gedit 5_ls_in_exec.c
(Ashwin S) silicondoc@arch ~/Documents/Class file/LDD-Placement/OS/SystemProgramming/Assignment/ProcessManagement$ ./5Exec
Parent process waiting .
Child Process :
Child PID : 27172
total 424
-rw-r--r-- 1 silicondoc silicondoc   1808 Jul  4 22:04 1_Print_num_with_fork.c
-rwxr-xr-x 1 silicondoc silicondoc  16480 Jul  5 21:53 2FORK
-rw-r--r-- 1 silicondoc silicondoc   2056 Jul  5 22:08 2_fork1_factorial_fork2_fibonacii.c
-rw-r--r-- 1 silicondoc silicondoc   2017 Jul  5 22:32 3_parent_with_child_process_show_exit_status.c
-rw-r--r-- 1 silicondoc silicondoc   2050 Jul  6 09:29 4_Display_pid_ppid.c
-rwxr-xr-x 1 silicondoc silicondoc  16280 Jul  6 09:29 4DisplayProcessID
-rwxr-xr-x 1 silicondoc silicondoc  16328 Jul  6 23:23 5Exec
-rw-r--r-- 1 silicondoc silicondoc    721 Jul  6 23:23 5_ls_in_exec.c
-rwxr-xr-x 1 silicondoc silicondoc  16136 Jul  4 22:03 PrintNumFork
-rw-r--r-- 1 silicondoc silicondoc 320964 Jul  4 20:52 unistd.pdf
-rwxr-xr-x 1 silicondoc silicondoc  16232 Jul  6 10:56 WEXITSTATUS
-rw-r--r-- 1 silicondoc silicondoc    478 Jul  6 10:15 WEXITSTATUS.c
CHild execution completed .
Parent PID = 27171

*/
