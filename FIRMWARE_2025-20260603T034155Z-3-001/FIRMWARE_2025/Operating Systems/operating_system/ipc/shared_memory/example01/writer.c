// writer.c
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define SIZE 1024

int main() {

    int fd;
    char *ptr;

    fd = shm_open("/myshm", O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        perror("shm_open");
        exit(1);
    }

    ftruncate(fd, SIZE);

    ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, fd, 0);

    sprintf(ptr, "Hello from Writer Process!");

    printf("Data written to shared memory\n");

    return 0;
}

