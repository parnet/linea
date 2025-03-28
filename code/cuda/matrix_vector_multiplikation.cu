#include <iostream>
#include <cuda_runtime.h>
#include "operations_cuda_kernel.hpp"


// CUDA-kernel för matris-vektor-multiplikation
__global__ void cu_matrix_vektor_multiplication_kernel_small (int M, int N, double alpha, const double *A, const double *x, double beta, double *y) {
    unsigned int row = blockIdx.x * blockDim.x + threadIdx.x;
    if (row < M) {
        double sum = 0.0;
        for (int j = 0; j < N; ++j) {
            sum += A[row * N + j] * x[j];
        }
        y[row] = alpha * sum + beta * y[row];
    }
}

#define BLOCK_SIZE 256

__global__ void cu_matrix_vektor_multiplication_kernel(int M, int N, double alpha, const double *A, const double *x, double beta, double *y) {
    __shared__ double x_shared[BLOCK_SIZE];  // Shared memory för x

    int row = blockIdx.x * blockDim.x + threadIdx.x;
    double sum = 0.0;

    for (int tile = 0; tile < (N + BLOCK_SIZE - 1) / BLOCK_SIZE; ++tile) {
        // Läs in en del av x i shared memory
        int col = tile * BLOCK_SIZE + threadIdx.x;
        if (col < N) x_shared[threadIdx.x] = x[col];
        __syncthreads();

        // Beräkna delsumma
        if (row < M) {
            for (int j = 0; j < BLOCK_SIZE && (tile * BLOCK_SIZE + j) < N; ++j) {
                sum += A[row * N + (tile * BLOCK_SIZE + j)] * x_shared[j];
            }
        }
        __syncthreads();
    }

    if (row < M) y[row] = alpha * sum + beta * y[row];
}


void cu_matrix_vektor_multiplication(int M, int N, double alpha, const double *A, const double *x, double beta, double *y) {
    double *d_A, *d_x, *d_y;
    cudaMalloc((void**)&d_A, M * N * sizeof(double));
    cudaMalloc((void**)&d_x, N * sizeof(double));
    cudaMalloc((void**)&d_y, M * sizeof(double));

    cudaMemcpy(d_A, A, M * N * sizeof(double), cudaMemcpyHostToDevice);
    cudaMemcpy(d_x, x, N * sizeof(double), cudaMemcpyHostToDevice);
    cudaMemcpy(d_y, y, M * sizeof(double), cudaMemcpyHostToDevice);

    int blockSize = BLOCK_SIZE;  // 256 eller 512
    int gridSize = (M + blockSize - 1) / blockSize;
    cu_matrix_vektor_multiplication_kernel<<<gridSize, blockSize>>>(M, N, alpha, d_A, d_x, beta, d_y);

    cudaMemcpy(y, d_y, M * sizeof(double), cudaMemcpyDeviceToHost);

    cudaFree(d_A);
    cudaFree(d_x);
    cudaFree(d_y);

}
