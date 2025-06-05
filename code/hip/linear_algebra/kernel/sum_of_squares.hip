#include <hip/hip_runtime.h>

#include <iostream>
#include "sum_of_squares.hpp"
#include "hip/definitions.hpp"

__global__ void hip_sum_of_squares_kernel(const double* x, double* block_sums, int N) {
    extern __shared__ double sdata[];

    int tid = threadIdx.x;
    int i = blockIdx.x * blockDim.x + tid;

    double val = (i < N) ? x[i] * x[i] : 0.0;
    sdata[tid] = val;
    __syncthreads();

    // Parallell reduction in shared memory
#pragma unroll
    for (unsigned int s = blockDim.x / 2; s > 0; s >>= 1) {
        if (tid < s) {
            sdata[tid] += sdata[tid + s];
        }
        __syncthreads();
    }

    // write to global memory
    if (tid == 0) {
        block_sums[blockIdx.x] = sdata[0];
    }
}







void hip_sum_of_squares_launcher(const double* x, double* block_sums, int N, hipStream_t stream =nullptr) {
    int threadsPerBlock = HIP_BLOCK_SIZE;
    int blocksPerGrid = (N + threadsPerBlock - 1) / threadsPerBlock;
    size_t sharedMemSize = threadsPerBlock * sizeof(double);

    double* d_block_sums;
    hipMalloc(&d_block_sums, blocksPerGrid * sizeof(double));

    hip_sum_of_squares_kernel<<<blocksPerGrid, threadsPerBlock,sharedMemSize, stream>>>(x, d_block_sums, N);
    hipError_t err = hipGetLastError();
    if (err != hipSuccess) {
        printf("Kernel launch error: %s\n", hipGetErrorString(err));
    }

    hipStreamSynchronize(stream);
    hipMemcpy(block_sums, d_block_sums, blocksPerGrid * sizeof(double), hipMemcpyDeviceToHost);

    hipFree(d_block_sums);
}



double hip_sum_of_squares(const HIP_Vector & x, hipStream_t stream) {
    constexpr int blockSize = HIP_BLOCK_SIZE;
    const int N = x._num_rows;
    const int gridSize = (N + blockSize - 1) / blockSize;
    auto* h_block_sums = new double[gridSize];

    hip_sum_of_squares_launcher(x.data, h_block_sums, x._num_rows, stream);

    double sum = 0.0;
    for (int i = 0; i < gridSize; i++) {
        sum += h_block_sums[i];
    }
    delete[] h_block_sums;
    return sum;
}