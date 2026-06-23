#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]){
	int x=6;
	if(fork() || fork()){
		printf("Executed by parent after fork call\n");
		fork();
	}

}
