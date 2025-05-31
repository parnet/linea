#pragma once


#include "../data/crs_matrix.hpp"
#include "../data/matrix.hpp"
#include "../data/vector.hpp"


struct Operations_CPU {

    static constexpr const char * name ="CPU";

    static void matrix_vector_multiplikation(const Matrix & matrix_A, double alpha,  const Vector & vektor_x, double beta, Vector & vektor_y);
    static void matrix_vector_multiplikation(const CRS_Matrix & matrix_A, double alpha,  const Vector & vektor_x, double beta, Vector & vektor_y);

    static double norm(const Vector & vector_x);

    static double dot_product(const Vector & vector_x, const Vector & vector_y);
};


