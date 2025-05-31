#pragma once
#ifdef USE_BLASE
#include "libs/blas.hpp"


#include <vector>

#include "data/matrix.hpp"
#include "data/vector.hpp"

struct Operations_BLAS {
    static void matrix_vector_multiplication(const Matrix & matrix_A, double alpha,  const Vector & vektor_x, double beta, Vector & vektor_y);
};


inline void Operations_BLAS:: matrix_vector_multiplication(const Matrix & matrix_A, double alpha,  const Vector & vektor_x, double beta, Vector & vektor_y){
    cblas_dgemv(CblasRowMajor,  // Row-major storage
                CblasNoTrans,    // No transpose (A * x)
                static_cast<int>(matrix_A._num_rows), static_cast<int>(matrix_A._num_cols),            // Matrix dimensions (M=3, N=3)
                alpha,           // Scalar alpha
                matrix_A._data.data(), static_cast<int>(matrix_A._num_rows),     // Matrix A and leading dimension
                vektor_x._data.data(), 1,     // Vector x and increment
                beta,            // Scalar beta
                vektor_y._data.data(), 1);    // Vector y and increment
}
#endif
