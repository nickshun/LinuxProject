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

/*int width = 50;
int height = 50;
int currentIndex = 0;

int writeToDevice(int fd, char* str, int length)
{
	int bytes_wrote = 0;
	
	while (length > 0)
	{
		if (currentIndex % width == 0)
		{
			if (write(fd, '\n', 1) > 0)
			{
				bytes_wrote++;
				currentIndex++;
			}
		}
		else
		{
			if (write(fd, str[bytes_wrote], 1) > 0)
			{
				bytes_wrote++;
				currentIndex++;
				length--;
			}
		}
	}
	
	if (currentIndex )
	
	return bytes_wrote;
}*/




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
		
		while (n > 0)
		{
			for (i = 0; i < n; i++)
			{
				printf("%c", buf[i]);
			}
					
			n = read(fd, buf, 10);
		}
		
		c = lseek(fd, (off_t)100, SEEK_SET);

		if (c < 0)
		{
			printf("\n\nfailed seeking\n\n");
		}

		printf ("\n\n");
		
		char writting[]  = "ThisIsA_test_This_should_be_in_there_this_should_also_probably_maybe_span_mulptiple_lines_definitely_now_becuasse_of_all_The_Extra_crap";
		

		n = write(fd, writting, sizeof(writting) - 1);
			
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
				
		c = ioctl(fd, IOCTL_RESET);

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
