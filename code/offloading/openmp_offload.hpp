#pragma once


#include "../data/matrix.hpp"
#include "../data/vector.hpp"
#include "../data/data.hpp"

struct Operations_OpenMP_Offload {
    static constexpr const char * name ="OpenMPOffload";

    static void matrix_vector_multiplikation(const Matrix & matrix_A, double alpha,  const Vector & vektor_x, double beta, Vector & vektor_y);

};




