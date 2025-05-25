//
// Created by maro on 2025-05-23.
//

#ifndef NORM_CUH
#define NORM_CUH
void cu_sum_of_squares_kernel(const double* x, double* block_sums, int N);
void cu_sum_of_squares_launcher(const double* x, double* block_sums, int N, cudaStream_t stream);

#endif //NORM_CUH
