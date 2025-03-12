#ifndef OPENMP_CSRMV_HPP
#define OPENMP_CSRMV_HPP
#include "../data/std_crs_matrix.hpp"
#include "../data/std_vector.hpp"

// y = alpha * A * x + beta * y
void MatrixVectorMultiplikation(const StdCRSMatrix & matrix_A, double alpha,  const StdVector & vektor_x, double beta, StdVector & vektor_y) {

    const size_t r_size = matrix_A._num_rows;

#pragma omp parallel for
    for (size_t i = 0; i < r_size; i++) {
        vektor_y[i] *= beta;
        for (size_t j = matrix_A._row_ptr[i]; j < matrix_A._row_ptr[i + 1]; j++) {
            size_t col = matrix_A._col_index[j];
            double val = matrix_A._data[j];
            vektor_y[i] += alpha * val * vektor_x[col];
        }
    }
}
#endif
