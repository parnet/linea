#pragma once


#include "../data/matrix.hpp"
#include "../data/vector.hpp"


struct Operations_CPU_NonConst {
    static constexpr const char* name ="CPU_NonConst";
    static void matrix_vector_multiplikation(const Matrix & matrix_A, double alpha,  const Vector & vektor_x, double beta, Vector & vektor_y);
};

