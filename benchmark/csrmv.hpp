
#ifndef BENCHMARK_CSRMV_HPP
#define BENCHMARK_CSRMV_HPP
#include <iostream>

#include "../code/generator/naive_matrix.h"
#include "../code/data/naive_matrix.hpp"
#include "../code/util/converter.hpp"
#include "../code/util/timer.hpp"

void benchmark_csrmv(){
    Timer timer = Timer();
    for (int k = 0; k < 8; ++k) {
        size_t gridsize = (2 * (1 << k) +1);
        //std::cout << k << ": " << gridsize << std::endl;
        NaiveMatrix matrix;// = NaiveMatrix(gridsize*gridsize,gridsize*gridsize);
        timer.start();
        create_laplacian_2d(matrix,gridsize);
        timer.stop();
        double time = timer.get();
        size_t memory_size = matrix.memory_size();
        //std::cout << "memory(A)=" << memory_size << std::endl;

        std::cout << k << "; " << gridsize << "; " << convert(memory_size, Unit::Giga) << "\t" << sizeof(matrix)<< "\t"<<sizeof(matrix.data) << std::endl;
        std::cout << "time="<< time << std::endl;
        double K;
        std::cin >> K;
        std::cout << K << std::endl;
        matrix.data[0] = K;
    }
    std::cout << "finished benchmark_csrmv"<< std::endl;
};
#endif
