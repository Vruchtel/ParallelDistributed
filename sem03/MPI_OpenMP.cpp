#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <mpi.h>
#include <omp.h>

/* Пример А. Долуденко */

using namespace std;

const int N = 1000000000;

double f(double x) {
    return (1.0 / (1.0 + x * x));
}

int main(int argc, char **argv)
{
    int i, myid, num_threads;
    int rank, size, chunk, begin;
    double dx, s, S;
    double time_start;

    MPI_Status Status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    dx = 1.0 / (double)N;
    s = 0.0;
    S = 0.0;

    omp_set_num_threads(2);

    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) time_start = MPI_Wtime();

    if (rank == 0) {
        chunk = N / size;
        if (N % size) chunk++;
        
        begin = 0;
        begin = begin + chunk;
        
        for (i = 1; i < size; i++) {
            chunk = N / size;
            if (i < N % size) {
                chunk = chunk + 1;
               // printf("ProceSS 0, begin for send = %d, chunk = %d, i = %d, remainder = %d \n", begin, chunk, i, N % size);
            }
            MPI_Send(&begin, 1, MPI_INT, i, i, MPI_COMM_WORLD);
            MPI_Send(&chunk, 1, MPI_INT, i, i, MPI_COMM_WORLD);
            begin = begin + chunk;
        }
        chunk = N / size;
        if (N % size) chunk ++;
        begin = 0;
        //printf("### 0 Begin = %d, chunk = %d, Remainder = %d \n", begin, chunk,  N % size);
    }

    if (rank != 0) {
        MPI_Recv(&begin, 1, MPI_INT, 0, rank, MPI_COMM_WORLD, &Status);
        MPI_Recv(&chunk, 1, MPI_INT, 0, rank, MPI_COMM_WORLD, &Status);

       // printf("I'm process # %d, begin = %d, chunk = %d \n", rank, begin, chunk);
    }
#pragma omp parallel private(myid)
    {
        myid = omp_get_thread_num();
        num_threads = omp_get_num_threads();
        printf("Parallel part. Process rank = %d. Thread # %d, num_threads = %d \n", rank, myid, num_threads);
#pragma omp for reduction(+: s) private (i)
        for (i = begin; i < begin + chunk; i++) {
            s = s + (f(dx * i) + f(dx * (i - 1))) * dx / 2;
        }
    }
    //printf("I'm # %d, s = %f \n", rank, s);

    if (rank != 0) MPI_Send(&s, 1, MPI_DOUBLE, 0, rank, MPI_COMM_WORLD);

    if (rank == 0) {
        S = S + s;
        for (i = 1; i < size; i++) {
            MPI_Recv(&s, 1, MPI_DOUBLE, i, i, MPI_COMM_WORLD, &Status);
            S = S + s;
        }
        printf("All sum = %f \n", 4 * S);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) printf("Time of calculation = %f \n", MPI_Wtime() - time_start);
       MPI_Finalize();
}

