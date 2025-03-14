#ifndef OPERATIONS_MKL_HPP
#define OPERATIONS_MKL_HPP
#ifdef USE_MKL
#include "libs/mkl.hpp"
#include "data/crs_matrix.hpp"
#include "data/vector.hpp"


struct Operations_MKL {
    static constexpr const char * name ="MKL";
    void matrix_vector_multiplikation(const Matrix & matrix_A, double alpha,  const Vector & vektor_x, double beta, Vector & vektor_y);

    void matrix_vector_multiplikation(const CRS_Matrix & matrix_A, double alpha,  const Vector & vektor_x, double beta, Vector & vektor_y);
};


Operations_MKL::matrix_vector_multiplikation(const Matrix & matrix_A, double alpha,  const Vector & vektor_x, double beta, Vector & vektor_y) {
    cblas_dgemv(CblasRowMajor,  // Row-major storage
            CblasNoTrans,    // No transpose (A * x)
            static_cast<int>(matrix_A._num_rows), static_cast<int>(matrix_A._num_cols),            // Matrix dimensions (M=3, N=3)
            alpha,           // Scalar alpha
            matrix_A._data.data(), static_cast<int>(matrix_A._num_rows),     // Matrix A and leading dimension
            vektor_x._data.data(), 1,     // Vector x and increment
            beta,            // Scalar beta
            vektor_y._data.data(), 1);    // Vector y and increment
}

Operations_MKL::matrix_vector_multiplikation(const CRS_Matrix & matrix_A, double alpha,  const Vector & vektor_x, double beta, Vector & vektor_y) {
    mkl_dcsrmv("T", &m, &n, &alpha, "G", values, column_indices, row_ptr, x, &beta, y);
}

#endif
#endif