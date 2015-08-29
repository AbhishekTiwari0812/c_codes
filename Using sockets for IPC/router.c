#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#define ROUTER_BASE_PORT 11100
#define NODE_BASE_PORT 22200
#define TABLE_SIZE 256

extern int set_up_socket(int port_number);
extern char *read_from_socket(int sockfd);
extern int *find_shorest_path_between_routers(int router_1,int router_2);
extern int check_node_list(int RouterId,int node_id);
int ROUTER_ID;

typedef struct message_format{
    int sender_id;      //port number of the sender 
    int receiver_id;    //port number of the receiver (on which it listens)
    int msg_type;			//1 for broadcast,2 for unicast,3 for control
    double timestamp ;  // =time(NULL);seconds since January 1, 1970;
    int coming_from;		//id of the node from where the message is received
    int message_id;		//message id.(Should be unique)
    char message[300];		//paylaod

}msg ;
int message_history_table[TABLE_SIZE];

long long int message_count;

int check_history(int message_id){
	int i;
	for(i=0;i<TABLE_SIZE;i++)
		if(message_id==message_history_table[i])
			return 1;
	return 0;
}
void insert_into_history_table(int message_id){
	static int table_current_size=0;
    if(table_current_size==TABLE_SIZE){
		memset(message_history_table,0,TABLE_SIZE);
		table_current_size=0;
	}
	message_history_table[table_current_size++]=message_id;
	return;
}

extern char *code_message(msg *a);
extern msg *decode_message(char *encoded_msg);
extern int write_to_socket(int port_number,char *message_to_send);
extern int get_parent_id(int node_id);

void do_stuff(msg *received_message){
    if(received_message->msg_type==1){
        //Broadcast message.
        if(check_history(received_message->message_id))
        	return;
        insert_into_history_table(received_message->message_id);
        int *neighbours=get_neighbours(ROUTER_ID);
        int size=neighbours[0];
        int i;
        printf("received by router id:%d from %d.\nSending to others.\n", ROUTER_ID,received_message->coming_from);
        received_message->coming_from=ROUTER_BASE_PORT+ROUTER_ID;
        for(i=1;i<=size;i++){
        	write_to_socket(ROUTER_BASE_PORT+neighbours[i],code_message(received_message));
        }
    }
		//unicast message
    else if(received_message->msg_type==2)
        if(received_message->receiver_id!=check_node_list(ROUTER_ID,received_message->receiver_id)) {
            int *path=find_shorest_path_between_routers(ROUTER_ID,get_parent_id(received_message->receiver_id));
            int next_node_to_send_to=path[1];
            if(next_node_to_send_to==-1){
                printf("Next node not found.Exiting...\n");
                exit(0);
            }
            received_message->coming_from=ROUTER_BASE_PORT+ROUTER_ID;
            printf("Passing from router %d to router %d\n",ROUTER_ID,next_node_to_send_to);
            write_to_socket(ROUTER_BASE_PORT+next_node_to_send_to,code_message(received_message));
        }
        else{
            received_message->coming_from=ROUTER_BASE_PORT+ROUTER_ID;
            printf("Passing from router %d to end node %d\n",ROUTER_ID,received_message->receiver_id );
            write_to_socket(NODE_BASE_PORT+received_message->receiver_id,code_message(received_message));
        }
    //Control message
    else if(received_message->msg_type==3){
      	//operations supported==> print the count of the messages received and forwarded on each link.
    	if(check_history(received_message->message_id))		//node is already visited.
        	return;
        insert_into_history_table(received_message->message_id);
        int *neighbours=get_neighbours(ROUTER_ID);
        int size=neighbours[0];
        int i;
      // printf("received by router id:%d.\nSending to others.\n", ROUTER_ID);
        received_message->coming_from=ROUTER_BASE_PORT+ROUTER_ID;
        printf("Message received by Router # %d are %llu (including the current one!)\n",ROUTER_ID,message_count+1 );
        for(i=1;i<=size;i++){
        	write_to_socket(ROUTER_BASE_PORT+neighbours[i],code_message(received_message));
        }
    }
    message_count++;


}

//gcc router.c -o router graph.c socket_open_read_write.c -w message_encode_decode.c -w
//./router port_number
int main(int argc, char *argv[]){
    /*Initializing globals*/
	memset(message_history_table,0,TABLE_SIZE);
    ROUTER_ID=atoi(argv[1]);
    message_count=0;
    
    //Setting up socket
	int port_number=ROUTER_BASE_PORT+ROUTER_ID;
    int my_socket=set_up_socket(port_number);
    char *msg_string;
    char c;
    msg *received_message;
	//listening now
    listen_again:
    initialize_graph();
    msg_string=read_from_socket(my_socket);
    received_message=decode_message(msg_string);    
   // free(msg_string);
    do_stuff(received_message);
   // printf("Press y to continue listening(you may have to press twice for assurance).n to kill the socket and exit.\n");
   // c=getchar();
   // c=getchar();
    //if(c=='y')
        goto listen_again;       
    close(my_socket);
    return 0; 

}
