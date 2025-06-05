
#include "jacobi_step.hpp"
#include "hip/definitions.hpp"

__global__ void hip_jacobi_step_kernel(
    double* __restrict__ x_new,
    const double* __restrict__ x_old,
    const double* __restrict__ omega_D_inv,// omega * D^{-1} precomputed
    const double* __restrict__ defect,
    int N)
{
    unsigned int row = blockIdx.x * blockDim.x + threadIdx.x;
    if (row >= N) return;

    x_new[row] = x_old[row] + (omega_D_inv[row]*defect[row]);
}


void hip_jacobi_step_launch(
    double* x_new,
    const double* x_old,
    const double* omega_D_inv,
    const double* defect,
    int num_rows,
    hipStream_t stream  // default: stream 0 (default stream)
) {
    constexpr int blockSize = HIP_BLOCK_SIZE;
    const int gridSize = (num_rows + blockSize - 1) / blockSize;

    hip_jacobi_step_kernel<<<gridSize, blockSize, 0, stream>>>(x_new, x_old, omega_D_inv, defect, num_rows);
}
