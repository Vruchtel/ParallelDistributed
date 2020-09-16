#include <iostream>
#include <cstdlib>
#include <omp.h>

double f(double x){
    return (4.0 / (1.0 + x*x) );
}

int main(int argc, char* argv[]) {
    
    //long const N = 1e6;
    int num_procs = atoi(argv[1]);
    long N = atol(argv[2]);
    
    omp_set_num_threads(num_procs);
    
    double integral = 0;
    double dx = 1 / (double)N;
    
    double begin = omp_get_wtime();
    
    # pragma omp parallel for reduction(+:integral)
    for (int i = 1; i <= N; i++) {
        double part = ( f(dx * i) + f(dx * (i - 1)) ) * dx / 2;  // площадь трапеции
        integral = integral + part;
    }
    
    double end = omp_get_wtime();
    std::cout << "Threads " << num_procs << " spend time " << end - begin << " got integral: " << integral << std::endl;
}
