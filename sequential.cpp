/*
* File Name:	Sequential.cpp
* Author:	Michael Glitzos
*
* Description:
* This file is going to handle all of the sequential instructions for
* the multi-core scheduler simulation.
*/

//Include libraries and headers
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <time.h>

//Define the amount of processes to simulate
#define NUM_PROCS 14
using namespace std;

/*
* Function to handle the permutaiton calculations
*/
void permute(int *array, int &min, int &max, int start,
int &difference, int *lineup){
		//Local variables for high and low sums
	int sumL, sumH, lDiff;

	/*cout<<"Comparing the sum of the first "<<start<<" procs"
	<<" to processes "<<start+1<<" thru "<<NUM_PROCS-1<<endl;*/

	do{
		sumL = sumH = lDiff = 0;	//set both = 0

		//Loop 1, go through the lower range
		for(int i = 0; i <= start; i++){
			sumL += array[i];

		}
		
		//Loop 2, go through the higher range
		for(int i = start+1; i < NUM_PROCS; i++){
			sumH += array[i];

		}

		lDiff = abs(sumH - sumL);

		//check the sums against the global max and min
		if(lDiff < difference){
			difference = lDiff;
			max = sumH;
			min = sumL;
			//copy the current contents into lineup
			for(int i=0; i<NUM_PROCS; i++){
				lineup[i] = array[i];
			}
			/*cout<<"Lower Sum:\t"<<min;
			cout<<"\tUpper Sum:\t"<<max;
			cout<<"\tDifference: \t"<<difference<<endl;*/
		}
	}while(next_permutation(array, array + NUM_PROCS));
}

/*
* Main function
*/
int main(int argc, char* argv[]){
	//Declare time variables for timekeeping
	time_t start, end;
	//start the clock
	start = time(0);
	
	//Declare the array of processes
	int processes[NUM_PROCS];
	int lineup[NUM_PROCS];	//Store the most efficient lineup
	int difference = 0;
	
	//DEBUG
	cout<<"Default process lineup:"<<endl;
	
	//Populate the array of processes with random numbers
	for(int i=0; i< NUM_PROCS; i++){
		processes[i] = rand()%500;
	}
	sort(processes, processes + NUM_PROCS);
	
	for(int i=0; i<NUM_PROCS;i++){
		cout<<processes[i]<<"\t";
	}
	
	cout<<endl;
	
	//lowerSum is going to start out as the first index of processes
	int lowerSum = processes[0];
	
	//upperSum is going to be the sum of indexes 1-(n-1)
	int upperSum = 0;
	
	for(int i=1; i < NUM_PROCS; i++){
//		cout<<processes[i]<<"\t";
		upperSum = upperSum + processes[i];
	}
	difference = abs(lowerSum - upperSum);
	//DEBUG
	cout<<endl<<"Default lower sum:\t"<<lowerSum<<
	"\tDefault upper sum:\t"<<upperSum
	<<"\tDefault difference:\t"<<difference<<endl;
	

	for(int i = 0; i < NUM_PROCS-1; i++){
		permute(processes, lowerSum, upperSum, i,
		difference, lineup);
		//reset the array after each run
		sort(processes, processes + NUM_PROCS);
		
		//DEBUG
		//Tell the program to wait for user input
//		cin.ignore(1);
	}
	
	//Set the end time
	end = time(0);
	
	cout<<"Most efficient path:"<<endl;
	cout<<"Lower sum:\t"<<lowerSum<<"\tUpper sum:\t"<<
	upperSum<<"\tDifference:\t"<<difference<<endl;
	cout<<"Process Lineup:\t"<<endl;
	for(int i = 0; i < NUM_PROCS; i++){
		cout<<lineup[i]<<"\t";
	}
	cout<<endl<<"It took:\t"<<end-start<<" seconds"<<endl;
	
	return 0;	//Tell the program we're done running	
}
