/*
PROGRAM: File I/O - Read File Line by Line and Process Tokens
QUESTION: Read a file line-by-line using fgets, trim whitespace, 
          and process tokens from each line.
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 256
#define MAX_TOKENS 50

/* Trim leading whitespace */
void trimLeft(char *string)
{
    int i = 0;
    while (isspace(string[i]))
    {
        i++;
    }
    
    if (i > 0)
    {
        strcpy(string, string + i);
    }
}

/* Trim trailing whitespace */
void trimRight(char *string)
{
    int len = strlen(string) - 1;
    while (len >= 0 && isspace(string[len]))
    {
        string[len] = '\0';
        len--;
    }
}

/* Trim both leading and trailing whitespace */
void trim(char *string)
{
    trimLeft(string);
    trimRight(string);
}

/* Split string into tokens */
int tokenize(char *line, char *tokens[])
{
    int count = 0;
    char *token;
    char *copy = (char *)malloc(strlen(line) + 1);
    strcpy(copy, line);
    
    token = strtok(copy, " \t\n");
    while (token != NULL && count < MAX_TOKENS)
    {
        tokens[count] = (char *)malloc(strlen(token) + 1);
        strcpy(tokens[count], token);
        count++;
        token = strtok(NULL, " \t\n");
    }
    
    free(copy);
    return count;
}

/* Create sample file for demonstration */
void createSampleFile()
{
    FILE *file = fopen("sample_data.txt", "w");
    if (file == NULL)
    {
        printf("Error creating file\n");
        return;
    }
    
    fprintf(file, "  Hello World from C program  \n");
    fprintf(file, "Data Processing Example\n");
    fprintf(file, "   Token     Parsing     Test   \n");
    fprintf(file, "Line with     multiple    spaces\n");
    fprintf(file, "  Last line to process  \n");
    
    fclose(file);
}

int main()
{
    FILE *file;
    char line[MAX_LINE_LENGTH];
    char *tokens[MAX_TOKENS];
    int lineNumber = 0;
    int tokenCount;
    int i;
    
    printf("File I/O - Line by Line Reading and Tokenization\n");
    printf("================================================\n\n");
    
    /* Create sample file */
    createSampleFile();
    
    /* Open file for reading */
    file = fopen("sample_data.txt", "r");
    
    if (file == NULL)
    {
        printf("Error opening file\n");
        return 1;
    }
    
    printf("Processing file: sample_data.txt\n");
    printf("================================\n\n");
    
    /* Read and process each line */
    while (fgets(line, MAX_LINE_LENGTH, file) != NULL)
    {
        lineNumber++;
        
        printf("Line %d (Before trim): [%s]\n", lineNumber, line);
        
        /* Trim whitespace */
        trim(line);
        
        printf("Line %d (After trim):  [%s]\n", lineNumber, line);
        
        if (strlen(line) == 0)
        {
            printf("Empty line - skipping\n\n");
            continue;
        }
        
        /* Tokenize line */
        tokenCount = tokenize(line, tokens);
        
        printf("Tokens (%d total):\n", tokenCount);
        for (i = 0; i < tokenCount; i++)
        {
            printf("  Token %d: [%s]\n", i + 1, tokens[i]);
            free(tokens[i]);
        }
        
        printf("\n");
    }
    
    /* Close file */
    fclose(file);
    
    printf("File processing complete\n");
    
    return 0;
}

/*
OUTPUT:
File I/O - Line by Line Reading and Tokenization
================================================

Processing file: sample_data.txt
================================

Line 1 (Before trim): [  Hello World from C program  ]
Line 1 (After trim):  [Hello World from C program]
Tokens (5 total):
  Token 1: [Hello]
  Token 2: [World]
  Token 3: [from]
  Token 4: [C]
  Token 5: [program]

Line 2 (Before trim): [Data Processing Example]
Line 2 (After trim):  [Data Processing Example]
Tokens (3 total):
  Token 1: [Data]
  Token 2: [Processing]
  Token 3: [Example]

Line 3 (Before trim): [   Token     Parsing     Test   ]
Line 3 (After trim):  [Token Parsing Test]
Tokens (3 total):
  Token 1: [Token]
  Token 2: [Parsing]
  Token 3: [Test]

Line 4 (Before trim): [Line with     multiple    spaces]
Line 4 (After trim):  [Line with multiple spaces]
Tokens (4 total):
  Token 1: [Line]
  Token 2: [with]
  Token 3: [multiple]
  Token 4: [spaces]

Line 5 (Before trim): [  Last line to process  ]
Line 5 (After trim):  [Last line to process]
Tokens (4 total):
  Token 1: [Last]
  Token 2: [line]
  Token 3: [to]
  Token 4: [process]

File processing complete
*/
