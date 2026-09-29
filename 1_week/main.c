#include <stdio.h>
#include "greet.h"

int main(int argc, char *argv[])
{
    const char *name = (argc > 1) ? argv[1] : "world";

    greet(name);
    printf("Version: %s\n", version());
    return 0;
}
