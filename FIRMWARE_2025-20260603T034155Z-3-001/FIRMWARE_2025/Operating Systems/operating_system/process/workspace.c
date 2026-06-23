#include <stdio.h>
#include<unistd.h>
int main(){


	int x = fork();
	printf("P1: %d\n",getpid());
	printf("P1: %d\n",getppid());
	x=fork();
	if(x==0){
		printf("Parent Process\n");
		printf("%d\n",getpid());
		printf("%d\n",getppid());
		printf("P2:Hello\n");
	}
	return 0;

}
