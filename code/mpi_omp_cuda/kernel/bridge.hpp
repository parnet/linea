
#include "mpi_omp_cuda/data/data.hpp"
#include "mpi_omp_cuda/kernel/kernel.cuh"

inline void x_crs_residual(X_Vector &defect, const X_CRS_Matrix &laplace_matrix, const X_Vector &x0, const X_Vector &rhs, cudaStream_t stream = nullptr) {
    residual_crs_kernel_launch(defect._values,
                defect._num_rows,
                laplace_matrix._d_row_ptr,
                laplace_matrix._d_col_idx,
                laplace_matrix._d_values,
                x0._values,
                rhs._values,
                stream);
}

inline void x_set_row_zero(X_Vector &defect, int cols, int target_row, cudaStream_t stream = nullptr) {
    zero_row_kernel_launch(defect._values, cols, target_row, stream);
}

inline double x_parallel_norm(X_Vector &vector, cudaStream_t stream = nullptr ) {
    double result = 0.0;
    parallel_norm(result, vector._values, vector._num_rows,stream);
    return result;
}


inline void x_jacobi_step(X_Vector &x_new, X_Vector &x_old, X_Vector &cu_laplace_diag_0, X_Vector &cu_defect_0, cudaStream_t stream = nullptr) {
    jacobi_step_launch(
        x_new._values,
        x_old._values,
        cu_laplace_diag_0._values,
        cu_defect_0._values,
        x_new._num_rows,
        stream
        );
}
