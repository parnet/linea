#ifndef OPENMP_MV_HPP
#define OPENMP_MV_HPP

#include <omp.h>

#include "../data/std_flat_matrix.hpp"
#include "../data/std_vector.hpp"

void MatrixVectorMultiplikationOpenMP(const StdFlatMatrix & matrix_A, double alpha,  const StdVector & vektor_x, double beta, StdVector & vektor_y) {
    const size_t mr = matrix_A._num_rows;
    const size_t mc = matrix_A._num_cols;

    for(size_t i = 0 ; i < mr; ++i) {
        vektor_y._data[i] *= beta;
    }

#pragma omp parallel for
    for (size_t i = 0; i < mr; ++i) {

        #pragma omp simd
        for (size_t j = 0; j < mc; j++) {
            double val = matrix_A._data[i*matrix_A._num_cols+j];
            vektor_y._data[i] += alpha * val * vektor_x._data[j];
        }
    }
}



#endif