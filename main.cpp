#include <iostream>

// #include "hdf5/version.hpp"
//#include "benchmark/csrmv.hpp"
//#include "code/util/compiler.hpp"
//#include "code/generator/std_matrix.hpp"
//#include "code/openmp/general.hpp"


//#include "test/std_matrix.hpp"

#include "problem/heat_equation.hpp"
//#include "benchmark/mv.hpp"
#include "code/operations_blas.hpp"
#include "libs/cuda.hpp"
#include "test/blas.h"
#include "util/memory.hpp"



int main()
{
    //linea::openmp::info();
    // print_arch();
    // auto version = version_hdf5();
    // std::cout << "HDF5: "<< version.str() << std::endl;
    // StdMatrix empty_matrix = StdMatrix(0,0);
    // StdCRSMatrix matrix = StdCRSMatrix(empty_matrix);

    // create_laplacian_2d(matrix,5);
    //std::cout << 7*7*7*7 << std::endl;
    // std::cout << matrix.str() << std::endl;

    // gneral_matrix_test();
    //benchmark_mv();
    //blas_mv_test();
    //double x;
    //std::cin >> x;
    //gneral_matrix_test_blas();
    //benchmark_mv();
    //std::cout <<  2* (1 << 7) +1  << std::endl;
    //memory::info(Giga);
    //check_cuda_device();
    //benchmark_mv();
    benchmark_heat_equation();

    return 0;
}

/***
 * dense max matrix size gridsize = 257
 * 257 * 257 total grid points
 * 257 * 257 x 257 * 257 matrix size
 * 66'049 x 66'049
 * 4'362'470'401 Einträge insgesamt
 ***/