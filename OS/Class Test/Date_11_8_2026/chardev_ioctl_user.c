/* chardev_ioctl_user.c - test app for chardev_ioctl
 * Compile: gcc -o chardev_ioctl_user chardev_ioctl_user.c
 * Usage: run after insmod and create device node: mknod /dev/chardev_ioctl c <major> 0
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define IOCTL_MAGIC 'k'
#define IOCTL_SET_VALUE _IOW(IOCTL_MAGIC, 1, int)
#define IOCTL_GET_VALUE _IOR(IOCTL_MAGIC, 2, int)
#define IOCTL_RESET     _IO(IOCTL_MAGIC, 3)
#define IOCTL_GET_STATUS _IOR(IOCTL_MAGIC, 4, int)

int main(void)
{
    int fd = open("/dev/chardev_ioctl", O_RDWR);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    int v = 0;
    int s = 0;

    printf("Set value: ");
    if (scanf("%d", &v) == 1)
        ioctl(fd, IOCTL_SET_VALUE, &v);

    if (ioctl(fd, IOCTL_GET_VALUE, &v) == 0)
        printf("Value from driver: %d\n", v);

    if (ioctl(fd, IOCTL_GET_STATUS, &s) == 0)
        printf("Status: %d\n", s);

    printf("Resetting...\n");
    ioctl(fd, IOCTL_RESET);

    if (ioctl(fd, IOCTL_GET_VALUE, &v) == 0)
        printf("After reset value: %d\n", v);

    close(fd);
    return 0;
}
