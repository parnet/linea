#include <iostream>

// #include "hdf5/version.hpp"
#include "util/compiler.hpp"
#include "generator/generator.hpp"
void daxpy() { }

int main()
{
    // print_arch();
    // auto version = version_hdf5();
    // std::cout << "HDF5: "<< version.str() << std::endl;
    StdMatrix empty_matrix = StdMatrix(0,0);
    StdCRSMatrix matrix = StdCRSMatrix(empty_matrix);

    create_laplacian_2d(matrix,5);
    //std::cout << 7*7*7*7 << std::endl;
    std::cout << matrix.str() << std::endl;

    return 0;
}
