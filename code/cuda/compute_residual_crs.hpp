#ifndef COMPUTE_RESIDUAL_CRS_HPP
#define COMPUTE_RESIDUAL_CRS_HPP
#include "cuda_datastructures.hpp"
#include "data/crs_matrix.hpp"

__global__ void cu_compute_residual_crs_kernel(
    int num_rows,
    const int* __restrict__ row_ptr,
    const int* __restrict__ col_idx,
    const double* __restrict__ values,
    const double* __restrict__ x,
    const double* __restrict__ b,
    double* __restrict__ d);

// CPU-launcher
void cu_compute_residual_crs_launch(
double* defect_values,
    int num_rows,
    const int* matrix_row_ptr,
    const int* matrix_col_idx,
    const double* matrix_a_values,
    const double* x_values,
    const double* rhs_values,
    cudaStream_t stream = nullptr);


void compute_residual_crs(CUDA_Vector& d, const CUDA_CRS_Matrix& A, const CUDA_Vector& x, const CUDA_Vector& rhs,cudaStream_t stream = nullptr) {
    cu_compute_residual_crs_launch(
                d.data,
                d._num_rows,
                A.d_row_ptr,
                A.d_col_idx,
                A.data,
                x.data,
                rhs.data,
                stream);
}

#endif //COMPUTE_RESIDUAL_CRS_HPP
