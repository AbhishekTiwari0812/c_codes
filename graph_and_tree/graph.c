#include <stdlib.h>
#include <stdio.h>
#define INFINITY (1<<31)-1	//max possible value of int
#define MAX_ROUTERS 30
#define MAX_END_NODES 100

typedef struct END_NODE{
	int end_node_id;
	int parent_router_id;
}end_node;

typedef struct ROUTER{
	int router_id;
	struct  ROUTER *neighbour_routers[5];
	int cost[5];
	int isVisited;			//to help in finding the shortest path,-1 denotes not yet.0 denotes in the queue,1 denotes completely visited
	int NodeValue;			//to help in finding the shortest path
	struct ROUTER *previous;//to help in finding the shortest path
	end_node *neighbour_end_nodes[20];
	int current_number_of_router_neighbours;	//to check for the limit on number of neighbours
	int current_number_of_end_node_neighbours;
}router;

router *RouterList[MAX_ROUTERS];
end_node *EndNodeList[MAX_END_NODES];

void initialize_graph(){
	int i;
	for(i=0;i<MAX_ROUTERS;i++){
		RouterList[i]=(router *)malloc(sizeof(router));
		RouterList[i]->router_id=i;
		RouterList[i]->isVisited=-1;
		RouterList[i]->current_number_of_router_neighbours=0;
		RouterList[i]->current_number_of_end_node_neighbours=0;
	}
	for(i=0;i<MAX_END_NODES;i++){
		EndNodeList[i]=(end_node *)malloc(sizeof(end_node));
		EndNodeList[i]->end_node_id=i;
	}
	printf("Graph has been initialized.\n");
	return ;
}
void connect_routers(int id1,int id2,int trans_cost){
	if(RouterList[id1]->current_number_of_router_neighbours>=5){
		printf("Number of neighbour routers limit exceeding!!\n");
		return;
	}
	RouterList[id1]->neighbour_routers[RouterList[id1]->current_number_of_router_neighbours]=RouterList[id2];
	RouterList[id1]->cost[RouterList[id1]->current_number_of_router_neighbours]=trans_cost;
	RouterList[id1]->current_number_of_router_neighbours++;
	printf("router:%d is now neighbour of router:%d\n",id2,id1);
	return;

}


void connect_node_to_router(int new_node_id,int new_router_id){
	EndNodeList[new_node_id]->parent_router_id=new_router_id;
	RouterList[new_router_id]->neighbour_end_nodes[RouterList[new_router_id]->current_number_of_end_node_neighbours++]=EndNodeList[new_node_id];
	printf("end node :%d is now connected with router:%d\n",new_node_id,new_router_id);
}
//returns the int array of router ids through which the message has to go through,including the start and the end point


//find the shortest path!!!!!!!!
int PriorityQueue[MAX_ROUTERS];
int find_min(){
	int min=INFINITY;
	int i;
	int index;
	for(i=0;i<MAX_ROUTERS;i++)
		if(PriorityQueue[i]<min){
			min=PriorityQueue[i];
			index=i;
		}
	return index;
}

find_path(router *start,router *end){
	if(start->router_id==end->router_id)
		return;
	int i;
	for(i=0;i<start->current_number_of_router_neighbours;i++)
		if(start->neighbour_routers[i]->isVisited==1)
			continue;
		else if(start->neighbour_routers[i]->isVisited==0||start->neighbour_routers[i]->isVisited==-1)
			if(start->cost[i]+start->NodeValue < start->neighbour_routers[i]->NodeValue){
				//change_cost(start->neighbour_routers[i],start->cost[i]+start->NodeValue);
				PriorityQueue[start->neighbour_routers[i]->router_id]=start->cost[i]+start->NodeValue;
				start->neighbour_routers[i]->NodeValue=start->cost[i]+start->NodeValue;
				start->neighbour_routers[i]->previous=start;
			}
	PriorityQueue[start->router_id]=INFINITY;
	int min_index=find_min();
	router *next;
	next =(router *)malloc(sizeof(router));
	next = RouterList[min_index];
	return find_path(next,end);

}
int PATH[100];		//max path length is assumed to be 100
void set_path(router *end){
	static int path_lenght=1;
	if(end->previous!=NULL){
		set_path(end->previous);
		path_lenght++;
	}
	PATH[path_lenght-1]=end->router_id;
}
int *get_path(){
	//the last element is the one after which INFINITY occurs.
	return PATH;
}

int *find_shorest_path_between_routers(int router_1,int router_2){
	int i;
	memset(PATH,INFINITY,100);
	for(i=0;i<MAX_ROUTERS;i++){
		if(i==router_1)
			RouterList[i]->NodeValue=0;
		else 
			RouterList[i]->NodeValue=INFINITY;
		PriorityQueue[i]=INFINITY;
		RouterList[i]->previous=(router *)malloc(sizeof(router));
		RouterList[i]->previous=NULL;
	}
	find_path(RouterList[router_1],RouterList[router_2]);
	set_path(RouterList[router_2]);
	return get_path();
}






//
void print_neighbours_r(){
	int i;
	int j;
	for(i=0;i<MAX_ROUTERS;i++){
		printf("%d->", i);
		for(j=0;j<RouterList[i]->current_number_of_router_neighbours;j++){
			printf("%d ",RouterList[i]->neighbour_routers[j]->router_id );
		}
		puts("\n");
	}
}


