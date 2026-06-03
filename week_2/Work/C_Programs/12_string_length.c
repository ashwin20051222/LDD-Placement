/*
PROGRAM: String Length and Basic String Operations
QUESTION: Read a string from input and print its length along with 
          other basic string operations.
*/

#include <stdio.h>
#include <string.h>

int main()
{
    char string[100];
    int length;
    int i;
    
    printf("String Length and Operations\n");
    printf("============================\n\n");
    
    /* Read string from input */
    printf("Enter a string: ");
    fgets(string, sizeof(string), stdin);
    
    /* Remove newline character if present */
    if (string[strlen(string) - 1] == '\n')
    {
        string[strlen(string) - 1] = '\0';
    }
    
    /* Get string length */
    length = strlen(string);
    
    printf("\n--- String Analysis ---\n\n");
    printf("Original string: %s\n", string);
    printf("Length: %d characters\n\n", length);
    
    /* Display character by character */
    printf("Characters:\n");
    for (i = 0; i < length; i++)
    {
        printf("Position %d: '%c' (ASCII: %d)\n", i, string[i], string[i]);
    }
    
    /* Display in reverse */
    printf("\nString in reverse: ");
    for (i = length - 1; i >= 0; i--)
    {
        printf("%c", string[i]);
    }
    printf("\n");
    
    /* Count vowels and consonants */
    int vowels = 0;
    int consonants = 0;
    int spaces = 0;
    
    for (i = 0; i < length; i++)
    {
        char ch = string[i];
        
        if (ch == ' ')
        {
            spaces++;
        }
        else if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
        {
            if (ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u' ||
                ch == 'A' || ch == 'E' || ch == 'I' || ch == 'O' || ch == 'U')
            {
                vowels++;
            }
            else
            {
                consonants++;
            }
        }
    }
    
    printf("\n--- Statistics ---\n");
    printf("Vowels: %d\n", vowels);
    printf("Consonants: %d\n", consonants);
    printf("Spaces: %d\n", spaces);
    
    return 0;
}

/*
OUTPUT:
String Length and Operations
============================

Enter a string: Hello World

--- String Analysis ---

Original string: Hello World
Length: 11 characters

Characters:
Position 0: 'H' (ASCII: 72)
Position 1: 'e' (ASCII: 101)
Position 2: 'l' (ASCII: 108)
Position 3: 'l' (ASCII: 108)
Position 4: 'o' (ASCII: 111)
Position 5: ' ' (ASCII: 32)
Position 6: 'W' (ASCII: 87)
Position 7: 'o' (ASCII: 111)
Position 8: 'r' (ASCII: 114)
Position 9: 'l' (ASCII: 108)
Position 10: 'd' (ASCII: 100)

String in reverse: dlroW olleH

--- Statistics ---
Vowels: 3
Consonants: 7
Spaces: 1
*/
