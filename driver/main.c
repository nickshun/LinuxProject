/****
 * main.c
 *
 * The ASCII character device driver mini test
 *
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#include "common.h"

int
main(argc, argv)
	int   argc;
	char* argv[];
{
    char buf[1024];
    int fd, i, j, n, c;
	
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
		
		char writting[]  = "ThisIsA_test_This_should_be_in_there_this_should_also_probably_maybe_span_mulptiple_lines_definitely_now_becuasse_of_all_The_Extra_crap";
		

		n = write(fd, writting, sizeof(writting));
			
		close(fd);
		if((fd = open("/dev/asciimap", O_RDWR)) >= 0)
	        {
		n = read(fd, buf, 10);

                while (n > 0)
                {
                        for (i = 0; i < n; i++)
                        {
                                printf("%c", buf[i]);
                        }
                                        
                        n = read(fd, buf, 10);
                }
		
		char endText[] = "This_text_should_be_atTheEndOfTheWallAndAddNEwLinesToTheHeightCheckDmesgForThePrintKOfTheHeightAndMakeSureITsOver50";
		
		n = write(fd, endText, sizeof(endText));

                printf ("\n\n");
		
		close(fd);
		}

		if((fd = open("/dev/asciimap", O_RDWR)) >= 0)
	        {
                n = read(fd, buf, 10);

                while (n > 0)
                {
                        for (i = 0; i < n; i++)
                        {
                                printf("%c", buf[i]);
                        }

                        n = read(fd, buf, 10);
                }
		
		c = ioctl(fd, IOCTL_RESET, 0);

		if (c < 0)
		{
			printf("\nfailed\n");
		}
		
		close(fd);
		}
		printf("\n\n");
	}
	else
	{
		perror("open(/dev/asciimap) failed");
		exit(1);
	}

    exit(0);
}

/* EOF */
