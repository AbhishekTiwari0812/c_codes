#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

typedef struct message_format{
    int sender_id;      //port number of the sender 
    int receiver_id;    //port number of the receiver (on which it listens)
    int msg_type;
    double timestamp ;  // =time(NULL);seconds since January 1, 1970;
    int coming_from;
    int message_id;
    char *message;
}msg;

char *code_message(msg *a){
    char *coded_msg=(char *)malloc(sizeof(int)*5+sizeof(double)+300*sizeof(char)+20);
    coded_msg[0]='\0';
    char num[10];
  
   	snprintf(num,10,"%d",a->sender_id);
    strcat(coded_msg,num);
    strcat(coded_msg," ");
  
    snprintf(num,10,"%d",a->receiver_id);
    strcat(coded_msg,num);
    strcat(coded_msg," ");
  
	snprintf(num,10,"%d",a->msg_type);
    strcat(coded_msg,num);
    strcat(coded_msg," ");	    


    int dec_pl,sign;
    strcat(coded_msg,fcvt((double) a->timestamp, 0, &dec_pl, &sign));
    strcat(coded_msg," ");
    
    snprintf(num,10,"%d",a->coming_from);
    strcat(coded_msg,num);
    strcat(coded_msg," ");      
  

    snprintf(num,10,"%d",a->message_id);
    strcat(coded_msg,num);
    strcat(coded_msg," ");

    strcat(coded_msg," ");
    strcat(coded_msg,a->message);
  
 // 	puts("This is encoded string:");
 // 	puts(coded_msg);
    return coded_msg;
}
msg *decode_message(char *encoded_msg){
    char* token;
    char* tofree;
    msg *my_message=malloc(sizeof(msg));
    if (encoded_msg != NULL) {
        tofree = encoded_msg;
        token = strsep(&encoded_msg, " ");
        my_message->sender_id=atoi(token);
        token = strsep(&encoded_msg, " ");
        my_message->receiver_id=atoi(token);
        token = strsep(&encoded_msg, " ");
        my_message->msg_type=atoi(token);
        token = strsep(&encoded_msg, " ");
        sscanf(token,"%lf",&my_message->timestamp);
        token = strsep(&encoded_msg, " ");
        my_message->coming_from=atoi(token);
       	
        token = strsep(&encoded_msg, " ");
        my_message->message_id=atoi(token);
        //some problem here!!
        token = strsep(&encoded_msg, "");
        int i;
        my_message->message=(char *)malloc(sizeof(char)*300);
        for(i=0;i<300;i++)
        	my_message->message[i]=token[i];
        free(tofree);
    }
    return my_message;
}


int check_function(){

	msg a;
	time_t current_time;
	a.sender_id=10;
	a.receiver_id=20;
	a.msg_type=1;
	current_time=time(&current_time);
	a.timestamp=(double)current_time;
	//printf("time:%lf\n",a.timestamp );
	a.message=(char *)malloc(sizeof(char)*300);
	a.message="This is the message I am trying to post.";
	char *m_msg=code_message(&a);
	msg *myMessage=decode_message(m_msg);
	printf("%d ,%d ,%d ,%lf, %s\n", myMessage->sender_id,myMessage->receiver_id,myMessage->msg_type,myMessage->timestamp,myMessage->message);
}










