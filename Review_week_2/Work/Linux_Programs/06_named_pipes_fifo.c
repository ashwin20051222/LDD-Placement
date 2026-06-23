/*
PROGRAM: Named Pipes (FIFOs) for Inter-Process Communication
QUESTION: Create a FIFO and demonstrate communication between 
          two processes using named pipes.
COMPILE: gcc -o fifo_demo fifo_demo.c
RUN: ./fifo_demo (run in terminal 1)
     echo "message" > myfifo (run in terminal 2)
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

#define FIFO_NAME "myfifo"
#define BUFFER_SIZE 100

int main()
{
    int fd;
    char buffer[BUFFER_SIZE];
    ssize_t bytesRead;
    
    printf("Named Pipes (FIFO) - Inter-Process Communication\n");
    printf("================================================\n\n");
    
    printf("Creating FIFO: %s\n", FIFO_NAME);
    
    /* Remove existing FIFO if it exists */
    unlink(FIFO_NAME);
    
    /* Create named pipe */
    if (mkfifo(FIFO_NAME, 0666) == -1)
    {
        perror("mkfifo");
        return 1;
    }
    
    printf("FIFO created successfully\n");
    printf("FIFO file: %s\n", FIFO_NAME);
    printf("FIFO permissions: 0666\n\n");
    
    printf("Waiting for input from FIFO...\n");
    printf("(In another terminal, try: echo 'Hello' > %s)\n\n", FIFO_NAME);
    
    /* Open FIFO for reading */
    fd = open(FIFO_NAME, O_RDONLY);
    
    if (fd == -1)
    {
        perror("open");
        unlink(FIFO_NAME);
        return 1;
    }
    
    printf("FIFO opened successfully (File Descriptor: %d)\n\n", fd);
    
    /* Read from FIFO */
    printf("Reading data from FIFO...\n");
    bytesRead = read(fd, buffer, BUFFER_SIZE - 1);
    
    if (bytesRead == -1)
    {
        perror("read");
        close(fd);
        unlink(FIFO_NAME);
        return 1;
    }
    
    if (bytesRead == 0)
    {
        printf("FIFO closed by writer\n");
    }
    else
    {
        buffer[bytesRead] = '\0';
        printf("Data received from FIFO (%ld bytes):\n", bytesRead);
        printf("Content: %s\n", buffer);
    }
    
    /* Close FIFO */
    close(fd);
    printf("\nFIFO closed\n");
    
    /* Clean up - remove FIFO */
    if (unlink(FIFO_NAME) == -1)
    {
        perror("unlink");
        return 1;
    }
    
    printf("FIFO removed from filesystem\n");
    
    return 0;
}

/*
OUTPUT (Terminal 1):
Named Pipes (FIFO) - Inter-Process Communication
================================================

Creating FIFO: myfifo
FIFO created successfully
FIFO file: myfifo
FIFO permissions: 0666

Waiting for input from FIFO...
(In another terminal, try: echo 'Hello' > myfifo)

FIFO opened successfully (File Descriptor: 3)

Reading data from FIFO...
Data received from FIFO (6 bytes):
Content: Hello

FIFO closed
FIFO removed from filesystem

=====================================

Terminal 2 (execute after running terminal 1):
$ echo "Hello" > myfifo
*/
