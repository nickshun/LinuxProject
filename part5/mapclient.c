#include "header.h"

int main(int argc, char *argv[])
{
    	int sockfd, portno, n, width, height;
    	struct sockaddr_in serv_addr;
    	struct hostent *server;
	
    	char buffer[50 * 50];
    	
	
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
	
	char type = ' ';
	width = 0; 
	height = 0;

	printf("Enter Type(M)");
	type = getchar();
	fflush(stdin);
	printf("Enter width or 0 for basic map: ");
	scanf("%d", &width);
	fflush(stdin);
	getchar();	
	if (width != 0)
	{
		printf("Enter height: ");
		scanf("%d", &height);
		printf("%d", height);
		fflush(stdin);
		printf("here");
		DimensionStruct message = {type, width, height};	
		printf("here2");
		n = write(sockfd, &message, sizeof(message));
        	if (n < 0)
                 	error("ERROR writing to socket");

	}
	
    	/*printf("Please enter the message Type: ");
    	bzero(buffer,256);
    	fgets(buffer,255,stdin);*/

    	/*
  	n = write(sockfd,buffer,strlen(buffer));
    	if (n < 0) 
        	 error("ERROR writing to socket");
	*/
	
    	bzero(buffer,sizeof(buffer)+1);
    	n = read(sockfd,buffer,sizeof(buffer));
    	if (n < 0) 
        	 error("ERROR reading from socket");
    	printf("%s\n",buffer);
    	close(sockfd);
    	return 0;
}
