#ifndef LINEA_LIB_OPENMP_HPP
#define LINEA_LIB_OPENMP_HPP
#ifdef USE_OPENMP
#include <iostream>

#include <omp.h>

namespace linea::openmp {

    void info();
}
#endif
#endif
