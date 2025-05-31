#ifndef CUDA_LINEAR_ALGEBRA_CRS_MATRIX_HPP
#define CUDA_LINEAR_ALGEBRA_CRS_MATRIX_HPP

#include "cuda_datastructures.hpp"
#include "../cuda/jacobi_method.cuh"


#include <cuda_runtime.h>
#include <iostream>


#include "cuda_datastructures.hpp"
#include "data/crs_matrix.hpp"


#include <cuda_runtime.h>
#include <iostream>




__global__ void cu_compute_residual_crs_kernel(
    int num_rows,
    const int* __restrict__ row_ptr,
    const int* __restrict__ col_idx,
    const double* __restrict__ values,
    const double* __restrict__ x,
    const double* __restrict__ b,
    double* __restrict__ d);

// CPU-launcher




__global__ void cu_vector_inverse_kernel(int N, double* D) {
    unsigned int row = blockIdx.x * blockDim.x + threadIdx.x;
    if (row >= N) {
        return;
    }
    D[row] = 1.0 / D[row];
}

__global__ void cu_jacobi_step_kernel(
    double* __restrict__ x_new,
    const double* __restrict__ x_old,
    const double* __restrict__ defect,
    const double* __restrict__ diagonal_A,
    double omega,
    int N)
{
    unsigned int row = blockIdx.x * blockDim.x + threadIdx.x;
    if (row >= N) return;

    x_new[row] = x_old[row] + omega * (defect[row] / diagonal_A[row]);
}





__global__ void cu_jacobi_step_full_kernel(
    double* __restrict__ x_new,
    int N,
    const int* __restrict__ A_rowPtr,
    const int* __restrict__ A_colIndex,
    const double* __restrict__ A_values,
    const double* __restrict__ b,
    const double* __restrict__ D,
    const double* __restrict__ x_old,

    double omega,
    cudaStream_t stream = nullptr)
{
    unsigned int row = blockIdx.x * blockDim.x + threadIdx.x;
    if (row < N) {
        double Ax_i = 0.0f;
        int row_start = A_rowPtr[row];
        int row_end = A_rowPtr[row+1];
        for (int idx = row_start; idx < row_end; idx++) {
            int col = A_colIndex[idx];
            Ax_i += A_values[idx] * x_old[col];
        }

        double d_i = b[row] - Ax_i;
        x_new[row] = x_old[row] + omega * (d_i / D[row]);
    }
}

__global__ void cu_jacobi_step_full_kernel(
    int N,
    const int* __restrict__ A_rowPtr,
    const int* __restrict__ A_colIndex,
    const double* __restrict__ A_values,
    const double* __restrict__ b,
    const double* __restrict__ omega_D_inv, // omega * D ^{-1}
    const double* __restrict__ x_old,
    double* __restrict__ x_new,
    cudaStream_t stream = nullptr)
{
    unsigned int row = blockIdx.x * blockDim.x + threadIdx.x;
    if (row < N) {
        // Beräkna A*x_old för rad i
        double Ax_i = 0.0f;
        int row_start = A_rowPtr[row];
        int row_end = A_rowPtr[row+1];
        for (int idx = row_start; idx < row_end; idx++) {
            int col = A_colIndex[idx];
            Ax_i += A_values[idx] * x_old[col];
        }

        double d_i = b[row] - Ax_i;
        x_new[row] = x_old[row] + d_i * omega_D_inv[row];
    }
}

void cu_jacobi_step_full_launch(
    int N,
    const int* A_rowPtr,
    const int* A_colIndex,
    const double* A_values,
    const double* b,
    const double* omega_D_inv,
    const double* x_old,
    double* x_new,
    cudaStream_t stream = nullptr  // valfri CUDA-stream, default = nullptr (default stream)
) {
    constexpr int BLOCK_SIZE = 256;
    int grid_size = (N + BLOCK_SIZE - 1) / BLOCK_SIZE;

    cu_jacobi_step_full_kernel<<<grid_size, BLOCK_SIZE, 0, stream>>>(
        N,
        A_rowPtr,
        A_colIndex,
        A_values,
        b,
        omega_D_inv,
        x_old,
        x_new
    );
    cudaError_t err = cudaGetLastError();
    if (err != cudaSuccess) {
        printf("Kernel launch error: %s\n", cudaGetErrorString(err));
    }
    cudaDeviceSynchronize();
}

void cu_jacobi_step_full(CUDA_Vector & x_new, CUDA_Vector & x_old,CUDA_CRS_Matrix &A, CUDA_Vector& D, CUDA_Vector &rhs) {
    cu_jacobi_step_full_launch(
    x_new._num_rows,
    A.d_row_ptr,
    A.d_col_idx,
    A.data,
    rhs.data,
    D.data,
    x_old.data,
    x_new.data,
    nullptr);
}




void cu_jacobi_prepare_diag_kernel(int N, double* D, float omega) ;


void cu_vector_inverse_kernel(int N, double* D);




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
