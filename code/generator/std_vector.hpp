#pragma once

#include <functional>
#include <cmath>

#include <random>
#include "../data/vector.hpp"

inline void create_zero(Vector &v) {
    const size_t size = v.size();
    for (size_t i = 0; i < size; ++i) {
        v.operator[](i) = 0;
    };
}

inline void create_sin_1d(Vector &v) {
    const size_t size = v.size();
    const auto dsize = static_cast<double>(size);
    for (size_t i = 0; i < size; ++i) {
        v.operator[](i) = sin(2 * M_PI * (static_cast<double>(i) / (dsize)));
    };
}

inline void create_cos_1d(Vector &v) {
    const size_t size = v.size();
    const auto dsize = static_cast<double>(size) -2 ;
    for (size_t i = 0; i < size; ++i) {
        v.operator[](i) = cos(2 * M_PI * (static_cast<double>(i) / (dsize)));
    };
}

inline void create_fun(Vector &v,const std::function<double (size_t)>& fun) {
    const size_t size = v.size();
    for (size_t i = 0; i < size; ++i) {
        v.operator[](i) = fun(i);
    };
}

inline void laplace_rhs_function(Vector &vec, size_t Nx, size_t Ny, double alpha) {
    for (std::size_t i = 0; i < Nx; ++i) {
        double x = static_cast<double>(i) / static_cast<double>(Nx-1);
        for (std::size_t j = 0; j < Ny; ++j) {
            double y = static_cast<double>(j) / static_cast<double>(Nx-1);
            vec._data[i*Nx+j] = 2 * alpha* M_PI*sin(M_PI*x)*sin(M_PI*y);
        }
    }
}

inline void fill_with_random(Vector& vec, unsigned int seed) {
    std::mt19937 generator(seed);  // Mersenne Twister random number engine seeded
    std::uniform_real_distribution distribution(-1.0, 1.0);  // Uniform distribution between -1.0 and 1.0

    const size_t size = vec.size();
    for (std::size_t i = 0; i < size; ++i) {
        vec._data[i] = distribution(generator);
    }
}

