#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "fcntl.h"
#include "string.h"

#define FILE_NAME "1_student_log.txt"

int main() {
	int fd;
	char buffer[100];
	
	// Create / open file 
	fd = open(FILE_NAME, O_CREAT | O_RDWR | O_TRUNC, 0644);
	
	if (fd == -1)
    	{
       	perror("File Open Failed");
        	exit(EXIT_FAILURE);
    	}
    	
    	printf("Enter details of 5 students\n");
    	printf("(Format: RollNo Name Marks)\n\n");

    	for (int i = 1; i <= 5; i++)
    	{
       	int roll;
        	char name[30];
        	float marks;

        	printf("Student %d\n", i);

        	printf("Roll No : ");
        	scanf("%d", &roll);

        	printf("Name    : ");
        	scanf("%s", name);

        	printf("Marks   : ");
        	scanf("%f", &marks);
        	
        	snprintf(buffer, sizeof(buffer), "Roll: %-3d  Name: %-15s  Marks: %.2f\n", roll, name, marks);

        	if (write(fd, buffer, strlen(buffer)) == -1)
        	{
            		perror("Write Failed");
            		close(fd);
            		exit(EXIT_FAILURE);
        	}

        	printf("\n");
        }
        
        // Display the file content
        printf("Contents of %s\n", FILE_NAME);
        
}
