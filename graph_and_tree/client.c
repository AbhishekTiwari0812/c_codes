#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#define handle_error(msg)  do { perror(msg); exit(EXIT_FAILURE); } while (0)

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in router_details;
    struct hostent *server;

    char buffer[256];
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }

    //at which the client will send the message.
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        handle_error("socket");

    /*********************************SET THE HOST NAME***********************************/
    server = (struct hostent *) "mPC";
    if (server == NULL) {
        puts("Host not found.One can check the host name of the router(host) by typing command \"hostname\" on the terminal.(without quotes)\n");
        exit(0);
    }
    /*************************************************************************************/
    
    memset((char *) &router_details, 0,sizeof(router_details));
    router_details.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&router_details.sin_addr.s_addr,
         server->h_length);
    router_details.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &router_details,sizeof(router_details)) < 0) 
        error("ERROR connecting");
    printf("Please enter the message: ");
    bzero(buffer,256);
    fgets(buffer,255,stdin);
    n = write(sockfd,buffer,strlen(buffer));
    if (n < 0) 
         error("ERROR writing to socket");
    bzero(buffer,256);
    n = read(sockfd,buffer,255);
    if (n < 0) 
         error("ERROR reading from socket");
    printf("%s\n",buffer);
    close(sockfd);
    return 0;
}