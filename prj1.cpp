#include <chrono>
#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>

using namespace std;
using namespace std::chrono;
using namespace std::this_thread;

mutex mtx;
bool round_completion = false; //used if n rounds are completed
bool *round_start;            //used for every round
long *outgoing_msg;           //This array stores outgoing messages
long *outgoing_int_msg;      //This array is used to intermediate processing of values
long *incoming_msg;          //this array stores incoming messages
int go_ahead_signal =0;
int num_threads;
int round_no;
bool start_next_round()
{ 
	return num_threads==go_ahead_signal;
}

void LCR(long threadid, int thread_no) {
    long tid = threadid;
    long max_id = tid;
    
    while(!round_completion){
    	while(!round_start[thread_no])
    	{
			sleep_for(milliseconds(1)); //waits till Master issues round_start
		}
    	round_start[thread_no] = false;
    	
    	if(round_no != 0)
		{
			long incoming = incoming_msg[thread_no];
			cout<<" ";
			
			if(incoming>tid)
			{
				max_id = incoming;
				outgoing_int_msg[thread_no] = incoming;
			}
			else if(incoming < tid)
			{
				outgoing_int_msg[thread_no] = 0;
			}
			else
			{
				cout<<endl;
				//status= "leader";
			}
			
			go_ahead_signal++; //This notifies that this particular round is complete
			
		}
		else{
			//cout << "Round 0 has begun";
			outgoing_msg[thread_no] = tid;
			outgoing_int_msg[thread_no] = tid;
			//cout<<"Thread no: "<<thread_no<<" Tid "<<tid <<" outgoing_msg: "<< outgoing_msg[thread_no]<<endl;
	   		++go_ahead_signal; //This notifies that this particular round is complete
		}	
	}
	mtx.lock();
	cout<<"I am Thread ID "<<tid<<" and my leader is "<<max_id<<endl;
	mtx.unlock();
}

int main(int argc, char* argv[]) {
    int rc;
    long i=0;
    int data;
    
	ifstream infile;
	infile.open("input.dat");            // This file contains the input data
	infile >> data;                      //This initial data contains the value of number of threads
	num_threads = data;
	
	
	outgoing_msg = new long[num_threads];
	outgoing_int_msg = new long[num_threads];
	incoming_msg = new long[num_threads];
	round_start = new bool[num_threads];
	
	for(int j =0;j<num_threads;j++){round_start[j]=false;
	}
	
	//go_ahead_signal = new int[num_threads];
	
	int pid_list[num_threads] = {};      //pid_list is the variable that contains the process_ids

	
	//create a number vector of size n and add the numbers to it in while loop
	while( !infile.eof() ){
		infile >> data;
		pid_list[i++] = data;
	}
	infile.close();
	
    thread threads[num_threads];
   
   //We run the threads for num_threads number of rounds 
	for(int k=0; k<num_threads; k++)
	{	
		cout<< "Master initiating Round " << k<<endl;
		round_no = k;
		if(!k){
			//only for round 0, the threads are created
				for( i = 0; i < num_threads; i++ ) 
				{
	    		threads[i] = thread(LCR, pid_list[i],i);
			    }
		}

		for(int j =0;j<num_threads;j++){round_start[j]=true;}

		while(!start_next_round())
		{
			sleep_for(milliseconds(10)); //Wait until we get a go ahead from all threads
		}
		go_ahead_signal=0;		//go_ahead_signal is set to 0 again, so that all threads can notify completion
		
		for(int j =0;j<num_threads;j++)
		{
		outgoing_msg[j] = outgoing_int_msg[j]; 
		}
		for(int j =0;j<num_threads;j++)
		{
		incoming_msg[j] = outgoing_msg[(j+1)%num_threads]; //Assigning the outgoing of round n-1 to incoming of round n
		}
	}
	for(int j =0; j<num_threads; j++){round_start[j]=true;}
	
    round_completion = true;
    
    for( i = 0; i < num_threads; i++ ) {
			        threads[i].join();
			    }
   //sleep_for(seconds(5));
   return(0);
}

