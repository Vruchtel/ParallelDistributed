#include <iostream>
#include <mpi.h>

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);
    
    int procid, num_procs;
    MPI_Comm_rank(MPI_COMM_WORLD, &procid);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    
    /// TODO: отправить свой id двум соседям
    int leftid = procid - 1;
    int rightid = procid + 1;
    if (procid == 0) { leftid = num_procs - 1; }
    if (procid == (num_procs - 1)) { rightid = 0; }
    
    MPI_Request reqs[4];
    MPI_Status stats[4];
    
    int buf[2];
    
    MPI_Isend(&procid, 1, MPI_INT, leftid, 0, MPI_COMM_WORLD, &reqs[0]);
    MPI_Isend(&procid, 1, MPI_INT, rightid, 1, MPI_COMM_WORLD, &reqs[1]);
    
    MPI_Irecv(&buf[0], 1, MPI_INT, leftid, 1, MPI_COMM_WORLD, &reqs[2]);
    MPI_Irecv(&buf[1], 1, MPI_INT, rightid, 0, MPI_COMM_WORLD, &reqs[3]);
    
    MPI_Waitall(4, reqs, stats);
    
    std::cout << "Process " << procid << " got " << buf[0] << " form " << leftid << " and " << buf[1] << " from " << rightid << std::endl;
     
    MPI_Finalize();
    return 0;
}
