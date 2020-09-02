#include <iostream>
#include <mpi.h>

/* Пусть все чётные процессы отправят сообщения нечётным процессом с id + 1 */

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);
	
    int procid, num_procs;
    MPI_Comm_rank(MPI_COMM_WORLD, &procid);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    int val = 5;
    MPI_Status status;
    
    if (procid % 2 == 0 and procid < num_procs - 1) {
        val += procid;

        MPI_Send(
            &val /* pointer to start */,
            1 /* number of words */,
            MPI_INT /* word type */, 
            procid + 1 /* rank of receiver */, 
            0 /* tag */,
            MPI_COMM_WORLD /* communicator: default - all world */
        );

        std::cout << "Process " << procid << " sended val " << val << " to process " << procid + 1 << std::endl;

    } else if (procid % 2 != 0) {
        int recv_val;
		
        MPI_Recv(
            &recv_val /* pointer to start */, 
            1 /* number of words */, 
            MPI_INT /* type of word */, 
            procid - 1 /* rank of sender */, 
            0 /* tag */, 
            MPI_COMM_WORLD /* communicator: default - all world */,
            &status
        );

        std::cout << "Process " << procid << " got val " << recv_val << " from process " << procid - 1 << std::endl;
    }

    MPI_Finalize();
    return 0;
}
