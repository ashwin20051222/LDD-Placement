#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]){
	int fork_ret=fork();
	if(fork_ret > 0){
		printf("Executed by child after fork call\n");
		
	
	}
	else if(fork_ret==0){
		printf("Executed by parent after fork call\n");
		fork();
	}
	else{
		printf("Fork failed\n");
	}
	

	return 0;
}
