#ifndef OPENMP_CSRMV_HPP
#define OPENMP_CSRMV_HPP
#include "../data/std_matrix.hpp"
#include "../data/std_vector.hpp"

void MatrixVectorMultiplikation(const StdMatrix & matrix_A, double alpha,  const StdVector & vektor_x, double beta, StdVector & vektor_y) {

    const size_t vector_size = vektor_y.size();

    #pragma omp parallel for
    for (size_t i = 0; i < vector_size; i++) {
        vektor_y(i) *= beta;
    }

    #pragma omp parallel for
    for (size_t i = 0; i < matrix_A._num_rows; i++) {
        for (size_t j = 0; j < matrix_A._num_cols; j++) {
            double val = matrix_A._data[i][j];
            vektor_y(i) += alpha * val * vektor_x(j);
        }
    }

}

#endif
