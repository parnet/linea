
#ifdef USE_CUDA

#include "vector.hpp"

#include "cuda/definitions.hpp"

#include "kernel/sum_of_squares.cuh"


double sum_of_squares(const CUDA_Vector & x, cudaStream_t stream) {
    constexpr int blockSize = CUDA_BLOCK_SIZE;
    const int N = x._num_rows;
    const int gridSize = (N + blockSize - 1) / blockSize;
    auto* h_block_sums = new double[gridSize];

    cu_sum_of_squares_launcher(x.data, h_block_sums, x._num_rows, stream);

    double sum = 0.0;
    for (int i = 0; i < gridSize; i++) {
        sum += h_block_sums[i];
    }
    delete[] h_block_sums;
    return sum;
}
#endif