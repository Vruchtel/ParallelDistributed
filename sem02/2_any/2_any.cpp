#include <iostream>
#include <mpi.h>

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);
    
    int procid, num_procs;
    MPI_Comm_rank(MPI_COMM_WORLD, &procid);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    
    int N = 10;
    int array[N];
    int recv_array[N];
    MPI_Status status;
    MPI_Request request;
    
    int part_len = N / num_procs;
    
    if (procid == 0) {
        for (int i = 0; i < N; i++) {
            array[i] = i;
        }
        for (int i = 0; i < num_procs; i++) {
            // Отправляем каждому рабочему процессу часть данных
            MPI_Isend(&array[i * part_len], part_len, MPI_INT, i, i, MPI_COMM_WORLD, &request);
        }
    }
        
    MPI_Recv(&recv_array[0], part_len, MPI_INT, 0, procid, MPI_COMM_WORLD, &status);
    
    // Каждый из процессов посчитает сумму своей части массива...
    int sum = 0;
    for (int i = 0; i < N / num_procs; i++) {
        sum += recv_array[i];
    }
    
    // ... и отправит её обратно нулевому процессу...
    MPI_Isend(&sum, 1, MPI_INT, 0, procid, MPI_COMM_WORLD, &request);
    
    // ... который получит данные не важно в каком порядке
    if (procid == 0) {
        int current_sum = 0;
        int recv_val;
        for (int i = 0; i < num_procs; i++) {
            MPI_Recv(&recv_val, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            std::cout << "Got " << recv_val << " from process " << status.MPI_SOURCE << std::endl;
            current_sum += recv_val;
        }
        
        std::cout << "All sum: " << current_sum << std::endl;
    }
    
    MPI_Finalize();
    return 0;
}
