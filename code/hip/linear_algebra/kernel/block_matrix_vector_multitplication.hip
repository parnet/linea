#ifdef SKIP_COMPILATION
#include <hip/hip_runtime.h>


#include "cuda/definitions.hpp"
#include "block_matrix_vector_multitplication.hpp"

__global__ void hip_matrix_vektor_multiplication_kernel(int M, int N, double alpha, const double *A, const double *x, double beta, double *y) {
    __shared__ double x_shared[CUDA_BLOCK_SIZE];  // Shared memory för x

    int row = blockIdx.x * blockDim.x + threadIdx.x;
    double sum = 0.0;

    for (int tile = 0; tile < (N + CUDA_BLOCK_SIZE - 1) / CUDA_BLOCK_SIZE; ++tile) {
        // Läs in en del av x i shared memory
        int col = tile * CUDA_BLOCK_SIZE + threadIdx.x;
        if (col < N) x_shared[threadIdx.x] = x[col];
        __syncthreads();

        if (row < M) {
            for (int j = 0; j < CUDA_BLOCK_SIZE && (tile * CUDA_BLOCK_SIZE + j) < N; ++j) {
                sum += A[row * N + (tile * CUDA_BLOCK_SIZE + j)] * x_shared[j];
            }
        }
        __syncthreads();
    }

    if (row < M) y[row] = alpha * sum + beta * y[row];
}
#endif