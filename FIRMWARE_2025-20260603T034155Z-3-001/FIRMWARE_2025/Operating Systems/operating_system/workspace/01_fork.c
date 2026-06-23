/* Program to undersand fork call*/
#include <stdio.h>
#include <unistd.h>

int main(){
	int fork_ret_value; //to store the fork return value
	printf("Hello Parent\n");
	printf("Parent process id: %d\n", getpid()); //get paren id
	fork_ret_value = fork(); // first fork call
	//fork();
	printf("fork_ret_value: %d\n",fork_ret_value);
	if(fork_ret_value < 0){
		printf("Fork Failed");
	}
	else if(fork_ret_value == 0){ // child process 
		printf("Hello from child\n");
		printf("Child process id: %d\n", getpid());//get child process id after fork
		printf("%d the parent of child process: %d\n", getppid(),getpid());//getting parent off the parent process
	}
	else{ // parent process
		printf("Hello from parent\n");
		printf("Parent process id: %d\n",getpid()); //getchild process
		printf("%d parent of parent process %d\n", getppid(), getpid());
		
	}
	return 0;
}
