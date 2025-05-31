#include "cuda/definitions.hpp"
#include "matrix_vector_multiplication.cuh"

#include "block_matrix_vector_multitplication.cuh"


// y=alpha*A*x + beta*y
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


void cu_matrix_vektor_multiplication(int M, int N, double alpha, const double *A, const double *x, double beta, double *y) {
    double *d_A, *d_x, *d_y;
    cudaMalloc((void**)&d_A, M * N * sizeof(double));
    cudaMalloc((void**)&d_x, N * sizeof(double));
    cudaMalloc((void**)&d_y, M * sizeof(double));

    cudaMemcpy(d_A, A, M * N * sizeof(double), cudaMemcpyHostToDevice);
    cudaMemcpy(d_x, x, N * sizeof(double), cudaMemcpyHostToDevice);
    cudaMemcpy(d_y, y, M * sizeof(double), cudaMemcpyHostToDevice);

    constexpr int blockSize = CUDA_BLOCK_SIZE;  // 256 eller 512
    int gridSize = (M + blockSize - 1) / blockSize;
    cu_matrix_vektor_multiplication_kernel<<<gridSize, blockSize>>>(M, N, alpha, d_A, d_x, beta, d_y);

    cudaMemcpy(y, d_y, M * sizeof(double), cudaMemcpyDeviceToHost);

    cudaFree(d_A);
    cudaFree(d_x);
    cudaFree(d_y);

}