
#include <cuda_runtime.h>
#include <iostream>

__global__ void cu_compute_residual_crs_kernel(
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



void cu_compute_residual_crs_launch(
    double* defect_values,
    int num_rows,
    const int* matrix_row_ptr,
    const int* matrix_col_idx,
    const double* matrix_a_values,
    const double* x_values,
    const double* rhs_values,
    cudaStream_t stream = nullptr)
{
    int threadsPerBlock = 256;
    int blocksPerGrid = (num_rows + threadsPerBlock - 1) / threadsPerBlock;

    cu_compute_residual_crs_kernel<<<blocksPerGrid, threadsPerBlock,0,stream>>>(defect_values,num_rows, matrix_row_ptr, matrix_col_idx, matrix_a_values, x_values, rhs_values);

    cudaError_t err = cudaGetLastError();
    if (err != cudaSuccess) {
        printf("CUDA kernel launch error: %s\n", cudaGetErrorString(err));
    }
}
