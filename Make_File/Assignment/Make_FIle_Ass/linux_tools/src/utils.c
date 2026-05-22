#include "utils.h"

#include <stdio.h>

int copy_file(const char *source_path, const char *destination_path)
{
    FILE *source = fopen(source_path, "rb");
    FILE *destination;
    char buffer[4096];
    size_t bytes_read;

    if (source == NULL) {
        perror(source_path);
        return 0;
    }

    destination = fopen(destination_path, "wb");
    if (destination == NULL) {
        perror(destination_path);
        fclose(source);
        return 0;
    }

    while ((bytes_read = fread(buffer, 1, sizeof(buffer), source)) > 0) {
        if (fwrite(buffer, 1, bytes_read, destination) != bytes_read) {
            perror(destination_path);
            fclose(source);
            fclose(destination);
            return 0;
        }
    }

    if (ferror(source)) {
        perror(source_path);
        fclose(source);
        fclose(destination);
        return 0;
    }

    fclose(source);
    fclose(destination);
    return 1;
}

int print_file(const char *path)
{
    FILE *file = fopen(path, "r");
    int ch;

    if (file == NULL) {
        perror(path);
        return 0;
    }

    while ((ch = fgetc(file)) != EOF) {
        putchar(ch);
    }

    if (ferror(file)) {
        perror(path);
        fclose(file);
        return 0;
    }

    fclose(file);
    return 1;
}

void print_usage(const char *program, const char *usage)
{
    fprintf(stderr, "Usage: %s %s\n", program, usage);
}
