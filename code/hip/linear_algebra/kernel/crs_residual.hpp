#ifdef USE_HIP

#pragma once

#include <hip/hip_runtime.h>

#include "hip/data/crs_matrix.hpp"
#include "hip/data/vector.hpp"

__global__ void hip_crs_residual_kernel(
        double* __restrict__ defect_values,
        int num_rows,
        const int* __restrict__ matrix_row_ptr,
        const int* __restrict__ matrix_col_idx,
        const double* __restrict__ matrix_a_values,
        const double* __restrict__ x_values,
        const double* __restrict__ rhs_values
    );



void hip_crs_residual_launch(
    double* defect_values,
    int num_rows,
    const int* matrix_row_ptr,
    const int* matrix_col_idx,
    const double* matrix_a_values,
    const double* x_values,
    const double* rhs_values,
    hipStream_t stream = nullptr);



inline void hip_crs_residual(HIP_Vector& d, const HIP_CRS_Matrix& A, const HIP_Vector& x, const HIP_Vector& rhs,hipStream_t stream = nullptr) {
    hip_crs_residual_launch(
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