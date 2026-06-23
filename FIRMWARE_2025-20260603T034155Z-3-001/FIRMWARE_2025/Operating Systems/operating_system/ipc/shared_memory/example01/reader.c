// reader.c
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define SIZE 1024

int main() {

    int fd;
    char *ptr;

    fd = shm_open("/myshm", O_RDONLY, 0666);
    if (fd == -1) {
        perror("shm_open");
        exit(1);
    }

    ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, fd, 0);

    printf("Received: %s\n", ptr);

    shm_unlink("/myshm");

    return 0;
}

