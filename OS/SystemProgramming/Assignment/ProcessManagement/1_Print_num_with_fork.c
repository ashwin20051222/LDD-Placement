#include "stdio.h"
#include "unistd.h"  // Use it for the unix system call like fork(), getpid(), sleep(), exec()
#include "sys/types.h"  // Use it for pid_t, uid_t, size_t, etc..

int main() {
	pid_t pid;
	
	// Initialization is better then declaration of fork()
	pid = fork();
	
	// Child process execute
	if(pid == 0) {
		printf("Child Process :\n");
		
		for(int i = 1; i <= 100; i++) {
			printf("%d ", i);
		}
		printf("\n");
	}
	else if(pid > 0) {
		printf("Parent Process :\n");
		
		for(int i = 2; i <= 100; i++) {
			printf("%d ", i);
		}
		printf("\n");
	}
	else {
		printf("Fork Failed!");
	}
	
	return 0;
}

/*

(Ashwin S) silicondoc@arch ~/Documents/Class file/LDD/OS/SystemProgramming/Assignment/Fork$ gedit 1_Print_num_with_fork.c
(Ashwin S) silicondoc@arch ~/Documents/Class file/LDD/OS/SystemProgramming/Assignment/Fork$ gedit 1_Print_num_with_fork.c &
[1] 25600
(Ashwin S) silicondoc@arch ~/Documents/Class file/LDD/OS/SystemProgramming/Assignment/Fork$ gcc 1_Print_num_with_fork.c -o PrintNumFork
[1]+  Done                       gedit 1_Print_num_with_fork.c
(Ashwin S) silicondoc@arch ~/Documents/Class file/LDD/OS/SystemProgramming/Assignment/Fork$ ./PrintNumFork
Parent Process :
2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63 64 65 66 67 68 69 70 71 72 73 74 75 76 77 78 79 80 81 82 83 84 85 86 87 88 89 90 91 92 93 94 95 96 97 98 99 100 
Child Process :
1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63 64 65 66 67 68 69 70 71 72 73 74 75 76 77 78 79 80 81 82 83 84 85 86 87 88 89 90 91 92 93 94 95 96 97 98 99 100 

*/
