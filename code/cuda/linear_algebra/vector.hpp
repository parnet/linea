#pragma once
#include <cuda_runtime.h>
#include "cuda/data/vector.hpp"
#include "kernel/jacobi_step.cuh"

double sum_of_squares(const CUDA_Vector & x, cudaStream_t stream);



inline void cu_jacobi_step(CUDA_Vector &x_new,  CUDA_Vector &x0,CUDA_Vector &omegaDinv,CUDA_Vector &defect,  cudaStream_t stream = nullptr) {
    cu_jacobi_step_launch(x_new.data,
        x0.data,
        omegaDinv.data,
        defect.data,
        x_new._num_rows,
        stream);
};
