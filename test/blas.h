#ifndef BLAS_TEST_H
#define BLAS_TEST_H

/*

#include <iostream>
int blas_mv_test() {
    // Matrix A (3x3)
    std::vector<double> A = {
        1.0, 2.0, 3.0,  // Row 1
        4.0, 5.0, 6.0,  // Row 2
        7.0, 8.0, 9.0   // Row 3
    };

    // Vector x (3x1)
    std::vector<double> x = {1.0, 2.0, 3.0};

    // Vector y (3x1), initially zero
    std::vector<double> y(3, 0.0);

    // Scalars
    double alpha = 1.0;
    double beta = 0.0;

    // Call cblas_dgemv
    cblas_dgemv(CblasRowMajor,  // Row-major storage
                CblasNoTrans,    // No transpose (A * x)
                3, 3,            // Matrix dimensions (M=3, N=3)
                alpha,           // Scalar alpha
                A.data(), 3,     // Matrix A and leading dimension
                x.data(), 1,     // Vector x and increment
                beta,            // Scalar beta
                y.data(), 1);    // Vector y and increment

    // Print result
    std::cout << "Result vector y:\n";
    for (double val : y) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    return 0;
}

void gneral_matrix_test_blas() {
    / **
     * creates a dense matrix and verifies the crs matrix for a matrix vector multiplication
     * /
    Matrix matrix = Matrix(5,5);
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

    Matrix smatrix = Matrix(matrix);
    std::cout << smatrix.str() << std::endl;

    result[0] = 0;
    result[1] = 0;
    result[2] = 0;
    result[3] = 0;
    result[4] = 0;
    std::cout << result.str() << std::endl;

    MatrixVectorMultiplikationBLAS(smatrix,1,vector,1,result);
    std::cout << result.str() << std::endl;
}*/

#endif
