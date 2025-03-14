#ifndef OPERATIONS_AVX512_HPP
#define OPERATIONS_AVX512_HPP


#include "data/crs_matrix.hpp"
#include "data/matrix.hpp"
#include "data/vector.hpp"

struct Operations_AVX512 {
    static constexpr const char * name ="AVX512";

    static void matrix_vector_multiplikation(const Matrix &matrix_A, double alpha, const Vector &vektor_x,double beta , Vector &vektor_y);

    static void matrix_vector_multiplikation(const CRS_Matrix &matrix_A, double alpha, const Vector &vektor_x, double beta, Vector &vektor_y);
};
#endif