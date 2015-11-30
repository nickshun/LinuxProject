/* A simple server in the internet domain using TCP
 *    The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include "header.h"

#include <sys/stat.h>
#include <fcntl.h>



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

int main(int argc, char *argv[])
{
	int sockfd, newsockfd, portno;
     	socklen_t clilen;
     	char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;
     	int n;
	char messageType;
	int width, height, defaultDim;
     	if (argc < 2) {
		fprintf(stderr,"ERROR, no port provided\n");
         	exit(1);
     	}
     	sockfd = socket(AF_INET, SOCK_STREAM, 0);
     	if (sockfd < 0) 
        	error("ERROR opening socket");
     	bzero((char *) &serv_addr, sizeof(serv_addr));
     	portno = atoi(argv[1]);
     	serv_addr.sin_family = AF_INET;
     	serv_addr.sin_addr.s_addr = INADDR_ANY;
     	serv_addr.sin_port = htons(portno);
     	if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        	error("ERROR on binding");
     	listen(sockfd,5);
     	clilen = sizeof(cli_addr);
     	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
     	if (newsockfd < 0) 
          	error("ERROR on accept");
     	bzero(buffer,256);
     	n = read(newsockfd,buffer,255);
     	if (n < 0) error("ERROR reading from socket");
     	printf("Here is the message: %s\n",buffer);
	if(sizeof(buffer) == (sizeof(char)+sizeof(int)))
	{
		if(buffer[0] == 'M')
			messageType = buffer[0];
		if(buffer[1] == ' ')
			width = buffer[2];
	}
	if(sizeof(buffer) == (sizeof(char)+(sizeof(int)*2)))
	{
		 if(buffer[0] == 'M')
                        messageType = buffer[0];
                if(buffer[1] == ' ')
                        width = buffer[2];
                if(buffer[3] == ' ')
                        height = buffer[4];
	}
     	n = write(newsockfd,"I got your message",18);
	
	int fd1 = open("/dev/asciimap", O_RDWR);
	if (fd1 >= 0)
        {
		printf("\n\nopened device\n\n");
		printMap(fd1);
	}
	else
	{
		printf("\n\ndidnt open dvice %d\n\n", fd1);
	}
	
	
	
	
	
	
     	if (n < 0) error("ERROR writing to socket");
     	close(newsockfd);
     	close(sockfd);
     	return 0; 
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
                        map[lengthWritten] = (char*)('\n');
                }
                else
                {
                        validChar = read(fd, ch, 1);

                        if (validChar == 0)
                        {
                                map[lengthWritten] = (char*)(' ');
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
