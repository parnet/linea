#pragma once

#include <functional>
#include <cmath>

#include "../data/c_vector.hpp"

inline void create_zero(C_Vector &v) {
    const size_t size = v.size();
    for (size_t i = 0; i < size; ++i) {
        v.operator[](i) = 0;
    };
}

inline void create_sin_1d(C_Vector &v) {
    const size_t size = v.size();
    const auto dsize = static_cast<double>(size);
    for (size_t i = 0; i < size; ++i) {
        v.operator[](i) = sin(2 * M_PI * (static_cast<double>(i) / (dsize)));
    };
}

inline void create_cos_1d(C_Vector &v) {
    const size_t size = v.size();
    const auto dsize = static_cast<double>(size) -2 ;
    for (size_t i = 0; i < size; ++i) {
        v.operator[](i) = cos(2 * M_PI * (static_cast<double>(i) / (dsize)));
    };
}

inline void create_fun(C_Vector &v,const std::function<double (size_t)>& fun) {
    const size_t size = v.size();
    for (size_t i = 0; i < size; ++i) {
        v.operator[](i) = fun(i);
    };
}
