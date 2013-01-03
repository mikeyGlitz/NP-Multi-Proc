/*
 * File Name:	parallel_common.h
 * Author:		Michael Glitzos
 * 
 * Description:
 * Not that I'm lazy, but it's rather inconvienient to re-write the
 * same code over and over again. This file contains methods that are
 * common to my parallel implementations.
 * 
 * Depends on: parallel_common.cpp
*/
#include <cstdlib>
#include <vector>

#ifndef PARALLEL_COMMON
#define PARALLEL_COMMON

//function for computing the factorial of a number
int factorial(int n);

//function for getting the nth permutation from an existing array
int *nthpermutation(int index, int *array, int size);

#endif
