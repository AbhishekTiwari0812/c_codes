#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#define ROUTER_BASE_PORT 11100
#define NODE_BASE_PORT 22200


extern int set_up_socket(int port_number);
extern char *read_from_socket(int sockfd);

int NODE_ID;

typedef struct message_format{
    int sender_id;      //port number of the sender 
    int receiver_id;    //port number of the receiver (on which it listens)
    int msg_type;			//1 for broadcast,2 for unicast,3 for control
    double timestamp ;  // =time(NULL);seconds since January 1, 1970;
    int coming_from;		//id of the node from where the message is received
    int message_id;		//message id.(Should be unique)
    char message[300];	//payload
}msg ;

extern char *code_message(msg *a);
extern msg *decode_message(char *encoded_msg);
extern int write_to_socket(int port_number,char *message_to_send);
extern int get_parent_id(int node_id);


void forward_to_router(msg *received_message){	
    if(received_message->msg_type==1){
        printf("End nodes don't handle broadcast messages\n");
        exit(0);
        }
	 //forwards the message to the parent router.
    else if(received_message->msg_type==2){
        int parent_router=get_parent_id(NODE_ID);
        printf("Passing from end node %d to router %d\n",NODE_ID,parent_router);
        received_message->coming_from=NODE_BASE_PORT+NODE_ID;
        write_to_socket(ROUTER_BASE_PORT+parent_router,code_message(received_message));
    }
    else if(received_message->msg_type==3){
        printf("End nodes don't handle control messages\n");
        exit(0);
        }


}

//gcc end_node.c -o end_node graph.c  socket_open_read_write.c message_encode_decode.c -w

int main(int argc, char *argv[]){
    NODE_ID=atoi(argv[1]);
    initialize_graph();
    int port_number=NODE_BASE_PORT+NODE_ID;
    int my_socket=set_up_socket(port_number);
    char *msg_string;
    char c;
    msg *received_message;
    listen_again:
    msg_string=read_from_socket(my_socket);
    received_message=decode_message(msg_string);    
   //free(msg_string);
    if(received_message->coming_from==-22){
    	forward_to_router(received_message);
    }
    else{
        printf("received message from router:%d\n",received_message->coming_from);
        puts(code_message(received_message));
    }
  //  printf("Press y to continue listening.n to kill the socket and exit.\n");
   // c=getchar();
   // if(c=='y')
        goto listen_again;
    close(my_socket);
    return 0; 

}
