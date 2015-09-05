#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <time.h>

#define MAX_QUEUE_SIZE 10000
#define NUM_OF_RES 100

typedef struct resource_data_structure{
	int resource_id;
	int capacity;		//10 - 50.Number of instructions get executed while using this resource
	int cost;			//30 - 70
	
}resource;

typedef struct Task{
	int tid;				        //task ID.
	int size_of_task;				//number of instructions needed to complete the task,range-> [100,1000]
	int priority;					//decided by the cost of resources the task requires
	int number_of_resources;		//random from 10-20
	int *list_of_resources;			//list of resources the task wants.size number_of_resourses
}task_node;


int CURRENT_SIZE_OF_QUEUE=0;
task_node *QUEUE[MAX_QUEUE_SIZE];

int parent(int i){return (i-1)/2;}
int left_child(int i){return (i*2+1);}
int right_child(int i){return i*2+2;}
void swap(int child_index,int parent_index ){
	task_node *temp=QUEUE[child_index];
	QUEUE[child_index]=QUEUE[parent_index];
	QUEUE[parent_index]=temp;
}
void max_heapify(int i){
	int l=left_child(i);
	int r=right_child(i);
	int largest=i;
	if( l < CURRENT_SIZE_OF_QUEUE && QUEUE[l]->priority > QUEUE[largest]->priority ){
		largest=l;
	}
	if( r < CURRENT_SIZE_OF_QUEUE && QUEUE[r]->priority > QUEUE[largest]->priority ){
		largest=r;
	}
	if(largest!=i){
		swap(largest,i);
		return max_heapify(largest);
	}
	else return ;
}

int get_queue_size(){
	return CURRENT_SIZE_OF_QUEUE;
}
task_node *extract_max(){
	if(CURRENT_SIZE_OF_QUEUE==0){
		printf("There's no element in the queue\n");
		return NULL;
	}
	else{
		swap(0,CURRENT_SIZE_OF_QUEUE-1);
		CURRENT_SIZE_OF_QUEUE-=1;
		max_heapify(0);
		return QUEUE[CURRENT_SIZE_OF_QUEUE]; 
	}
}
void insert_in_queue(task_node *new_task){
	CURRENT_SIZE_OF_QUEUE+=1;
	if(CURRENT_SIZE_OF_QUEUE>MAX_QUEUE_SIZE){
		printf("Queue has memory overflow.\n");
		return ;
	}
	else{
		QUEUE[CURRENT_SIZE_OF_QUEUE-1]=new_task;
		task_node *temp=new_task;
		int current_location=CURRENT_SIZE_OF_QUEUE-1;
		while(parent(current_location)>0){
			if(QUEUE[parent(current_location)]->priority < QUEUE[current_location]->priority){
				swap(current_location,parent(current_location));
				current_location=parent(current_location);
			}
			else break;
		}
		if(current_location==1 || current_location==2)
			if(QUEUE[parent(current_location)]->priority < QUEUE[current_location]->priority){
				swap(current_location,parent(current_location));
				current_location=0;
			}
		}
}
