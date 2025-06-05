#pragma once
#include <hip/hip_runtime.h>
#include "hip/data/vector.hpp"

__global__ void hip_jacobi_step_kernel(
    double*  x_new,
    const double*  x_old,
    const double*  omega_D_inv, // omega * D^{-1} precomputed
    const double*  d,
    int N
    );

void hip_jacobi_step_launch(
    double*  x_new,
    const double*  x_old,
    const double*  omegaDinv,
    const double*  defect,
    int N,
    hipStream_t stream = nullptr
    );

inline void hip_jacobi_step(HIP_Vector &x_new,
    HIP_Vector &x0,
    HIP_Vector &omegaDinv,
    HIP_Vector &defect,
    hipStream_t stream = nullptr) {
    hip_jacobi_step_launch(x_new.data,
        x0.data,
        omegaDinv.data,
        defect.data,
        x_new._num_rows,
        stream);
};
