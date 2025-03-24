#ifndef OPERATIONS_OPENMP_OFFLOAD_HPP
#define OPERATIONS_OPENMP_OFFLOAD_HPP



#include "data/matrix.hpp"
#include "data/vector.hpp"
#include "data/crs_matrix.hpp"

struct Operations_OpenMP_Offload {
    static constexpr const char * name ="OpenMPOffload";

    static void matrix_vector_multiplikation(const Matrix & matrix_A, double alpha,  const Vector & vektor_x, double beta, Vector & vektor_y);

};





#endif //OPERATIONS_OPENMP_OFFLOAD_HPP
