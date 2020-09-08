#include <iostream>
#include <mpi.h>

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);
    
    int procid, num_procs;
    MPI_Comm_rank(MPI_COMM_WORLD, &procid);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    
    /// TODO: отправить свой id двум соседям
     
    MPI_Finalize();
    return 0;
}
