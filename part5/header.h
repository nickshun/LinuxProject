#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>



long defaultPort = 23032;
char defaultIP[] = "127.0.0.1";

typedef struct DimensionStruct
{
	char messagetype;
	int width;
	int height;
} DimensionStruct;

void error(const char *msg)
{
    perror(msg);
    exit(0);
}
