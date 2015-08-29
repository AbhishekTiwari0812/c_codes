The network input can be edited in the NetworkInput.txt file.
For convenience I have changed the format a little(it maybe inconvenient for the tester though)!!

In each line format is like-->
(standard regex representation)

@ \d{1,2} (([r] \d{1,2} \d{1,2})|([e] \d{1,2}))+

where first number denotes the router id to which everything else will be connected.
then follows one character r or e
	r means it's a router,so we mention the router id to be connected with and cost affiliated with the edge.
	e means it's an end node so we just mention the end node id.

Notice that there is a @ in the begining of each line too.And the graph made is a DIRECTED GRAPH!

And due to lack of time,I didn't implement any method to check whether the graph is connected or not.
So please make sure that the input is correct otherwise code won't work properly.
One can check the graph structure by uncommenting the printf() statements in graph.c file.
There are functions to print path,get neighbours of all the routers and end nodes parents etc to verify the network!

There is already a graph in the NetworkInput.txt file which is same as mentioned in the assignment 2 figure.


To compile the program open terminal(let's call it term1) 

go to the working directory.
type hostname to find hostname.
Now open "socket_open_read_write.c" and edit the HOSTNAME value to your PC's hostname.
save and close it.
Now again at term1,type 
./compile.sh
it compiles all the necessary files.
Now run ./network  to see the routers and end nodes ready to be executed.(optional.In case you don't remember the end nodes and routers to be started)

NOW,
open a terminal (not a new tab in the same terminal,for conveniece!) 
let's call it term2
In this terminal ,open all routers in different tabs using 

./router router_id
where router_id is mentioned by the ./network output.
example==> ./router 12 

These tabs work as the servers,whenever a message is arrived ,these tabs will show the message received!
All routers are now up and listening to the requests.

NOW,
Open a new terminal,
call it term3,

In this terminal ,open all end nodes in different tabs using 

./end_node end_node_id

exmaple ===> ./end_node 12
where end_node_id is mentioned by the ./network output.

These tabs work as the servers,whenever a message is arrived ,these tabs will show the message received!
All end nodes are now up and listening to the requests.

EVERYTHING IS SET UP.WE ARE GOOD TO GO..!!!
Now,come back to the term1
.......
run ./test_client
and fill the necessary info to send the messages accordingly.
Make sure the input is in correct format.
and the IDs provided actally exist in the network!!!
(couldn't automate this due to lack of time)


==========================
Port number format ==>
If it's a router 
port number would be 
11100 +  router_id
else if it's end node
port number would be
22200 + end_node_id


