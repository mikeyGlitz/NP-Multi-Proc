/*
 * File Name:	parallel_common.cpp
 * Author:		Michael Glitzos
 * 
 * Descrition:
 * This is the implementation to the file parallel_common.cpp.
 * I'm creating this library to make running parallel implementation
 * easier.
*/

#include <iostream>
#include "parallel_common.h"

using namespace std;

/**
 * Function to handle the computation of factorials
 * @param n the number we want to get the factorial of
 * @return the factorial of n
*/
int factorial(int n){
	return (n == 1 || n == 0) ? 1 : factorial(n - 1) * n;
}

/**
 * Function to fetch the nth permutation of an array
 * This algorithm uses factoradic calculations
 * reference: http://www.codeproject.com/Articles/21362/Permutations-in-C-Part-2
 * @param index represents the nth permutation we want to fetch
 * @param *array represents the array that we wish to permute
 * @param size size of the array
 * @return an array sorted out to the nth permutation
*/
int *nthpermutation(int index, int *array, int size){
	//Create a vector to store array values
	vector<int> tmpArray(size);
	//Populate the vector
	for(vector<int>::size_type i = 0; i<size; i++){
		tmpArray[i] = array[i];
	}
	
	//Create an array in the stack that we're going to return later
	int *permuted = new int[size];
	int numeric = index;
	
	//DEBUG
//	cout<<size<<"\t"<<numeric<<endl<<endl;
	
	int newIndex = 0;
	for(int i = size; i > 0; i--){
		int numeric2=0;		//holds the value of i-1 factorial * whatever the counter is
		int tmp;			//Just a little hack to get this to work right
		int counter = 0;	//Counter always defaults to 0
		int counter2 = 0;	//another hack
		while(!(numeric2 >= numeric)){
			counter2 = counter;
			tmp = numeric2;
//			cout<<"num"<<"\t"<<"fact(num-1) * counter"<<endl;
//			cout<<i<<"\t"<<factorial(i-1)*counter<<endl;
			numeric2 = factorial(i-1) * counter;
			counter ++;
		}
		
		//DEBUG
//		cout<<endl<<numeric<<"\t"<<numeric2<<"\t"<<counter2<<endl;
		
		permuted[newIndex] = tmpArray[counter2 -1];
		//DEBUG
//		cout<<newIndex<<"\t"<<tmpArray[counter2-1]<<endl;
		
		tmpArray.erase(tmpArray.begin() + counter2-1);
		
		numeric = numeric - tmp;
		newIndex ++;
	}
	
	//DEBUG
	/*for(int i = 0; i<size-1; i++){
		cout<<permuted[i]<<"\t";
	}
	cout<<endl;*/
	
	return permuted;
}
