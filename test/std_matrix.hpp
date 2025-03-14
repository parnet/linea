#ifndef TEST_STD_MATRIX_HPP
#define TEST_STD_MATRIX_HPP
#include <iostream>

#include "../code/converter/std_crs_matrix.hpp"
#include "../code/cpu/mv.hpp"
#include "../code/data/std_matrix.hpp"

void gneral_matrix_test() {
    /**
     * creates a dense matrix and verifies the crs matrix for a matrix vector multiplication
     */
    StdMatrix matrix = StdMatrix(5,5);
    matrix(0,0) = 2;
    matrix(0,4) = -3;
    matrix(1,2) = 5;
    matrix(1,4) = 1;
    matrix(2,0) = 2;
    matrix(2,1) = -2;
    matrix(3,1) = 3;
    matrix(3,2) = 7;
    matrix(3,3) = -2;
    matrix(4,0) = -1;
    matrix(4,4) = 1;

    std::cout << matrix.str() << std::endl;

    Vector vector = Vector(5);
    vector[0] = 3;
    vector[1] = 1;
    vector[2] = -2;
    vector[3] = 1;
    vector[4] = 7;

    std::cout << vector.str() << std::endl;
    Vector result = Vector(5);
    result[0] = 0;
    result[1] = 0;
    result[2] = 0;
    result[3]= 0;
    result[4] = 0;
    std::cout << result.str() << std::endl;

    MatrixVectorMultiplikation(matrix,1,vector,1,result);
    std::cout << result.str() << std::endl;

    CRS_Matrix smatrix = std_crs_from_matrix(matrix);
    std::cout << smatrix.str() << std::endl;

    result[0] = 0;
    result[1] = 0;
    result[2] = 0;
    result[3] = 0;
    result[4] = 0;
    std::cout << result.str() << std::endl;

    MatrixVectorMultiplikation(smatrix,1,vector,1,result);
    std::cout << result.str() << std::endl;
}

#endif
