#ifndef CPU_NORM_HPP
#define CPU_NORM_HPP
#include "../data/naive_vector.hpp"
#include "../data/std_vector.hpp"

double ScalarProduct(const NaiveVector & vector_x) {
    double sum = 0.0;
    const size_t v_size = vector_x.size();
    for(size_t i = 0; i < v_size; ++i) {
        sum += vector_x[i] * vector_x[i];
    }
    return sum;
}

double ScalarProduct(const StdVector & vector_x) {
    double sum = 0.0;
    const size_t v_size = vector_x.size();
    for(size_t i = 0; i < v_size; ++i) {
        sum += vector_x[i] * vector_x[i];
    }
    return sum;
}
#endif //NORM_HPP
