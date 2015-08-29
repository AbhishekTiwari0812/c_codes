#include <stdlib.h>
#include <stdio.h>
#define INFINITY (1<<31)-1	//max possible value of int
#define MAX_ROUTERS 30
#define MAX_END_NODES 100
#define NODE_BASE_PORT 22200
#define ROUTER_BASE_PORT 11100

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


int Routers[MAX_ROUTERS];
int router_count;
int EndNodes[MAX_END_NODES];
int end_node_count;

void read_from_file_and_set_connections(char *filename){	
	FILE *file=fopen(filename,"r");
	int main_router_id;
	int new_router_id;
	int end_node_id;
	int connection_cost;
	char buffer_word[20];
	char c;
	router_count=0;
	end_node_count=0;
	if(file!=NULL){
		do{
			c=fscanf(file,"%s",buffer_word);
			if(buffer_word[0]=='@'){
				c=fscanf(file,"%s",buffer_word);
				main_router_id=atoi(buffer_word);
				Routers[router_count++]=main_router_id;
			}else if(buffer_word[0]=='r'){
				c=fscanf(file,"%s",buffer_word);
				new_router_id=atoi(buffer_word);
				c=fscanf(file,"%s",buffer_word);
				connection_cost=atoi(buffer_word);
				connect_routers(main_router_id,new_router_id,connection_cost);
			}else if(buffer_word[0]=='e'){
				c=fscanf(file,"%s",buffer_word);
				end_node_id=atoi(buffer_word);
				EndNodes[end_node_count++]=end_node_id;
				connect_node_to_router(end_node_id,main_router_id);				
			}
		}while(c!=EOF);
	}
}


void write_to_file(int _new_socket_id){
    FILE *fp=fopen("socket_list.txt","a");
    char *socket_id=(char *)malloc(10);
    snprintf(socket_id,10,"%d",_new_socket_id);
    fprintf(fp, "%s\n",socket_id);
    fclose(fp);
}


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
	char *filename="NetworkInput.txt";
    read_from_file_and_set_connections(filename);
	//printf("Graph has been initialized.\n");
	return ;
}
void connect_routers(int id1,int id2,int trans_cost){
	if(RouterList[id1]->current_number_of_router_neighbours>=5){
		//printf("Number of neighbour routers limit exceeding!!\n");
		return;
	}
	RouterList[id1]->neighbour_routers[RouterList[id1]->current_number_of_router_neighbours]=RouterList[id2];
	RouterList[id1]->cost[RouterList[id1]->current_number_of_router_neighbours]=trans_cost;
	RouterList[id1]->current_number_of_router_neighbours++;
//	printf("router:%d is now neighbour of router:%d\n",id2,id1);
	return;

}


void connect_node_to_router(int new_node_id,int new_router_id){
	EndNodeList[new_node_id]->parent_router_id=new_router_id;
	RouterList[new_router_id]->neighbour_end_nodes[RouterList[new_router_id]->current_number_of_end_node_neighbours++]=EndNodeList[new_node_id];
	//printf("end node :%d is now connected with router:%d\n",new_node_id,new_router_id);
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
	//printf("%d ",end->router_id );
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



int check_node_list(int RouterId,int node_id){
	router *m_router=RouterList[RouterId];
	int i;
	for(i=0;i<m_router->current_number_of_end_node_neighbours;i++){
		if(m_router->neighbour_end_nodes[i]->end_node_id==node_id)
			return node_id;
	}
	if(i==m_router->current_number_of_end_node_neighbours)
		return -1;

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

int get_parent_id(int node_id){			//for end nodes
		return EndNodeList[node_id]->parent_router_id;	
}


//for routers
int *get_neighbours(int node_id){
	router *m_router=RouterList[node_id];
	int size =m_router->current_number_of_router_neighbours;
	//printf("Size:%d\n",size );
	int *A=malloc(sizeof(int)*(size+1));
	int i;
	//first element of the array is the size of the array.
	A[0]=size;
	//printf("neighbours of %d are:",node_id );
	for(i=1;i<=size;i++){
		A[i]=m_router->neighbour_routers[i-1]->router_id;
	//	printf("%d ", A[i]);
	}
	//printf("\n");
	return A;

}


void initialize_path_traveled(){}
void add_in_visited(){}
int is_node_visited(){}


