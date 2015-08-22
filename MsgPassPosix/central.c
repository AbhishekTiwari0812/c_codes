//this is the central machine 
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>//not needed for newer versions of linux
#include <sys/ipc.h>	//not needed for newer versions of linux
#include <sys/msg.h>
#include <sys/shm.h>
#include <string.h>
#include <time.h>
#define KEY1 1234
#define KEY2 2341
#define KEY3 3412
#define KEY4 4123

struct msqid_ds dummyParam;			//to remove the msg queues

typedef struct msg_to_pass{			//message format
	long priority;
	int pid;		//process id
	int temp;		//temperature;
	int stable;
}msg;


int msqid1,msqid2,msqid3,msqid4;
int main(int argc,char * argv[]){	//input must be the initial temp of the central machine and of the other machines.
	/*********VARIABLES****************/
	char *cmd;			    //shell command
	int i;				    //loop variable
	int status;			    //status of send receive flag
	int msg_send_stat;	    //NO USE,I THINK
	cmd=malloc(100);		//biggest possible length of the command.
	cmd[0]='\0';
	int isStable=-1;		//to check whether the processes are in equilibrium or not.
	char *exec="./external ";
	int stable1,stable2,stable3,stable4;
	int central_temp=atoi(argv[1]);
	msg *msg_to_send[argc-2];					//number of external processes is argc-2.i.e. 4
	msg *proc_msg;								//messages from the external processes
	clock_t time1,time2;
	time1=clock();
	int temp1=atoi(argv[2]);
	int temp2=atoi(argv[3]);
	int temp3=atoi(argv[4]);
	int temp4=atoi(argv[5]);
	char proc_num[3];	
	/************/

	for(i=2;i<argc;i++){
		strcat(cmd,exec);
		proc_num[0]=i-2+'0';
		strcat(cmd,proc_num);
		strcat(cmd," ");
		strcat(cmd,argv[i]);
		strcat(cmd,"&");
	}
	system(cmd);		//starting off the the other processes	
	
	//set up the tunnel of communication
	msqid1=msgget(KEY1 , 0600 | IPC_CREAT);   
	if(msqid1<0){
		printf("Error Occured while creating the msg box\nError returned value:%d\nTry again.\n",msqid1);
		return 0;
	}
	msqid2=msgget(KEY2 , 0600 | IPC_CREAT);  
	if(msqid2<0){
		printf("Error Occured while creating the msg box\nError returned value:%d\nTry again.\n",msqid2);
		return 0;
	}
	msqid3=msgget(KEY3 , 0600 | IPC_CREAT);   
	if(msqid3<0){
		printf("Error Occured while creating the msg box\nError returned value:%d\nTry again.\n",msqid3);
		return 0;
	}
	msqid4=msgget(KEY4 , 0600 | IPC_CREAT);   
	if(msqid4<0){
		printf("Error Occured while creating the msg box\nError returned value:%d\nTry again.\n",msqid4);
		return 0;
	}
	
	//send the temperature to the external processes.
	for(i=0;i<4;i++){
		msg_to_send[i]=(msg *)malloc(sizeof(msg));
		msg_to_send[i]->pid=i+2;				//process Ids are 2,3,4,5...
		msg_to_send[i]->priority=2;				//default for all.
	}

	proc_msg=(msg *)malloc(sizeof(msg));
	repeat:
	i=0;
	while(i < 4){
		msg_to_send[i]->temp=central_temp;		//central temperature
		msg_to_send[i]->stable=isStable;		//flag to signal the external process that everything is in equilibrium.
		if(i==0){
			msg_send_stat=msgsnd(msqid1,msg_to_send[i],sizeof(msg)-sizeof(long),0);		
			if(msg_send_stat<0)
				continue;
		}
		else if(i==1){	
			msg_send_stat=msgsnd(msqid2,msg_to_send[i],sizeof(msg)-sizeof(long),0);
			if(msg_send_stat<0)
				continue;
		}
		else if(i==2){	
			msg_send_stat=msgsnd(msqid3,msg_to_send[i],sizeof(msg)-sizeof(long),0);		
			if(msg_send_stat<0)
				continue;
		}
		else if(i==3){	
			msg_send_stat=msgsnd(msqid4,msg_to_send[i],sizeof(msg)-sizeof(long),0);
			if(msg_send_stat<0)
				continue;
		}
		i++;
	}
	printf("c -> p;temp:%d\n",central_temp);
	if(isStable==1)
		goto exit_prog;		//Everything's good.Exiting now.
	printf("Center went to sleep\n");
	sleep(0.1);				//waiting for the other processes to reply.
	i=4;
	while(i>0){
		if(i==4){
			status=msgrcv(msqid1,proc_msg,sizeof(msg)-sizeof(long),2,0);
			if(status<0){
				printf("Some error in reading the messages from 1.\nTrying again.\n");
				continue;
			}
			else{
				//printf("message received from proc 1\n");
				stable1=proc_msg->stable;
				temp1=proc_msg->temp;
				printf("c <- t1;temp %d\n",temp1 );
			}
		}
		else if(i==3){
			status=msgrcv(msqid2,proc_msg,sizeof(msg)-sizeof(long),2,0);
			if(status<0){
				printf("Some error in reading the messages from 2.\nTrying again.\n");
				continue;
			}
			else{
				//printf("message received from proc 2\n");
				stable2=proc_msg->stable;
				temp2=proc_msg->temp;
				printf("c <- t2;temp %d\n",temp2 );
			}
		}
		else if(i==2){
			status=msgrcv(msqid3,proc_msg,sizeof(msg)-sizeof(long),2,0);
			if(status<0){
				printf("Some error in reading the messages from 3.\nTrying again.\n");
				continue;
			}
			else{
				//printf("message received from proc 3\n");
				stable3=proc_msg->stable;
				temp3=proc_msg->temp;
				printf("c <- t3;temp %d\n",temp3);
			}
		}
		else if(i==1){
			status=msgrcv(msqid4,proc_msg,sizeof(msg)-sizeof(long),2,0);
			if(status<0){
				printf("Some error in reading the messages from 4.\nTrying again.\n");
				continue;
			}
			else{
				//printf("message received from proc 4\n");
				stable4=proc_msg->stable;
				temp4=proc_msg->temp;
				printf("c <- t4 ;temp %d\n",temp4 );
			}
		}
		i--;
	}

	if((stable1==1 )&&( stable2==1 )&& (stable3==1) && (stable4==1)){
		//system is now stable.
		isStable=1;
		//printf("Central process has signaled for the stabilty\n");
	}else{
		central_temp=(2*central_temp+temp4+temp3+temp2+temp1)/6;
	}
	//repeat the whole process again.
	//Even if the system is stable,we'll have repeat again.To signal other processes.
	goto repeat;
	exit_prog:
		printf("All the systems are stable now.Exiting main process\n");
		system("killall ./external");
		msgctl(msqid1,IPC_RMID,&dummyParam);
		msgctl(msqid2,IPC_RMID,&dummyParam);
		msgctl(msqid3,IPC_RMID,&dummyParam);
		msgctl(msqid4,IPC_RMID,&dummyParam);
		system("clear");
		printf("Stable temperatures are-->\nCentral temp: %d,\nexternal process1 temp:%d,\nexternal process2 temp:%d,\nexternal process3 temp:%d,\nexternal process4 temp:%d,\n",central_temp,temp1,temp2,temp3,temp4);
		return 0;

}
