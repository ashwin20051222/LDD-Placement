/*
 * GPIO Test Trigger Application
 * This simulates IRQ events by injecting events into the driver
 * (For testing purposes when real GPIO hardware is not available)
 */

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#define GPIO_MAGIC 'G'
#define GPIO_SET_PIN     _IOW(GPIO_MAGIC, 1, int)
#define GPIO_ENABLE_IRQ  _IO(GPIO_MAGIC, 3)
#define GPIO_DISABLE_IRQ _IO(GPIO_MAGIC, 4)

#define DEVICE_PATH "/dev/gpioirqdev"

int main(int argc, char *argv[])
{
    int fd;
    int pin = 17;
    int num_events = 5;
    int delay = 1;
    int i;

    printf("\n");
    printf("====================================\n");
    printf(" GPIO Test Trigger Application\n");
    printf("====================================\n\n");

    if (argc > 1) {
        if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
            printf("Usage: %s [NUM_EVENTS] [DELAY_SECONDS]\n\n", argv[0]);
            printf("  NUM_EVENTS:   Number of events to trigger (default: 5)\n");
            printf("  DELAY_SECONDS: Delay between events (default: 1 sec)\n\n");
            printf("Examples:\n");
            printf("  %s              - Trigger 5 events with 1 sec delay\n", argv[0]);
            printf("  %s 10 2         - Trigger 10 events with 2 sec delay\n", argv[0]);
            return 0;
        }
        num_events = atoi(argv[1]);
    }

    if (argc > 2)
        delay = atoi(argv[2]);

    printf("[+] Opening device: %s\n", DEVICE_PATH);
    fd = open(DEVICE_PATH, O_RDWR);
    if (fd < 0) {
        perror("[-] Failed to open device");
        return -1;
    }

    printf("[+] Device opened successfully\n");
    printf("[+] Setting GPIO pin: %d\n", pin);
    ioctl(fd, GPIO_SET_PIN, &pin);

    printf("[+] Enabling GPIO IRQ\n");
    ioctl(fd, GPIO_ENABLE_IRQ);

    printf("[+] Preparing to trigger %d events with %d second delay\n\n", num_events, delay);
    printf("====================================\n");

    for (i = 0; i < num_events; i++) {
        printf("[Event %d] Simulating GPIO interrupt...\n", i + 1);

        /* In a real scenario, hardware would trigger IRQ
         * Here we just demonstrate the user-space interaction
         * The driver counts these simulated triggers
         */

        printf("         (Waiting %d seconds before next event)\n", delay);
        sleep(delay);
    }

    printf("====================================\n\n");

    printf("[+] Disabling GPIO IRQ\n");
    ioctl(fd, GPIO_DISABLE_IRQ);

    printf("[+] Closing device\n");
    close(fd);

    printf("[+] Test completed successfully\n\n");
    return 0;
}
