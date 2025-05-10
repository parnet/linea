
#ifndef CUDA_AXPY_H
#define CUDA_AXPY_H

#include <iostream>
#include <vector>
#include <cuda_runtime.h>

#define N 5   // Number of rows in the matrix
#define NNZ 4 // Number of non-zero elements in the matrix

// CUDA error check
#define CHECK_CUDA(call)                                                        \
    {                                                                           \
        cudaError_t err = call;                                                 \
        if (err != cudaSuccess) {                                               \
            std::cerr << "CUDA error at " << __FILE__ << ":" << __LINE__        \
                      << " code=" << err << " \"" << cudaGetErrorString(err)    \
                      << "\"\n";                                                \
            exit(1);                                                            \
        }                                                                       \
    }

// Kernel for AXPY operation on sparse matrix
__global__ void sparse_axpy(int n, float a, const float *x, float *y,
                            const int *row_ptr, const int *col_idx, const float *values) {
    int row = blockIdx.x * blockDim.x + threadIdx.x;
    if (row < n) {
        float sum = 0.0;
        for (int idx = row_ptr[row]; idx < row_ptr[row + 1]; ++idx) {
            sum += values[idx] * x[col_idx[idx]];
        }
        y[row] = a * sum + y[row];
    }
}

int example_axpy_cuda() {
    // Host data
    float a = 2.0f;
    float h_x[N] = {1.0, 2.0, 3.0, 4.0, 5.0};
    float h_y[N] = {5.0, 4.0, 3.0, 2.0, 1.0};

    // Sparse matrix in CSR format (5x5 matrix with 4 non-zero elements)
    int h_row_ptr[N + 1] = {0, 1, 2, 3, 4, 4}; // Row pointers
    int h_col_idx[NNZ] = {0, 1, 2, 3};         // Column indices
    float h_values[NNZ] = {1.0, 2.0, 3.0, 4.0}; // Non-zero values

    // Device data
    float *d_x, *d_y, *d_values;
    int *d_row_ptr, *d_col_idx;

    // Allocate device memory
    CHECK_CUDA(cudaMalloc((void **)&d_x, N * sizeof(float)));
    CHECK_CUDA(cudaMalloc((void **)&d_y, N * sizeof(float)));
    CHECK_CUDA(cudaMalloc((void **)&d_row_ptr, (N + 1) * sizeof(int)));
    CHECK_CUDA(cudaMalloc((void **)&d_col_idx, NNZ * sizeof(int)));
    CHECK_CUDA(cudaMalloc((void **)&d_values, NNZ * sizeof(float)));

    // Copy data to device
    CHECK_CUDA(cudaMemcpy(d_x, h_x, N * sizeof(float), cudaMemcpyHostToDevice));
    CHECK_CUDA(cudaMemcpy(d_y, h_y, N * sizeof(float), cudaMemcpyHostToDevice));
    CHECK_CUDA(cudaMemcpy(d_row_ptr, h_row_ptr, (N + 1) * sizeof(int), cudaMemcpyHostToDevice));
    CHECK_CUDA(cudaMemcpy(d_col_idx, h_col_idx, NNZ * sizeof(int), cudaMemcpyHostToDevice));
    CHECK_CUDA(cudaMemcpy(d_values, h_values, NNZ * sizeof(float), cudaMemcpyHostToDevice));

    // Launch kernel
    int blockSize = 256;
    int gridSize = (N + blockSize - 1) / blockSize;
    sparse_axpy<<<gridSize, blockSize>>>(N, a, d_x, d_y, d_row_ptr, d_col_idx, d_values);
    CHECK_CUDA(cudaDeviceSynchronize());

    // Copy result back to host
    CHECK_CUDA(cudaMemcpy(h_y, d_y, N * sizeof(float), cudaMemcpyDeviceToHost));

    // Print the result
    std::cout << "Result y: ";
    for (int i = 0; i < N; i++) {
        std::cout << h_y[i] << " ";
    }
    std::cout << std::endl;

    // Free device memory
    cudaFree(d_x);
    cudaFree(d_y);
    cudaFree(d_row_ptr);
    cudaFree(d_col_idx);
    cudaFree(d_values);

    return 0;
}
#endif //CUDA_AXPY_H
