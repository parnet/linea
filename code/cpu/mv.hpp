#ifndef CPU_MV_H
#define CPU_MV_H

#include "../data/std_vector.hpp"
#include "../data/naive_vector.hpp"
#include "../data/naive_matrix.hpp"
#include "../data/std_matrix.hpp"


/**
 * Matrix Vektor Multiplication
 *      $$y = alpha * A * x + beta * y$$
 * for a Naive Matrix and a Naive Vector (double*)
 * without cache optimization
 */
void MatrixVectorMultiplikation(const NaiveMatrix & matrix_A, double alpha,  const NaiveVector & vektor_x, double beta, NaiveVector & vektor_y) {
    const size_t vector_size = vektor_y.size();
    for (size_t i = 0; i < vector_size; i++) {
        vektor_y._data[i] *= beta;
    }

    const size_t nr = matrix_A._num_rows;
    const size_t nc = matrix_A._num_cols;
    for (size_t i = 0; i < nr; i++) {
        for (size_t j = 0; j < nc; j++) {
            double val = matrix_A._data[i*nc+j];
            vektor_y._data[i] += alpha * val * vektor_x._data[j];
        }
    }
}

/**
 * Matrix Vektor Multiplication
 *      $$y = alpha * A * x + beta * y$$
 * for a Naive Matrix and a Naive Vector (double*)
 * with slight cache optimization
 */
void MatrixVectorMultiplikationCache(const NaiveMatrix & matrix_A, double alpha,  const NaiveVector & vektor_x, double beta, NaiveVector & vektor_y) {

    const size_t nr = matrix_A._num_rows;
    const size_t nc = matrix_A._num_cols;
    for (size_t i = 0; i < nr; i++) {
        vektor_y._data[i] *= beta;
        for (size_t j = 0; j < nc; j++) {
            double val = matrix_A._data[i*nc+j];
            vektor_y._data[i] += alpha * val * vektor_x._data[j];
        }
    }
}

/**
 * Matrix Vektor Multiplication
 *      $$y = alpha * A * x + beta * y$$
 * for a Naive Matrix and a Standard Vector std::vector<double>
 * without cache optimization
 */
void MatrixVectorMultiplikation(const NaiveMatrix & matrix_A, double alpha,  const StdVector & vektor_x, double beta, StdVector & vektor_y) {
    const size_t vector_size = vektor_y.size();
    for (size_t i = 0; i < vector_size; i++) {
        vektor_y._data[i] *= beta;
    }

    size_t nr = matrix_A._num_rows;
    size_t nc = matrix_A._num_cols;
    for (size_t i = 0; i < nr; i++) {
        const size_t ni = i*nc;
        for (size_t j = 0; j < nc; j++) {
            double val = matrix_A._data[ni+j];
            vektor_y._data[i] += alpha * val * vektor_x._data[j];
        }
    }
}

void MatrixVectorMultiplikation(const StdFlatMatrix & matrix_A, double alpha,  const StdVector & vektor_x, double beta, StdVector & vektor_y) {
    const size_t vector_size = vektor_y.size();
    for (size_t i = 0; i < vector_size; i++) {
        vektor_y._data[i] *= beta;
    }

    for (size_t i = 0; i < matrix_A._num_rows; i++) {
        for (size_t j = 0; j < matrix_A._num_cols; j++) {
            double val = matrix_A._data[i*matrix_A._num_cols+j];
            vektor_y._data[i] += alpha * val * vektor_x._data[j];
        }
    }
}


void MatrixVectorMultiplikationConstQ(const StdFlatMatrix & matrix_A, double alpha,  const StdVector & vektor_x, double beta, StdVector & vektor_y) {
    const size_t vector_size = vektor_y.size();
    for (size_t i = 0; i < vector_size; i++) {
        vektor_y._data[i] *= beta;
    }

    const size_t numrows =  matrix_A._num_rows;
    const size_t numcols =  matrix_A._num_cols;
    for (size_t i = 0; i < numrows; i++) {
        for (size_t j = 0; j < numcols; j++) {
            double val = matrix_A._data[i*numcols+j];
            vektor_y._data[i] += alpha * val * vektor_x._data[j];
        }
    }
}




#pragma GCC optimize ("no-tree-vectorize")
void MatrixVectorMultiplikationNoVectorization(const StdFlatMatrix & matrix_A, double alpha,  const StdVector & vektor_x, double beta, StdVector & vektor_y) {
    const size_t vector_size = vektor_y.size();
#pragma clang loop vectorize(disable)
    for (size_t i = 0; i < vector_size; i++) {
        vektor_y._data[i] *= beta;
    }

#pragma clang loop vectorize(disable)
    for (size_t i = 0; i < matrix_A._num_rows; i++) {
#pragma clang loop vectorize(disable)
        for (size_t j = 0; j < matrix_A._num_cols; j++) {
            double val = matrix_A._data[i*matrix_A._num_cols+j];
            vektor_y._data[i] += alpha * val * vektor_x._data[j];
        }
    }
}
#pragma GCC optimize ("tree-vectorize")


void MatrixVectorMultiplikation(const StdMatrix & matrix_A, double alpha,  const StdVector & vektor_x, double beta, StdVector & vektor_y) {
    const size_t vector_size = vektor_y.size();
    for (size_t i = 0; i < vector_size; i++) {
        vektor_y._data[i] *= beta;
    }


    for (size_t i = 0; i < matrix_A._num_rows; i++) {
        for (size_t j = 0; j < matrix_A._num_cols; j++) {
            double val = matrix_A._data[i][j];
            vektor_y._data[i] += alpha * val * vektor_x._data[j];
        }
    }
}


// -----

void MatrixVectorMultiplikationCache(const StdMatrix & matrix_A, double alpha,  const StdVector & vektor_x, double beta, StdVector & vektor_y) {
    const size_t rows = matrix_A._num_rows;
    const size_t cols = matrix_A._num_cols;

    for (size_t i = 0; i < rows; i++) {
        double temp = beta * vektor_y[i];
        for (size_t j = 0; j < cols; j++) {
            double val = matrix_A._data[i][j];
            temp += alpha * val * vektor_x._data[j];
        }
        vektor_y._data[i] = temp;
    }
}

#endif