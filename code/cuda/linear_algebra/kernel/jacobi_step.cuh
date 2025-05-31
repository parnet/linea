#pragma once

__global__ void cu_jacobi_step_kernel(
    double*  x_new,
    const double*  x_old,
    const double*  omega_D_inv, // omega * D^{-1} precomputed
    const double*  d,
    int N
    );

void cu_jacobi_step_launch(
    double*  x_new,
    const double*  x_old,
    const double*  omegaDinv,
    const double*  defect,
    int N,
    cudaStream_t stream = nullptr
    );
