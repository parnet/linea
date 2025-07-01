#pragma once
#include "data/vector.hpp"
#include "data/matrix.hpp"


struct  Operations_CUDA {
    static constexpr const char * name ="CUDA_Kernel";

    static void matrix_vector_multiplikation(const Matrix &matrix_A, double alpha, const Vector &vektor_x, double beta, Vector &vektor_y);

    static void norm();
};



inline void Operations_CUDA::matrix_vector_multiplikation(const Matrix &matrix_A, double alpha, const Vector &vektor_x, double beta, Vector &vektor_y) {
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
