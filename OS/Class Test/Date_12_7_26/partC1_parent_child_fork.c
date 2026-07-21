/*
Question : 
 Part C - Question 1: Parent-Child Process using fork()
 Problem:
 1. Read an integer N from the user.
 2. Parent process calculates the sum of all even numbers from 1 to N.
 3. Child process calculates the sum of all odd numbers from 1 to N.
 4. Both processes print their PID, PPID, and computed result.
 5. Parent waits for the child to finish before terminating.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) {
    int N;
    pid_t pid;

    printf("Enter N: ");
    if (scanf("%d", &N) != 1) {
        printf("Invalid input.\n");
        return EXIT_FAILURE;
    }

    // Create the child process
    pid = fork();

    if (pid < 0) {
        /* fork() failed */
        perror("fork failed");
        return EXIT_FAILURE;
    }
    
    // Child Process
    else if (pid == 0) {
        int sumOdd = 0, i;
        for (i = 1; i <= N; i++) {
            if (i % 2 != 0) {
                sumOdd += i;
            }
        }
        printf("\nChild Process\n");
        printf("PID  : %d\n",  getpid());
        printf("PPID : %d\n",  getppid());
        printf("Sum of Odd Numbers = %d\n", sumOdd);
        _exit(EXIT_SUCCESS);
    }
    
    // Parent Process
    else {
        int sumEven = 0, i;
        waitpid(pid, NULL, 0);

        for (i = 1; i <= N; i++) {
            if (i % 2 == 0) {
                sumEven += i;
            }
        }
        printf("\nParent Process\n");
        printf("PID  : %d\n",  getpid());
        printf("PPID : %d\n",  getppid());
        printf("Sum of Even Numbers = %d\n", sumEven);
    }

    return EXIT_SUCCESS;
}
