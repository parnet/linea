#ifndef OPERATIONS_CUDA_KERNEL_HPP
#define OPERATIONS_CUDA_KERNEL_HPP
#include "cuda/matrix_vector_multiplikation.cuh"
#include "data/matrix.hpp"
#include "data/vector.hpp"


struct  Operations_CUDA_Kernel {
    static constexpr const char * name ="CUDA_Kernel";
    static void matrix_vector_multiplikation(const Matrix &matrix_A, double alpha, const Vector &vektor_x, double beta, Vector &vektor_y);
};


inline void Operations_CUDA_Kernel::matrix_vector_multiplikation(const Matrix &matrix_A, double alpha, const Vector &vektor_x, double beta, Vector &vektor_y) {

    cu_matrix_vektor_multiplication(
        matrix_A._num_cols,
        matrix_A._num_rows,
        alpha,
        matrix_A._data.data(),
        vektor_x._data.data(),
        beta,
        vektor_y._data.data()
        );
}
#endif
