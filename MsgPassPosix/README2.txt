Program logic--->

At runtime central.c takes temperatures of 
central process and temperature of other external processes respectively.

Then central process starts the other external processes,assigning their initial temperature to 
the ones provided in the command line.
And creates 4 different message queues for the four different external processes.
Then the external processes wait for the message from the central process.

Now,
Center:

Central process sends the temperature to the external processes.
And goes to sleep for a small amount of time (0.1 second )till the processes receive and send their
data back to the central process.
And after waking up,it reads the msgs and checks the stabilty flag of all the messages from 
different processes.
If all of them are 1 then it means that the  previous temperature of the external processes was the same as which 
it became after the calculation using the new central temperature.
In this case central process sends another message to the external processes to shut down.And to be on the safer side,executs killall command too.
and Shows the temperatures on the screen.

External Process:
External processes check for messages in the queue by polling their respective queue. 
If there's a message,external process takes out the temp value.

puts it into the formula and checks for the stabilty condition.
if the stable flag has a value -1.
	If the new temp== old temp
	It sets the stable flag to 1.
	-1 otherwise.
	And replies to the Central process and goes to sleep for a small time to provide Central process 
	sufficient time to calculate things and reply back.
else the stable flag has the value 1.
	the external process terminates.
=====================================================================================================================


The program flow is a little different than what was instructed due to some misconception(or ambiguity or hustling while reading the problem).
At the time of initialzition,
Central process sends its temperature to other processes but it sshuold be the other way around.
That's why I have coded another check.cpp file to check for the authenticity of my output.

















