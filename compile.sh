#!/bin/bash

#Compile command for sequential
g++ sequential.cpp -o sequential


#Currently unimplemented. Uncomment these to get them to work

#Compile command for openmp
g++ -c parallel_common.cpp
g++ -c omp.cpp
g++ parallel_common.o omp.o -fopenmp -o omp

#Compile command for MPI
mpiCC mpi.cpp parallel_common.cpp
