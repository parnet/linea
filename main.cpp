#include <iostream>

// #include "hdf5/version.hpp"
#include "benchmark/csrmv.hpp"
#include "code/util/compiler.hpp"
#include "code/generator/std_matrix.hpp"
#include "code/openmp/general.hpp"
void daxpy() { }
#include "test/std_matrix.hpp"

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
    benchmark_csrmv();
    double x;
    std::cin >> x;
    return 0;
}
