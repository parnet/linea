#pragma once
#ifdef USE_OPENMP
#include <iostream>

#include <omp.h>

namespace linea::openmp {

    void info();
}
#endif
