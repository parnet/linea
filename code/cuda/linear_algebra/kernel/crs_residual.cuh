#pragma once
#ifdef USE_CUDA
#include "cuda/data/crs_matrix.hpp"
#include "cuda/data/vector.hpp"

__global__ void cu_crs_residual_kernel(
        double* __restrict__ defect_values,
        int num_rows,
        const int* __restrict__ matrix_row_ptr,
        const int* __restrict__ matrix_col_idx,
        const double* __restrict__ matrix_a_values,
        const double* __restrict__ x_values,
        const double* __restrict__ rhs_values
    );



void cu_crs_residual_launch(
    double* defect_values,
    int num_rows,
    const int* matrix_row_ptr,
    const int* matrix_col_idx,
    const double* matrix_a_values,
    const double* x_values,
    const double* rhs_values,
    cudaStream_t stream = nullptr);



inline void crs_residual(CUDA_Vector& d, const CUDA_CRS_Matrix& A, const CUDA_Vector& x, const CUDA_Vector& rhs,cudaStream_t stream = nullptr) {
    cu_crs_residual_launch(
                d.data,
                d._num_rows,
                A.d_row_ptr,
                A.d_col_idx,
                A.data,
                x.data,
                rhs.data,
                stream);
}

#endif