/* irq_circbuf_user.c - user app to poll and read events
 * Compile: gcc -o irq_circbuf_user irq_circbuf_user.c
 */

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/poll.h>

int main(void)
{
    int fd = open("/dev/irq_circbuf", O_RDONLY);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    struct pollfd pfd = { .fd = fd, .events = POLLIN };

    printf("Polling for events... (Ctrl+C to stop)\n");

    while (1) {
        int r = poll(&pfd, 1, 5000);

        if (r > 0 && (pfd.revents & POLLIN)) {
            char buf[128];
            int n = read(fd, buf, sizeof(buf));
            if (n > 0) {
                write(1, buf, n);
                write(1, "\n", 1);
            }
        } else if (r == 0) {
            printf("timeout\n");
        } else if (r < 0) {
            perror("poll");
            break;
        }
    }

    close(fd);
    return 0;
}
