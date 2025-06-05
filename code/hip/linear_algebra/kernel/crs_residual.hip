#include <hip/hip_runtime.h>
#include "crs_residual.hpp"
#include "hip/definitions.hpp"

__global__ void hip_crs_residual_kernel(
        double* __restrict__ defect_values,
    int num_rows,
    const int* __restrict__ matrix_row_ptr,
    const int* __restrict__ matrix_col_idx,
    const double* __restrict__ matrix_a_values,
    const double* __restrict__ x_values,
    const double* __restrict__ rhs_values
    )
{
    const unsigned int row = blockIdx.x * blockDim.x + threadIdx.x;
    if (row >= num_rows) return;

    double Ax = 0.0;
    const int row_start = matrix_row_ptr[row];
    const int row_end   = matrix_row_ptr[row + 1];

    for (int i = row_start; i < row_end; ++i) { // only few iterations for sparse matrix
        const int col = matrix_col_idx[i];
        Ax += matrix_a_values[i] * x_values[col];
    }

    defect_values[row] = rhs_values[row] - Ax;
}



void hip_crs_residual_launch(
    double* defect_values,
    int num_rows,
    const int* matrix_row_ptr,
    const int* matrix_col_idx,
    const double* matrix_a_values,
    const double* x_values,
    const double* rhs_values,
    hipStream_t stream)
{
    int threadsPerBlock = HIP_BLOCK_SIZE;
    int blocksPerGrid = (num_rows + threadsPerBlock - 1) / threadsPerBlock;

    hip_crs_residual_kernel<<<blocksPerGrid, threadsPerBlock,0,stream>>>(defect_values,num_rows, matrix_row_ptr, matrix_col_idx, matrix_a_values, x_values, rhs_values);

    hipError_t err = hipGetLastError();
    if (err != hipSuccess) {
        printf("HIP kernel launch error: %s\n", hipGetErrorString(err));
    }

}