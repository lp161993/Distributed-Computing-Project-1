#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <stdint.h>
#include <fstream>

using namespace std;

void *LCR(void *threadid) {
    long tid;
    tid = (intptr_t)threadid;
    cout<< "Inside thread id " << tid << endl;
    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    int rc;
    long i=0;
    int data;
    
	ifstream infile;
	infile.open("input.dat");            // This file contains the input data
	infile >> data;                      //This initial data contains the value of number of threads
	int num_threads = data;
	
	int pid_list[num_threads] = {};      //pid_list is the variable that contains the process_ids
	//cout << "Reading from the file" << endl;
	//cout << "Initial number is " << data << endl;
	
	//create a number vector of size n and add the numbers to it in while loop
	while( !infile.eof() ){
		infile >> data;
		pid_list[i++] = data;
		//cout << "The next number is " << pid_list[i-1] << endl;
	}
	infile.close();
	
    pthread_t threads[num_threads];
   
    for( i = 0; i < num_threads; i++ ) {
        rc = pthread_create(&threads[i], NULL, LCR, (void *)(intptr_t)pid_list[i]);
      
        if (rc) {
            cout << "Error:unable to create thread," << rc << endl;
            exit(-1);
        }
    }
   pthread_exit(NULL);
}
