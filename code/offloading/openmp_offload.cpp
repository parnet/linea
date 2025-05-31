
#include "openmp_offload.hpp"

void operation(int N, const double * matrix_A, double alpha,  const double *  vektor_x, double beta, double * vektor_y) {
#pragma omp target teams distribute parallel for map(to: matrix_A[0:N*N], vektor_x[0:N]) map(from: vektor_y[0:N])
    for (int i = 0; i < N; i++) {
        double sum = 0.0;
        for (int j = 0; j < N; j++) {
            sum += matrix_A[i * N + j] * vektor_x[j];
        }
        vektor_y[i] = sum;
    }
}


void Operations_OpenMP_Offload::matrix_vector_multiplikation(const Matrix & matrix_A, double alpha,  const Vector & vektor_x, double beta, Vector & vektor_y) {
    operation(matrix_A._num_rows, matrix_A._data.data(), alpha, vektor_x._data.data(), beta, vektor_y._data.data());
}


