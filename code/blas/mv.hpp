
#ifndef BLAS_MV_HPP
#define BLAS_MV_HPP
#include <cblas.h> // OpenBLAS  or Intel MKL

#include <vector>

#include "data/std_flat_matrix.hpp"
#include "data/std_vector.hpp"
#include "data/std_crs_matrix.hpp"

void MatrixVectorMultiplikationBLAS(const StdFlatMatrix & matrix_A, double alpha,  const StdVector & vektor_x, double beta, StdVector & vektor_y) {
    cblas_dgemv(CblasRowMajor,  // Row-major storage
                CblasNoTrans,    // No transpose (A * x)
                static_cast<int>(matrix_A._num_rows), static_cast<int>(matrix_A._num_cols),            // Matrix dimensions (M=3, N=3)
                alpha,           // Scalar alpha
                matrix_A._data.data(), static_cast<int>(matrix_A._num_rows),     // Matrix A and leading dimension
                vektor_x._data.data(), 1,     // Vector x and increment
                beta,            // Scalar beta
                vektor_y._data.data(), 1);    // Vector y and increment
}

// y = alpha * A * x + beta * y
void MatrixVectorMultiplikationBLAS(const StdCRSMatrix & matrix_A, double alpha,  const StdVector & vektor_x, double beta, StdVector & vektor_y) {
    cblas_ztrsv()
    const size_t vector_size = vektor_y.size();
    for (size_t i = 0; i < vector_size; i++) {
        vektor_y[i] *= beta;
    }

    for (size_t i = 0; i < vector_size; i++) {
        for (size_t j = matrix_A._row_ptr[i]; j < matrix_A._row_ptr[i + 1]; j++) {
            size_t col = matrix_A._col_index[j];
            double val = matrix_A._data[j];
            vektor_y[i] += alpha * val * vektor_x[col];
        }
    }
}
#endif