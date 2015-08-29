#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#define ROUTER_BASE_PORT 11100
#define NODE_BASE_PORT 22200


typedef struct message_format{
	int sender_id;      //port number of the sender 
    int receiver_id;    //port number of the receiver (on which it listens)
    int msg_type;			//1 for broadcast,2 for unicast,3 for control
    double timestamp ;  // =time(NULL);seconds since January 1, 1970;
    int coming_from;		//id of the node from where the message is received
    int message_id;		//message id.(Should be unique)
    char *message;
    
}msg ;


extern int write_to_socket(int port_number,char *message_to_send);
extern char *code_message(msg *a);

//gcc test_client.c -o test_client message_encode_decode.c socket_open_read_write.c graph.c -w
int main(){
	msg *new_msg;
	char c='y';
	int stat;	
	time_t current_time;
	puts("IF MSG TYPE IS BROADCAST OR CONTROL.PLEASE ENTER SENDER ID AND RECEIVER ID(STARTING ROUTER ID) SAME!");
	while(c=='y'){
		new_msg=(msg *)malloc(sizeof(msg));
		printf("Input the sender_id\n");
		scanf("%d",&(new_msg->sender_id));
		printf("Input the receiver_id\n");
		scanf("%d",&(new_msg->receiver_id));
		printf("Input the message type.\n1 for BROADCAST,2 for UNICAST,3 for CONTROL\n");
		scanf("%d",&(new_msg->msg_type));
		printf("Input the message\n");
		new_msg->message=(char *)malloc(sizeof(char)*300);
		scanf(" %[^\n]s",new_msg->message);		//to input message in one line.Spaces allowed.
		/***************CLIENT ID.SHOULD NOT BE USED BY ANY OTHER NODE.************************/
		new_msg->coming_from=-22;		//test client id.......!!!!
		current_time=time(&current_time);
		new_msg->timestamp=(double)current_time;
		c=getchar();		//to ignore the new line character
		srand(time(NULL));
		int r = rand();
		new_msg->message_id=r;
		try_again:
		puts("Sending message(all info included)\n");
		if(new_msg->msg_type==2){
			printf("sender id:%d\n",new_msg->sender_id);
			printf("receiver id:%d\n",new_msg->receiver_id);
			printf("message type:%d\n",new_msg->msg_type );
			printf("timestamp:%f\n",new_msg->timestamp );
			printf("coming from:%d\n",new_msg->coming_from);
			printf("message id:%d\n",new_msg->message_id);			
			printf("message:%d\n",new_msg->message);
			stat=write_to_socket(new_msg->sender_id+NODE_BASE_PORT,code_message(new_msg));
		}		
		else if(new_msg->msg_type==1||new_msg->msg_type==3){
			printf("sender id:%d\n",new_msg->sender_id);
			printf("message type:%d\n",new_msg->msg_type );
			printf("timestamp:%f\n",new_msg->timestamp );
			printf("coming from:%d\n",new_msg->coming_from);
			printf("message id:%d\n",new_msg->message_id);
			printf("message:%d\n",new_msg->message);
			stat=write_to_socket(ROUTER_BASE_PORT+new_msg->sender_id,code_message(new_msg));
		}
		if(stat!=0)
				goto try_again;
		printf("\nDo you want to send another message?\nPress y for \"YES\".\nAny other key (except power button) for \"NO\"\n");
		c=getchar();		

	}
	return 0;
}
