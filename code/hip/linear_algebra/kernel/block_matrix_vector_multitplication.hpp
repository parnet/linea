#ifdef USE_HIP
#pragma once
__global__ void cu_matrix_vektor_multiplication_kernel(int M, int N, double alpha, const double *A, const double *x, double beta, double *y);

#endif