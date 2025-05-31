#pragma once

#include "../libs/intrinsics.hpp"
#ifdef USE_AVX
#include "data/matrix.hpp"
#include "data/vector.hpp"

struct Operations_AVX {
    static constexpr const char * name ="AVX";
    static void matrix_vector_multiplikation(const Matrix &matrix_A, double alpha, const Vector &vektor_x, double beta, Vector &vektor_y);
};
#endif
