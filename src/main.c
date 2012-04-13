#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "misc.h"
#include "linkedlist.h"
#include "ini.h"

void usage(char *name)
{
    fprintf(stderr, "USAGE: %s <file>\n", name);
    exit(1);
}

int main(int argc, char *argv[])
{
    INI *ini;

    if (argc != 2)
        usage(argv[0]);

    ini = parse_ini(argv[1]);
    print_ini(ini);

    return 0;
}
