#ifndef OPERATIONS_CUBLAS_HPP
#define OPERATIONS_CUBLAS_HPP
#include <cublas_v2.h>

#include "data/matrix.hpp"
#include "data/vector.hpp"


struct  Operations_cuBLAS {
    static constexpr const char * name ="cuBLAS";
    static void matrix_vector_multiplikation(const Matrix &matrix_A, double alpha, const Vector &vektor_x, double beta, Vector &vektor_y);
};

inline void Operations_cuBLAS::matrix_vector_multiplikation(const Matrix &matrix_A, double alpha, const Vector &vektor_x, double beta, Vector &vektor_y) {
    cublasHandle_t handle;
    cublasCreate(&handle);
    cublasDgemv_v2_64(handle, CUBLAS_OP_N,
        static_cast<int>(matrix_A._num_rows),
        static_cast<int>(matrix_A._num_cols),
        &alpha,
        matrix_A._data.data(),
        static_cast<int>(matrix_A._num_rows),
        vektor_x._data.data(),
        1,
        &beta,
        vektor_y._data.data(),
        1);
    cublasDestroy(handle);
}

#endif //OPERATIONS_CUBLAS_HPP
