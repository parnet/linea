#pragma once
#include "cpu_heat_equation.hpp"
#include "hipsparse_heat_equation.hpp"
#include "hip_heat_equation.hpp"
#include "cuda_heat_equation.hpp"


inline  void benchmark_heat_equation() {
    //omp_set_num_threads(8);
    //heat_equation(5, 5, 0, 2.0, 512);
    //omp_set_num_threads(8);
    //heat_equation(5, 5, 0, 2.0, 512);



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

    //hip_heat_equation(1, 1, 0, 2.0, 512);
    //hip_heat_equation(2, 2, 0, 2.0, 512);
    //hip_heat_equation(4, 4, 0, 2.0, 512);
    //hip_heat_equation(8, 8, 0, 2.0, 512);
    //hip_heat_equation(16, 16, 0, 2.0, 512);
    //hip_heat_equation(32, 32, 0, 2.0, 512);
    //hip_heat_equation(64, 64, 0, 2.0, 512);
    //hip_heat_equation(128, 128, 0, 2.0, 512);
    //hip_heat_equation(256, 256, 0, 2.0, 512);
    //hip_heat_equation(512, 512, 0, 2.0, 512);
    //hip_heat_equation(1024, 1024, 0, 2.0, 512);
    //hip_heat_equation(1448, 1448, 0, 2.0, 512);
    //hip_heat_equation(2048, 2048, 0, 2.0, 512);
    //hip_heat_equation(2896, 2896, 0, 2.0, 512);
    //hip_heat_equation(1, 1, 0, 2.0, 512);
    //hip_heat_equation(2048, 2048, 0, 2.0, 512);
    // hipsparse_heat_equation(2048, 2048, 0, 2.0, 512);

    cuda_heat_equation(1, 1, 0, 2.0, 512); // dummy to warm up gpu
    cuda_heat_equation(2048, 2048, 0, 2.0, 512);
    cpu_heat_equation( 2048, 2048, 0, 2.0, 512);


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



}
