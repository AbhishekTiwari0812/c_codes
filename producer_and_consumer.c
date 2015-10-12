#include <pthread.h>
#include <stdio.h>

#define PRODUCT_LIMIT 10					//Maximum number of products that can be "stored" for consumption
#define PRODUCTION_LIMIT_PER_HEAD  100		//maximum number of products one Producer thread can produce.
#define CONSUMPTION_LIMIT_PER_HEAD  100		//maximum number of products one Consumer thread can consume.

int NUMBER_OF_PRODUCTS;			//current number of products.All (producers & consumers try to change it simultaneously (race condition))
pthread_mutex_t lock;			// mutex lock to avoid race condition


//a producer thread calls it  to increase number of products if it hasn't reached the PRODUCT_LIMIT.
//param i is the ID of the producer thread.

void increase_products(int i){
	pthread_mutex_lock(&lock);  						//acquiring lock for safe operation on shared variable NUMBER_OF_PRODUCTS
	if(NUMBER_OF_PRODUCTS < PRODUCT_LIMIT){
		printf("Producer #%d is producing\n",i );
		NUMBER_OF_PRODUCTS+=1;
	}
	else 
		printf("Production limit reached.\n\n");
	pthread_mutex_unlock(&lock);						//releasing lock
	sleep(rand()%3+1);
	return ;
}

void decrease_products(int j){
	pthread_mutex_lock(&lock);						//acquiring lock for safe operation on shared variable NUMBER_OF_PRODUCTS
	if(NUMBER_OF_PRODUCTS>0){
		sleep(1);
		printf("Consumer #%d is  consuming\n", j);
		NUMBER_OF_PRODUCTS-=1;
	}
	else 
		printf("No product available to consume\n\n");
	pthread_mutex_unlock(&lock);					//releaing lock
	sleep(rand()%3+1);
	return ;
}


//producer thread
void *produce(void *var_args){
//	long int cid= pthread_self();
	int pid=(*(int *)var_args);
	int count=0;
	while(count++<PRODUCTION_LIMIT_PER_HEAD)
		increase_products(pid);
	return NULL;
}


//consumer thread
void *consume(void *var_args){
//	long int pid = pthread_self();
	int cid=(*(int *)var_args);
	int count=0;
	while(count++<CONSUMPTION_LIMIT_PER_HEAD)
		decrease_products(cid);
	return NULL;
}



int main(){
	int num_of_producer;
	int num_of_consumer;

	NUMBER_OF_PRODUCTS=0;
	printf("Enter the number of prodcers and the number of consumers\n");
	//num_of_producer = 20;
	//num_of_consumer = 8;
	scanf("%d",&num_of_producer);
	scanf("%d",&num_of_consumer);
	pthread_t A[num_of_consumer];
	pthread_t B[num_of_producer];
	srand(time(NULL));
	while(pthread_mutex_init(&lock,NULL) !=0)
		printf("Mutex lock creation failed,trying again\n");

	
	int i;
	int prod_id[num_of_producer];
	int cons_id[num_of_consumer];
	
	for(i=0;i<num_of_producer;i++){
		prod_id[i]=i+1;
	}
	for(i=0;i<num_of_consumer;i++){
		cons_id[i]=i+1;
	}
	
	for(i=0;i<num_of_consumer;i++)
		pthread_create(&A[i],NULL,consume,(void *)&cons_id[i]);
	for(i=0;i<num_of_producer;i++)
		pthread_create(&B[i],NULL,produce,(void *)&prod_id[i]);

	for(i=0;i<num_of_consumer;i++)
		pthread_join(A[i], NULL);
	
	for(i=0;i<num_of_producer;i++)
		pthread_join(B[i], NULL);
	
	pthread_mutex_destroy(&lock);
	
	return 0;
}
