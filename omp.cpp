/*
 * File Name:	omp.h
 * Author:		Michael Glitzos
 * 
 * Description:
 * This is the Open MP implementation of the Dual-Core scheduling
 * simulator. The objective of this program is to divide up the
 * permutation calculations evenly across n number of threads
 * 
 * Dependencies: parallel_common.h, parallel_common.cpp
*/

//Include headers and libraries
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <time.h>
#include "omp.h"
#include "parallel_common.h"

using namespace std;

//Define constants
#define NUM_THREADS 4
#define NUM_PROCS 5

/**
 * Permute function
*/
void permute(int interval, int *array, int &lSum, int &hSum, int &diff,
int start, int *lineup){
	//local variables for sums and differences
	int localLSum;
	int localHSum;
	int localDiff;
	
	//set localLsum and localHsum to 0
	localLSum = localHSum = 0;
	
	for(int i = 0; i < interval; i++){
		for(int j = 0; j <= start; j++){
			localLSum += array[j];
		}
		for(int j = start +1; j < NUM_PROCS; j++){
			localHSum += array[j];
		}
		localDiff = abs(localLSum - localHSum);
		
		if(localDiff < diff){
			hSum = localHSum;
			lSum = localLSum;
			diff = localDiff;
		}
		//Move on to the next permutation
		next_permutation(array, array + NUM_PROCS);
	}
}

/**
 * Main function
*/
int main(int argc, char * argv[]){
	//Some variables for timekeeping purposes
	time_t start, end;
	start = time(0);	//start the clock
	
	//Array to represent processes
	int processes[NUM_PROCS];
	//populate array with numbers represnting runtimes
	for(int i = 0; i < NUM_PROCS; i++){
		processes[i] = rand()%500;
	}
	sort(processes, processes + NUM_PROCS);	//Sort the array from smallest to biggest
	
	//DEBUG
	cout<<"Default lineup:"<<endl;
	for(int i = 0; i < NUM_PROCS; i++){
		cout<<processes[i]<<"\t";
	}
	cout<<endl;
	
	/*find the factorial of the array size and divide it by NUM_THREADS
	 * to find the interval */
	 //DEBUG
	 //cout<<factorial(NUM_PROCS)<<"\t"<<NUM_THREADS<<endl;
	 int interval = factorial(NUM_PROCS) / NUM_THREADS;
	 
	 int lowerSum, higherSum, difference;
	 int lineup[NUM_PROCS];
	 
	 for(int i = 0; i < NUM_PROCS; i++){
		 lineup[i] = processes[i];
	 }
	 
	 lowerSum = processes[0];
	 higherSum = 0;
	 for(int i = 1; i < NUM_PROCS; i++){
		 higherSum += processes[i];
	 }
	 difference = abs(lowerSum - higherSum);
	 
	 int tid;
	 
	 int lSum[NUM_THREADS];
	 int hSum[NUM_THREADS];
	 int tDiff[NUM_THREADS];
	 int lineup2[NUM_THREADS][NUM_PROCS];
	 
	 for(int i = 0; i < NUM_THREADS; i++){
		 lSum[i] = lowerSum;
		 hSum[i] = higherSum;
		 tDiff[i] = difference;
		 for(int j = 0; j < NUM_PROCS; j++){
			 lineup2[i][j] = lineup[j];
		 }
	 }
	 
	 cout<<"Entering parallel block"<<endl;
	 #pragma omp parallel for private(tid) num_threads(NUM_THREADS)
	 {
		 tid = omp_get_thread_num();
		 
		 cout<<"Hello from thread: "<<tid<<endl;
		 
		 int *nthperm;	//pointer for nth permutation
		 //DEBUG
		 cout<<"Thread: "<<tid<<endl;
		 
		 //Get an array for each permutation
		 if(tid == 0){
			 cout<<"tid is 0!"<<endl;
			nthperm = processes;
		}
		else{
			cout<<"something other than thread 0"<<endl;
			nthperm = nthpermutation(interval * tid, processes, NUM_PROCS);
		}
		
		cout<<"Cleared permutation generator for tid: "<<tid<<endl;
		 
		 for(int i = 0; i < NUM_THREADS; i++){
				permute(interval, nthperm, lSum[tid], hSum[tid] ,tDiff[tid],
				i, lineup2[tid]);
				if(tid == 0){
					sort(nthperm, nthperm + NUM_PROCS);
				}
				else{
					//Delete the reference to nthperm
					delete nthperm;
					nthperm = nthpermutation(interval * tid, processes, NUM_PROCS);
				}
		 }
	 }
	 
	 for(int i = 0; i < NUM_THREADS; i++){
		 if(tDiff[i] < difference){
			 lowerSum = lSum[i];
			 higherSum = hSum[i];
			 difference = tDiff[i];
			 for(int j = 0; j < NUM_PROCS; j++){
				 lineup[j] = lineup2[i][j];
			 }
		 }
	 }
	 
	 //Print out results
	end = time(0);
	cout<<"Most efficient path:"<<endl;
	cout<<"Lower sum:\t"<<lowerSum<<"\tUpper sum:\t"<<
	higherSum<<"\tDifference:\t"<<difference<<endl;
	cout<<"Process Lineup:\t"<<endl;
	for(int i = 0; i < NUM_PROCS; i++){
		cout<<lineup[i]<<"\t";
	}
	cout<<endl<<"It took:\t"<<end-start<<" seconds"<<endl;
	
	//Tell the program we're done running
	return 0;
}
