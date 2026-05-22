#include <dirent.h>
#include <stdio.h>

int main(void)
{
    DIR *directory = opendir(".");
    struct dirent *entry;

#ifdef DEBUG
    fprintf(stderr, "[debug] listing current directory\n");
#endif

    if (directory == NULL) {
        perror(".");
        return 1;
    }

    while ((entry = readdir(directory)) != NULL) {
        if (entry->d_name[0] != '.') {
            puts(entry->d_name);
        }
    }

    closedir(directory);
    return 0;
}
