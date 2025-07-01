#pragma once
#include <cuda_runtime_api.h>
#include <iostream>
#include <util/timer.hpp>
#include <cmath>
#include <vector>

#include "host/host.hpp"
#include "kernel/bridge.hpp"


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



inline void laplacian(int Nx, int Ny, double t0, double tn, int Nt) {
    int mpi_size = 4;

    Timer walltime;
    walltime.start();

    double alpha = 1.0;


    CRS_Matrix laplace_matrix_0 = create_laplacian_2d(Nx, Ny);
    CRS_Matrix laplace_matrix_1 = create_laplacian_2d(Nx, Ny);
    CRS_Matrix laplace_matrix_2 = create_laplacian_2d(Nx, Ny);
    CRS_Matrix laplace_matrix_3 = create_laplacian_2d(Nx, Ny);


    std::vector<std::vector<double>> dense_matrix_0;
    std::vector<std::vector<double>> dense_matrix_1;
    std::vector<std::vector<double>> dense_matrix_2;
    std::vector<std::vector<double>> dense_matrix_3;


    Timer transfer_timer = Timer();
    transfer_timer.stop();

    auto cu_laplace_matrix_0 = CUDA_CRS_Matrix(laplace_matrix);
    auto cu_laplace_matrix_1 = CUDA_CRS_Matrix(laplace_matrix);
    auto cu_laplace_matrix_2 = CUDA_CRS_Matrix(laplace_matrix);
    auto cu_laplace_matrix_3 = CUDA_CRS_Matrix(laplace_matrix);

    cudaStreamSynchronize(cu_laplace_matrix_0.stream);
    cudaStreamSynchronize(cu_laplace_matrix_1.stream);
    cudaStreamSynchronize(cu_laplace_matrix_2.stream);
    cudaStreamSynchronize(cu_laplace_matrix_3.stream);

    transfer_timer.stop();
    std::cout << "transfer_time= " << transfer_timer.get() << " [s]" << std::endl;
    Vector laplace_diag_0 = Vector(laplace_matrix_0._num_rows);
    Vector laplace_diag_1 = Vector(laplace_matrix_1._num_rows);
    Vector laplace_diag_2 = Vector(laplace_matrix_2._num_rows);
    Vector laplace_diag_3 = Vector(laplace_matrix_3._num_rows);

    extract_diagonal(laplace_diag_0, laplace_matrix_0, 0.66);
    extract_diagonal(laplace_diag_1, laplace_matrix_1, 0.66);
    extract_diagonal(laplace_diag_2, laplace_matrix_2, 0.66);
    extract_diagonal(laplace_diag_3, laplace_matrix_3, 0.66);

    //vec_print("cu_laplace_diag",laplace_diag,  Nx, Ny)

    auto cu_laplace_diag_0 = CUDA_Vector(laplace_diag);
    auto cu_laplace_diag_1 = CUDA_Vector(laplace_diag);
    auto cu_laplace_diag_2 = CUDA_Vector(laplace_diag);
    auto cu_laplace_diag_3 = CUDA_Vector(laplace_diag);

    Vector x0_0 = Vector(Nx*Nx);
    Vector x0_1 = Vector(Nx*Nx);
    Vector x0_2 = Vector(Nx*Nx);
    Vector x0_3 = Vector(Nx*Nx);

    create_zero(x0_0);
    create_zero(x0_1);
    create_zero(x0_2);
    create_zero(x0_3);

    auto cu_x0_0 = CUDA_Vector(x0_0);
    auto cu_x0_1 = CUDA_Vector(x0_1);
    auto cu_x0_2 = CUDA_Vector(x0_2);
    auto cu_x0_3 = CUDA_Vector(x0_3);

    cudaStreamSynchronize(cu_x0_0.stream);
    cudaStreamSynchronize(cu_x0_1.stream);
    cudaStreamSynchronize(cu_x0_2.stream);
    cudaStreamSynchronize(cu_x0_3.stream);

    //    vec_print("x0",x0,  Nx, Ny);

    Vector rhs_0 = Vector(Nx*Nx);
    Vector rhs_1 = Vector(Nx*Nx);
    Vector rhs_2 = Vector(Nx*Nx);
    Vector rhs_3 = Vector(Nx*Nx);

    laplace_rhs_function(rhs_0,Nx,Nx,alpha);
    laplace_rhs_function(rhs_1,Nx,Nx,alpha);
    laplace_rhs_function(rhs_2,Nx,Nx,alpha);
    laplace_rhs_function(rhs_3,Nx,Nx,alpha);

    for ( int i = 0; i < Nx; i++ ) {
        int j = Nx - 1;
        rhs_0[i*Nx+j] = 0.0;
        rhs_1[i*Nx+j] = 0.0;
        rhs_2[i*Nx+j] = 0.0;
        rhs_3[i*Nx+j] = 0.0;

        rhs_0[j*Nx+i] = 0.0;
        rhs_1[j*Nx+i] = 0.0;
        rhs_2[j*Nx+i] = 0.0;
        rhs_3[j*Nx+i] = 0.0;
    }

    auto cu_rhs_0 = CUDA_Vector(rhs_0);
    auto cu_rhs_1 = CUDA_Vector(rhs_1);
    auto cu_rhs_2 = CUDA_Vector(rhs_2);
    auto cu_rhs_3 = CUDA_Vector(rhs_3);

    cudaStreamSynchronize(cu_rhs_0.stream);
    cudaStreamSynchronize(cu_rhs_1.stream);
    cudaStreamSynchronize(cu_rhs_2.stream);
    cudaStreamSynchronize(cu_rhs_3.stream);

    auto defect_0 = Vector(Nx*Nx);
    auto defect_1 = Vector(Nx*Nx);
    auto defect_2 = Vector(Nx*Nx);
    auto defect_3 = Vector(Nx*Nx);

    //compute_residual_crs(cu_defect, cu_laplace_matrix, cu_x0, cu_rhs);

    CUDA_Vector cu_x_current_0(cu_x0_0._num_rows);
    CUDA_Vector cu_x_current_1(cu_x0_1._num_rows);
    CUDA_Vector cu_x_current_2(cu_x0_2._num_rows);
    CUDA_Vector cu_x_current_3(cu_x0_3._num_rows);

    CUDA_Vector cu_defect_0(cu_x0_0._num_rows);
    CUDA_Vector cu_defect_1(cu_x0_1._num_rows);
    CUDA_Vector cu_defect_2(cu_x0_2._num_rows);
    CUDA_Vector cu_defect_3(cu_x0_3._num_rows);

    cudaStreamSynchronize(cu_x_current_0.stream);
    cudaStreamSynchronize(cu_x_current_1.stream);
    cudaStreamSynchronize(cu_x_current_2.stream);
    cudaStreamSynchronize(cu_x_current_3.stream);

    double norm_0 = 0.0;
    double norm_1 = 0.0;
    double norm_2 = 0.0;
    double norm_3 = 0.0;

    cudaStream_t stream_0;
    cudaStream_t stream_1;
    cudaStream_t stream_2;
    cudaStream_t stream_3;

    cudaStreamCreate(&stream_0);
    cudaStreamCreate(&stream_1);
    cudaStreamCreate(&stream_2);
    cudaStreamCreate(&stream_3);

    const int max_iteration = 10'000;

    auto normer = std::vector<double>(max_iteration, 0.0);

    Timer timer;
    timer.start();

    for (int k = 0 ; k < max_iteration; k++){
        crs_residual(cu_defect_0, cu_laplace_matrix_0, cu_x0_0, cu_rhs_0, stream_0);
        crs_residual(cu_defect_1, cu_laplace_matrix_1, cu_x0_1, cu_rhs_1, stream_1);
        crs_residual(cu_defect_2, cu_laplace_matrix_2, cu_x0_2, cu_rhs_2, stream_2);
        crs_residual(cu_defect_3, cu_laplace_matrix_3, cu_x0_3, cu_rhs_3, stream_3);

        norm_0 = parallel_norm(cu_defect_0, stream_0);
        norm_1 = parallel_norm(cu_defect_1, stream_1);
        norm_2 = parallel_norm(cu_defect_2, stream_2);
        norm_3 = parallel_norm(cu_defect_3, stream_3);

        normer[k] =  sqrt(norm_0 + norm_1 + norm_2 + norm_3);

        jacobi_step(cu_x_current_0, cu_x0_0, cu_laplace_diag_0, cu_defect_0, stream_0);
        jacobi_step(cu_x_current_1, cu_x0_1, cu_laplace_diag_1, cu_defect_1, stream_1);
        jacobi_step(cu_x_current_2, cu_x0_2, cu_laplace_diag_2, cu_defect_2, stream_2);
        jacobi_step(cu_x_current_3, cu_x0_3, cu_laplace_diag_3, cu_defect_3, stream_3);


        cu_x0_0.swap(cu_x_current_0);
        cu_x0_1.swap(cu_x_current_1);
        cu_x0_2.swap(cu_x_current_2);
        cu_x0_3.swap(cu_x_current_3);

    }

    timer.stop();
    walltime.stop();


    std::cout << "time=" << timer.get() <<" [s]"<< std::endl;
    std::cout << "walltime=" << walltime.get() <<" [s]"<< std::endl;

    std::cout << "A=" << convert(laplace_matrix.memory_size(),Mega) <<" ["<<symbol(Mega)<<"]"<< std::endl;
    std::cout << "D=" << convert(laplace_diag.memory_size(),Mega) <<" ["<<symbol(Mega)<<"]"<< std::endl;

    std::cout << "x0=" << convert(x0.memory_size(),Mega) <<" ["<<symbol(Mega)<<"]"<< std::endl;
    std::cout << "rhs=" << convert(rhs.memory_size(),Mega) <<" ["<<symbol(Mega)<<"]"<< std::endl;
    std::cout << normer[max_iteration-1] << std::endl;
}
