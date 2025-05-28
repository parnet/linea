//
// Created by maro on 2025-05-23.
//

#ifndef JACOBI_METHOD_CUH
#define JACOBI_METHOD_CUH

void cu_jacobi_prepare_diag_kernel(int N, double* D, float omega) ;


void cu_vector_inverse_kernel(int N, double* D);

void cu_jacobi_step_kernel(
    int N,
    const double*  d,
    const double*  D,
    const double*  x_old,
    double*  x_new,
    double omega);

void cu_jacobi_step_kernel(
    int N,
    const double*  d,
    const double*  omega_D_inv, // omega * D^{-1} precomputed
    const double*  x_old,
    double*  x_new);

void cu_jacobi_step_full_kernel(
    double*  x_new,
    int N,
    const int*  A_rowPtr,
    const int*  A_colIndex,
    const double*  A_values,
    const double*  b,
    const double*  D,
    const double*  x_old,
    double omega);

void cu_jacobi_step_full_kernel(
    int N,
    const int*  A_rowPtr,
    const int*  A_colIndex,
    const double*  A_values,
    const double*  b,
    const double*  omega_D_inv, // omega * D ^{-1}
    const double*  x_old,
    double*  x_new);





void cu_jacobi_prepare_diag_launch(int N, double* D, float omega) ;


void cu_vector_inverse_launch(int N, double* D);

void cu_jacobi_step_launch(
    double*  x_new,
    const double*  x_old,
    double omega,
    const double*  D,
    const double*  d,
    int N);

void cu_jacobi_step_launch(
    double*  x_new,
    const double*  x_old,
    const double*  omega_D_inv, // omega * D^{-1} precomputed
    const double*  d,
    int num_rows,
    cudaStream_t stream
    );

void cu_jacobi_step_full_launch(
    double*  x_new,
    int N,
    const int*  A_rowPtr,
    const int*  A_colIndex,
    const double*  A_values,
    const double*  b,
    const double*  D,
    const double*  x_old,
    double omega);

void cu_jacobi_step_full_launch (
    int N,
    const int* A_rowPtr,
    const int* A_colIndex,
    const double* A_values,
    const double* b,
    const double* omega_D_inv,
    const double* x_old,
    double* x_new,
    cudaStream_t stream = nullptr  // valfri CUDA-stream, default = nullptr (default stream)
);
#endif