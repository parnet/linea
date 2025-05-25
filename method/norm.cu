
#include <cuda_runtime.h>
#include <iostream>


__global__ void cu_sum_of_squares_kernel(const double* x, double* block_sums, int N) {
    extern __shared__ double sdata[];



    int tid = threadIdx.x;
    int i = blockIdx.x * blockDim.x + tid;

    // Ladda in element eller 0 om utanför
    double val = (i < N) ? x[i] * x[i] : 0.0;
    sdata[tid] = val;
    __syncthreads();

    // Parallell reduktion i delad minne
#pragma unroll
    for (unsigned int s = blockDim.x / 2; s > 0; s >>= 1) {
        if (tid < s) {
            sdata[tid] += sdata[tid + s];
        }
        __syncthreads();
    }

    // Skriv ut blockets summa i globalt minne
    if (tid == 0) {
        block_sums[blockIdx.x] = sdata[0];
    }
}

void cu_sum_of_squares_launcher(const double* x, double* block_sums, int N, cudaStream_t stream =nullptr) {
    int threadsPerBlock = 256;
    int blocksPerGrid = (N + threadsPerBlock - 1) / threadsPerBlock;
    size_t sharedMemSize = threadsPerBlock * sizeof(double);
    //std::cout << blocksPerGrid << std::endl;
    //std::cout << threadsPerBlock << std::endl;
    //std::cout << sharedMemSize << std::endl;

    double* d_block_sums;
    cudaMalloc(&d_block_sums, blocksPerGrid * sizeof(double));

    cu_sum_of_squares_kernel<<<blocksPerGrid, threadsPerBlock,sharedMemSize, stream>>>(x, d_block_sums, N);
    cudaError_t err = cudaGetLastError();
    if (err != cudaSuccess) {
        printf("Kernel launch error: %s\n", cudaGetErrorString(err));
    }
    cudaStreamSynchronize(stream);
    cudaMemcpy(block_sums, d_block_sums, blocksPerGrid * sizeof(double), cudaMemcpyDeviceToHost);

    cudaFree(d_block_sums);

}

