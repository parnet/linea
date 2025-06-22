#pragma once

/*
#include <iostream>
#include <hipsparse/hipsparse.h>
#include <hipsparse/hipblas.h>
#include <util/timer.hpp>

#include "data/crs_matrix.hpp"
#include "data/vector.hpp"

#include "generator/extract_diag.hpp"
#include "generator/std_crs_matrix.hpp"
#include "generator/std_vector.hpp"


void compute_defect(
    int m, int n, int nnz,
    const int* csrRowPtrA,
    const int* csrColIndA,
    const float* csrValA,
    const float* x,
    const float* r,
    float* d)
{
    cusparseHandle_t spHandle;
    cusparseCreate(&spHandle);

    // Create matrix and vector descriptors
    cusparseSpMatDescr_t matA;
    cusparseDnVecDescr_t vecX, vecAx;

    cusparseCreateCsr(&matA, m, n, nnz,
        (void*)csrRowPtrA, (void*)csrColIndA, (void*)csrValA,
        CUSPARSE_INDEX_32I, CUSPARSE_INDEX_32I,
        CUSPARSE_INDEX_BASE_ZERO, CUDA_R_32F);

    cusparseCreateDnVec(&vecX, n, (void*)x, CUDA_R_32F);
    cusparseCreateDnVec(&vecAx, m, (void*)d, CUDA_R_32F); // d will hold A * x

    float alpha = 1.0f, beta = 0.0f;
    void* dBuffer = nullptr;
    size_t bufferSize = 0;

    cusparseSpMV_bufferSize(
        spHandle, CUSPARSE_OPERATION_NON_TRANSPOSE,
        &alpha, matA, vecX, &beta, vecAx,
        CUDA_R_32F, CUSPARSE_MV_ALG_DEFAULT, &bufferSize);

    cudaMalloc(&dBuffer, bufferSize);

    cusparseSpMV(
        spHandle, CUSPARSE_OPERATION_NON_TRANSPOSE,
        &alpha, matA, vecX, &beta, vecAx,
        CUDA_R_32F, CUSPARSE_MV_ALG_DEFAULT, dBuffer);

    // d = A * x at this point

    // Subtract: d = r - d
    // Use cublas: d = r - d => d = (-1)*d + 1*r
    cublasHandle_t blasHandle;
    cublasCreate(&blasHandle);

    float minus1 = -1.0f;
    float plus1 = 1.0f;
    // d = r - d
    // d = (-1) * d + 1 * r
    cublasSscal(blasHandle, m, &minus1, d, 1);        // d = -d
    cublasSaxpy(blasHandle, m, &plus1, r, 1, d, 1);   // d = r + (-d)

    // Done. d = r - A * x

    // Cleanup
    cusparseDestroySpMat(matA);
    cusparseDestroyDnVec(vecX);
    cusparseDestroyDnVec(vecAx);
    cusparseDestroy(spHandle);
    cublasDestroy(blasHandle);
    cudaFree(dBuffer);
}

inline void vec_print(const char * name, Vector & vec, int Nx,int  Ny) {
    std::cout << name << std::endl;
    for ( int i = 0; i < Nx; i++ ) {
        for ( int j = 0; j < Ny; j++ ) {
            std::cout << vec._data[i*Nx+j] << ",";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

inline void crs_residual_omp_simd(
    double* defect_values,
    const int num_rows,
    const int* matrix_row_ptr,
    const int* matrix_col_idx,
    const double* matrix_a_values,
    const double* x_values,
    const double* rhs_values
) {
#pragma omp parallel for
    for (int row = 0; row < num_rows; ++row) {
        double Ax = 0.0;
        const int row_start = matrix_row_ptr[row];
        const int row_end = matrix_row_ptr[row + 1];

#pragma omp simd reduction(+:Ax)
        for (int i = row_start; i < row_end; ++i) {
            const int col = matrix_col_idx[i];
            Ax += matrix_a_values[i] * x_values[col];
        }

        defect_values[row] = rhs_values[row] - Ax;
    }
}


inline double sum_of_squares(Vector& x) {
    double sum = 0.0;
    const int num = x._num_elements;
#pragma omp parallel for simd reduction(+:sum)
    for (size_t i = 0; i < num; ++i) {
        sum += x._data[i] * x._data[i];
    }
    return sum;
}

inline void jacobi_step(Vector &x_new,  Vector &x0, Vector &omegaDinv, Vector &defect) {
    const int N = x0._num_elements;

#pragma omp parallel for simd
    for (int i = 0; i < N; ++i) {
        x_new._data[i] = x0._data[i] + omegaDinv._data[i] * defect._data[i];
    }
}

inline void hipsparse_heat_equation(int Nx, int Ny, double t0, double tn, int Nt) {
    Timer walltime = Timer();
    walltime.start();

    std::cout << "heat_equation" <<std::endl;
    std::cout << "Nx="<< Nx << " Ny=" << Ny << " t0="<<t0<< " tn="<< tn <<" Nt="<<Nt << std::endl;
    double alpha = 1.0;
    CRS_Matrix laplace_matrix = create_laplacian_2d(Nx);

    Vector laplace_diag = Vector(laplace_matrix._num_rows);
    extract_diagonal(laplace_diag, laplace_matrix);
    Vector x0 = Vector(Nx*Nx);
    create_zero(x0);

    Vector rhs = Vector(Nx*Nx);
    laplace_rhs_function(rhs,Nx,Nx,alpha);

    for ( int i = 0; i < Nx; i++ ) {
        int j = Nx - 1;
        rhs[i*Nx+j] = 0.0;
        rhs[j*Nx+i] = 0.0;
    }

    Vector defect = Vector(Nx*Nx);

    double norm = 0.0;

    constexpr int max_iteration = 10'000;
    auto normer = std::vector<double>(max_iteration, 0.0);
    Vector x_current = Vector(Nx*Nx);

    Timer timer;
    timer.start();
    for (int k = 0 ; k < max_iteration; k++){
        if (k% 50 == 49){
            std::cout<< std::endl << k << std::endl;
        }
        crs_residual_omp_simd(defect._data.data(),defect._num_elements, laplace_matrix._row_ptr.data() ,laplace_matrix._col_index.data(),laplace_matrix._data.data(), x0._data.data(), rhs._data.data());

        norm = sum_of_squares(defect);
        normer[k] =  norm;

        jacobi_step(x_current, x0, laplace_diag,defect);
        x0.swap(x_current);

    }
    timer.stop();
    std::cout << "time=" << timer.get() <<" [s]"<< std::endl;
    std::cout << normer[max_iteration-1] << std::endl;
    walltime.stop();
    std::cout << "walltime=" << walltime.get() <<" [s]"<< std::endl;
}

*/