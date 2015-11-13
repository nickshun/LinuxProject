/****
 * main.c
 *
 * The ASCII character device driver mini test
 *
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#include "common.h"

int
main(argc, argv)
	int   argc;
	char* argv[];
{
    char buf[BSIZE];
    int fd, i, j, n;
	
	if((fd = open("/dev/asciimap", O_RDWR)) >= 0)
	{
		n = read(fd, buf, 10);
		
		j = 0;
		while (j < 12)
		{
			for (i = 0; i < n; i++)
			{
				printf("%c", buf[i]);
			}
					
			n = read(fd, buf, 10);
			j++;
		}

		printf ("\n\n");
		
		char writting[]  = "ThisIsA_test_This_should_be_in_there";
		

		n = write(fd, writting, sizeof(writting) - 1);
			
		close(fd);
		
		fd = open("/dev/asciimap", O_RDWR);

		n = read(fd, buf, 10);

                while (n > 0)
                {
                        for (i = 0; i < n; i++)
                        {
                                printf("%c", buf[i]);
                        }
                                        
                        n = read(fd, buf, 10);
                }

                printf ("\n\n");
		
		close(fd);
	}
	else
	{
		perror("open(/dev/asciimap) failed");
		exit(1);
	}

    exit(0);
}

/* EOF */
