#include "stdio.h"

int is_letter_or_digit(char ch)
{
    return (ch >= 'a' && ch <= 'z') ||
           (ch >= 'A' && ch <= 'Z') ||
           (ch >= '0' && ch <= '9');
}

char make_lower(char ch)
{
    if (ch >= 'A' && ch <= 'Z') {
        return (char)(ch + 32);
    }

    return ch;
}

int main(void)
{
    char text[200];
    int left = 0;
    int right = 0;
    int palindrome = 1;

    printf("Enter a string: ");
    fgets(text, sizeof(text), stdin);

    while (text[right] != '\0' && text[right] != '\n') {
        right++;
    }
    right--;

    while (left < right) {
        while (left < right && !is_letter_or_digit(text[left])) {
            left++;
        }
        while (left < right && !is_letter_or_digit(text[right])) {
            right--;
        }

        if (make_lower(text[left]) != make_lower(text[right])) {
            palindrome = 0;
            break;
        }

        left++;
        right--;
    }

    if (palindrome) {
        printf("The string is a palindrome.\n");
    } else {
        printf("The string is not a palindrome.\n");
    }

    return 0;
}
