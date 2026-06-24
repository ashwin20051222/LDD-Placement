#include "stdio.h"
#include "fcntl.h"
#include "unistd.h"
#include "string.h"

int main() {
	int fd = open("test.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	
	if(fd == -1) {
		perror("Error opening file");
		return 1;
	}
	int n;
	printf("Enter the exact size of string want to store in file :");
	scanf("%d", &n);
	
	getchar();
	char msg[n + 1];
	printf("Enter the string for the file handling :");
	fgets(msg, sizeof(msg), stdin);
	
	msg[strcspn(msg, "\n")] = '\0';
	
	int byte_written = write(fd, msg, strlen(msg));
	
	if(byte_written == -1) {
		perror("Error write to file");
		return 1;
	}
	
	printf("Successfully write %d byte and message %s to the test.txt, ", byte_written, msg);
	close(fd);
	
	return 0;
}
