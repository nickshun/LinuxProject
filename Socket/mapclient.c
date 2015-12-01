#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include "header.h"

int main(int argc, char *argv[])
{
    	int sockfd, portno, n, width, height;
    	struct sockaddr_in serv_addr;
    	struct hostent *server;
	
    	char buffer[50 * 50];
    	/*if (argc < 3) {
       	fprintf(stderr,"usage %s hostname port\n", argv[0]);
       	exit(0);
    	}*/
	
	if (argc < 2)
	{
		fprintf(stderr,"usage %s hostname\n", argv[0]);
        	exit(0);
	}
	portno = defaultPort;
	
    	sockfd = socket(AF_INET, SOCK_STREAM, 0);
    	if (sockfd < 0) 
        	error("ERROR opening socket");
    	server = gethostbyname(argv[1]);
    	if (server == NULL) {
        	fprintf(stderr,"ERROR, no such host\n");
        	exit(0);
    	}
    	bzero((char *) &serv_addr, sizeof(serv_addr));
    	serv_addr.sin_family = AF_INET;
    	bcopy((char *)server->h_addr, 
        	 (char *)&serv_addr.sin_addr.s_addr,
         	server->h_length);
    	serv_addr.sin_port = htons(portno);
    	if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        	error("ERROR connecting");
    	printf("Please enter the message: ");
    	bzero(buffer,256);
    	fgets(buffer,255,stdin);

    	n = write(sockfd,buffer,strlen(buffer));
    	if (n < 0) 
        	 error("ERROR writing to socket");
    	bzero(buffer,sizeof(buffer)+1);
    	n = read(sockfd,buffer,sizeof(buffer));
    	if (n < 0) 
        	 error("ERROR reading from socket");
    	printf("%s\n",buffer);
    	close(sockfd);
    	return 0;
}
