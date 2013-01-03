/*
 * File Name:	mpi.cpp
 * Author:		Michael Glitzos
 * 
 * Description:
 * This is the MPI implementation of the dual core scheduler simulation
 * program. This program uses an array to represent processes running
 * on a system. Each index in the array has a number value that
 * represents the time required for the process to run.
 * 
*/

//Include headers and libraries
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <mpi.h>
#include "parallel_common.h"
#include <time.h>

using namespace std;

//Define the number of processes to simulate
#define NUM_PROCS 14

/**
 * permute function
*/
void permute(int *array, int *lineup, int start, int &lSum,
int &hSum, int &diff, int interval){
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
			for(int j = 0; j < NUM_PROCS; j++){
				lineup[j] = array[j];
				//cout<<lineup[j]<<"\t";
			}
			//cout<<endl;
		}
		//Move on to the next permutation
		next_permutation(array, array + NUM_PROCS);
	}
}

/**
 * Main function
*/
int main(int argc, char *argv[]){
	//Set up timekeeping variables
	time_t start, end;
	//start the clock
	start = time(0);
	//Set up MPI variables
	int mpiRank, nprocs;
	//mpIRank holds the "id" of the MPI process
	//nprocs is the total number of MPI processes
	
	MPI_Status stat;
	
	MPI_Init(&argc, &argv);
	
	MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &mpiRank);
	
	
	//DEBUG
//	printf("Hello from processor %d of %d\n", mpiRank, nprocs);
	
	//get the interval from NUM_PROCS! / number of MPI processes
	int interval = factorial(NUM_PROCS) / nprocs;
	
	int processes[NUM_PROCS];
	int lineup[NUM_PROCS];	//Lineup is the variable we use to store the most efficient path
	
	//Variables for comparison
	int lowerSum, upperSum, difference;
	//initialize the array to simulate processes
	
	upperSum = 0;
	
	//Start the process on the master thread
	if(mpiRank == 0){
		//Populate processes and lineup
		for(int i = 0; i < NUM_PROCS; i++){
			processes[i] = rand()%500;
			lineup[i] = processes[i];
		}
		
		//sort out the processes
		sort(processes, processes + NUM_PROCS);
		sort(lineup, lineup + NUM_PROCS);	//sorts the lineup
		
		//DEBUG
		cout<<"Interval:\t"<<interval<<endl;
		cout<<"Default Lineup:"<<endl<<processes[0]<<"\t";
		
		lowerSum = processes[0];
		
		//Find the upper sum
		for(int i = 1; i < NUM_PROCS;i++){
			upperSum += processes[i];
			cout<<processes[i]<<"\t";
		}
		difference = abs(lowerSum - upperSum);
		cout<<endl<<"Lower sum\t"<<lowerSum<<"\tUpper sum:\t"<<upperSum
		<<"\tDifference:\t"<<difference<<endl;
		
	}
		//get the values of diff and procs from root thread
		MPI_Bcast(&difference, 1, MPI_INT, 0, MPI_COMM_WORLD);
		MPI_Bcast(processes, NUM_PROCS, MPI_INT, 0, MPI_COMM_WORLD);
	
		//DEBUG BLOCK
		/*
		cout<<"Difference from\t"<<mpiRank<<"\tis\t"<<difference<<endl;
		cout<<"Processes from "<<mpiRank<<":"<<endl;

		for(int i = 0; i < NUM_PROCS; i++){
			cout<<processes[i]<<"\t";
		}
		
		cout<<endl;
		*/
		int *nthperm;
		if(mpiRank == 0){
			nthperm = processes;
		}
		else{
			nthperm = nthpermutation(interval * mpiRank, processes, NUM_PROCS);
		}
		
		for(int i = 0; i < NUM_PROCS; i++){
			permute(nthperm, lineup, i, lowerSum, upperSum, difference, interval);
			if(mpiRank == 0)
				sort(nthperm, nthperm + NUM_PROCS);
			else{
				delete nthperm;
				nthperm = nthpermutation(interval * mpiRank, processes, NUM_PROCS);
			} 
		}
		
		if(mpiRank != 0){
			MPI_Send(&difference, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
			MPI_Send(&lowerSum, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
			MPI_Send(&upperSum, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
			MPI_Send(lineup, NUM_PROCS, MPI_INT, 0, 1, MPI_COMM_WORLD);
		}
		if(mpiRank == 0){
			//variable to store the differences from incoming differences variables
			int differences[nprocs-1];
			int lSums[nprocs - 1];
			int hSums[nprocs - 1];
			int *lineups[nprocs - 1];
			for(int i = 1; i < nprocs; i++){
				MPI_Recv(&differences[i-1], 1, MPI_INT, i, 1, MPI_COMM_WORLD, &stat);
				MPI_Recv(&lSums[i-1], 1, MPI_INT, i, 1, MPI_COMM_WORLD, &stat);
				MPI_Recv(&hSums[i-1], 1, MPI_INT, i, 1, MPI_COMM_WORLD, &stat);
				MPI_Recv(&lineups[i-1], NUM_PROCS, MPI_INT, i, 1, MPI_COMM_WORLD, &stat);
			}
			for(int i = 0; i < nprocs - 1; i++){
				//cout<<"difference at: "<< i + 1<<" is "<<differences[i]
				//<<endl;
				
				if(differences[i] < difference){
					difference = differences[i];
					lowerSum = lSums[i];
					upperSum = hSums[i];
					for(int j = 0; j < NUM_PROCS; j++){
						lineup[j] = lineups[i][j];
					}
				}
			}
			end = time(0);
			cout<<"Most efficient path:"<<endl;
			cout<<"Lower sum:\t"<<lowerSum<<"\tUpper sum:\t"<<
			upperSum<<"\tDifference:\t"<<difference<<endl;
			cout<<"Lineup:"<<endl;
			for(int i = 0; i < NUM_PROCS; i++){
					cout<<lineup[i]<<"\t";
			}
			cout<<endl<<"It took "<<end - start<<" seconds."<<endl;
		}
	
	//Close the MPI environment
	MPI_Finalize();
	
	//Tell the program we're done running
	return 0;
}
