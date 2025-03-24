#ifndef OPERATIONS_SSE2_HPP
#define OPERATIONS_SSE2_HPP

#include "libs/intrinsics.hpp"

#include "data/matrix.hpp"
#include "data/vector.hpp"

struct Operations_SSE2 {
    static constexpr const char * name ="SSE2";
    static void matrix_vector_multiplikation(const Matrix &matrix_A, double alpha, const Vector &vektor_x, double beta, Vector &vektor_y);
};

#endif
