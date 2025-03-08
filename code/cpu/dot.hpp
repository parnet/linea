#ifndef DOT_HPP
#define DOT_HPP
#include "../data/std_vector.hpp"
double ScalarProduct(const StdVector & vector_x, const StdVector & vector_y) {
  double sum = 0.0;
  for(size_t i = 0; i < vector_x.size(); ++i) {
    sum += vector_x(i) * vector_y(i);
  }
  return sum;
}

/*double ScalarProductBracket(const StdVector & vector_x, const StdVector & vector_y) {
  double sum = 0.0;
  for(size_t i = 0; i < vector_x.size(); ++i) {
    sum += vector_x[i] * vector_y[i];
  }
  return sum;
}*/

double ScalarProductDirect(const StdVector & vector_x, const StdVector & vector_y) {
  double sum = 0.0;
  for(size_t i = 0; i < vector_x.size(); ++i) {
    sum += vector_x.data[i] * vector_y.data[i];
  }
  return sum;
}

#endif
