#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define NUM_OF_PHILOSOPHERS 5
#define NUM_OF_ITERATIONS 10
#define DEADLOCK_TIME_LIMIT 100000000			//waits for almost 20 second to declare a deadlock

//#define DEADLOCK_TIME_LIMIT 500000000	  	      //waits for almost 200 second to declare a deadlock
//#define DEADLOCK_TIME_LIMIT 1000000000	      //waits for almost 400 second to declare a deadlock


//a bit-mask to check whether the chopstick-i is available or not.
//index of the array represents the chopstick number. (starting from 1 ending at 5)

int chopsticks[NUM_OF_PHILOSOPHERS];
pthread_mutex_t lock;


int try_chopstick(int chopstick_num){
	int flag=0;
	pthread_mutex_lock(&lock);
	if(chopsticks[chopstick_num-1]==0){
		printf("chopstick #%d is acquired\n",chopstick_num );
		chopsticks[chopstick_num-1]=1;
		flag=1;
	}
	pthread_mutex_unlock(&lock);
	return flag;
}

void release_chopsticks(int chopstick_num1,int chopstick_num2){
	pthread_mutex_lock(&lock);
	chopsticks[chopstick_num1-1]=0;
	chopsticks[chopstick_num2-1]=0;
	printf("released chopsticks #%d,#%d\n",chopstick_num1,chopstick_num2);
	pthread_mutex_unlock(&lock);
}


//to count the number of times the food is eaten (by all philosophers).
int count;

void *start_dine(void *var_args){
	int phil_id=(*(int *)var_args);
	int flag=0;
	long long int deadlock_detector=0;
	while(count<NUM_OF_ITERATIONS){			
		deadlock_detector=0;
		while(flag!=1){
			deadlock_detector++;
			if(deadlock_detector > (DEADLOCK_TIME_LIMIT)){
				printf("Deadlock detected.Getting out\n");
				exit(0);
			}
			flag=try_chopstick(phil_id);
		}
		printf("philosopher #%d acquired chopstick number #%d\n", phil_id,phil_id);
		//sleep(1);			//to see the deadlock situation, uncomment this line
		if(flag){			//if condtition is not needed.Just for reader's convinience!
			deadlock_detector=0;
			do{
			deadlock_detector++;
			if(deadlock_detector > (DEADLOCK_TIME_LIMIT)){
				printf("Deadlock detected.Getting out\n");
				exit(0);
			}
			//trying the next chopstick.
			//The long mathematical expression is just to ensure that when phil_id 5, chopstick pair has to be (5,1)
			flag=try_chopstick((phil_id+1)%(NUM_OF_PHILOSOPHERS+1)+(phil_id+1)/(NUM_OF_PHILOSOPHERS+1));		
			}while(flag!=1);
			
			printf("philosopher #%d has acquired chopstick number #%d and #%d\n", phil_id, phil_id,(phil_id+1)%(NUM_OF_PHILOSOPHERS+1)+(phil_id+1)/(NUM_OF_PHILOSOPHERS+1));
			if(flag){		//if condtition is not needed.Just for reader's convinience!
				pthread_mutex_lock(&lock);
				count++;
				pthread_mutex_unlock(&lock);
				printf("philosopher #%d is eating\n", phil_id);
				sleep(rand()%3+1);			//to see the deadlock situation, comment this line
				printf("philosopher #%d has eaten\n", phil_id);
				release_chopsticks(phil_id,(phil_id+1)%6+(phil_id+1)/6);
				
			}
		}
		//printf("\n");
	}
	return NULL;
}



int main(){
	int i;
	count=0;
	while(pthread_mutex_init(&lock,NULL) !=0)
		printf("Mutex lock creation failed,trying again\n");
	
	pthread_t philosophers[NUM_OF_PHILOSOPHERS];
	int philosopher_id[NUM_OF_PHILOSOPHERS];
	for(i=0;i<NUM_OF_PHILOSOPHERS;i++){
		philosopher_id[i]=i+1;
		chopsticks[i]=0;
	}

	
	for(i=0;i<NUM_OF_PHILOSOPHERS;i++){
		pthread_create(&philosophers[i],NULL,start_dine,(void *)&philosopher_id[i]);
	}


	for(i=0;i<NUM_OF_PHILOSOPHERS;i++)
		pthread_join(philosophers[i],NULL);

	pthread_mutex_destroy(&lock);
	return 0;
}
