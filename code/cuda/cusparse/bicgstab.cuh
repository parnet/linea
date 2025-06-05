#pragma once

#ifdef USE_CUDA

#include <cuda_runtime.h>
#include <cublas_v2.h>

#include <cusparse.h>

#include "data/crs_matrix.hpp"

int orig_main();


int gpu_BiCGStab(cublasHandle_t       cublasHandle,
                 cusparseHandle_t     cusparseHandle,
                 int                  m,
                 cusparseSpMatDescr_t matA,
                 cusparseSpMatDescr_t matM_lower,
                 cusparseSpMatDescr_t matM_upper,
                 double*                  d_B,
                 double*                  d_X,
                 double*                  d_R0,
                 double*                  d_R,
                 double*                  d_P,
                 double*                  d_P_aux,
                 double*                  d_S,
                 double*                  d_S_aux,
                 double*                  d_V,
                 double*                  d_T,
                 double*                  d_tmp,
                 void*                d_bufferMV,
                 int                  maxIterations,
                 double               tolerance);
int test_main(CRS_Matrix mat);

#endif