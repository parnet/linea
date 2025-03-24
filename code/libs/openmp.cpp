
#include "openmp.hpp"

namespace linea::openmp {
    void info(){


#pragma omp parallel
        {
            int tid = omp_get_thread_num();
            int total = omp_get_num_threads();
#pragma omp critical
            std::cout << "Thread " << tid << " av " << total << std::endl;
        }
    }
}