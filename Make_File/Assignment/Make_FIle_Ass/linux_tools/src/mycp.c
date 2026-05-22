#include "utils.h"

int main(int argc, char *argv[])
{
    if (argc != 3) {
        print_usage(argv[0], "source.txt destination.txt");
        return 1;
    }

    return copy_file(argv[1], argv[2]) ? 0 : 1;
}
