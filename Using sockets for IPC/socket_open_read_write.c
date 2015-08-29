#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <sys/time.h>
#define ROUTER_BASE_PORT  11100
#define NODE_BASE_PORT 22200
/**************EDIT THE HOST NAME HERE******************************/
#define HOSTNAME   "mPC"
/********************************************************************/


typedef struct message_format{
    int sender_id;      //port number of the sender 
    int receiver_id;    //port number of the receiver (on which it listens)
    int msg_type;
    double timestamp ;  // =time(NULL);seconds since January 1, 1970;
    int coming_from;
    char *message;
}msg ;
extern void write_to_file(int _new_socket_id);

int write_to_socket(int port_number,char *message_to_send){
    int my_socket;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    
    my_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (my_socket < 0) {
        error("opening socket error.Exiting.....");
        exit(0);
    }
    server = gethostbyname(HOSTNAME);
    if (server == NULL) {
        printf("No such host.Check the hostname and edit in file socket_open_read_write\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(port_number);
    if (connect(my_socket,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
        ("connection error.Exiting..\n");
        exit(0);
    }
    
   // printf("Writing to port_number:%d\n",port_number );
    if(write(my_socket,message_to_send,strlen(message_to_send))<0){
        puts("writing error.Exiting....\n");
        exit(0);

    }
    //printf("%s\n",message_to_send);
    close(my_socket);
    return 0;
}









int set_up_socket(int port_number){
    int my_socket = socket(AF_INET, SOCK_STREAM, 0);
    
    if (my_socket < 0) {
        printf("An error occured while opening the socket.\n");
        exit(0);
    }
    /*****************Redundant*****************/
    write_to_file(my_socket);       //to close the sockets if something goes wrong
    write_to_file(port_number);
    write_to_file(111111);
    /*************************/
    struct sockaddr_in router_address;
    memset((char *) &router_address,0, sizeof(router_address));    
    router_address.sin_family = AF_INET;
    router_address.sin_addr.s_addr = INADDR_ANY;
    router_address.sin_port = htons(port_number);

    if (bind(my_socket, (struct sockaddr *) &router_address,sizeof(router_address)) < 0){ 
        printf("Error occured while binding");
        exit(0);
    }
    
    return my_socket;    

}

char *read_from_socket(int my_socket){
    int back_log=10;
    printf("Listening...\n");
    listen(my_socket,back_log);
    struct sockaddr_in  sender_address;
    socklen_t sender_len;
    int temp_listener_socket;
    sender_len = sizeof(sender_address);
    temp_listener_socket = accept(my_socket,(struct sockaddr *) &sender_address,&sender_len);
    if (temp_listener_socket < 0) 
        printf("Error in accepting.\n");
    char *buffered_reader=(char *)malloc(sizeof(char)*512);
    memset(buffered_reader,0,512);
    if (read(temp_listener_socket,buffered_reader,255) < 0) {
        printf("Error while reading from socket\n");
        exit(0);

    }
    //printf("The message received: %s\n",buffered_reader);
    close(temp_listener_socket);
    return buffered_reader;

}




