/*
PROGRAM: Character Classification and String Processing with ctype.h
QUESTION: Use ctype.h to implement case-insensitive comparisons 
          and alphanumeric filtering.
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* Case-insensitive string comparison */
int strcasecmp_custom(const char *string1, const char *string2)
{
    while (*string1 || *string2)
    {
        int char1 = tolower(*string1);
        int char2 = tolower(*string2);
        
        if (char1 != char2)
        {
            return char1 - char2;
        }
        
        string1++;
        string2++;
    }
    
    return 0;
}

/* Extract only alphanumeric characters */
void filterAlphanumeric(const char *input, char *output)
{
    int j = 0;
    int i;
    
    for (i = 0; input[i] != '\0'; i++)
    {
        if (isalnum(input[i]))
        {
            output[j++] = input[i];
        }
    }
    
    output[j] = '\0';
}

/* Convert to uppercase */
void toUppercase(char *string)
{
    int i;
    for (i = 0; string[i] != '\0'; i++)
    {
        string[i] = toupper(string[i]);
    }
}

/* Convert to lowercase */
void toLowercase(char *string)
{
    int i;
    for (i = 0; string[i] != '\0'; i++)
    {
        string[i] = tolower(string[i]);
    }
}

/* Count character types in a string */
void countCharacterTypes(const char *string)
{
    int letters = 0;
    int digits = 0;
    int spaces = 0;
    int special = 0;
    int i;
    
    for (i = 0; string[i] != '\0'; i++)
    {
        char ch = string[i];
        
        if (isalpha(ch))
        {
            letters++;
        }
        else if (isdigit(ch))
        {
            digits++;
        }
        else if (isspace(ch))
        {
            spaces++;
        }
        else
        {
            special++;
        }
    }
    
    printf("Character breakdown:\n");
    printf("  Letters: %d\n", letters);
    printf("  Digits: %d\n", digits);
    printf("  Spaces: %d\n", spaces);
    printf("  Special characters: %d\n", special);
}

int main()
{
    printf("Character Classification with ctype.h (Runtime Input)\n");
    printf("===================================================\n\n");
    
    int choice;
    char string1[100];
    char string2[100];
    char output[100];
    
    while (1)
    {
        printf("\n--- Menu ---\n");
        printf("1. Case-insensitive string comparison\n");
        printf("2. Alphanumeric filtering\n");
        printf("3. Case conversion\n");
        printf("4. Character type counting\n");
        printf("5. Character analysis\n");
        printf("6. Exit\n");
        printf("Choose option: ");
        scanf("%d", &choice);
        getchar();  /* Consume newline */
        
        switch (choice)
        {
            case 1:
                printf("Enter first string: ");
                fgets(string1, sizeof(string1), stdin);
                if (string1[strlen(string1) - 1] == '\n')
                    string1[strlen(string1) - 1] = '\0';
                
                printf("Enter second string: ");
                fgets(string2, sizeof(string2), stdin);
                if (string2[strlen(string2) - 1] == '\n')
                    string2[strlen(string2) - 1] = '\0';
                
                if (strcasecmp_custom(string1, string2) == 0)
                {
                    printf("Result: EQUAL (case-insensitive)\n");
                }
                else
                {
                    printf("Result: NOT EQUAL\n");
                }
                break;
                
            case 2:
                printf("Enter string: ");
                fgets(string1, sizeof(string1), stdin);
                if (string1[strlen(string1) - 1] == '\n')
                    string1[strlen(string1) - 1] = '\0';
                
                filterAlphanumeric(string1, output);
                printf("Filtered: %s\n", output);
                break;
                
            case 3:
                printf("Enter string: ");
                fgets(string1, sizeof(string1), stdin);
                if (string1[strlen(string1) - 1] == '\n')
                    string1[strlen(string1) - 1] = '\0';
                
                strcpy(string2, string1);
                toUppercase(string2);
                printf("Uppercase: %s\n", string2);
                
                strcpy(string2, string1);
                toLowercase(string2);
                printf("Lowercase: %s\n", string2);
                break;
                
            case 4:
                printf("Enter string: ");
                fgets(string1, sizeof(string1), stdin);
                if (string1[strlen(string1) - 1] == '\n')
                    string1[strlen(string1) - 1] = '\0';
                
                countCharacterTypes(string1);
                break;
                
            case 5:
                printf("Enter string: ");
                fgets(string1, sizeof(string1), stdin);
                if (string1[strlen(string1) - 1] == '\n')
                    string1[strlen(string1) - 1] = '\0';
                
                printf("\nAnalysis:\n");
                int i;
                for (i = 0; string1[i] != '\0'; i++)
                {
                    char ch = string1[i];
                    printf("'%c' - ", ch);
                    
                    if (isalpha(ch))
                        printf("Letter ");
                    if (isdigit(ch))
                        printf("Digit ");
                    if (isalnum(ch))
                        printf("Alphanumeric ");
                    if (isupper(ch))
                        printf("Uppercase ");
                    if (islower(ch))
                        printf("Lowercase ");
                    if (isspace(ch))
                        printf("Space ");
                    
                    printf("\n");
                }
                break;
                
            case 6:
                printf("Exiting...\n");
                return 0;
                
            default:
                printf("Invalid option!\n");
        }
    }
    
    return 0;
}

/*
OUTPUT:
Character Classification with ctype.h
======================================

--- Test 1: Case-Insensitive Comparison ---
String 1: "Hello World"
String 2: "hello world"
Result: EQUAL (case-insensitive)

--- Test 2: Alphanumeric Filtering ---
Input: Hello-World! 123 @#$
Filtered: HelloWorld123

--- Test 3: Case Conversion ---
Original: Convert This Text
Uppercase: CONVERT THIS TEXT
Lowercase: convert this text

--- Test 4: Character Classification ---
String: "Hello World 123!"
Character breakdown:
  Letters: 10
  Digits: 3
  Spaces: 2
  Special characters: 1

--- Test 5: Individual Character Analysis ---
String: "Test123"

'T' - Letter Uppercase Alphanumeric
'e' - Letter Lowercase Alphanumeric
's' - Letter Lowercase Alphanumeric
't' - Letter Lowercase Alphanumeric
'1' - Digit Alphanumeric
'2' - Digit Alphanumeric
'3' - Digit Alphanumeric
*/
