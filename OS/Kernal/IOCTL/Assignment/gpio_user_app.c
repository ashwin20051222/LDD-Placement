#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <poll.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

#define GPIO_MAGIC 'G'

#define GPIO_SET_PIN     _IOW(GPIO_MAGIC, 1, int)
#define GPIO_SET_EDGE    _IOW(GPIO_MAGIC, 2, int)
#define GPIO_ENABLE_IRQ  _IO(GPIO_MAGIC, 3)
#define GPIO_DISABLE_IRQ _IO(GPIO_MAGIC, 4)
#define GPIO_GET_EVENT   _IOR(GPIO_MAGIC, 5, int)

#define DEVICE_PATH "/dev/gpioirqdev"

static int fd = -1;
static int running = 1;

void signal_handler(int sig)
{
    printf("\n[*] Received signal %d, cleaning up...\n", sig);
    running = 0;
}

int main(int argc, char *argv[])
{
    int pin = 17;
    int event = 0;
    int edge = 0; /* 0=rising, 1=falling, 2=both */
    struct pollfd pfd;
    int poll_timeout = -1; /* blocking */

    printf("\n");
    printf("====================================\n");
    printf(" GPIO IRQ Driver - User Application \n");
    printf("====================================\n\n");

    /* Parse command line arguments */
    if (argc > 1) {
        if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
            printf("Usage: %s [PIN] [EDGE] [TIMEOUT]\n\n", argv[0]);
            printf("  PIN:     GPIO pin number (default: 17)\n");
            printf("  EDGE:    0=rising, 1=falling, 2=both (default: 0)\n");
            printf("  TIMEOUT: poll timeout in ms (default: -1 = blocking)\n\n");
            printf("Examples:\n");
            printf("  %s              - Monitor pin 17, rising edge\n", argv[0]);
            printf("  %s 26 1 5000    - Monitor pin 26, falling edge, 5 sec timeout\n", argv[0]);
            return 0;
        }
        pin = atoi(argv[1]);
    }

    if (argc > 2)
        edge = atoi(argv[2]);

    if (argc > 3)
        poll_timeout = atoi(argv[3]);

    /* Register signal handlers */
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    /* Open device */
    printf("[+] Opening device: %s\n", DEVICE_PATH);
    fd = open(DEVICE_PATH, O_RDWR);
    if (fd < 0) {
        perror("[-] Failed to open device");
        printf("\n    Make sure device node exists: sudo mknod /dev/gpioirqdev c <major> 0\n");
        printf("    Or run: make setup-auto\n\n");
        return -1;
    }
    printf("[+] Device opened successfully (fd=%d)\n\n", fd);

    /* Set GPIO pin */
    printf("[+] Setting GPIO pin: %d\n", pin);
    if (ioctl(fd, GPIO_SET_PIN, &pin) < 0) {
        perror("[-] GPIO_SET_PIN failed");
        goto cleanup;
    }

    /* Set edge type */
    printf("[+] Setting edge type: %d (0=rising, 1=falling, 2=both)\n", edge);
    if (ioctl(fd, GPIO_SET_EDGE, &edge) < 0) {
        perror("[-] GPIO_SET_EDGE failed");
        goto cleanup;
    }

    /* Enable IRQ */
    printf("[+] Enabling GPIO IRQ\n");
    if (ioctl(fd, GPIO_ENABLE_IRQ) < 0) {
        perror("[-] GPIO_ENABLE_IRQ failed");
        goto cleanup;
    }

    printf("\n");
    printf("====================================\n");
    printf(" Waiting for GPIO interrupts...\n");
    printf(" (Press Ctrl+C to exit)\n");
    printf("====================================\n\n");

    /* Setup poll */
    pfd.fd = fd;
    pfd.events = POLLIN;

    int interrupt_count = 0;

    /* Main loop: wait for interrupts */
    while (running) {

        int ret = poll(&pfd, 1, poll_timeout);

        if (ret < 0) {
            if (running)
                perror("[-] poll() failed");
            break;
        } else if (ret == 0) {
            printf("[!] Poll timeout (no interrupt received)\n");
            break;
        } else if (ret > 0) {

            if (pfd.revents & POLLIN) {

                if (ioctl(fd, GPIO_GET_EVENT, &event) < 0) {
                    perror("[-] GPIO_GET_EVENT failed");
                    break;
                }

                interrupt_count++;

                printf("[IRQ #%d] GPIO INTERRUPT DETECTED!\n", interrupt_count);
                printf("         Event: %d (timestamp: %ld ms)\n", event, (long)time(NULL) * 1000);
                printf("\n");
            }
        }
    }

    printf("\n");
    printf("====================================\n");
    printf(" Shutting down gracefully...\n");
    printf(" Total interrupts received: %d\n", interrupt_count);
    printf("====================================\n\n");

cleanup:
    /* Disable IRQ */
    if (fd >= 0) {
        printf("[+] Disabling GPIO IRQ\n");
        ioctl(fd, GPIO_DISABLE_IRQ);

        /* Close device */
        printf("[+] Closing device\n");
        close(fd);
    }

    printf("[+] Application exited successfully\n\n");
    return 0;
}
