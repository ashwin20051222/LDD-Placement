#include "student.h"

#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int has_only_trailing_space(const char *text)
{
    while (*text != '\0') {
        if (!isspace((unsigned char)*text)) {
            return 0;
        }
        text++;
    }

    return 1;
}

void clear_input_buffer(void)
{
    int ch;

    while ((ch = getchar()) != '\n' && ch != EOF) {
    }
}

void read_line(const char *prompt, char *buffer, int size)
{
    printf("%s", prompt);

    if (fgets(buffer, size, stdin) == NULL) {
        buffer[0] = '\0';
        return;
    }

    if (strchr(buffer, '\n') == NULL) {
        clear_input_buffer();
    }

    buffer[strcspn(buffer, "\n")] = '\0';
}

int read_int(const char *prompt)
{
    char input[32];
    char *endptr;
    long value;

    for (;;) {
        read_line(prompt, input, sizeof(input));

        errno = 0;
        value = strtol(input, &endptr, 10);

        if (input[0] != '\0' &&
            errno == 0 &&
            value >= INT_MIN &&
            value <= INT_MAX &&
            has_only_trailing_space(endptr)) {
            return (int)value;
        }

        printf("Invalid number. Please enter an integer value.\n");
    }
}

float read_float(const char *prompt)
{
    char input[32];
    char *endptr;
    float value;

    for (;;) {
        read_line(prompt, input, sizeof(input));

        errno = 0;
        value = strtof(input, &endptr);

        if (input[0] != '\0' && errno == 0 && has_only_trailing_space(endptr)) {
            return value;
        }

        printf("Invalid number. Please enter a numeric value.\n");
    }
}
