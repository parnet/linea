#pragma once

#include <iostream>
#include <ostream>
#include <util/timer.hpp>
#include <omp.h>

#include "cuda/data/crs_matrix.hpp"
#include "cuda/data/vector.hpp"
#include "cuda/linear_algebra/vector.hpp"


#include "generator/extract_diag.hpp"
#include "generator/std_crs_matrix.hpp"
#include "generator/std_vector.hpp"


#include "cuda/cusparse/bicgstab.cuh"
#include "cuda/linear_algebra/kernel/crs_residual.cuh"
#include "util/converter.hpp"



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

inline void cuda_heat_equation(int Nx, int Ny, double t0, double tn, int Nt) {
    Timer walltime;
    walltime.start();
    std::cout << "heat_equation" <<std::endl;
    std::cout << "Nx="<< Nx << " Ny=" << Ny << " t0="<<t0<< " tn="<< tn <<" Nt="<<Nt << std::endl;
    double alpha = 1.0;
    CRS_Matrix laplace_matrix = create_laplacian_2d(Nx);

    std::vector<std::vector<double>> dense_matrix;
    /*
    crs_to_dense(dense_matrix, laplace_matrix._data,laplace_matrix._col_index,laplace_matrix._row_ptr,laplace_matrix._num_rows,laplace_matrix._num_cols);



    std::cout << "matrix" << std::endl;
    for ( int i = 0; i < Nx*Ny; i++ ) {
        std::cout << "[";
        for ( int j = 0; j < Nx*Ny; j++ ) {
            std::cout << dense_matrix[i][j] << ", ";
        }
        std::cout <<"],"<< std::endl;
    }
    std::cout << std::endl;*/


    Timer transfer_timer = Timer();
    transfer_timer.stop();
    auto cu_laplace_matrix = CUDA_CRS_Matrix(laplace_matrix);
    cudaStreamSynchronize(cu_laplace_matrix.stream);
    transfer_timer.stop();
    std::cout << "transfer_time= " << transfer_timer.get() << " [s]" << std::endl;
    Vector laplace_diag = Vector(laplace_matrix._num_rows);
    extract_diagonal(laplace_diag, laplace_matrix, 0.66);

    //vec_print("cu_laplace_diag",laplace_diag,  Nx, Ny)

    auto cu_laplace_diag = CUDA_Vector(laplace_diag);

    Vector x0 = Vector(Nx*Nx);
    //fill_with_random(x0, 6142);
    create_zero(x0);
    auto cu_x0 = CUDA_Vector(x0);
    cudaStreamSynchronize(cu_x0.stream);

    //    vec_print("x0",x0,  Nx, Ny);

    Vector rhs = Vector(Nx*Nx);
    laplace_rhs_function(rhs,Nx,Nx,alpha);

    for ( int i = 0; i < Nx; i++ ) {
        int j = Nx - 1;
        rhs[i*Nx+j] = 0.0;
        rhs[j*Nx+i] = 0.0;
        //std::cout << std::endl;
    }

    //    vec_print("rhs",rhs,  Nx, Ny);


    auto cu_rhs = CUDA_Vector(rhs);
    cudaStreamSynchronize(cu_rhs.stream);

    Vector defect = Vector(Nx*Nx);

    //compute_residual_crs(cu_defect, cu_laplace_matrix, cu_x0, cu_rhs);

    CUDA_Vector cu_x_current(cu_x0._num_rows);
    CUDA_Vector cu_defect(cu_x0._num_rows);
    cudaStreamSynchronize(cu_x_current.stream);

    double norm = 0.0;

    cudaStream_t stream;
    cudaStreamCreate(&stream);
    const int max_iteration = 10'000;
    auto normer = std::vector<double>(max_iteration, 0.0);
    Timer timer;
    timer.start();
    for (int k = 0 ; k < max_iteration; k++){
        crs_residual(cu_defect, cu_laplace_matrix, cu_x0, cu_rhs, stream);
        //cudaStreamSynchronize(stream);
        //Vector def = cu_defect.get_vector();
        //    vec_print("defect",def,  Nx, Ny);

        /*Vector diag = cu_laplace_diag.get_vector();
        std::cout << "diag" << std::endl;
        for ( int i = 0; i < Nx; i++ ) {
            for ( int j = 0; j < Ny; j++ ) {
                std::cout << diag._data[i*Nx+j] << ",";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;*/
        norm = sum_of_squares(cu_defect, stream);
        //std::cout << norm  << ",";
        normer[k] =  norm;
        cu_jacobi_step(cu_x_current, cu_x0, cu_laplace_diag,cu_defect,  stream);
        //cudaStreamSynchronize(stream);
        cu_x0.swap(cu_x_current);

        //Vector x0 = cu_x0.get_vector();
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

inline void cuda_bicgstab_heat_equation(int Nx, int Ny, double t0, double tn, int Nt) {
    Timer walltime;
    walltime.start();
    std::cout << "heat_equation" <<std::endl;
    std::cout << "Nx="<< Nx << " Ny=" << Ny << " t0="<<t0<< " tn="<< tn <<" Nt="<<Nt << std::endl;
    double alpha = 1.0;
    CRS_Matrix laplace_matrix = create_laplacian_2d(Nx);

    std::vector<std::vector<double>> dense_matrix;
    /*
 */


    Timer transfer_timer = Timer();
    transfer_timer.stop();
    auto cu_laplace_matrix = CUDA_CRS_Matrix(laplace_matrix);
    cudaStreamSynchronize(cu_laplace_matrix.stream);
    transfer_timer.stop();
    std::cout << "transfer_time= " << transfer_timer.get() << " [s]" << std::endl;
    Vector laplace_diag = Vector(laplace_matrix._num_rows);
    extract_diagonal(laplace_diag, laplace_matrix, 0.66);

    //vec_print("cu_laplace_diag",laplace_diag,  Nx, Ny)

    auto cu_laplace_diag = CUDA_Vector(laplace_diag);

    Vector x0 = Vector(Nx*Nx);
    //fill_with_random(x0, 6142);
    create_zero(x0);
    auto cu_x0 = CUDA_Vector(x0);
    cudaStreamSynchronize(cu_x0.stream);

    //    vec_print("x0",x0,  Nx, Ny);

    Vector rhs = Vector(Nx*Nx);
    laplace_rhs_function(rhs,Nx,Nx,alpha);

    for ( int i = 0; i < Nx; i++ ) {
        int j = Nx - 1;
        rhs[i*Nx+j] = 0.0;
        rhs[j*Nx+i] = 0.0;
        //std::cout << std::endl;
    }

    //    vec_print("rhs",rhs,  Nx, Ny);


    auto cu_rhs = CUDA_Vector(rhs);
    cudaStreamSynchronize(cu_rhs.stream);

    Vector defect = Vector(Nx*Nx);

    //compute_residual_crs(cu_defect, cu_laplace_matrix, cu_x0, cu_rhs);

    CUDA_Vector cu_x_current(cu_x0._num_rows);
    CUDA_Vector cu_defect(cu_x0._num_rows);
    cudaStreamSynchronize(cu_x_current.stream);

    double norm = 0.0;

    cudaStream_t stream;
    cudaStreamCreate(&stream);
    const int max_iteration = 10'000;
    auto normer = std::vector<double>(max_iteration, 0.0);
    Timer timer;
    timer.start();
    for (int k = 0 ; k < max_iteration; k++){
        crs_residual(cu_defect, cu_laplace_matrix, cu_x0, cu_rhs, stream);
        //cudaStreamSynchronize(stream);
        //Vector def = cu_defect.get_vector();
        //    vec_print("defect",def,  Nx, Ny);

        /*Vector diag = cu_laplace_diag.get_vector();
        std::cout << "diag" << std::endl;
        for ( int i = 0; i < Nx; i++ ) {
            for ( int j = 0; j < Ny; j++ ) {
                std::cout << diag._data[i*Nx+j] << ",";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;*/
        norm = sum_of_squares(cu_defect, stream);
        //std::cout << norm  << ",";
        normer[k] =  norm;
        cu_jacobi_step(cu_x_current, cu_x0, cu_laplace_diag,cu_defect,  stream);
        //cudaStreamSynchronize(stream);
        cu_x0.swap(cu_x_current);

        //Vector x0 = cu_x0.get_vector();
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


inline  void benchmark_heat_equation() {
    //omp_set_num_threads(8);
    //heat_equation(5, 5, 0, 2.0, 512);

    cuda_heat_equation(1, 1, 0, 2.0, 512);

    //cuda_heat_equation(1024, 1024, 0, 2.0, 512);
    //cuda_heat_equation(1448, 1448, 0, 2.0, 512);
    //cuda_heat_equation(2048, 2048, 0, 2.0, 512);
    //cuda_heat_equation(2896, 2896, 0, 2.0, 512);

    //cuda_heat_equation(2048, 2048, 0, 2.0, 512);


    //std::linear_algebra<std::linear_algebra<double>> dense_matrix;
    //crs_to_dense(dense_matrix, laplace_matrix._data,laplace_matrix._col_index,laplace_matrix._row_ptr,laplace_matrix._num_rows,laplace_matrix._num_cols);
    //    std::cout << "matrix" << std::endl;
    //    for ( int i = 0; i < Nx*Nx; i++ ) {
    //        std::cout << "[";
    //        for ( int j = 0; j < Nx*Nx; j++ ) {
    //            std::cout << dense_matrix[i][j] << ", ";
    //        }
    //        std::cout <<"],"<< std::endl;
    //}
    //std::cout << std::endl;


    //int Nx = 16;
    //CRS_Matrix laplace_matrix = create_laplacian_2d(Nx);
    //test_main(laplace_matrix);

    orig_main();


}