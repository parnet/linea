
#ifndef JACOBI_METHOD_HPP
#define JACOBI_METHOD_HPP

#include "cuda_datastructures.hpp"
#include "../cuda/jacobi_method.cuh"


void cu_jacobi_step_full(CUDA_Vector & x_new, CUDA_Vector & x_old,CUDA_CRS_Matrix &A, CUDA_Vector& D, CUDA_Vector &rhs) {
    cu_jacobi_step_full_launch(
    x_new._num_rows,
    A.d_row_ptr,
    A.d_col_idx,
    A.data,
    rhs.data,
    D.data,
    x_old.data,
    x_new.data,
    nullptr);
}

void cu_jacobi_step(CUDA_Vector &x_new,  CUDA_Vector &x0,CUDA_Vector &omegaDinv,CUDA_Vector &defect,  cudaStream_t stream = nullptr) {
    cu_jacobi_step_launch(x_new.data,
        x0.data,
        omegaDinv.data,
        defect.data,
        x_new._num_rows,
        stream);
}

#endif //JACOBI_METHOD_HPP
