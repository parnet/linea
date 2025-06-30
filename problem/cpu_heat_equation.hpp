#pragma once

#include <iostream>
#include <ostream>
#include <util/timer.hpp>
#include <omp.h>



#include "generator/extract_diag.hpp"
#include "generator/std_crs_matrix.hpp"
#include "generator/std_vector.hpp"

#include "util/converter.hpp"




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

inline void cpu_heat_equation(int Nx, int Ny, double t0, double tn, int Nt) {
    Timer walltime = Timer();
    walltime.start();

    std::cout << "heat_equation" <<std::endl;
    std::cout << "Nx="<< Nx << " Ny=" << Ny << " t0="<<t0<< " tn="<< tn <<" Nt="<<Nt << std::endl;
    double alpha = 1.0;
    CRS_Matrix laplace_matrix = create_laplacian_2d(Nx);


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



    //auto cu_laplace_matrix = CUDA_CRS_Matrix(laplace_matrix);

    Vector laplace_diag = Vector(laplace_matrix._num_rows);
    extract_diagonal(laplace_diag, laplace_matrix);

    //vec_print("cu_laplace_diag",laplace_diag,  Nx, Ny)

    //auto cu_laplace_diag = CUDA_Vector(laplace_diag);

    Vector x0 = Vector(Nx*Nx);
    //fill_with_random(x0, 6142);
    create_zero(x0);
    //auto cu_x0 = CUDA_Vector(x0);
    //cudaStreamSynchronize(cu_x0.stream);

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


    //auto cu_rhs = CUDA_Vector(rhs);
    //cudaStreamSynchronize(cu_rhs.stream);

    Vector defect = Vector(Nx*Nx);

    //compute_residual_crs(cu_defect, cu_laplace_matrix, cu_x0, cu_rhs);

    //CUDA_Vector cu_x_current(cu_x0._num_rows);
    //CUDA_Vector cu_defect(cu_x0._num_rows);
    //cudaStreamSynchronize(cu_x_current.stream);

    double norm = 0.0;

    //cudaStream_t stream;
    //cudaStreamCreate(&stream);
    //const int max_iteration = 10'000;
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
        norm = sum_of_squares(defect);
        //std::cout <<"norm="<< norm  << ",";
        normer[k] =  norm;

        jacobi_step(x_current, x0, laplace_diag,defect);
        //cudaStreamSynchronize(stream);
        x0.swap(x_current);

        //Vector x0 = cu_x0.get_vector();
    }
    timer.stop();
    std::cout << "time=" << timer.get() <<" [s]"<< std::endl;
    //std::cout << std::endl;
    std::cout << normer[max_iteration-1] << std::endl;
    walltime.stop();
    std::cout << "walltime=" << walltime.get() <<" [s]"<< std::endl;
}

