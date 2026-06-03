#include "stdio.h"

int main(void)
{
    char ch;
    int isLower;
    int isUpper;
    int isDigit;
    int isVowel;

    printf("Enter one character: ");
    scanf(" %c", &ch);

    isLower = (ch >= 'a' && ch <= 'z');
    isUpper = (ch >= 'A' && ch <= 'Z');
    isDigit = (ch >= '0' && ch <= '9');
    isVowel = (ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u' ||
               ch == 'A' || ch == 'E' || ch == 'I' || ch == 'O' || ch == 'U');

    if (isDigit) {
        printf("It is a digit.\n");
    } else if (isVowel) {
        printf("It is a vowel.\n");
    } else if (isLower || isUpper) {
        printf("It is a consonant.\n");
    } else {
        printf("It is a special symbol.\n");
    }

    return 0;
}
