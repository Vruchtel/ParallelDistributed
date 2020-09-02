#include <iostream>
#include <mpi.h>

int main(int argc, char *argv[]) {
	MPI_Init(&argc, &argv);
	
	int procid, num_procs;
	MPI_Comm_rank(MPI_COMM_WORLD, &procid);
	MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
	
	int array[10];
	MPI_Status status;
	
	if (procid == 0) {
		for (int i = 0; i < 10; i++) {
			array[i] = i;
		}
		
		 MPI_Send(
            &array[5] /* pointer to start */,
            5 /* number of words */,
            MPI_INT /* word type */, 
            1 /* rank of receiver */, 
            0 /* tag */,
            MPI_COMM_WORLD /* communicator: default - all world */
         );
         
	} else if (procid == 1) {
		MPI_Recv(
            &array /* pointer to start */, 
            5 /* number of words */, 
            MPI_INT /* type of word */, 
            0 /* rank of sender */, 
            0 /* tag */, 
            MPI_COMM_WORLD /* communicator: default - all world */,
            &status
        );
        int sum = 0;
        for (int i = 0; i < 5; i++) {
			std::cout << array[i] << std::endl;
			sum += array[i];
		}
        std::cout << "Sum: " << sum << std::endl;
	}
	
	MPI_Finalize();
	return 0;
}
