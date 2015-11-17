#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

void scan(int fid);
void usage(int argc, char **argv);

int main(int argc, char **argv)
{
	int input_fd;
	int numIDs = argc-1;
	int i;

	if(numIDs == 0)
	{
		usage(argc, argv);
	}

	for(i = 1; i<=numIDs; i++)
	{
		printf("\nCurrent File: ");
		printf (argv[i]);
		printf("\n");

		/*get File from input*/
		input_fd = open(argv[i], O_RDONLY);

		if(input_fd == -1)
		{
			perror("\nProblem reading in file: ");/*REASON*/
		}
		else
		{
			scan(input_fd);
		}

		close(input_fd);
	}
	printf("\n Part12 terminates.\n");
}
void scan(int fid)
{
	char fileBuf[4000];
	int linesOfText = 0;
	int wordsNo = 0;
	int length = 0;
	int WIDTH = 51;
	int row = 0;
 
	int sizeT = read(fid, fileBuf, sizeof(fileBuf));
	int counter;	
	

	while(sizeT !=0)
	{
		for (counter = 0; counter<sizeT;counter++)
		{
			/*if \n*/
			if(fileBuf[counter] == '\n')
			{
				linesOfText++;
				wordsNo++;

				if(length <= WIDTH)
				{
					while(length <= WIDTH)
					{
						printf("%c",' ');
						length++;
					}
					length = 0;
					printf("\n");
				}

			}
			/*if " ", \n \r or \t*/
			if ((fileBuf[counter]== ' ') || (fileBuf[counter]== '\r') || (fileBuf[counter]== '\t') )
			{
				wordsNo++;
			}
			else
			{
				if (fileBuf[counter] != '\n')
				{
					length++;
					if (linesOfText<=WIDTH)
					{
						if(length<=WIDTH)
						{
							printf("%c",fileBuf[counter]);
						}
					}
				}					
			}	
		}
		sizeT = read(fid, fileBuf, sizeof(fileBuf));
	}
	/*end of file
	printf("\nLines of text: ");
	printf("%d", linesOfText);
	printf("\n");
	printf("Words: ");
	printf("%d", wordsNo);
	printf("\n");*/	
	
}
void usage(int argc, char ** argv)
{
	int count;
	printf("\n");
	for (count = 0; count < argc; count++)
	{
		printf("Usage: ");
		printf("%s", argv[count]);
		printf(" ");
	}	
}
