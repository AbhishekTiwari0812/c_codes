#include <stdio.h>
#define MAX_ROUTERS 30
#define MAX_END_NODES 100

extern void initialize_graph();
extern void connect_routers(int id1,int id2,int trans_cost);
extern void connect_node_to_router(int new_node_id,int new_router_id);
extern void find_shorest_path_between_routers(int router_1,int router_2);	//returns the array of router ids through which the message should pass.
extern void print_neighbours();
extern int Routers[MAX_ROUTERS];
extern int router_count;
extern int EndNodes[MAX_END_NODES];
extern int end_node_count;
extern void read_from_file_and_set_connections(char *filename);


//gcc network.c -o network graph.c -w
int main(int argc,char* argv[]){
	initialize_graph();
	int i;
	printf("start routers:");
	for(i=0;i<router_count;i++)
		printf("%d ",Routers[i] );
	printf("\nStart end nodes:");

	for(i=0;i<end_node_count;i++)
		printf("%d \n",EndNodes[i]);

	printf("\n");

	return 0;
}
