#include <iostream>
#include <mpi.h>

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);
    
    int procid, num_procs;
    MPI_Comm_rank(MPI_COMM_WORLD, &procid);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    
    // соседи
    int previd = procid - 1;
    int nextid = procid + 1;
    if (procid == 0) {
        previd = num_procs - 1;
    }
    if (procid == (num_procs - 1)) {
        nextid = 0;
    }
    
    MPI_Request requests[4];
    MPI_Status statuses[4];
    int buf[2];
    int tag1 = 0;
    int tag2 = 0;
    
    MPI_Irecv(&buf[0], 1, MPI_INT, previd, tag1, MPI_COMM_WORLD, &requests[0]);
    MPI_Irecv(&buf[1], 1, MPI_INT, nextid, tag2, MPI_COMM_WORLD, &requests[1]);
    
    MPI_Isend(&procid, 1, MPI_INT, previd, tag2, MPI_COMM_WORLD, &requests[2]);
    MPI_Isend(&procid, 1, MPI_INT, nextid, tag1, MPI_COMM_WORLD, &requests[3]);
    
    MPI_Waitall(4, requests, statuses);
    std::cout << "Process " << procid << " communicated with processes " << previd << " and " << nextid << std::endl;
    
    MPI_Finalize();
    return 0;
}
