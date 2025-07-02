
#ifndef KERNEL_CUH
#define KERNEL_CUH

void residual_crs_kernel_launch(
    double* defect_values,
    int num_rows,
    const int* matrix_row_ptr,
    const int* matrix_col_idx,
    const double* matrix_a_values,
    const double* x_values,
    const double* rhs_values,
    cudaStream_t stream = nullptr);


void zero_row_kernel_launch(
    double* d_data, int cols, int target_row,
    cudaStream_t stream = nullptr);

void parallel_norm(double &norm, double *x, int num_elem, cudaStream_t stream = nullptr);


void jacobi_step_launch(
    double*  x_new,
    const double*  x_old,
    const double*  omegaDinv,
    const double*  defect,
    int N,
    cudaStream_t stream = nullptr
    );


#endif //KERNEL_CUH
