#include <iostream>
#include <cstdlib>
#include <omp.h>

int main(int argc, char **argv) {
    int sum = 0;
    
    int asked_threads_num = atoi(argv[1]);
    //std::cout << "Asked threads num: " << asked_threads_num << std::endl;
    
    omp_set_num_threads(asked_threads_num);
    
    double begin = omp_get_wtime();
    
    # pragma omp parallel
    {
        int working_threads_count = omp_get_num_threads();
        
        //#pragma omp master
        //std::cout << "Working threads count: " << working_threads_count << std::endl;
        
        # pragma omp for schedule(dynamic, 1000) reduction(+:sum)
        for (int i = 0; i < 1000000; i++) {
            sum += 1; 
        }
    }
    
    double end = omp_get_wtime();
    
    std::cout << "Got time: "  << end - begin << " and sum " << sum << std::endl;
}
