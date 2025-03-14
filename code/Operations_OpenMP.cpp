
#include "libs/openmp.hpp"
#include "Operations_OpenMP.hpp"

#ifdef USE_OPENMP
void Operations_OpenMP::matrix_vector_multiplikation(const Matrix & matrix_A, double alpha,  const Vector & vektor_x, double beta, Vector & vektor_y) {
    const size_t mr = matrix_A._num_rows;
    const size_t mc = matrix_A._num_cols;

#pragma omp parallel for simd
    for(size_t i = 0 ; i < mr; ++i) {
        vektor_y._data[i] *= beta;
    }

#pragma omp parallel for
    for (size_t i = 0; i < mr; ++i) {

#pragma omp simd
        for (size_t j = 0; j < mc; j++) {
            double val = matrix_A._data[i*matrix_A._num_cols+j];
            vektor_y._data[i] += alpha * val * vektor_x._data[j];
        }
    }
}


// y = alpha * A * x + beta * y
void Operations_OpenMP::matrix_vector_multiplikation(const CRS_Matrix & matrix_A, double alpha,  const Vector & vektor_x, double beta, Vector & vektor_y) {

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

double Operations_OpenMP::dot_product(const Vector &vector_x, const Vector &vector_y) {

    const size_t v_size = vector_x.size();
    double sum = 0.0;

#pragma omp parallel for reduction(+:sum)
    for(size_t i = 0; i < v_size; ++i) {
        sum += vector_x._data[i] * vector_y._data[i];
    }
    return sum;
}

double Operations_OpenMP::norm(const Vector &vector_x) {

    const size_t v_size = vector_x.size();
    double sum = 0.0;

#pragma omp parallel for reduction(+:sum)
    for(size_t i = 0; i < v_size; ++i) {
        sum += vector_x._data[i] * vector_x._data[i];
    }
    return sum;
}
#endif