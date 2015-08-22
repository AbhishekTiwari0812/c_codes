#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>//not needed for newer versions of linux
#include<sys/ipc.h>	//not needed for newer versions of linux
#include<sys/msg.h>
#define KEY1 1234
#define KEY2 2341
#define KEY3 3412
#define KEY4 4123

//same format of the message should be maintained across all the messages
typedef struct msg_to_pass{
	long priority;
	int pid;		//process id.Not needed in this program
	int temp;		//temperature;
	int stable;		//stabilty flag.1 means stable,-1 means unstable.
}msg;



int main(int argc,char* argv[]){
	int proc_num=atoi(argv[1]);				// should be 0,1,2,3.
	int init_temp=atoi(argv[2]);			//initial temperature.
	int old_temp=init_temp;
	int new_temp=init_temp;
	int status;							//for error check in msgrcv and msgsnd
	int i;								//loop counter
	int msqid_rcv;
	int central_temp;
	msg *proc_msg,*msg_to_send;
	proc_msg = (msg *)malloc(sizeof(msg));
	msg_to_send=(msg *)malloc(sizeof(msg));
	
	//printf("Process:%d has been started\n",proc_num);
	if(proc_num==0)
		msqid_rcv=msgget(KEY1 , 0600 | IPC_CREAT);   
	else if(proc_num==1)
		msqid_rcv=msgget(KEY2 , 0600 | IPC_CREAT);   
	else if(proc_num==2)
		msqid_rcv=msgget(KEY3 , 0600 | IPC_CREAT);   
	else if(proc_num==3)
		msqid_rcv=msgget(KEY4 , 0600 | IPC_CREAT);   
	
	if(msqid_rcv<0){
		printf("Error Occured while creating the msg box\nError returned value:%d\nTry again.\n",msqid_rcv);
		return 0;
	}

	//	printf("MSQID of process:%d is ==>%d\n",proc_num,msqid_rcv );
	try:

	status=msgrcv(msqid_rcv,proc_msg,sizeof(msg)-sizeof(long),2,0);
	if(status<0){
	//	printf("No messages for process:%d.\nSleeping...\n",proc_num);
		goto try;
	}
	else{
		//	printf("process no. %d got msg from central process\n",proc_num);
		central_temp=proc_msg->temp;	//central temperature
	//	printf("This is the temp received from central:%d\n",central_temp );
		if(proc_msg->stable==1){		//central process has been stablized.
		   // printf("Whole system has been stablized.\nExiting process:%d\n",proc_num);
			goto exit_proc;
		}
		//Not stable yet
		old_temp=new_temp;
		new_temp=(old_temp*3+2*central_temp)/5;		//calculating new temperature.
		if(old_temp==new_temp){
	//		printf("\nProcess with PID:%d is stable now.(at temperature:%d)\n",proc_msg->pid,new_temp);
			msg_to_send->stable=1;
			//this system got stable.
			//hurrah!
		}
		else {
			msg_to_send->stable=-1;
			//sending msg back to the central process.	
			//with temp and stability check flag.
		}
		msg_to_send->pid=proc_msg->pid;
		msg_to_send->priority=proc_msg->priority;
		msg_to_send->temp=new_temp;
	//	printf("Sending msg to central process from pid:%d\n",msg_to_send->pid );
		try2:
		status=msgsnd(msqid_rcv,msg_to_send,sizeof(msg)-sizeof(long),0);	//Sending temp back to the central process.
		if(status<0)
			goto try2;
	}
	printf("process:%d went to sleep\n",proc_num );
	sleep(1);	//waiting for the central process to reply.
	goto try;
	exit_proc:
		printf("Exiting the process no. %d\n",proc_num );
}