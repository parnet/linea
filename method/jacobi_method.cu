#ifndef JACOBI_STEP_HPP
#define JACOBI_STEP_HPP

#include <cuda_runtime.h>
#include <iostream>

__global__ void cu_jacobi_prepare_diag_kernel(int N, double* D, float omega) {
    unsigned int row = blockIdx.x * blockDim.x + threadIdx.x;
    if (row >= N) {
        return;
    }
    D[row] = omega / D[row];
}

__global__ void cu_vector_inverse_kernel(int N, double* D) {
    unsigned int row = blockIdx.x * blockDim.x + threadIdx.x;
    if (row >= N) {
        return;
    }
    D[row] = 1.0 / D[row];
}

__global__ void cu_jacobi_step_kernel(
    int N,
    const double* __restrict__ d,
    const double* __restrict__ D,
    const double* __restrict__ x_old,
    double* __restrict__ x_new,
    double omega,
    cudaStream_t stream = nullptr)
{
    unsigned int row = blockIdx.x * blockDim.x + threadIdx.x;
    if (row >= N) return;

    x_new[row] = x_old[row] + omega * (d[row] / D[row]);
}

__global__ void cu_jacobi_step_kernel(
    double* __restrict__ x_new,
    int N,
    const double* __restrict__ x_old,
    const double* __restrict__ omega_D_inv,// omega * D^{-1} precomputed
    const double* __restrict__ defect
    )
{
    unsigned int row = blockIdx.x * blockDim.x + threadIdx.x;
    if (row >= N) return;

    x_new[row] = x_old[row] + (defect[row] * omega_D_inv[row]);
}

void cu_jacobi_step_launch(
    double* x_new,
    int num_rows,
    const double* x_old,
    const double* omega_D_inv,
    const double* defect,
    cudaStream_t stream = nullptr  // default: stream 0 (default stream)
) {
    constexpr int blockSize = 256;
    const int gridSize = (num_rows + blockSize - 1) / blockSize;

    cu_jacobi_step_kernel<<<gridSize, blockSize, 0, stream>>>(x_new, num_rows, x_old, omega_D_inv, defect);
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
        // Beräkna A*x_old för rad i
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


#endif
