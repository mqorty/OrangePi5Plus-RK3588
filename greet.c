#include <stdio.h>
#include "greet.h"

void greet(const char *name)
{
    printf("Hello, %s! Greetings from embedded linux developemnt.\n", name);
}

const char *version(void)
{
    return "1.0.0";
}
