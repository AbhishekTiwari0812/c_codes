#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <time.h>
#define MAX_QUEUE_SIZE 10000
#define NUM_OF_RES 1000			//type of resources available 
#define READY_QUEUE_SIZE 20		
#define NUM_OF_TASKS  10		//number of tasks to be executed
#define MAX_INSTRUCTIONS_AT_ONE_TIME 1000

typedef struct resource_data_structure{
	int resource_id;			//redundant 
	int capacity;				
	int cost;	
	
}resource;

typedef struct Task{
	int tid;				        //task ID.
	int number_of_resources;		//random number of resources needed by the task.range 1-1000
	int *list_of_resources;			//list of resources the task wants.size number_of_resources.Stores resource id
	int *required_flag;				//to check if the resource is required anymore or not
	int size_of_task;				//number of instructions needed to complete the task.
	int priority;					//decided by the cost of resources the task requires.
	int total_cost;
}task_node;


resource *set_of_res[NUM_OF_RES];
task_node *task_list[NUM_OF_TASKS];

//helper queue functions 
extern void max_heapify(int i);
extern task_node *extract_max();
extern void insert_in_queue(task_node *new_task);
extern int get_queue_size();

//initializations
void fill_values_for_resources();
void fill_tasks_requirement();

void set_size_and_priority_of_tasks();

void insertion_sort(task_node *input[],int size);		//using this for simplicity.Can be used other sorting techniques to improve performance!

	
//if the task was not completed.It gets back to the queue.
//this function computes the resource and number of instructions 
//and size_of_task which have been changed !

void update_values(task_node *task_working_on){
	int i;
	int new_size=task_working_on->size_of_task;	
	//to compute the old size of the task.Restoring old values.
	set_size_and_priority_of_tasks(task_working_on->tid);
	int old_size=task_working_on->size_of_task;
	int difference = old_size - new_size;

	//remove the un-necessary resources by setting the required_flag
	for(i=0;i < task_working_on->number_of_resources;i++){
		if(difference<=0)
			break;
		else
			if(set_of_res[task_working_on->list_of_resources[i]]->capacity <= difference && task_working_on->required_flag[i]==1){
				task_working_on->required_flag[i]=-1;		//i-th resource in the resource requirement list  is not required anymore.
			//	printf("Resource %d was freed by task %d \n",set_of_res[task_working_on->list_of_resources[i]]->resource_id,task_working_on->tid );
				difference-=set_of_res[task_working_on->list_of_resources[i]]->capacity ;
			}
	}
	printf("%d is BTQ.", task_working_on->tid);
	set_size_and_priority_of_tasks(task_working_on->tid);

}



//task is going in the CPU
int work_on_it(task_node *new_task ){
	int MAX_TIME_ALLOWED=MAX_INSTRUCTIONS_AT_ONE_TIME;		//maximum 70 instructions will be executed at a time...!!
	int i;
	for(i=0;i<MAX_TIME_ALLOWED;i++)
		if(new_task->size_of_task<=0)
			return 1;		//task got completed...
		else new_task->size_of_task--;
	return -1;			//task didn't get completed yet...
}


//start executing tasks
void do_the_dance(){
	if(get_queue_size()<=0){
		printf("No task left.\n");
		return ;
	}
	task_node *current_tasks[READY_QUEUE_SIZE];
	int number_of_extracted_tasks=0;

	//get tasks from the Queue.
	while(get_queue_size() > 0 ){
		if( number_of_extracted_tasks >= READY_QUEUE_SIZE)
			break;
		else{
			current_tasks[number_of_extracted_tasks++]=extract_max();
		}
	}
	insertion_sort(current_tasks,number_of_extracted_tasks);	//sorting on the basis of total cost.Could perform better using a priority queue.
	while(number_of_extracted_tasks > 0){
		task_node *new_task=current_tasks[--number_of_extracted_tasks];		//starting with the highest priority 
		int status=work_on_it(new_task);			//processing the task
		if(status==1)								//if task gets completed
			printf("\nTask %d completed.\n",new_task->tid);
		else{
		//	printf("Couldn't complete task (id : %d).\nPutting it back in the queue.\n",new_task->tid);
			update_values(new_task);
			insert_in_queue(new_task);
		}
	}
	do_the_dance();
}
//to print all the tasks which are to be completed
void print_task_info(){
	int i;
	for(i=0;i<NUM_OF_TASKS;i++)
	printf("TID:%d Number Of Res Needed:%d Size :%d Priority:%d \n",task_list[i]->tid,task_list[i]->number_of_resources,task_list[i]->size_of_task,task_list[i]->priority);
}

int main(int argc,char *argv[]){ 
    int i;  
    time_t t;
    t=time(&t);
    srand(t);
    printf("Number of tasks to be completed is %d\n",NUM_OF_TASKS);
    puts("BTQ stands for back to queue.\n");
    
    sleep(2);
    
    fill_values_for_resources();
	fill_tasks_requirement();
	for(i=0;i<NUM_OF_TASKS;i++){
		set_size_and_priority_of_tasks(i);     //fill everything in the queue    
		insert_in_queue(task_list[i]);     
	}
	//print_task_info();
	do_the_dance();
	return 0;
}


void fill_tasks_requirement(){
	int i,j;		//iterators
	time_t t;
    t=time(&t);
    srand(t);
	for(i=0;i<NUM_OF_TASKS;i++){
		task_list[i]=(task_node *)malloc(sizeof(task_node));
		task_list[i]->tid=i;
		task_list[i]->number_of_resources=1+rand()%1000;			
		task_list[i]->list_of_resources=(int *)malloc(sizeof(int)*task_list[i]->number_of_resources);
		task_list[i]->required_flag=(int *)malloc(sizeof(int)*task_list[i]->number_of_resources);
		
		for(j=0;j<task_list[i]->number_of_resources;j++){
			task_list[i]->list_of_resources[j]=rand()%NUM_OF_RES;
			task_list[i]->required_flag[j]=1;
		}
		
	}
}


void set_size_and_priority_of_tasks(int i){
	int j;
	task_list[i]->size_of_task=0;
	task_list[i]->total_cost=0;
	int count=0;
	//printf("Task %d ",i );
	//printf("%d \n", task_list[i]->number_of_resources);
	for(j=0;j<task_list[i]->number_of_resources;j++){
		if(task_list[i]->required_flag[j]==1){
			task_list[i]->size_of_task+=set_of_res[task_list[i]->list_of_resources[j]]->capacity;
			task_list[i]->total_cost+=set_of_res[task_list[i]->list_of_resources[j]]->cost;
			count++;
		//	printf("count %d \n",count );
		}else{
		//	printf("comehersfnwE\n");
		}
	
	}
	if(count<=0){
		//printf("task already completed\n");
		return ;
	}
	task_list[i]->priority= task_list[i]->total_cost*100/count;//average cost of resources needed

	//printf("pri:%d task :%d \n",task_list[i]->priority,i);
	return ;
}


//filling random values to the resources which are available in the system
void fill_values_for_resources(){
	int i;		//iterator
	time_t t;
    t=time(&t);
    srand(t);
	for(i=0;i<NUM_OF_RES;i++){
		set_of_res[i]=(resource *)malloc(sizeof(resource));
		set_of_res[i]->resource_id=i;
		set_of_res[i]->capacity=10+(rand()%41);		//range 10-50
		set_of_res[i]->cost=10+(rand()%399);		//range 10-409
	}
	return ;

}

void insertion_sort(task_node *input[],int size){
	int i;
	int j;
	task_node *temp;
	for(i=1;i<size;i++){
		j=i-1;
		temp=input[i];
		while(j >= 0 && temp->total_cost < input[j]->total_cost){
			input[j+1]=input[j];
			j--;
		}		
		input[j+1]=temp;
	}
}