#include <stdio.h>
#include "greet.h"

int main(int argc, char *argv[])
{
    const char *name = (argc > 1) ? argv[1] : "world";

    greet(name);
    printf("版本: %s\n", version());
    return 0;
}
