/*
 Part C - Question 2: Pipe Communication (Unnamed Pipe / IPC)
 Problem:
 1. Create a pipe.
 2. Create a child process using fork().
 3. Parent reads a string from the user and sends it through the pipe.
 4. Child reads the string from the pipe and counts the vowels.
 5. Child displays the received string and the number of vowels.
 6. Close all unused pipe ends properly.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) {
    int fd[2];                 // fd[0] = read end, fd[1] = write end
    pid_t pid;
    char writeMsg[256];
    char readMsg[256];
    
    // Create the pipe 
    if (pipe(fd) == -1) {
        perror("pipe failed");
        return EXIT_FAILURE;
    }

    // Creating the child process
    pid = fork();
    if (pid < 0) {
        perror("Process Creation failed");
        return EXIT_FAILURE;
    }
    
    // Child Process
    if (pid == 0) {
        int n, i, vowels = 0;
        char ch;

        // Close the unused write end of the pipe
        close(fd[1]);

        n = read(fd[0], readMsg, sizeof(readMsg) - 1);
        if (n < 0) {
            perror("read failed");
            close(fd[0]);
            exit(EXIT_FAILURE);
        }
        readMsg[n] = '\0';      // null terminate the received string

        // Count vowels
        for (i = 0; readMsg[i] != '\0'; i++) {
            ch = readMsg[i];
            if (ch == 'A' || ch == 'E' || ch == 'I' || ch == 'O' || ch == 'U' ||
                ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u') {
                vowels++;
            }
        }

        // Display the received string and vowel count
        printf("\nChild Process:\n");
        printf("Received String : %s\n", readMsg);
        printf("Number of Vowels : %d\n", vowels);

        // Close the read end of the pipe
        close(fd[0]);
        exit(EXIT_SUCCESS);
    }
    
    // Parent Process
    else {
        // Close the unused read end of the pipe
        close(fd[0]);
        
        printf("Enter a string:\n");
        
        if (fgets(writeMsg, sizeof(writeMsg), stdin) == NULL) {
            printf("Failed to read input.\n");
            close(fd[1]);
            return EXIT_FAILURE;
        }

        // Send the string to the child
        if (write(fd[1], writeMsg, strlen(writeMsg) + 1) < 0) {
            perror("write failed");
            close(fd[1]);
            return EXIT_FAILURE;
        }

        printf("\nParent Process:\n");
        printf("String Sent Successfully\n");

        // Close the write end of the pipe
        close(fd[1]);

        // Wait for the child
        waitpid(pid, NULL, 0);
    }

    return EXIT_SUCCESS;
}
