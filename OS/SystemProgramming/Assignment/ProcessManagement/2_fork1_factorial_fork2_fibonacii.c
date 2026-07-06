#include "stdio.h"
#include "unistd.h"
#include "stdlib.h"
#include "sys/types.h"
#include "sys/wait.h"

// Factorial Function
unsigned long long Factorial(int n) {
	unsigned long long fact = 1;
	
	// Factorial operation
	for(int i = 1; i <=  n; i++) {
		fact *= i;
	}

	return fact;	
}

// Fibonacci Function
void Fibonacci(int n) {
	int a = 0, b = 1, c;
	
	for(int i = 0; i < n; i++) {
		printf("%d ", a);
		c = a + b;
		a = b;
		b = c;
	}
}

int main() {
	pid_t pid1, pid2;
	
	// First child process
	pid1 = fork();
	
	if(pid1 < 0) {
		printf("Child process 1 failed, due to memory limation.\n");
		exit(EXIT_FAILURE);
	}
	
	if(pid1 == 0) {
		// Get the input for the factorial
		int n;
		s1: printf("Condition apply only less than 20 and greater than 0.\nEnter the number for factorial :");
		if(scanf("%d", &n) != 1) {
			printf("Invalid input.\nEnter the valid input.");
			goto s1;
		}
		
		unsigned long long fact = Factorial(n);
		
		printf("Child process 1:\n");
		printf("PID of Child 1: %d\n", getpid());
		printf("Parent PID of Child 1: %d\n", getppid());
		printf("Factorial of %d : %llu\n", n, fact);
		
		// Terminate Process
		printf("Child Process 1 completed.\n");
		exit(0);
	}
	
	// Second child process
    	pid2 = fork();

    	if (pid2 < 0)
    	{
        	printf("Second fork failed\n");
        	exit(EXIT_FAILURE);
    	}
    	
    	if(pid2 == 0) {
		// Get the input for the factorial
		int n;
		s2 : printf("Condition apply only less than 20 and greater than 0.\nEnter the number for fibonacci :");
		if(scanf("%d", &n) != 1) {
			printf("Invalid input.\nEnter the valid input.");
			goto s2;
		}
		
		printf("Child process 2:\n");
		printf("PID of Child 2: %d\n", getpid());
		printf("Parent PID of Child 2: %d\n", getppid());
		printf("Fibonacci :\n");
		
		Fibonacci(n);
		
		// Terminate Process
		printf("Child Process 2 completed.\n");
		exit(0);
	}
	
	// Parent wait for child process
	wait(NULL);
	wait(NULL);
	
	printf("Parent process : %d\n", getpid());
	printf("Both child process has completed.\n");
	
	return 0;
}
