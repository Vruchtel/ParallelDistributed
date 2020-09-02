#include <iostream>
#include <mpi.h>

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);
    
    int procid, num_procs;
    MPI_Comm_rank(MPI_COMM_WORLD, &procid);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    
    int N = 10;
    int array[N];
    MPI_Status status;
    
    if (procid == 0) {
        for (int i = 0; i < N; i++) {
            array[i] = i;
        }
        
        for (int i = 1; i < num_procs and i < N; i++) {
            MPI_Send(
                &array[i * N / num_procs] /* pointer to start */,
                N / num_procs /* number of words */,
                MPI_INT /* word type */, 
                i /* rank of receiver */, 
                i /* tag */,
                MPI_COMM_WORLD /* communicator: default - all world */
            );
        }
        
    } else if (procid > 0) {
        MPI_Recv(
            &array[0] /* pointer to start */, 
            N / num_procs /* number of words */, 
            MPI_INT /* type of word */, 
            0 /* rank of sender */, 
            procid /* tag */, 
            MPI_COMM_WORLD /* communicator: default - all world */,
            &status
        );
        int sum = 0;
        for (int i = 0; i < N / num_procs; i++) {
            sum += array[i];
        }
        std::cout << "Process " << procid << " got sum: " << sum << std::endl;
    }
    
    MPI_Finalize();
    return 0;
}
