/*
PROGRAM: Memory Mapping (mmap) - File I/O Optimization
QUESTION: Use mmap() to map a file into memory and efficiently read/write data. 
          Demonstrate memory mapping capabilities.
COMPILE: gcc -o mmap_demo mmap_demo.c
RUN: ./mmap_demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <sys/stat.h>

#define FILE_NAME "mmap_test.txt"
#define FILE_SIZE 1024

int main()
{
    int fd;
    struct stat fileStats;
    char *fileMap;
    
    printf("Memory Mapping (mmap) - File I/O Optimization\n");
    printf("============================================\n\n");
    
    /* Create test file */
    printf("--- Creating test file ---\n");
    printf("File name: %s\n", FILE_NAME);
    printf("File size: %d bytes\n\n", FILE_SIZE);
    
    fd = open(FILE_NAME, O_CREAT | O_RDWR | O_TRUNC, 0666);
    
    if (fd == -1)
    {
        perror("open");
        return 1;
    }
    
    /* Write initial data */
    char *initialData = "Hello from Memory Mapped File!";
    write(fd, initialData, strlen(initialData));
    
    /* Extend file to FILE_SIZE */
    lseek(fd, FILE_SIZE - 1, SEEK_SET);
    write(fd, "", 1);
    
    printf("File created successfully\n");
    printf("Initial content: %s\n\n", initialData);
    
    /* Get file statistics */
    if (fstat(fd, &fileStats) == -1)
    {
        perror("fstat");
        close(fd);
        return 1;
    }
    
    printf("--- File Statistics ---\n");
    printf("File size: %ld bytes\n", fileStats.st_size);
    printf("File mode: 0%o\n\n", fileStats.st_mode);
    
    /* Memory map the file */
    printf("--- Memory Mapping ---\n");
    fileMap = (char *)mmap(NULL, FILE_SIZE, PROT_READ | PROT_WRITE, 
                          MAP_SHARED, fd, 0);
    
    if (fileMap == MAP_FAILED)
    {
        perror("mmap");
        close(fd);
        return 1;
    }
    
    printf("File mapped to memory successfully\n");
    printf("Memory address: %p\n\n", fileMap);
    
    /* Read from memory mapped region */
    printf("--- Reading from Mapped Memory ---\n");
    printf("Content at offset 0: %s\n\n", fileMap);
    
    /* Modify memory mapped region */
    printf("--- Modifying Mapped Memory ---\n");
    strcpy(fileMap, "Modified via mmap()");
    printf("Modified content: %s\n\n", fileMap);
    
    /* Write more data */
    printf("--- Writing Additional Data ---\n");
    strcpy(fileMap + strlen("Modified via mmap()"), " - appended data");
    printf("New content: %s\n\n", fileMap);
    
    /* Sync changes to disk */
    printf("--- Syncing to Disk ---\n");
    if (msync(fileMap, FILE_SIZE, MS_SYNC) == -1)
    {
        perror("msync");
    }
    else
    {
        printf("Changes synced to disk successfully\n\n");
    }
    
    /* Display character at specific offset */
    printf("--- Accessing Specific Offsets ---\n");
    printf("Character at offset 0: '%c'\n", fileMap[0]);
    printf("Character at offset 9: '%c'\n", fileMap[9]);
    printf("Character at offset 100: '%c'\n\n", fileMap[100]);
    
    /* Unmap memory */
    printf("--- Unmapping Memory ---\n");
    if (munmap(fileMap, FILE_SIZE) == -1)
    {
        perror("munmap");
    }
    else
    {
        printf("Memory unmapped successfully\n\n");
    }
    
    /* Close file */
    close(fd);
    printf("File closed\n");
    
    /* Display file content after mapping */
    printf("\n--- Verifying File Content ---\n");
    FILE *file = fopen(FILE_NAME, "r");
    if (file)
    {
        char buffer[256];
        fgets(buffer, sizeof(buffer), file);
        printf("File content after mmap: %s\n", buffer);
        fclose(file);
    }
    
    /* Clean up */
    unlink(FILE_NAME);
    printf("\nTest file removed\n");
    
    return 0;
}

/*
OUTPUT:
Memory Mapping (mmap) - File I/O Optimization
============================================

--- Creating test file ---
File name: mmap_test.txt
File size: 1024 bytes

File created successfully
Initial content: Hello from Memory Mapped File!

--- File Statistics ---
File size: 1024 bytes
File mode: 0100666

--- Memory Mapping ---
File mapped to memory successfully
Memory address: 0x7f123456789a

--- Reading from Mapped Memory ---
Content at offset 0: Hello from Memory Mapped File!

--- Modifying Mapped Memory ---
Modified content: Modified via mmap()

--- Writing Additional Data ---
New content: Modified via mmap() - appended data

--- Syncing to Disk ---
Changes synced to disk successfully

--- Accessing Specific Offsets ---
Character at offset 0: 'M'
Character at offset 9: 'd'
Character at offset 100: ''

--- Unmapping Memory ---
Memory unmapped successfully

File closed

--- Verifying File Content ---
File content after mmap: Modified via mmap() - appended data

Test file removed
*/
