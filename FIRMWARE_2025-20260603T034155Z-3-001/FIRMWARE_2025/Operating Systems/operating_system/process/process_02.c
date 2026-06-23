#include <stdio.h>
#include <unistd.h>

int main() {

    fork();   
    

    printf("Hello LINUX | PID: %d | PPID: %d\n",
           getpid(), getppid());

    return 0;
}

