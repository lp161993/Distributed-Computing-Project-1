# Distributed-Computing-Project-1

README file:

This code is used to identify Leader among a set of synchronous processes. 
The input.dat file contains n- the num of synchronous processes and the process ids. 

In the main() function of the code, we read the input.dat file to find n. Then the main function runs n rounds for the threads to identify the leader. 

In round 0, the threads are created.

In general, the threads wait for a round_start signal from the master and the master waits until all threads complete a specific round(go_ahead_signal variable).

In each round the outgoing and incoming messages are processed according to the LCR algorithm and after n rounds, the leader is identified and all threads output their ID along with their leader.

Run the code: Compile LCRalgorithm_main.cpp, and run the code. If you want to change the input, open input.dat file and alter the value of n and their respective process IDs.

Sample input:
5
5 7 2 3 10


Sample Output:


Master initiating Round 0
Master initiating Round 1
Master initiating Round 2
Master initiating Round 3
Master initiating Round 4
I am Thread ID 5 and my leader is 10
I am Thread ID 7 and my leader is 10
I am Thread ID 3 and my leader is 10
I am Thread ID 2 and my leader is 10
I am Thread ID 10 and my leader is 10

--------------------------------
Process exited after 3.597 seconds with return value 0
Press any key to continue . . .
