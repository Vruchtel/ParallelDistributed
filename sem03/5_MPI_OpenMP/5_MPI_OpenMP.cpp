#include <iostream>
#include <mpi.h>
#include <omp.h>


int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    
    int procid, num_procs;
    MPI_Comm_rank(MPI_COMM_WORLD, &procid);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    
    omp_set_num_threads(2);
    
    MPI_Barrier(MPI_COMM_WORLD);
    
    // Инициализация
    int N = 100000000;
    int *array = new int[N];
    if (procid == 0) {
        for (int i = 0; i < N; i++) {
            array[i] = 1;
        }
        
        // 0й процесс отправляет части массива всем остальным
        for (int i = 1; i < num_procs and i < N; i++) {
            MPI_Send(&array[i * N / num_procs], N / num_procs, MPI_INT, i, i, MPI_COMM_WORLD);
        }
    }
    
    else {
        // Остальные процессы получают свои части
        MPI_Status status1;
        MPI_Recv(array, N / num_procs, MPI_INT, 0, procid, MPI_COMM_WORLD, &status1);
    }
    
    // Все считают сумму на своём участке
    int sum = 0;
    # pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < N / num_procs; i++) {
        sum += array[i];
    }
    std::cout << "Process " << procid << " got sum: " << sum << std::endl;
    
    
    // Отправляют результат 0му процессу
    MPI_Request request;
    MPI_Isend(&sum, 1, MPI_INT, 0, procid, MPI_COMM_WORLD, &request);
    
    
    int final_sum = 0;
    // 0й процесс получает итоговый результат
    if (procid == 0) {
        int recv_val;
        MPI_Status status2;
        for (int i = 0; i < num_procs; i++) {
            MPI_Recv(&recv_val, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status2);
            std::cout << "Got " << recv_val << " from process " << status2.MPI_SOURCE << std::endl;
            final_sum += recv_val;
        }
        
        std::cout << "All sum: " << final_sum << std::endl;
    }
    
    MPI_Finalize();
    
    return 0;
}
