#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "fcntl.h"
#include "unistd.h"
#include "sys/stat.h"

#define FIFO_FILE "/tmp/myfifo"

int main() {
	// Remove FIFO if it already exists
	unlink(FIFO_FILE);
	
	// Create the named pipe (FIFO)
	if(mkfifo(FIFO_FILE, 0666) == -1) {
		perror("mkfifo failed");
		return 1;
	}
	
	printf("Named pipe created at %s\n", FIFO_FILE);
	printf("Opening FIFO for writing...\n");
	
	// Open the FIFO for writing
	int fd = open(FIFO_FILE, O_WRONLY);
	
	if(fd == -1) {
		perror("open failed");
		return 1;
	}
	
	char message[] = "Hello from Named Pipe";
	ssize_t written = write(fd, message, strlen(message));
	
	if(written == -1) {
		perror("write failed");
		return 1;
	}
	
	printf("Message written to FIFO: %s\n", message);
	
	close(fd);
	
	// Clean up
	unlink(FIFO_FILE);
	
	return 0;
}
