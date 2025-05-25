#ifndef NORM_HPP
#define NORM_HPP

__device__ void cu_compute_residual_crs_kernel( const double* x, double* block_sums, int N);

// CPU-launcher
void cu_sum_of_squares_launcher( const double* x, double* block_sums, int N, cudaStream_t stream);


inline double sum_of_squares(const CUDA_Vector & x, cudaStream_t stream) {
    int blockSize = 256;
    int N = x._num_rows;
    int gridSize = (N + blockSize - 1) / blockSize;
    auto* h_block_sums = new double[gridSize];


    cu_sum_of_squares_launcher(x.data, h_block_sums, x._num_rows, stream);




    double sum = 0.0;
    for (int i = 0; i < gridSize; i++) {
        //std::cout << i << " --> " << h_block_sums[i]<<std::endl;
        sum += h_block_sums[i];
    }
    delete[] h_block_sums;
    return sum;
}
#endif //NORM_HPP
