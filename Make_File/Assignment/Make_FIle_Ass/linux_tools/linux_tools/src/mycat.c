#include "utils.h"

int main(int argc, char *argv[])
{
    if (argc != 2) {
        print_usage(argv[0], "file.txt");
        return 1;
    }

    return print_file(argv[1]) ? 0 : 1;
}
