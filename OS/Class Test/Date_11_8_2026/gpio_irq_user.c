/* gpio_irq_user.c - simple tester for gpio_irq driver
 * Compile: gcc -o gpio_irq_user gpio_irq_user.c
 * Usage: run after insmod and create /dev/gpio_irq
 */

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

int main(void)
{
    int fd = open("/dev/gpio_irq", O_RDWR);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    int cmd = 0;
    unsigned long count = 0;

    printf("1-enable 2-disable 3-getcount 4-reset\nChoose: ");
    if (scanf("%d", &cmd) != 1) {
        close(fd);
        return 1;
    }

    if (cmd == 1) {
        ioctl(fd, 0);
    } else if (cmd == 2) {
        ioctl(fd, 1);
    } else if (cmd == 3) {
        if (ioctl(fd, 2, &count) == 0)
            printf("Count=%lu\n", count);
    } else if (cmd == 4) {
        ioctl(fd, 3);
    }

    close(fd);
    return 0;
}
