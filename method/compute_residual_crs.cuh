#ifndef COMPUTE_RESIDUAL_HPP
#define COMPUTE_RESIDUAL_HPP

void cu_compute_residual_crs_kernel(int M, int N, double alpha, const double *A, const double *x, double beta, double *y);

void cu_compute_residual_crs_launcher(
    int N,
    const int* d_row_ptr,
    const int* d_col_idx,
    const double* d_values,
    const double* d_x,
    const double* d_b,
    double* d_d,
    cudaStream_t stream = nullptr
    );

#endif
