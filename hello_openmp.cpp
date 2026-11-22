#include <iostream>
#include <omp.h>

int main() {
    std::cout << "OpenMP version macro: " << _OPENMP << '\n';

    #pragma omp parallel num_threads(4)
    {
        #pragma omp critical
        std::cout << "Hello from thread " << omp_get_thread_num()
                  << " of " << omp_get_num_threads() << '\n';
    }

    return 0;
}
