#include <stdio.h>
#include <stdlib.h>

FILE *source;

int main(int argc, char const *argv[])
{
    switch (argc) {
        case 1:
            fprintf(stderr, "%s: missing source file name\n", argv[0]);
            exit(-2);
        case 2:
            source = fopen(argv[1], "r");

            if (source == NULL) {
                fprintf(stderr, "%s: not found\n", argv[1]);
                exit(-3);
            }

            break;
    }

    return 0;
}