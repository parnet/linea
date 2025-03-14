#ifndef BENCHMARK_MV_HPP
#define BENCHMARK_MV_HPP

#include <iostream>

#include "operations_avx.hpp"
#include "operations_avx512.hpp"
#include "operations_cpu.hpp"
#include "operations_mkl.hpp"
#include "operations_sse2.hpp"
#include "../code/operations_blas.hpp"
#include "../code/util/timer.hpp"

#include "../code/data/c_vector.hpp"
#include "../code/generator/naive_vector.hpp"

#include "../code/data/vector.hpp"
#include "../code/generator/std_vector.hpp"

#include "../code/data/c_matrix.hpp"
#include "../code/generator/naive_matrix.hpp"

#include "../code/data/std_matrix.hpp"
#include "../code/generator/std_matrix.hpp"

#include "../code/data/matrix.hpp"
#include "../code/generator/std_flat_matrix.hpp"

#include "../code/cpu/mv.hpp"

#include "../code/Operations_OpenMP.hpp"
#include "../code/util/converter.hpp"

/*
void benchmark_mv_nn(){
    std::cout << "NaiveMatrix * NaiveVector" << std::endl;
    Timer timer = Timer();
    double time = 0.0;

    for (int k = 0; k < 7; ++k) {
        size_t gridsize = (2 * (1 << k) +1);
        //std::cout << k << ": " << gridsize << std::endl;
        C_Matrix matrix;// = NaiveMatrix(gridsize*gridsize,gridsize*gridsize);

        timer.start();
        create_laplacian_2d(matrix,gridsize);
        timer.stop();
        time = timer.get();
        std::cout << "time(laplacian)="<< time << std::endl;

        //std::cout << "memory(A)=" << memory_size << std::endl;
        C_Vector vector = C_Vector(gridsize*gridsize);


        timer.start();
        create_sin_1d(vector);
        timer.stop();
        time = timer.get();
        std::cout << "time(interpolate)="<< time << std::endl;


        C_Vector result = C_Vector(gridsize*gridsize);
        timer.start();
        create_zero(result);
        timer.stop();
        time = timer.get();
        std::cout << "time(zerofill)="<< time << std::endl;

        timer.start();
        MatrixVectorMultiplikation(matrix,1,vector,1,result);
        timer.stop();
        time = timer.get();
        std::cout << "time(alpha*A*x+beta*y)="<< time << std::endl;

        size_t memory_size = matrix.memory_size();
        std::cout << k << "; " << gridsize << "; " << convert(memory_size, Unit::Giga) << "\t" << sizeof(matrix)<< "\t"<<sizeof(matrix._data) << std::endl<< std::endl;

        //double K;
        //std::cin >> K;
        //std::cout << K << std::endl;
        //matrix.operator()(0,0) = K;
    }
    std::cout << "finished benchmark_csrmv"<< std::endl;
};

void benchmark_mv_nnc(){
    std::cout << "NaiveMatrix * NaiveVector CO" << std::endl;
    Timer timer = Timer();
    double time = 0.0;

    for (int k = 0; k < 7; ++k) {
        size_t gridsize = (2 * (1 << k) +1);
        //std::cout << k << ": " << gridsize << std::endl;
        NaiveMatrix matrix;// = NaiveMatrix(gridsize*gridsize,gridsize*gridsize);

        timer.start();
        create_laplacian_2d(matrix,gridsize);
        timer.stop();
        time = timer.get();
        std::cout << "time(laplacian)="<< time << std::endl;

        //std::cout << "memory(A)=" << memory_size << std::endl;
        C_Vector vector = C_Vector(gridsize*gridsize);


        timer.start();
        create_sin_1d(vector);
        timer.stop();
        time = timer.get();
        std::cout << "time(interpolate)="<< time << std::endl;


        C_Vector result = C_Vector(gridsize*gridsize);
        timer.start();
        create_zero(result);
        timer.stop();
        time = timer.get();
        std::cout << "time(zerofill)="<< time << std::endl;

        timer.start();
        MatrixVectorMultiplikationCache(matrix,1,vector,1,result);
        timer.stop();
        time = timer.get();
        std::cout << "time(alpha*A'*x+beta*y)="<< time << std::endl;

        size_t memory_size = matrix.memory_size();
        std::cout << k << "; " << gridsize << "; " << convert(memory_size, Unit::Giga) << "\t" << sizeof(matrix)<< "\t"<<sizeof(matrix._data) << std::endl<< std::endl;

        //double K;
        //std::cin >> K;
        //std::cout << K << std::endl;
        //matrix.operator()(0,0) = K;
    }
    std::cout << "finished benchmark_csrmv"<< std::endl;
};


void benchmark_mv_ns(){
    std::cout << "NaiveMatrix * NaiveVector" << std::endl;
    Timer timer = Timer();
    double time = 0.0;

    for (int k = 0; k < 7; ++k) {
        size_t gridsize = (2 * (1 << k) +1);
        //std::cout << k << ": " << gridsize << std::endl;
        NaiveMatrix matrix;// = NaiveMatrix(gridsize*gridsize,gridsize*gridsize);

        timer.start();
        create_laplacian_2d(matrix,gridsize);
        timer.stop();
        time = timer.get();
        std::cout << "time(laplacian)="<< time << std::endl;

        //std::cout << "memory(A)=" << memory_size << std::endl;
        Vector vector = Vector(gridsize*gridsize);


        timer.start();
        create_sin_1d(vector);
        timer.stop();
        time = timer.get();
        std::cout << "time(interpolate)="<< time << std::endl;


        Vector result = Vector(gridsize*gridsize);
        timer.start();
        create_zero(result);
        timer.stop();
        time = timer.get();
        std::cout << "time(zerofill)="<< time << std::endl;

        timer.start();
        MatrixVectorMultiplikation(matrix,1,vector,1,result);
        timer.stop();
        time = timer.get();
        std::cout << "time(alpha*A*x+beta*y)="<< time << std::endl;

        size_t memory_size = matrix.memory_size();
        std::cout << k << "; " << gridsize << "; " << convert(memory_size, Unit::Giga) << "\t" << sizeof(matrix)<< "\t"<<sizeof(matrix._data) << std::endl<< std::endl;

        //double K;
        //std::cin >> K;
        //std::cout << K << std::endl;
        //matrix.operator()(0,0) = K;
    }
    std::cout << "finished benchmark_csrmv"<< std::endl;
};


void benchmark_mv_ss(){
    std::cout << "StdMatrix * Vector" << std::endl;
    Timer timer = Timer();
    double time = 0.0;

    for (int k = 0; k < 7; ++k) {
        size_t gridsize = (2 * (1 << k) +1);
        //std::cout << k << ": " << gridsize << std::endl;
        StdMatrix matrix;// = NaiveMatrix(gridsize*gridsize,gridsize*gridsize);

        timer.start();
        create_laplacian_2d(matrix,gridsize);
        timer.stop();
        time = timer.get();
        std::cout << "time(laplacian)="<< time << std::endl;

        //std::cout << "memory(A)=" << memory_size << std::endl;
        Vector vector = Vector(gridsize*gridsize);


        timer.start();
        create_sin_1d(vector);
        timer.stop();
        time = timer.get();
        std::cout << "time(interpolate)="<< time << std::endl;


        Vector result = Vector(gridsize*gridsize);
        timer.start();
        create_zero(result);
        timer.stop();
        time = timer.get();
        std::cout << "time(zerofill)="<< time << std::endl;

        timer.start();
        MatrixVectorMultiplikation(matrix,1,vector,1,result);
        timer.stop();
        time = timer.get();
        std::cout << "time(alpha*A*x+beta*y)="<< time << std::endl;

        size_t memory_size = matrix.memory_size();
        std::cout << k << "; " << gridsize << "; " << convert(memory_size, Unit::Giga) << "\t" << sizeof(matrix)<< "\t"<<sizeof(matrix._data) << std::endl<< std::endl;

        //double K;
        //std::cin >> K;
        //std::cout << K << std::endl;
        //matrix.operator()(0,0) = K;
    }
    std::cout << "finished benchmark_csrmv"<< std::endl;
};


void benchmark_mv_sfs(){
    std::cout << "Matrix * NaiveVector" << std::endl;
    Timer timer = Timer();
    double time = 0.0;

    for (int k = 0; k < 7; ++k) {
        size_t gridsize = (2 * (1 << k) +1);
        //std::cout << k << ": " << gridsize << std::endl;
        Matrix matrix;// = NaiveMatrix(gridsize*gridsize,gridsize*gridsize);

        timer.start();
        create_laplacian_2d(matrix,gridsize);
        timer.stop();
        time = timer.get();
        std::cout << "time(laplacian)="<< time << std::endl;

        //std::cout << "memory(A)=" << memory_size << std::endl;
        Vector vector = Vector(gridsize*gridsize);


        timer.start();
        create_sin_1d(vector);
        timer.stop();
        time = timer.get();
        std::cout << "time(interpolate)="<< time << std::endl;


        Vector result = Vector(gridsize*gridsize);
        timer.start();
        create_zero(result);
        timer.stop();
        time = timer.get();
        std::cout << "time(zerofill)="<< time << std::endl;

        timer.start();
        MatrixVectorMultiplikation(matrix,1,vector,1,result);
        timer.stop();
        time = timer.get();
        std::cout << "time(alpha*A*x+beta*y)="<< time << std::endl;

        size_t memory_size = matrix.memory_size();
        std::cout << k << "; " << gridsize << "; " << convert(memory_size, Unit::Giga) << "GiB\t" << sizeof(matrix)<< "\t"<<sizeof(matrix._data) << std::endl<< std::endl;

        //double K;
        //std::cin >> K;
        //std::cout << K << std::endl;
        //matrix.operator()(0,0) = K;
    }
    std::cout << "finished benchmark_csrmv"<< std::endl;
};


void benchmark_mv_sfs_openmp(){
    std::cout << "Matrix * NaiveVector" << std::endl;
    Timer timer = Timer();
    double time = 0.0;

    for (int k = 0; k < 7; ++k) {
        size_t gridsize = (2 * (1 << k) +1);
        //std::cout << k << ": " << gridsize << std::endl;
        Matrix matrix;// = NaiveMatrix(gridsize*gridsize,gridsize*gridsize);

        timer.start();
        create_laplacian_2d(matrix,gridsize);
        timer.stop();
        time = timer.get();
        std::cout << "time(laplacian)="<< time << std::endl;

        //std::cout << "memory(A)=" << memory_size << std::endl;
        Vector vector = Vector(gridsize*gridsize);


        timer.start();
        create_sin_1d(vector);
        timer.stop();
        time = timer.get();
        std::cout << "time(interpolate)="<< time << std::endl;


        Vector result = Vector(gridsize*gridsize);
        timer.start();
        create_zero(result);
        timer.stop();
        time = timer.get();
        std::cout << "time(zerofill)="<< time << std::endl;

        timer.start();
        MatrixVectorMultiplikationOpenMP(matrix,1,vector,1,result);
        timer.stop();
        time = timer.get();
        std::cout << "time(alpha*A*x+beta*y)_openmp="<< time << std::endl;

        size_t memory_size = matrix.memory_size();
        std::cout << k << "; " << gridsize << "; " << convert(memory_size, Unit::Giga) << "GiB\t" << sizeof(matrix)<< "\t"<<sizeof(matrix._data) << std::endl<< std::endl;

        //double K;
        //std::cin >> K;
        //std::cout << K << std::endl;
        //matrix.operator()(0,0) = K;
    }
    std::cout << "finished benchmark_csrmv"<< std::endl;
};
void benchmark_mv_sfs_blas(){
    std::cout << "Matrix * NaiveVector" << std::endl;
    Timer timer = Timer();
    double time = 0.0;

    for (int k = 0; k < 7; ++k) {
        size_t gridsize = (2 * (1 << k) +1);
        //std::cout << k << ": " << gridsize << std::endl;
        Matrix matrix;// = NaiveMatrix(gridsize*gridsize,gridsize*gridsize);

        timer.start();
        create_laplacian_2d(matrix,gridsize);
        timer.stop();
        time = timer.get();
        std::cout << "time(laplacian)="<< time << std::endl;

        //std::cout << "memory(A)=" << memory_size << std::endl;
        Vector vector = Vector(gridsize*gridsize);


        timer.start();
        create_sin_1d(vector);
        timer.stop();
        time = timer.get();
        std::cout << "time(interpolate)="<< time << std::endl;


        Vector result = Vector(gridsize*gridsize);
        timer.start();
        create_zero(result);
        timer.stop();
        time = timer.get();
        std::cout << "time(zerofill)="<< time << std::endl;

        timer.start();
        MatrixVectorMultiplikationBLAS(matrix,1,vector,1,result);
        timer.stop();
        time = timer.get();
        std::cout << "time(alpha*A*x+beta*y)_blas="<< time << std::endl;

        size_t memory_size = matrix.memory_size();
        std::cout << k << "; " << gridsize << "; " << convert(memory_size, Unit::Giga) << "GiB\t" << sizeof(matrix)<< "\t"<<sizeof(matrix._data) << std::endl<< std::endl;

        //double K;
        //std::cin >> K;
        //std::cout << K << std::endl;
        //matrix.operator()(0,0) = K;
    }
    std::cout << "finished benchmark_csrmv"<< std::endl;
};

void benchmark_mv_sfs_avx512(){
    std::cout << "Matrix * NaiveVector" << std::endl;
    Timer timer = Timer();
    double time = 0.0;

    for (int k = 0; k < 7; ++k) {
        size_t gridsize = (2 * (1 << k) +1);
        //std::cout << k << ": " << gridsize << std::endl;
        Matrix matrix;// = NaiveMatrix(gridsize*gridsize,gridsize*gridsize);

        timer.start();
        create_laplacian_2d(matrix,gridsize);
        timer.stop();
        time = timer.get();
        std::cout << "time(laplacian)="<< time << std::endl;

        //std::cout << "memory(A)=" << memory_size << std::endl;
        Vector vector = Vector(gridsize*gridsize);


        timer.start();
        create_sin_1d(vector);
        timer.stop();
        time = timer.get();
        std::cout << "time(interpolate)="<< time << std::endl;


        Vector result = Vector(gridsize*gridsize);
        timer.start();
        create_zero(result);
        timer.stop();
        time = timer.get();
        std::cout << "time(zerofill)="<< time << std::endl;

        timer.start();
        MatrixVectorMultiplikationAVX512(matrix,1,vector,1,result);
        timer.stop();
        time = timer.get();
        std::cout << "time(alpha*A*x+beta*y)_avx512="<< time << std::endl;

        size_t memory_size = matrix.memory_size();
        std::cout << k << "; " << gridsize << "; " << convert(memory_size, Unit::Giga) << "GiB\t" << sizeof(matrix)<< "\t"<<sizeof(matrix._data) << std::endl<< std::endl;

        //double K;
        //std::cin >> K;
        //std::cout << K << std::endl;
        //matrix.operator()(0,0) = K;
    }
    std::cout << "finished benchmark_csrmv"<< std::endl;
};

void benchmark_mv_sfs_noopt(){
    std::cout << "Matrix * NaiveVector" << std::endl;
    Timer timer = Timer();
    double time = 0.0;

    for (int k = 0; k < 7; ++k) {
        size_t gridsize = (2 * (1 << k) +1);
        //std::cout << k << ": " << gridsize << std::endl;
        Matrix matrix;// = NaiveMatrix(gridsize*gridsize,gridsize*gridsize);

        timer.start();
        create_laplacian_2d(matrix,gridsize);
        timer.stop();
        time = timer.get();
        std::cout << "time(laplacian)="<< time << std::endl;

        //std::cout << "memory(A)=" << memory_size << std::endl;
        Vector vector = Vector(gridsize*gridsize);


        timer.start();
        create_sin_1d(vector);
        timer.stop();
        time = timer.get();
        std::cout << "time(interpolate)="<< time << std::endl;


        Vector result = Vector(gridsize*gridsize);
        timer.start();
        create_zero(result);
        timer.stop();
        time = timer.get();
        std::cout << "time(zerofill)="<< time << std::endl;

        timer.start();
        MatrixVectorMultiplikationNoVectorization(matrix,1,vector,1,result);
        timer.stop();
        time = timer.get();
        std::cout << "time(alpha*A*x+beta*y)_noopt="<< time << std::endl;

        size_t memory_size = matrix.memory_size();
        std::cout << k << "; " << gridsize << "; " << convert(memory_size, Unit::Giga) << "GiB\t" << sizeof(matrix)<< "\t"<<sizeof(matrix._data) << std::endl<< std::endl;

        //double K;
        //std::cin >> K;
        //std::cout << K << std::endl;
        //matrix.operator()(0,0) = K;
    }
    std::cout << "finished benchmark_csrmv"<< std::endl;
};
*/

template <typename Operation = Operations_CPU>
void _benchmark_mv(){
    std::cout << ":::" << Operation::name <<":::"<< std::endl;
    Timer timer = Timer();
    double time = 0.0;

    for (int k = 3; k < 257; k+=8) {

        size_t gridsize = k;
        std::cout << "gridsize="<<k<<std::endl;
        std::cout <<"vectorsize="<<gridsize*gridsize<<std::endl;
        std::cout<<"matrixsize="<< gridsize*gridsize*gridsize*gridsize << std::endl;
        Matrix matrix;

        timer.start();
        create_laplacian_2d(matrix,gridsize);
        timer.stop();
        time = timer.get();
        //std::cout << "time(laplacian)="<< time << std::endl;
        Vector vector = Vector(gridsize*gridsize);

        timer.start();
        create_sin_1d(vector);
        timer.stop();
        time = timer.get();
        //std::cout << "time(interpolate)="<< time << std::endl;


        Vector result = Vector(gridsize*gridsize);
        timer.start();
        create_zero(result);
        timer.stop();
        time = timer.get();

        //std::cout << "time(zerofill)="<< time << std::endl;

        timer.start();
        Operation::matrix_vector_multiplikation(matrix,1,vector,1,result);
        timer.stop();
        time = timer.get();
        std::cout << "time(mv)="<< time << std::endl;


        //double K;
        //std::cin >> K;
        //std::cout << K << std::endl;
        //matrix.operator()(0,0) = K;
        std::cout << std::endl;
    }
    std::cout << "finished benchmark_csrmv"<< std::endl;
};

void benchmark_mv() {
    _benchmark_mv<Operations_CPU>();
    _benchmark_mv<Operations_SSE2>();
    _benchmark_mv<Operations_AVX>();
    _benchmark_mv<Operations_AVX512>();
    _benchmark_mv<Operations_OpenMP>();
    //_benchmark_mv<Operations_BLAS>();
    //_benchmark_mv<Operations_MKL>();
}
#endif