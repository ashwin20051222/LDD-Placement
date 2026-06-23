/*
 * user_app.c
 * 
 * User-space application that reads keyboard timeout events from the driver.
 * 
 * This application:
 * - Opens the character device created by the kernel module
 * - Periodically reads timeout status
 * - Displays "Keyboard timeout detected" when inactivity is detected
 * - Runs continuously to monitor keyboard activity
 * 
 * Compilation: gcc user_app.c -o user_app
 * Usage: ./user_app
 * 
 * Author: Linux Device Driver Student
 * License: GPL
 */

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int fd;
    char buffer[100];
    int count = 0;

    printf("========================================\n");
    printf("Keyboard Timeout Monitor\n");
    printf("========================================\n");
    printf("Opening device: /dev/kb_timeout\n");

    /* Open character device */
    fd = open("/dev/kb_timeout", O_RDONLY);

    if (fd < 0)
    {
        perror("Error: Failed to open /dev/kb_timeout");
        printf("\nMake sure:\n");
        printf("1. Driver is loaded: sudo insmod keyboard_timeout.ko\n");
        printf("2. Device file exists: sudo mknod /dev/kb_timeout c <major> 0\n");
        printf("3. You have permission to read the device\n");
        return 1;
    }

    printf("Device opened successfully (fd = %d)\n", fd);
    printf("========================================\n");
    printf("Monitoring keyboard activity...\n");
    printf("(Press keys to reset timeout, or wait 5 seconds for timeout)\n");
    printf("Press Ctrl+C to stop monitoring\n");
    printf("========================================\n\n");

    /* Continuously read from device */
    while (1)
    {
        /* Clear buffer */
        memset(buffer, 0, sizeof(buffer));

        /* Reset file offset to beginning */
        lseek(fd, 0, SEEK_SET);

        /* Read status from driver */
        ssize_t bytes_read = read(fd, buffer, sizeof(buffer) - 1);

        if (bytes_read < 0)
        {
            perror("Error: Read failed");
            break;
        }

        /* Print with timestamp counter */
        count++;
        printf("[%3d] %s", count, buffer);
        fflush(stdout);

        /* Check every 1 second */
        sleep(1);
    }

    /* Close device */
    close(fd);

    printf("\n========================================\n");
    printf("Monitoring stopped\n");
    printf("========================================\n");

    return 0;
}
