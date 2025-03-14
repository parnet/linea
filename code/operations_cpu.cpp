
#include "operations_cpu.hpp"

void Operations_CPU::matrix_vector_multiplikation(const Matrix & matrix_A, double alpha,  const Vector & vektor_x, double beta, Vector & vektor_y) {
    const size_t vector_size = vektor_y.size();
    for (size_t i = 0; i < vector_size; i++) {
        vektor_y._data[i] *= beta;
    }
    const size_t row_size = matrix_A._num_rows;
    const size_t col_size = matrix_A._num_cols;
    for (size_t i = 0; i < row_size; i++) {
        for (size_t j = 0; j < col_size ; j++) {
            double val = matrix_A._data[i*matrix_A._num_cols+j];
            vektor_y._data[i] += alpha * val * vektor_x._data[j];
        }
    }

}

void Operations_CPU::matrix_vector_multiplikation(const CRS_Matrix &matrix_A, double alpha, const Vector &vektor_x,
double beta, Vector &vektor_y)  {

    const size_t vector_size = vektor_y.size();
    for (size_t i = 0; i < vector_size; i++) {
        vektor_y._data[i] *= beta;
    }

    for (size_t i = 0; i < vector_size; i++) {
        for (size_t j = matrix_A._row_ptr[i]; j < matrix_A._row_ptr[i + 1]; j++) {
            size_t col = matrix_A._col_index[j];
            double val = matrix_A._data[j];
            vektor_y._data[i] += alpha * val * vektor_x._data[col];
        }
    }
}

double Operations_CPU::norm(const Vector &vector_x) {
    double sum = 0.0;
    const size_t v_size = vector_x.size();
    for(size_t i = 0; i < v_size; ++i) {
        sum += vector_x[i] * vector_x[i];
    }
    return sum;
}

double Operations_CPU::dot_product(const Vector &vector_x, const Vector &vector_y)  {
    double sum = 0.0;
    const size_t v_size = vector_x.size();
    for(size_t i = 0; i < v_size; ++i) {
        sum += vector_x._data[i]* vector_y._data[i];
    }
    return sum;
}