#include <cstdlib>
#include <iostream>
#include <algorithm>
#include "omp.h"
#include <fstream>
#include <string>
#include <sstream>

using namespace std;
#define NUM_INDEXES 4

int main(int argc, char *argv[]){
	int nthreads, tid;
	
	int test[NUM_INDEXES] = {1,2,3,4};
	int test2[NUM_INDEXES] = {1,2,3,4};
	
	
	omp_set_num_threads(2);
	#pragma omp parallel private(tid)
	{
		
		/* Obtain and print thread id */
		tid = omp_get_thread_num();
		//cout << "Hello World from thread = " << tid << endl;

		/* Only master thread does this */
		if (tid == 0) 
		{
			nthreads = omp_get_num_threads();
			cout << "Number of threads = " << nthreads << endl;
			ofstream output;
			output.open("thread0.txt");
			if (!output)
			{
				cout << "Epic fail opening output file thread0.txt" << endl;
			}
			else 
			{
				do
				{	
					output << test[0] << " " << test[1] << " " << test[2] << " " << test[3] <<endl;
				} while (next_permutation(test, test+4));
				output.close();
			}
		}
		else if (tid == 1)
		{
                        ofstream output;
                        output.open("thread1.txt");
                        if (!output)
                        {
                                cout << "Epic fail opening output file thread1.txt" << endl;
                        }
                        else
                        {
                                do
                                {
                                        output << test2[0] << " " << test2[1] << " " << test2[2] << " " << test2[3] <<endl;
                                } while (next_permutation(test2, test2+4));
				output.close();
                        }
 
		}
	}
}
