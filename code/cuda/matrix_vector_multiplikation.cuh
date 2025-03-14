#ifndef MATRIX_VECTOR_MULTIPLIKATION_CUH
#define MATRIX_VECTOR_MULTIPLIKATION_CUH

void cu_matrix_vektor_multiplication(int M, int N, double alpha, const double *A, const double *x, double beta, double *y);

#endif //MATRIX_VECTOR_MULTIPLIKATION_CUH
