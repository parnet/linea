#ifdef USE_HIP
#pragma once
#include <hip/hip_runtime_api.h>

#include "hip/data/vector.hpp"

#ifdef USE_HIP
__global__ void hip_sum_of_squares_kernel(const double* x, double* block_sums, int N);

void hip_sum_of_squares_launcher(const double* x, double* block_sums, int N, hipStream_t stream);

double hip_sum_of_squares(const HIP_Vector & x, hipStream_t stream);
#endif
#endif