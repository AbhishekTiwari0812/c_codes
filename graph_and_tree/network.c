#include <stdio.h>
#define MAX_ROUTERS 30
#define MAX_END_NODES 100

extern void initialize_graph();
extern void connect_routers(int id1,int id2,int trans_cost);
extern void connect_node_to_router(int new_node_id,int new_router_id);
extern void find_shorest_path_between_routers(int router_1,int router_2);	//returns the array of router ids through which the message should pass.
extern void print_neighbours();


//returns the first number it finds after start and the index of the next white space.

int Routers[MAX_ROUTERS];
int router_count;
int EndNodes[MAX_END_NODES];
int end_node_count;
void read_from_file(char *filename){
	
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



int main(int argc,char* argv[]){
	initialize_graph();
	char *filename="NetworkInput.txt";
	read_from_file(filename);
	//start off the routers and end nodes.
	//for listening.


	return 0;
}
