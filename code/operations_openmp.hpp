#ifndef OPERATIONS_OPENMP_HPP
#define OPERATIONS_OPENMP_HPP

#include "libs/openmp.hpp"
#ifdef USE_OPENMP
#include "data/matrix.hpp"
#include "data/vector.hpp"
#include "data/crs_matrix.hpp"

struct Operations_OpenMP {
    static constexpr const char * name ="OpenMP";

    static void matrix_vector_multiplikation(const Matrix & matrix_A, double alpha,  const Vector & vektor_x, double beta, Vector & vektor_y);

    static void matrix_vector_multiplikation(const CRS_Matrix & matrix_A, double alpha,  const Vector & vektor_x, double beta, Vector & vektor_y);

    static double dot_product(const Vector &vector_x, const Vector &vector_y);

    static double norm(const Vector &vector_x);
};
#endif

#endif