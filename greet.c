#include <stdio.h>
#include "greet.h"

void greet(const char *name)
{
    printf("Hello, %s! 来自嵌入式 Linux 的问候。\n", name);
}

const char *version(void)
{
    return "1.0.0";
}
