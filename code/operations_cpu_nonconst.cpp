#include "operations_cpu_nonconst.hpp"



void Operations_CPU_NonConst::matrix_vector_multiplikation(const Matrix &matrix_A, double alpha, const Vector &vektor_x,
                                                         double beta, Vector &vektor_y) {

    for (size_t i = 0; i < vektor_y._num_elements; i++) {
        vektor_y._data[i] *= beta;
    }

    for (size_t i = 0; i < matrix_A._num_rows; i++) {
        for (size_t j = 0; j < matrix_A._num_cols; j++) {
            double val = matrix_A._data[i*matrix_A._num_cols+j];
            vektor_y._data[i] += alpha * val * vektor_x._data[j];
        }
    }
}
