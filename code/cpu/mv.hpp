#pragma once

#include "../data/vector.hpp"
#include "../data/c_vector.hpp"
#include "../data/c_matrix.hpp"
#include "../data/std_matrix.hpp"


/**
 * Matrix Vektor Multiplication
 *      $$y = alpha * A * x + beta * y$$
 * for a Naive Matrix and a Naive Vector (double*)
 * without cache optimization
 */
void MatrixVectorMultiplikation(const C_Matrix & matrix_A, double alpha,  const C_Vector & vektor_x, double beta, C_Vector & vektor_y) {
    const size_t vector_size = vektor_y.size();
    for (size_t i = 0; i < vector_size; i++) {
        vektor_y._data[i] *= beta;
    }

    const size_t nr = matrix_A._num_rows;
    const size_t nc = matrix_A._num_cols;
    for (size_t i = 0; i < nr; i++) {
        for (size_t j = 0; j < nc; j++) {
            double val = matrix_A._data[i*nc+j];
            vektor_y._data[i] += alpha * val * vektor_x._data[j];
        }
    }
}

/**
 * Matrix Vektor Multiplication
 *      $$y = alpha * A * x + beta * y$$
 * for a Naive Matrix and a Naive Vector (double*)
 * with slight cache optimization
 */
void MatrixVectorMultiplikationCache(const C_Matrix & matrix_A, double alpha,  const C_Vector & vektor_x, double beta, C_Vector & vektor_y) {

    const size_t nr = matrix_A._num_rows;
    const size_t nc = matrix_A._num_cols;
    for (size_t i = 0; i < nr; i++) {
        vektor_y._data[i] *= beta;
        for (size_t j = 0; j < nc; j++) {
            double val = matrix_A._data[i*nc+j];
            vektor_y._data[i] += alpha * val * vektor_x._data[j];
        }
    }
}

/**
 * Matrix Vektor Multiplication
 *      $$y = alpha * A * x + beta * y$$
 * for a Naive Matrix and a Standard Vector std::linear_algebra<double>
 * without cache optimization
 */
void MatrixVectorMultiplikation(const C_Matrix & matrix_A, double alpha,  const Vector & vektor_x, double beta, Vector & vektor_y) {
    const size_t vector_size = vektor_y.size();
    for (size_t i = 0; i < vector_size; i++) {
        vektor_y._data[i] *= beta;
    }

    size_t nr = matrix_A._num_rows;
    size_t nc = matrix_A._num_cols;
    for (size_t i = 0; i < nr; i++) {
        const size_t ni = i*nc;
        for (size_t j = 0; j < nc; j++) {
            double val = matrix_A._data[ni+j];
            vektor_y._data[i] += alpha * val * vektor_x._data[j];
        }
    }
}







void MatrixVectorMultiplikation(const Std_Matrix & matrix_A, double alpha,  const Vector & vektor_x, double beta, Vector & vektor_y) {
    const size_t vector_size = vektor_y.size();
    for (size_t i = 0; i < vector_size; i++) {
        vektor_y._data[i] *= beta;
    }


    for (size_t i = 0; i < matrix_A._num_rows; i++) {
        for (size_t j = 0; j < matrix_A._num_cols; j++) {
            double val = matrix_A._data[i][j];
            vektor_y._data[i] += alpha * val * vektor_x._data[j];
        }
    }
}


// -----

void MatrixVectorMultiplikationCache(const Std_Matrix & matrix_A, double alpha,  const Vector & vektor_x, double beta, Vector & vektor_y) {
    const size_t rows = matrix_A._num_rows;
    const size_t cols = matrix_A._num_cols;

    for (size_t i = 0; i < rows; i++) {
        double temp = beta * vektor_y[i];
        for (size_t j = 0; j < cols; j++) {
            double val = matrix_A._data[i][j];
            temp += alpha * val * vektor_x._data[j];
        }
        vektor_y._data[i] = temp;
    }
}

