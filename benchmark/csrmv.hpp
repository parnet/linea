
#ifndef BENCHMARK_CSRMV_HPP
#define BENCHMARK_CSRMV_HPP
#include <iostream>

#include "../code/data/naive_matrix.hpp"
#include "../code/util/converter.hpp"

void benchmark_csrmv(){
    for (int k = 0; k < 9; ++k) {
        size_t gridsize = (2 * (1 << k) +1);
        NaiveMatrix matrix = NaiveMatrix(gridsize*gridsize,gridsize*gridsize);
        size_t memory_size = matrix.memory_size();
        std::cout << k << "; " << gridsize << "; " << convert(memory_size, Unit::Giga) << std::endl;
    }
};
#endif
