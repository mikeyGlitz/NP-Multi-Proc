/*
* File Name:	permutation.cpp
* Author:	Michael Glitzos
*
* Description:
* This file is a simple test for a permutaiton function.
* The goal is to run a permutations using the next_permutation
* function side-by-side with a mathematical computation that
* uses factorials to get the number of permutations.
* Then see if the numbers match up
*/

//Include header files and libraries
#include <cstdlib>
#include <iostream>
#include <algorithm>

using namespace std;
#define NUM_INDEXES 3

/*
* Permutation function
* This function calculates the permutation using a call to
* next_permutation and sends back the number of permutations
* once the number's been gathered (think brute force)
*/
int permutations(int *array){
	int permutations = 0;
//	cout<<NUM_INDEXES<<endl;
//	cout<<"Perm:\tArray:"<<endl;
	do{
/*		cout<<permutations<<"\t";
		for(int i = 0; i < NUM_INDEXES; i++){
			cout<<array[i]<<"\t";
		}
		cout<<endl;*/
		permutations++;
	}while(next_permutation(array, array + NUM_INDEXES));
	return permutations;
}


/*
* factorial function
* This function is designed to run a recursive step to find the
* factorial of a number. Once the factorial is generated, the 
* function returns the factorial value.
*/
int factorial(int n){
	int temp;
	
	if(n <= 1) return 1;
	temp = n * factorial(n - 1);
	return temp;
}

/*
* Main function
*/
int main(int argc, char* argv[]){
	//Create an array
	int array[NUM_INDEXES];
	
	//Populate the array with random numbers
	for(int i = 0; i < NUM_INDEXES; i++){
		array[i] = rand();
	}
	sort(array, array + NUM_INDEXES);
	//Variables to store the array permutation and the
	//permutation gathered from the formula
	int arrayperm, formperm;
	
	arrayperm = permutations(array);
	formperm = factorial(NUM_INDEXES);
	
	//Print the results
	cout<<"Result gathered from cycling through each array perm:\t"<<arrayperm<<endl;
	cout<<"Result gathered from calculating the factorial of NUM_INDEXES:\t"<<formperm<<endl;
	
	return 0;	//Tell the program we're done running.
}
