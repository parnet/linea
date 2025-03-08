#ifndef CPU_CSRMV_HPP
#define CPU_CSRMV_HPP
#include "../data/std_crs_matrix.hpp"
#include "../data/std_vector.hpp"

void MatrixVectorMultiplikation(const StdMatrix & matrix_A, double alpha,  const StdVector & vektor_x, double beta, StdVector & vektor_y) {

    const size_t vector_size = vektor_y.size();
    for (size_t i = 0; i < vector_size; i++) {
        vektor_y(i) *= beta;
    }


    for (size_t i = 0; i < matrix_A._num_rows; i++) {
        for (size_t j = 0; j < matrix_A._num_cols; j++) {
            double val = matrix_A._data[i][j];
            vektor_y(i) += alpha * val * vektor_x(j);
        }
    }
}


void matVecProduct(const StdMatrix & matrix_A, double alpha,  const StdVector & vektor_x, double beta, StdVector & vektor_y) {
    const size_t rows = matrix_A._num_rows;
    const size_t cols = matrix_A._num_cols;

    for (size_t i = 0; i < rows; i++) {
        double temp = beta * vektor_y(i);
        for (size_t j = 0; j < cols; j++) {
            double val = matrix_A._data[i][j];
            temp += alpha * val * vektor_x(j);
        }
        vektor_y(i) = temp;
    }
}


// y = alpha * A * x + beta * y
void MatrixVectorMultiplikation(const StdCRSMatrix & matrix_A, double alpha,  const StdVector & vektor_x, double beta, StdVector & vektor_y) {

    const size_t vector_size = vektor_y.size();
    for (size_t i = 0; i < vector_size; i++) {
        vektor_y(i) *= beta;
    }

    for (size_t i = 0; i < vector_size; i++) {
        for (size_t j = matrix_A._row_ptr[i]; j < matrix_A._row_ptr[i + 1]; j++) {
            size_t col = matrix_A._col_index[j];
            double val = matrix_A._data[j];
            vektor_y(i) += alpha * val * vektor_x(col);
        }
    }
}

#endif
