#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    int fd = open("test.txt", O_WRONLY | O_CREAT, 0644);
    int fd2 = dup(fd);

    write(fd, "Hello ", 6);
    write(fd2, "World", 5);

    close(fd);
    close(fd2);
}

