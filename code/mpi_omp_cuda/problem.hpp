#pragma once
#include <cuda_runtime_api.h>
#include <iostream>
#include <util/timer.hpp>
#include <cmath>
#include <cstring>
#include <iomanip>
#include <vector>

#include "host/host.hpp"
#include "kernel/bridge.hpp"
#include "util/converter.hpp"


inline void vec_print(const char * name, Vector & vec, int Nx,int  Ny) {
    std::cout << name << std::endl;
    for ( int i = 0; i < Nx; i++ ) {
        for ( int j = 0; j < Ny; j++ ) {
            std::cout << vec._values[i*Nx+j] << ",";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}



inline void problem_dist_seq(int Nx, int Ny) {

    // const int max_iteration = 10'000;
    const int max_iteration = 10'000;

    int mpi_size = 4;

    Timer walltime;
    walltime.start();

    double alpha = 1.0;


    CRS_Matrix laplace_matrix_0 = create_laplacian_2d(Nx, Ny+1);
    CRS_Matrix laplace_matrix_1 = create_laplacian_2d(Nx, Ny+2); // additional boundary for overlap
    CRS_Matrix laplace_matrix_2 = create_laplacian_2d(Nx, Ny+2); // additional boundary for overlap
    CRS_Matrix laplace_matrix_3 = create_laplacian_2d(Nx, Ny+1);



    /*
    std::cout << "laplacian_2d" << std::endl;
    for ( int i = 0; i < laplace_matrix_0._rows_pointer.size()-1; i++ ) {
        int start = laplace_matrix_0._rows_pointer[i];
        int stop = laplace_matrix_0._rows_pointer[i+1];
        std::cout << "row=" <<  start << ",";

        for ( int j = start; j < stop; j++ ) {
            std::cout << laplace_matrix_0._col_index[j] << " => " << laplace_matrix_0._values[j] << std::endl << ",";
        }
        std::cout << std::endl;
    }*/

    //std::vector<std::vector<double>> dense_matrix_0;
    //std::vector<std::vector<double>> dense_matrix_1;
    //std::vector<std::vector<double>> dense_matrix_2;
    //std::vector<std::vector<double>> dense_matrix_3;


    Timer transfer_timer = Timer();
    transfer_timer.stop();

    auto cu_laplace_matrix_0 = X_CRS_Matrix(laplace_matrix_0);
    auto cu_laplace_matrix_1 = X_CRS_Matrix(laplace_matrix_1);
    auto cu_laplace_matrix_2 = X_CRS_Matrix(laplace_matrix_2);
    auto cu_laplace_matrix_3 = X_CRS_Matrix(laplace_matrix_3);

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

    auto cu_laplace_diag_0 = X_Vector(laplace_diag_0);
    auto cu_laplace_diag_1 = X_Vector(laplace_diag_1);
    auto cu_laplace_diag_2 = X_Vector(laplace_diag_2);
    auto cu_laplace_diag_3 = X_Vector(laplace_diag_3);

    auto x0_0 = Vector(Nx*(Ny+1));
    auto x0_1 = Vector(Nx*(Ny+2));
    auto x0_2 = Vector(Nx*(Ny+2));
    auto x0_3 = Vector(Nx*(Ny+1));

    create_zero(x0_0);
    create_zero(x0_1);
    create_zero(x0_2);
    create_zero(x0_3);

    auto cu_x0_0 = X_Vector(x0_0);
    auto cu_x0_1 = X_Vector(x0_1);
    auto cu_x0_2 = X_Vector(x0_2);
    auto cu_x0_3 = X_Vector(x0_3);

    cudaStreamSynchronize(cu_x0_0.stream);
    cudaStreamSynchronize(cu_x0_1.stream);
    cudaStreamSynchronize(cu_x0_2.stream);
    cudaStreamSynchronize(cu_x0_3.stream);

    //    vec_print("x0",x0,  Nx, Ny);

    auto rhs_0 = Vector(Nx*(Ny+1));
    auto rhs_1 = Vector(Nx*(Ny+2));
    auto rhs_2 = Vector(Nx*(Ny+2));
    auto rhs_3 = Vector(Nx*(Ny+1));

    laplace_rhs_function(rhs_0,Nx,Ny+1,alpha, 0);
    laplace_rhs_function(rhs_1,Nx,Ny+2,alpha, Ny-1);
    laplace_rhs_function(rhs_2,Nx,Ny+2,alpha, 2*Ny-1);
    laplace_rhs_function(rhs_3,Nx,Ny+1,alpha, 3*Ny-1);


    for ( int i = 0; i < Nx; i++ ) {
        rhs_0._values[          i] = 0.0; // first row
        rhs_3._values[(Ny)*Nx+i] = 0.0; // last row
    }
    for ( int i = 0; i < Ny; i++ ) {

        rhs_0._values[i*Nx+     0] = 0.0; // first row
        rhs_0._values[i*Nx+(Nx-1)] = 0.0; // last row


        rhs_3._values[i*Nx+     0] = 0.0; // first row
        rhs_3._values[i*Nx+(Nx-1)] = 0.0; // last row


    }
    for ( int i = 0; i < Ny+1; i++ ) {
        rhs_1._values[i*Nx+     0] = 0.0; // first row
        rhs_1._values[i*Nx+(Nx-1)] = 0.0; // last row

        rhs_2._values[i*Nx+     0] = 0.0; // first row
        rhs_2._values[i*Nx+(Nx-1)] = 0.0; // last row

    }


    /*std::cout << "rhs_1=" << std::endl;
    for (int i = 0; i < rhs_3._values.size(); i++ ) {
        std::cout << rhs_3._values[i] << std::endl;
    }*/

    auto cu_rhs_0 = X_Vector(rhs_0);
    auto cu_rhs_1 = X_Vector(rhs_1);
    auto cu_rhs_2 = X_Vector(rhs_2);
    auto cu_rhs_3 = X_Vector(rhs_3);

    cudaStreamSynchronize(cu_rhs_0.stream);
    cudaStreamSynchronize(cu_rhs_1.stream);
    cudaStreamSynchronize(cu_rhs_2.stream);
    cudaStreamSynchronize(cu_rhs_3.stream);

    auto defect_0 = Vector(Nx*(Ny+1));
    auto defect_1 = Vector(Nx*(Ny+2));
    auto defect_2 = Vector(Nx*(Ny+2));
    auto defect_3 = Vector(Nx*(Ny+1));


    //compute_residual_crs(cu_defect, cu_laplace_matrix, cu_x0, cu_rhs);

    X_Vector cu_x_current_0(cu_x0_0._num_rows);
    X_Vector cu_x_current_1(cu_x0_1._num_rows);
    X_Vector cu_x_current_2(cu_x0_2._num_rows);
    X_Vector cu_x_current_3(cu_x0_3._num_rows);

    X_Vector cu_defect_0(cu_x0_0._num_rows);
    X_Vector cu_defect_1(cu_x0_1._num_rows);
    X_Vector cu_defect_2(cu_x0_2._num_rows);
    X_Vector cu_defect_3(cu_x0_3._num_rows);

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


    auto normer = std::vector<double>(max_iteration, 0.0);

    Timer timer;
    timer.start();

    double * buffer_01; //= (double*)malloc(sizeof(double) * Nx);
    cudaMalloc((void**)&buffer_01, sizeof(double) * Nx);

    double * buffer_10; //= (double*)malloc(sizeof(double) * Nx);
    cudaMalloc((void**)&buffer_10, sizeof(double) * Nx);
    double * buffer_12; //= (double*)malloc(sizeof(double) * Nx);
    cudaMalloc((void**)&buffer_12, sizeof(double) * Nx);

    double * buffer_21; //= (double*)malloc(sizeof(double) * Nx);
    cudaMalloc((void**)&buffer_21, sizeof(double) * Nx);
    double * buffer_23; //= (double*)malloc(sizeof(double) * Nx);
    cudaMalloc((void**)&buffer_23, sizeof(double) * Nx);

    double * buffer_32; //= (double*)malloc(sizeof(double) * Nx);
    cudaMalloc((void**)&buffer_32, sizeof(double) * Nx);

    for (int k = 0 ; k < max_iteration; k++){
        x_crs_residual(cu_defect_0, cu_laplace_matrix_0, cu_x0_0, cu_rhs_0, stream_0);
        x_crs_residual(cu_defect_1, cu_laplace_matrix_1, cu_x0_1, cu_rhs_1, stream_1);
        x_crs_residual(cu_defect_2, cu_laplace_matrix_2, cu_x0_2, cu_rhs_2, stream_2);
        x_crs_residual(cu_defect_3, cu_laplace_matrix_3, cu_x0_3, cu_rhs_3, stream_3);

        x_set_row_zero(cu_defect_0, Nx, 0, stream_0);
        x_set_row_zero(cu_defect_0, Nx, Ny, stream_0);

        x_set_row_zero(cu_defect_1, Nx, 0, stream_1);
        x_set_row_zero(cu_defect_1, Nx, Ny+1, stream_1);

        x_set_row_zero(cu_defect_2, Nx, 0, stream_2);
        x_set_row_zero(cu_defect_2, Nx, Ny+1, stream_2);

        x_set_row_zero(cu_defect_3, Nx, 0, stream_3);
        x_set_row_zero(cu_defect_3, Nx, Ny, stream_3);

        cudaStreamSynchronize(stream_0);
        cudaStreamSynchronize(stream_1);
        cudaStreamSynchronize(stream_2);
        cudaStreamSynchronize(stream_3);

        /*std::cout << "v=" << std::endl;
        auto v = cu_defect_0.get_vector();
        for (int i = 0; i < v._values.size(); i++) {
            std::cout << v._values[i] << std::endl;

        }
        return;*/

        norm_0 = x_parallel_norm(cu_defect_0, stream_0);
        norm_1 = x_parallel_norm(cu_defect_1, stream_1);
        norm_2 = x_parallel_norm(cu_defect_2, stream_2);
        norm_3 = x_parallel_norm(cu_defect_3, stream_3);

        double t_norm =  sqrt(norm_0 + norm_1 + norm_2 + norm_3);
        std::cout << "nrm_0 = " << norm_0 << std::endl;
        std::cout << "nrm_1 = " << norm_1 << std::endl;
        std::cout << "nrm_2 = " << norm_2 << std::endl;
        std::cout << "nrm_3 = " << norm_3 << std::endl;

        std::cout << "\t\t\t\t\t" << std::setprecision(15) << t_norm << std::endl;
        normer[k] = t_norm;

        x_jacobi_step(cu_x_current_0, cu_x0_0, cu_laplace_diag_0, cu_defect_0, stream_0);
        x_jacobi_step(cu_x_current_1, cu_x0_1, cu_laplace_diag_1, cu_defect_1, stream_1);
        x_jacobi_step(cu_x_current_2, cu_x0_2, cu_laplace_diag_2, cu_defect_2, stream_2);
        x_jacobi_step(cu_x_current_3, cu_x0_3, cu_laplace_diag_3, cu_defect_3, stream_3);


        cudaMemcpyAsync(buffer_01, cu_x_current_0._values+Nx*Ny, Nx*sizeof(double), cudaMemcpyDeviceToDevice, stream_0);
        cudaMemcpyAsync(buffer_10, cu_x_current_1._values, Nx*sizeof(double), cudaMemcpyDeviceToDevice, stream_1);
        cudaMemcpyAsync(buffer_12, cu_x_current_1._values+Nx*(Ny+1), Nx*sizeof(double), cudaMemcpyDeviceToDevice, stream_1);
        cudaMemcpyAsync(buffer_21, cu_x_current_2._values, Nx*sizeof(double), cudaMemcpyDeviceToDevice, stream_2);
        cudaMemcpyAsync(buffer_23, cu_x_current_2._values+Nx*(Ny+1), Nx*sizeof(double), cudaMemcpyDeviceToDevice, stream_2);
        cudaMemcpyAsync(buffer_32, cu_x_current_3._values, Nx*sizeof(double), cudaMemcpyDeviceToDevice, stream_3);

        cudaDeviceSynchronize();

        cudaMemcpyAsync(cu_x_current_0._values+Nx*Ny, buffer_10,Nx*sizeof(double), cudaMemcpyDeviceToDevice, stream_0);
        cudaMemcpyAsync(cu_x_current_1._values, buffer_01,Nx*sizeof(double), cudaMemcpyDeviceToDevice, stream_1);
        cudaMemcpyAsync(cu_x_current_1._values+Nx*(Ny+1), buffer_21,Nx*sizeof(double), cudaMemcpyDeviceToDevice, stream_1);
        cudaMemcpyAsync(cu_x_current_2._values, buffer_12,Nx*sizeof(double), cudaMemcpyDeviceToDevice, stream_2);
        cudaMemcpyAsync(cu_x_current_2._values+Nx*(Ny+1), buffer_32,Nx*sizeof(double), cudaMemcpyDeviceToDevice, stream_2);
        cudaMemcpyAsync(cu_x_current_3._values, buffer_23,Nx*sizeof(double), cudaMemcpyDeviceToDevice, stream_3);
        cudaDeviceSynchronize();

        cu_x0_0.swap(cu_x_current_0);
        cu_x0_1.swap(cu_x_current_1);
        cu_x0_2.swap(cu_x_current_2);
        cu_x0_3.swap(cu_x_current_3);

    }

    timer.stop();
    walltime.stop();


    std::cout << "time=" << timer.get() <<" [s]"<< std::endl;
    std::cout << "walltime=" << walltime.get() <<" [s]"<< std::endl;
    std::cout << "time_per_iter=" << walltime.get() / max_iteration <<" [s]"<< std::endl;

    std::cout << "A=" << convert(laplace_matrix_0.memory_size(),Mega) <<" ["<<symbol(Mega)<<"]"<< std::endl;
    std::cout << "A=" << convert(laplace_matrix_1.memory_size(),Mega) <<" ["<<symbol(Mega)<<"]"<< std::endl;
    std::cout << "A=" << convert(laplace_matrix_2.memory_size(),Mega) <<" ["<<symbol(Mega)<<"]"<< std::endl;
    std::cout << "A=" << convert(laplace_matrix_3.memory_size(),Mega) <<" ["<<symbol(Mega)<<"]"<< std::endl;

    std::cout << "D=" << convert(laplace_diag_0.memory_size(),Mega) <<" ["<<symbol(Mega)<<"]"<< std::endl;
    std::cout << "D=" << convert(laplace_diag_1.memory_size(),Mega) <<" ["<<symbol(Mega)<<"]"<< std::endl;
    std::cout << "D=" << convert(laplace_diag_2.memory_size(),Mega) <<" ["<<symbol(Mega)<<"]"<< std::endl;
    std::cout << "D=" << convert(laplace_diag_3.memory_size(),Mega) <<" ["<<symbol(Mega)<<"]"<< std::endl;

    std::cout << "x0=" << convert(x0_0.memory_size(),Mega) <<" ["<<symbol(Mega)<<"]"<< std::endl;
    std::cout << "x0=" << convert(x0_1.memory_size(),Mega) <<" ["<<symbol(Mega)<<"]"<< std::endl;
    std::cout << "x0=" << convert(x0_2.memory_size(),Mega) <<" ["<<symbol(Mega)<<"]"<< std::endl;
    std::cout << "x0=" << convert(x0_3.memory_size(),Mega) <<" ["<<symbol(Mega)<<"]"<< std::endl;

    std::cout << "rhs=" << convert(rhs_0.memory_size(),Mega) <<" ["<<symbol(Mega)<<"]"<< std::endl;
    std::cout << "rhs=" << convert(rhs_1.memory_size(),Mega) <<" ["<<symbol(Mega)<<"]"<< std::endl;
    std::cout << "rhs=" << convert(rhs_2.memory_size(),Mega) <<" ["<<symbol(Mega)<<"]"<< std::endl;
    std::cout << "rhs=" << convert(rhs_3.memory_size(),Mega) <<" ["<<symbol(Mega)<<"]"<< std::endl;
    std::cout << normer[max_iteration-1] << std::endl;
}
