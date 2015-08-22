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


int main(){
	int msqid1,msqid2,msqid3,msqid4;
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

		msgctl(msqid1,IPC_RMID,&dummyParam);
		msgctl(msqid2,IPC_RMID,&dummyParam);
		msgctl(msqid3,IPC_RMID,&dummyParam);
		msgctl(msqid4,IPC_RMID,&dummyParam);
		system("killall ./external");


}