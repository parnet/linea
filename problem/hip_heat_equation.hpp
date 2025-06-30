#ifdef USE_HIP
#pragma once

#include <omp.h>

#include <iostream>
#include <ostream>
#include <util/timer.hpp>
#include "util/converter.hpp"


#include "generator/extract_diag.hpp"
#include "generator/std_crs_matrix.hpp"
#include "generator/std_vector.hpp"


#include "hip/data/crs_matrix.hpp"
#include "hip/data/vector.hpp"
#include "hip/linear_algebra/vector.hpp"

#include "hip/linear_algebra/kernel/crs_residual.hpp"
#include "hip/linear_algebra/kernel/jacobi_step.hpp"
#include "hip/linear_algebra/kernel/sum_of_squares.hpp"
#include "macro/hip_error.hpp"



inline void hip_heat_equation(int Nx, int Ny, double t0, double tn, int Nt) {
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
    auto hip_laplace_matrix = HIP_CRS_Matrix(laplace_matrix);
    HIP_CHECK(hipStreamSynchronize(hip_laplace_matrix.stream));
    transfer_timer.stop();
    std::cout << "transfer_time= " << transfer_timer.get() << " [s]" << std::endl;
    Vector laplace_diag = Vector(laplace_matrix._num_rows);
    extract_diagonal(laplace_diag, laplace_matrix, 0.66);

    //vec_print("cu_laplace_diag",laplace_diag,  Nx, Ny)

    auto hip_laplace_diag = HIP_Vector(laplace_diag);

    Vector x0 = Vector(Nx*Nx);
    //fill_with_random(x0, 6142);
    create_zero(x0);
    ///std::cout << "." << std::endl;
    auto hip_x0 = HIP_Vector(x0);
    HIP_CHECK(hipStreamSynchronize(hip_x0.stream));

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


    auto hip_rhs = HIP_Vector(rhs);
    HIP_CHECK(hipStreamSynchronize(hip_rhs.stream));

    Vector defect = Vector(Nx*Nx);

    //compute_residual_crs(hip_defect, hip_laplace_matrix, hip_x0, hip_rhs);

    HIP_Vector hip_x_current(hip_x0._num_rows);
    HIP_Vector hip_defect(hip_x0._num_rows);
    HIP_CHECK(hipStreamSynchronize(hip_x_current.stream));

    double norm = 0.0;

    hipStream_t stream;
    HIP_CHECK(hipStreamCreate(&stream));
    const int max_iteration = 10'000;
    //const int max_iteration = 2;
    auto normer = std::vector<double>(max_iteration, 0.0);
    Timer timer;
    timer.start();
    for (int k = 0 ; k < max_iteration; k++){

        hip_crs_residual(hip_defect, hip_laplace_matrix, hip_x0, hip_rhs, stream);
        //hipStreamSynchronize(stream);
        //Vector def = hip_defect.get_vector();
        //    vec_print("defect",def,  Nx, Ny);

        /*Vector diag = hip_laplace_diag.get_vector();
        std::cout << "diag" << std::endl;
        for ( int i = 0; i < Nx; i++ ) {
            for ( int j = 0; j < Ny; j++ ) {
                std::cout << diag._data[i*Nx+j] << ",";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;*/
        norm = hip_sum_of_squares(hip_defect, stream);
        //std::cout << norm  << ",";
        normer[k] =  norm;
        hip_jacobi_step(hip_x_current, hip_x0, hip_laplace_diag, hip_defect,  stream);
        //hipStreamSynchronize(stream);
        hip_x0.swap(hip_x_current);

        //Vector x0 = hip_x0.get_vector();
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

inline void hip_bicgstab_heat_equation(int Nx, int Ny, double t0, double tn, int Nt) {
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
    auto hip_laplace_matrix = HIP_CRS_Matrix(laplace_matrix);
    HIP_CHECK(hipStreamSynchronize(hip_laplace_matrix.stream));
    transfer_timer.stop();
    std::cout << "transfer_time= " << transfer_timer.get() << " [s]" << std::endl;
    Vector laplace_diag = Vector(laplace_matrix._num_rows);
    extract_diagonal(laplace_diag, laplace_matrix, 0.66);

    //vec_print("hip_laplace_diag",laplace_diag,  Nx, Ny)

    auto hip_laplace_diag = HIP_Vector(laplace_diag);

    Vector x0 = Vector(Nx*Nx);
    //fill_with_random(x0, 6142);
    create_zero(x0);
    auto hip_x0 = HIP_Vector(x0);
    HIP_CHECK(hipStreamSynchronize(hip_x0.stream));

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


    auto hip_rhs = HIP_Vector(rhs);
    HIP_CHECK(hipStreamSynchronize(hip_rhs.stream));

    Vector defect = Vector(Nx*Nx);

    //compute_residual_crs(hip_defect, hip_laplace_matrix, hip_x0, hip_rhs);

    HIP_Vector hip_x_current(hip_x0._num_rows);
    HIP_Vector hip_defect(hip_x0._num_rows);
    HIP_CHECK(hipStreamSynchronize(hip_x_current.stream));

    double norm = 0.0;

    hipStream_t stream;
    HIP_CHECK(hipStreamCreate(&stream));
    const int max_iteration = 10'000;
    auto normer = std::vector<double>(max_iteration, 0.0);
    Timer timer;
    timer.start();
    for (int k = 0 ; k < max_iteration; k++){
        hip_crs_residual(hip_defect, hip_laplace_matrix, hip_x0, hip_rhs, stream);
        //hipStreamSynchronize(stream);
        //Vector def = hip_defect.get_vector();
        //    vec_print("defect",def,  Nx, Ny);

        /*Vector diag = hip_laplace_diag.get_vector();
        std::cout << "diag" << std::endl;
        for ( int i = 0; i < Nx; i++ ) {
            for ( int j = 0; j < Ny; j++ ) {
                std::cout << diag._data[i*Nx+j] << ",";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;*/
        norm = hip_sum_of_squares(hip_defect, stream);
        //std::cout << norm  << ",";
        normer[k] =  norm;
        hip_jacobi_step(hip_x_current, hip_x0, hip_laplace_diag,hip_defect,  stream);
        //hipStreamSynchronize(stream);
        hip_x0.swap(hip_x_current);

        //Vector x0 = hip_x0.get_vector();
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
#endif