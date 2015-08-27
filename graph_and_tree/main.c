#include <stdio.h>
#include <time.h>


typedef enum {
	BROADCAST=1,UNICAST=2,CONTROL=3

}MESSAGE_TYPE;	

typedef struct message_format{
	int sender_id;	//port number of the sender 
	int receiver_id;	//port number of the receiver (on which it listens)
	MESSAGE_TYPE msg_type;
	time_t timestamp ;	// =time(NULL);seconds since January 1, 1970;

}msg ;


int main(){
	return 0;
}