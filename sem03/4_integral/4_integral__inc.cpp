#include <iostream>
#include <cstdlib>
#include <omp.h>

double f(double x){
    return (4.0 / (1.0 + x*x) );
}

int main(int argc, char* argv[]) {
    
    long const N = 1e6;
    
    double integral = 0;
    double dx = 1 / (double)N;
    
    for (int i = 1; i <= N; i++) {
        double part = ( f(dx * i) + f(dx * (i - 1)) ) * dx / 2;  // площадь трапеции
        integral = integral + part;
    }
    
    std::cout << integral << std::endl;
}
