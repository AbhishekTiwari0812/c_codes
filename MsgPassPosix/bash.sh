gcc external.c -o external;
gcc central.c -o central ;
gcc clear_the_data.c -o clear_old_data;
sleep 3 ;
echo 'Executing ./central 100 22 50 40 60';
./central 100 22 50 40 60;
echo 'waiting for 30 seconds';
sleep 30;
clear;
echo 'executing another file..'; 
./clear_old_data;
clear;
./central 100 22 50 40 60;



