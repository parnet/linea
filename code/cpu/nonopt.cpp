
#include "nonopt.hpp"



#pragma GCC optimize ("no-tree-vectorize")
void Operations_Nonopt::matrix_vector_multiplikation(const Matrix & matrix_A, double alpha,  const Vector & vektor_x, double beta, Vector & vektor_y) {
        const size_t vector_size = vektor_y.size();
#pragma clang loop vectorize(disable)
        for (size_t i = 0; i < vector_size; i++) {
            vektor_y._data[i] *= beta;
        }

#pragma clang loop vectorize(disable)
        for (size_t i = 0; i < matrix_A._num_rows; i++) {
#pragma clang loop vectorize(disable)
            for (size_t j = 0; j < matrix_A._num_cols; j++) {
                double val = matrix_A._data[i*matrix_A._num_cols+j];
                vektor_y._data[i] += alpha * val * vektor_x._data[j];
            }
        }
    }
#pragma GCC optimize ("tree-vectorize")

