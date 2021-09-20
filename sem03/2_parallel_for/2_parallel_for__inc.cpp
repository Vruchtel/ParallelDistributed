#include <iostream>
#include <omp.h>

int main(int argc, char **argv) {
    // TODO: вычислить значение суммы параллельно с помощью группы потоков
    
    int sum = 0;
    
    for (int i = 0; i < 10000; i++) {
        sum += 1;
    }
    
    std::cout << "Result sum: "  << sum << std::endl;
}
