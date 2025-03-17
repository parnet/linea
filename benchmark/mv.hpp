#ifndef BENCHMARK_MV_HPP
#define BENCHMARK_MV_HPP

#include <iostream>

#include "operations_avx.hpp"
#include "operations_avx512.hpp"
#include "operations_cpu.hpp"
#include "operations_mkl.hpp"
#include "operations_sse2.hpp"
#include "operations_blas.hpp"
#include "operations_openmp.hpp"

#include "code/util/timer.hpp"

#include "code/data/c_vector.hpp"

#include "code/data/vector.hpp"
#include "code/generator/std_vector.hpp"


#include "code/data/matrix.hpp"
#include "code/generator/std_flat_matrix.hpp"

#include "code/cpu/mv.hpp"


template <typename Operation = Operations_CPU>
void _benchmark_mv(){
    std::cout << ":::" << Operation::name <<":::"<< std::endl;
    Timer timer = Timer();
    double time = 0.0;

    for (int k = 3; k < 257; k+=8) {

        size_t gridsize = k;
        std::cout << "gridsize="<<k<<std::endl;
        std::cout <<"vectorsize="<<gridsize*gridsize<<std::endl;
        std::cout<<"matrixsize="<< gridsize*gridsize*gridsize*gridsize << std::endl;
        Matrix matrix;

        timer.start();
        create_laplacian_2d(matrix,gridsize);
        timer.stop();
        time = timer.get();
        //std::cout << "time(laplacian)="<< time << std::endl;
        Vector vector = Vector(gridsize*gridsize);

        timer.start();
        create_sin_1d(vector);
        timer.stop();
        time = timer.get();
        //std::cout << "time(interpolate)="<< time << std::endl;


        Vector result = Vector(gridsize*gridsize);
        timer.start();
        create_zero(result);
        timer.stop();
        time = timer.get();

        //std::cout << "time(zerofill)="<< time << std::endl;

        timer.start();
        Operation::matrix_vector_multiplikation(matrix,1,vector,1,result);
        timer.stop();
        time = timer.get();
        std::cout << "time(mv)="<< time << std::endl;


        //double K;
        //std::cin >> K;
        //std::cout << K << std::endl;
        //matrix.operator()(0,0) = K;
        std::cout << std::endl;
    }
    std::cout << "finished benchmark_csrmv"<< std::endl;
};

void benchmark_mv() {
    _benchmark_mv<Operations_CPU>();
    _benchmark_mv<Operations_SSE2>();
    _benchmark_mv<Operations_AVX>();
    _benchmark_mv<Operations_AVX512>();
    _benchmark_mv<Operations_OpenMP>();
    //_benchmark_mv<Operations_BLAS>();
    //_benchmark_mv<Operations_MKL>();
}
#endif