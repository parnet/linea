#ifndef JACOBI_HPP
#define JACOBI_HPP
#include <iostream>
#include <ostream>
#include <util/timer.hpp>
#include <omp.h>

#include "compute_residual_crs.hpp"
#include "generator/extract_diag.hpp"
#include "generator/std_crs_matrix.hpp"
#include "generator/std_vector.hpp"
#include "cuda_datastructures.hpp"
#include "jacobi_method.hpp"
#include "norm.hpp"


//create_matrix()
//create_identity_matrix()
//transfer_matrix()
//source_function()
//solve_timestep()


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



    auto cu_laplace_matrix = CUDA_CRS_Matrix(laplace_matrix);

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
        compute_residual_crs(cu_defect, cu_laplace_matrix, cu_x0, cu_rhs, stream);
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
    std::cout << "time=" << timer.get() <<" [s]"<< std::endl;
    //std::cout << std::endl;
    std::cout << normer[max_iteration-1] << std::endl;
}



void compute_residual_crs(Vector& d, const CRS_Matrix& A, const Vector& x, const Vector& rhs) {
    const int n_rows = A._num_rows;
#pragma omp parallel for
    for (int i = 0; i < n_rows; ++i) {
        double Ax_i = 0.0;

#pragma omp simd reduction(+:Ax_i)
        for (int idx = A._row_ptr[i]; idx < A._row_ptr[i + 1]; ++idx) {
            Ax_i += A._data[idx] * x._data[A._col_index[idx]];
        }
        d._data[i] = rhs._data[i] - Ax_i;
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
        compute_residual_crs(defect, laplace_matrix, x0, rhs);
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
        std::cout <<"norm="<< norm  << ",";
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
}


inline  void benchmark_heat_equation() {
    //omp_set_num_threads(8);
    //heat_equation(5, 5, 0, 2.0, 512);
    //cuda_heat_equation(2048, 2048, 0, 2.0, 512);
    cuda_heat_equation(2048, 2048, 0, 2.0, 512);
}
#endif
