#pragma once

#include "../data/matrix.hpp"
#include "../data/vector.hpp"
#include "../libs/opencl.hpp"





struct Operations_OpenCL {
    static constexpr const char * name = "OpenCL";
    static void matrix_vector_multiplikation(const Matrix & matrix_A, double alpha,  const Vector & vektor_x, double beta, Vector & vektor_y);
};




