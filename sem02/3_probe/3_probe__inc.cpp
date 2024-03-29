#include <iostream>
#include <ctime>
#include <cstdlib>
#include <mpi.h>

/*Процесс 0 отправляет процессу 1 массив случайного размера*/

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);
    
    int procid, num_procs;
    MPI_Comm_rank(MPI_COMM_WORLD, &procid);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    
    const int MAX_NUMBERS = 100;
    int numbers_amount = 0;
    
    if (procid == 0) {
        srand(time(NULL));
        
        int numbers[MAX_NUMBERS];
        numbers_amount = (rand() / float(RAND_MAX)) * MAX_NUMBERS;
        
        MPI_Send(numbers, numbers_amount, MPI_INT, 1, 0, MPI_COMM_WORLD);
        
        std::cout << "Process 0 sent " << numbers_amount << " numbers to process 1" << std::endl;
    }
    else if (procid == 1) {
        /// TODO: Создать массив нужного размера и записать в него полученные данные
        
        std::cout << "Process 1 received " << numbers_amount << " numbers from process 0" << std::endl;
        std::cout << "Message source is " << status.MPI_SOURCE << ", tag is " << status.MPI_TAG << std::endl;
    }
    
    MPI_Finalize();
    return 0;
}
