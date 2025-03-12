#include <iostream>

// #include "hdf5/version.hpp"
//#include "benchmark/csrmv.hpp"
//#include "code/util/compiler.hpp"
//#include "code/generator/std_matrix.hpp"
//#include "code/openmp/general.hpp"


//#include "test/std_matrix.hpp"

#include "benchmark/mv.hpp"
#include "code/blas/mv.hpp"
#include "test/blas.h"

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
    benchmark_mv();
    return 0;
}
