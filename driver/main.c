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

int width = 50;
int height = 50;
int currentPos = 0;
int currentRow = 0;

void printMap(int fd);
int writeToDevice(int fd, char* str, int length);
int readFromDevice(int fd, char* str, int length);
int seekTo(int fd, int offset, int orig);
int writeLineToDevice(int fd, char* str, int length);
int getMapPos(int pos);

int getMapPos(int currPos)
{
	int pos = currPos;
	int mapPos = 0;
	
	while (pos > width - 1)
	{
		mapPos += width;
		pos -= (width - 1);
	}
	
	mapPos += pos;
	
	return mapPos;
}

int readFromDevice(int fd, char* str, int length)
{
	int mapPos = getMapPos(currentPos);
	int endPos = getMapPos(mapPos + length);

	length -= ((int)(endPos / width) - (int)(mapPos / width));

	int bytes_read = read(fd, str, length);
	
	if (bytes_read >= 0)
	{	
		currentPos += bytes_read;
	}
		
	return bytes_read;
}

int writeLineToDevice(int fd, char* str, int length)
{
        int i;
        int n = 1;
        int bytes_wrote = 0;
        char ch[] = " ";

	if (length > width)
	{
		length = width - 1 ;
	}

        while (bytes_wrote < length && n > 0)
        {
                ch[0] = str[bytes_wrote];

                if (ch[0] == '\n')
                {
                        ch[0] = ' ';
                        while (n > 0 && (currentPos) % (width - 1) != 0)
                        {
                                n = write(fd, ch, 1);
                                currentPos++;
                        }
                        bytes_wrote++;
                }
                else if (ch[0] == '\t')
                {
                        i = 0;
                        ch[0] = ' ';
                        while (n > 0 && (currentPos) % (width - 1) != 0 && i < 8)
                        {
                                n = write(fd, ch, 1);
                                currentPos++;
                                i++;
                        }
                        bytes_wrote++;
                }
                else
                {
                        n = write(fd, ch, 1);
                        currentPos++;
                        bytes_wrote++;
                }
        }

        return bytes_wrote;
}

int writeToDevice(int fd, char* str, int length)
{
	int i;
	int n = 1;
	int bytes_wrote = 0;
	char ch[] = " ";
	
	while (bytes_wrote < length && n > 0)
	{
		ch[0] = str[bytes_wrote];
			
		if (ch[0] == '\n')
		{
			ch[0] = ' ';
			while (n > 0 && (currentPos) % (width - 1) != 0)
			{
				n = write(fd, ch, 1);
				currentPos++;
			}
			bytes_wrote++;
		}
		else if (ch[0] == '\t')
		{
			i = 0;
			ch[0] = ' ';
			while (n > 0 && (currentPos) % (width - 1) != 0 && i < 8)
                        {
                                n = write(fd, ch, 1);
                                currentPos++;
				i++;
                        }
                        bytes_wrote++;
		}
		else
		{
			n = write(fd, ch, 1);
			currentPos++;
			bytes_wrote++;
		}
	}

	return bytes_wrote;
}

void printMap(int fd)
{
	int mapSize = width * height;
	char* map[mapSize];
	int validChar;
	int lengthWritten = 0;	
	char* ch[1];
	int i;
	int  n;
	char buf[1024];

	memset(map, '0', sizeof(map));

	seekTo(fd, (off_t)0, SEEK_SET);
	
	while (lengthWritten < mapSize)
	{
		if (lengthWritten % width == 0)
		{
			map[lengthWritten] = '\n';
		}
		else
		{
			validChar = read(fd, ch, 1);
			
			if (validChar == 0)
			{	
				map[lengthWritten] = ' ';
			}
			else if (validChar > 0)
			{
				map[lengthWritten] = ch[0];
				currentPos++;
			}
		}
		lengthWritten++;
	}
	
	printf("\n");
	
	for (i = 0; i < mapSize; i++)
	{
		printf("%c", map[i]);
	}
	
	printf("\n");
	
}

int seekTo(int fd, int offset, int orig)
{
	int mapPos = 0;
        int endPos = 0;
	
	switch(orig)
	{
	case 0:
		endPos = getMapPos(mapPos + offset);
		offset -= ((int)(endPos / width) - (int)(mapPos / width));
		break;
	case 1:
		mapPos = getMapPos(currentPos);
		endPos = getMapPos(mapPos + offset);
		offset -= ((int)(endPos / width) - (int)(mapPos / width));
		break;
	case 2:
		mapPos = width * height - 1;
		endPos = getMapPos(mapPos - offset);
		offset -= ((int)(endPos / width) - (int)(mapPos / width));
		break;
	}

	currentPos = lseek(fd, (off_t)offset, orig);
	
	return currentPos;
}

int
main(argc, argv)
	int   argc;
	char* argv[];
{
    char buf[1024];
    int fd, i, j, n, c;
	
	if ((fd = open("/dev/asciimap", O_RDWR)) >= 0)
	{
		printMap(fd);
		
		seekTo(fd, 0, SEEK_SET);
		
		readFromDevice(fd, buf, 100);

		char writting[]  = "ThisIsAi_test\t_This_should\n_be_in_there_this_should_also_probably_maybe_span_mulptiple_lines_definitely_now_becuasse_of_all_The_Extra_stuff";


		writeToDevice(fd, writting, sizeof(writting) - 1);
		

		char line1[] = "this_line_better_be_over_fifty_characters_long_because_I'm_not_counting";
		char line2[] = "this_one_is_less";
		
		
		seekTo(fd, 7 * width, SEEK_SET);

		writeLineToDevice(fd, line1, sizeof(line1) - 1);
		writeLineToDevice(fd, line2, sizeof(line2) - 1);
			
		printMap(fd);

		close(fd);
	}
	else
	{
                perror("open(/dev/asciimap) failed");
                exit(1);
        }



	/*if((fd = open("/dev/asciimap", O_RDWR)) >= 0)
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
	}*/

    exit(0);
}

/* EOF */
