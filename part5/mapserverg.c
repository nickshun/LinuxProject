/* A simple server in the internet domain using TCP
 *    The port number is passed as an argument */
#include "header.h"

#include <sys/stat.h>
#include <fcntl.h>



int width = 50;
int height = 50;
int currentPos = 0;
int currentRow = 0;

void getMap(int fd, int sockfd, int width, int height);
int writeToDevice(int fd, char* str, int length);
int readFromDevice(int fd, char* str, int length);
int seekTo(int fd, int offset, int orig);
int writeLineToDevice(int fd, char* str, int length);
int getMapPos(int pos);
int getData(char* input, int length, int *width, int *height);
int main(int argc, char *argv[])
{
	int sockfd, newsockfd, portno;
     	socklen_t clilen;
	char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;
     	int n;
	char messageType;
	int width = 0;
	int height = 0;
	int  defaultDim;
     	
	portno = defaultPort;
     	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) 
      		error("ERROR opening socket");
     	bzero((char *) &serv_addr, sizeof(serv_addr));
     	serv_addr.sin_family = AF_INET;
     	serv_addr.sin_addr.s_addr = INADDR_ANY;
     	serv_addr.sin_port = htons(portno);
     	if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        	error("ERROR on binding");
     	listen(sockfd,5);
     	clilen = sizeof(cli_addr);
     	
	while(1)
	{
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
     		if (newsockfd < 0) 
          		error("ERROR on accept");
     		bzero(buffer,256);
     		

		int length = readFromBuffer(newsockfd, buffer, sizeof(buffer) - 1);
		
		if (length > 0)
		{
			printf("got a message\n");
		}
		
		if (length == sizeof(char) + sizeof(int) + sizeof(int))
		{	
			char* type = buffer;
			int* w = buffer + sizeof(char);
			int* h = buffer + sizeof(char) + sizeof(int);
			
			printf("width: %d height: %d\n", *w, *h);
                        int fd1 = open("/dev/asciimap", O_RDWR);
                        if (fd1 >= 0)
                        {
                                getMap(fd1, newsockfd, *w, *h);
                                close(fd1);
                        }
                        else
                        {
                                printf("\n\ndidnt open device %d\n\n", fd1);
                        }       
			break;
		}

		/*
		n = read(newsockfd,buffer,255);
     		if (n < 0) 
			error("ERROR reading from socket");
		printf("Here is the message: %s\n",buffer);
		
		if(getData(buffer, sizeof(buffer) - 1, &width, &height) != 0)
		{
			printf("width: %d height: %d\n", width, height);
			int fd1 = open("/dev/asciimap", O_RDWR);
			if (fd1 >= 0)
        		{
				getMap(fd1, newsockfd, width, height);
				close(fd1);
			}
			else
			{
				printf("\n\ndidnt open device %d\n\n", fd1);
			}	
		}
		else
		{
			char errormsgs[] = 
			n = write(sockfd,map,sizeof(map));
       			 if (n < 0) error("ERROR writing to socket");
		}

		*/
		
		
     		close(newsockfd);
	}
     	return 0; 
}

int readFromBuffer(int fd, char* buff, int length)
{
	int bytesRead = 0;
	int n;

	while (bytesRead < length)
	{
		n = read(fd,buff, 1);
                if (n < 0) 
		{
			error("ERROR writing to socket");
			break;
		}
		if (n == 0)
			break;
		
		bytesRead += n;
	}
	
	return bytesRead;
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

void getMap(int fd, int sockfd, int width, int height)
{
        int mapSize = width * height;
	char map[mapSize];
        int validChar;
        int lengthWritten = 0;
        char ch[1];
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
	
	n = write(sockfd,map,sizeof(map));
        if (n < 0) error("ERROR writing to socket");
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

int getData(char* input, int length, int *width, int *height)
{
	int fp = fopen("logFile.txt", "a");
	fprintf(fp, input);
	fflush(fp);
	fclose(fp);
	
	
	int bytesRead = 0;
	int num1 = -1;
	int num2 = -1;
	char* dataType;

	char* type = strtok(input,"~");

	if (type != NULL)
	{
		dataType = type;
	}
		
	if (dataType[0] != 'M')
	{
                return 0;
	}
	type = strtok(NULL, "~");
	
	if (type != NULL)
	{
		num1 = atoi(type);
	}

	type = strtok(NULL, "~");

        if (type != NULL)
        {
                num2 = atoi(type);
        }
		
	if (num1 == 0)
	{
		*width = 50;
		*height = 50;
		return 1;
	}
	else
	{
		*width = num1;
		*height = num2;
		return 1;
	}
}

/* new stuff */
void printMap()
{
	/*open new Xterm window*/

	int mapSize = width * height;
	char map[mapSize];
        int validChar;
        int lengthWritten = 0;
        char ch[1];
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

	int allEmpty = 1;

	int k = 0;
	
	for (k = 0; k < mapSize; ++k)
	{
		printf('%c', map[k]);
		if (map[k] != ' ')
		{
			allEmpty = 0;
		}
	}

	if(allEmpty)
	{
		atExit();
	{
}
void validateMessage(char inputMessage, char recievedLetter, char* map)
{
	/*update protocol side on the server*/
	/*Errors should actually log and be E Messages*/
	int currentIndex;	
	if(inputMessage == 'K')
	{
		if(x >= 0 && y >= 0 && x<width && y<height)
		{
			currentIndex = y * width + x;
			
			if(recievedLetter == map[currentIndex])
			{
				/*Update Map accordingly*/
				map[currentIndex] = ' ';
				
			}
			else
			{
				printf("ERROR: Letter dne");
			}
		}
		else
		{
			printf("ERROR: OUT OF BOUNDS");
		}
	}
	else if (inputMessage == "GO")
	{
		char * s = socketDescriptorID + " " + IPaddress + " " + PortNumber;
		printf("Game Over for Client %s");
		/*Reset map with device IOCTL to default*/
	}
}

