#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

long defaultPort = 23032;
char defaultIP[] = "127.0.0.1";

void error(const char *msg)
{
    perror(msg);
    exit(0);
}
