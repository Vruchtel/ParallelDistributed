#include <iostream>
#include <cstdlib>
#include <omp.h>

double f(double x){
    return (4.0 / (1.0 + x*x) );
}

int main(int argc, char* argv[]) {
    
    //long const N = 1e6;
    int proc_num = atoi(argv[1]);
    long N = atol(argv[2]);
    omp_set_num_threads(proc_num);
    
    double integral = 0;
    double dx = 1 / (double)N;
    
    double start = omp_get_wtime();
    
    # pragma omp parallel for reduction(+:integral)
    for (int i = 1; i <= N; i++) {
        double part = ( f(dx * i) + f(dx * (i - 1)) ) * dx / 2;  // площадь трапеции
        integral = integral + part;
    }
    
    double end = omp_get_wtime();
    
    //std::cout << "Got integral: " << integral << std::endl;
    std::cout << "Processes " << proc_num << " took time " << end - start << " and got integral " << integral << std::endl;
}
