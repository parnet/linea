#ifndef BENCHMARK_MV_HPP
#define BENCHMARK_MV_HPP

#include <iostream>

#include "../code/blas/mv.hpp"
#include "../code/util/timer.hpp"

#include "../code/data/naive_vector.hpp"
#include "../code/generator/naive_vector.hpp"

#include "../code/data/std_vector.hpp"
#include "../code/generator/std_vector.hpp"

#include "../code/data/naive_matrix.hpp"
#include "../code/generator/naive_matrix.hpp"

#include "../code/data/std_matrix.hpp"
#include "../code/generator/std_matrix.hpp"

#include "../code/data/std_flat_matrix.hpp"
#include "../code/generator/std_flat_matrix.hpp"

#include "../code/cpu/mv.hpp"
#include "../code/simd/mv.hpp"
#include "../code/openmp/mv.hpp"
#include "../code/util/converter.hpp"


void benchmark_mv_nn(){
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
        NaiveVector vector = NaiveVector(gridsize*gridsize);


        timer.start();
        create_sin_1d(vector);
        timer.stop();
        time = timer.get();
        std::cout << "time(interpolate)="<< time << std::endl;


        NaiveVector result = NaiveVector(gridsize*gridsize);
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
        NaiveVector vector = NaiveVector(gridsize*gridsize);


        timer.start();
        create_sin_1d(vector);
        timer.stop();
        time = timer.get();
        std::cout << "time(interpolate)="<< time << std::endl;


        NaiveVector result = NaiveVector(gridsize*gridsize);
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
        StdVector vector = StdVector(gridsize*gridsize);


        timer.start();
        create_sin_1d(vector);
        timer.stop();
        time = timer.get();
        std::cout << "time(interpolate)="<< time << std::endl;


        StdVector result = StdVector(gridsize*gridsize);
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
    std::cout << "StdMatrix * StdVector" << std::endl;
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
        StdVector vector = StdVector(gridsize*gridsize);


        timer.start();
        create_sin_1d(vector);
        timer.stop();
        time = timer.get();
        std::cout << "time(interpolate)="<< time << std::endl;


        StdVector result = StdVector(gridsize*gridsize);
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
    std::cout << "StdFlatMatrix * NaiveVector" << std::endl;
    Timer timer = Timer();
    double time = 0.0;

    for (int k = 0; k < 7; ++k) {
        size_t gridsize = (2 * (1 << k) +1);
        //std::cout << k << ": " << gridsize << std::endl;
        StdFlatMatrix matrix;// = NaiveMatrix(gridsize*gridsize,gridsize*gridsize);

        timer.start();
        create_laplacian_2d(matrix,gridsize);
        timer.stop();
        time = timer.get();
        std::cout << "time(laplacian)="<< time << std::endl;

        //std::cout << "memory(A)=" << memory_size << std::endl;
        StdVector vector = StdVector(gridsize*gridsize);


        timer.start();
        create_sin_1d(vector);
        timer.stop();
        time = timer.get();
        std::cout << "time(interpolate)="<< time << std::endl;


        StdVector result = StdVector(gridsize*gridsize);
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
    std::cout << "StdFlatMatrix * NaiveVector" << std::endl;
    Timer timer = Timer();
    double time = 0.0;

    for (int k = 0; k < 7; ++k) {
        size_t gridsize = (2 * (1 << k) +1);
        //std::cout << k << ": " << gridsize << std::endl;
        StdFlatMatrix matrix;// = NaiveMatrix(gridsize*gridsize,gridsize*gridsize);

        timer.start();
        create_laplacian_2d(matrix,gridsize);
        timer.stop();
        time = timer.get();
        std::cout << "time(laplacian)="<< time << std::endl;

        //std::cout << "memory(A)=" << memory_size << std::endl;
        StdVector vector = StdVector(gridsize*gridsize);


        timer.start();
        create_sin_1d(vector);
        timer.stop();
        time = timer.get();
        std::cout << "time(interpolate)="<< time << std::endl;


        StdVector result = StdVector(gridsize*gridsize);
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
    std::cout << "StdFlatMatrix * NaiveVector" << std::endl;
    Timer timer = Timer();
    double time = 0.0;

    for (int k = 0; k < 7; ++k) {
        size_t gridsize = (2 * (1 << k) +1);
        //std::cout << k << ": " << gridsize << std::endl;
        StdFlatMatrix matrix;// = NaiveMatrix(gridsize*gridsize,gridsize*gridsize);

        timer.start();
        create_laplacian_2d(matrix,gridsize);
        timer.stop();
        time = timer.get();
        std::cout << "time(laplacian)="<< time << std::endl;

        //std::cout << "memory(A)=" << memory_size << std::endl;
        StdVector vector = StdVector(gridsize*gridsize);


        timer.start();
        create_sin_1d(vector);
        timer.stop();
        time = timer.get();
        std::cout << "time(interpolate)="<< time << std::endl;


        StdVector result = StdVector(gridsize*gridsize);
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
    std::cout << "StdFlatMatrix * NaiveVector" << std::endl;
    Timer timer = Timer();
    double time = 0.0;

    for (int k = 0; k < 7; ++k) {
        size_t gridsize = (2 * (1 << k) +1);
        //std::cout << k << ": " << gridsize << std::endl;
        StdFlatMatrix matrix;// = NaiveMatrix(gridsize*gridsize,gridsize*gridsize);

        timer.start();
        create_laplacian_2d(matrix,gridsize);
        timer.stop();
        time = timer.get();
        std::cout << "time(laplacian)="<< time << std::endl;

        //std::cout << "memory(A)=" << memory_size << std::endl;
        StdVector vector = StdVector(gridsize*gridsize);


        timer.start();
        create_sin_1d(vector);
        timer.stop();
        time = timer.get();
        std::cout << "time(interpolate)="<< time << std::endl;


        StdVector result = StdVector(gridsize*gridsize);
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
    std::cout << "StdFlatMatrix * NaiveVector" << std::endl;
    Timer timer = Timer();
    double time = 0.0;

    for (int k = 0; k < 7; ++k) {
        size_t gridsize = (2 * (1 << k) +1);
        //std::cout << k << ": " << gridsize << std::endl;
        StdFlatMatrix matrix;// = NaiveMatrix(gridsize*gridsize,gridsize*gridsize);

        timer.start();
        create_laplacian_2d(matrix,gridsize);
        timer.stop();
        time = timer.get();
        std::cout << "time(laplacian)="<< time << std::endl;

        //std::cout << "memory(A)=" << memory_size << std::endl;
        StdVector vector = StdVector(gridsize*gridsize);


        timer.start();
        create_sin_1d(vector);
        timer.stop();
        time = timer.get();
        std::cout << "time(interpolate)="<< time << std::endl;


        StdVector result = StdVector(gridsize*gridsize);
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
void benchmark_mv_sfs_constq(){
    std::cout << "StdFlatMatrix * NaiveVector" << std::endl;
    Timer timer = Timer();
    double time = 0.0;

    for (int k = 0; k < 7; ++k) {
        size_t gridsize = (2 * (1 << k) +1);
        //std::cout << k << ": " << gridsize << std::endl;
        StdFlatMatrix matrix;// = NaiveMatrix(gridsize*gridsize,gridsize*gridsize);

        timer.start();
        create_laplacian_2d(matrix,gridsize);
        timer.stop();
        time = timer.get();
        std::cout << "time(laplacian)="<< time << std::endl;

        //std::cout << "memory(A)=" << memory_size << std::endl;
        StdVector vector = StdVector(gridsize*gridsize);


        timer.start();
        create_sin_1d(vector);
        timer.stop();
        time = timer.get();
        std::cout << "time(interpolate)="<< time << std::endl;


        StdVector result = StdVector(gridsize*gridsize);
        timer.start();
        create_zero(result);
        timer.stop();
        time = timer.get();
        std::cout << "time(zerofill)="<< time << std::endl;

        timer.start();
        MatrixVectorMultiplikationConstQ(matrix,1,vector,1,result);
        timer.stop();
        time = timer.get();
        std::cout << "time(alpha*A*x+beta*y)_constQ="<< time << std::endl;

        size_t memory_size = matrix.memory_size();
        std::cout << k << "; " << gridsize << "; " << convert(memory_size, Unit::Giga) << "GiB\t" << sizeof(matrix)<< "\t"<<sizeof(matrix._data) << std::endl<< std::endl;

        //double K;
        //std::cin >> K;
        //std::cout << K << std::endl;
        //matrix.operator()(0,0) = K;
    }
    std::cout << "finished benchmark_csrmv"<< std::endl;
};

void benchmark_mv() {
    //benchmark_mv_nn();
    //benchmark_mv_nnc();
    //benchmark_mv_ns();
    //benchmark_mv_ss();
    benchmark_mv_sfs();
    //benchmark_mv_sfs_openmp();
    //benchmark_mv_sfs_blas();
    benchmark_mv_sfs_avx512();
    benchmark_mv_sfs_noopt();
    benchmark_mv_sfs_constq();
}
#endif