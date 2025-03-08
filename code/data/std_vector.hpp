#ifndef DATA_STD_VECTOR_HPP
#define DATA_STD_VECTOR_HPP
#include <vector>

class StdVector {
public:

    explicit StdVector(size_t num_elements) : _num_elements(num_elements) {
        data.resize(num_elements);

    }

    StdVector(const StdVector &other) :_num_elements(other._num_elements){
        data.resize(_num_elements);
        for (size_t i = 0; i < _num_elements; ++i) {
            data[i] = other.data[i];
        }
    }

    ~StdVector() = default;

    double operator()(size_t index) const {
        return data[index];
    }

    double & operator()(size_t index) {
        return data[index];
    }

    size_t size() const {
        return _num_elements;
    }

    std::string str() const {
        std::stringstream ss;
        for (size_t i = 0; i < _num_elements -1; ++i) {
            ss << data[i] << ", ";
        }
        ss << data[_num_elements-1];
        return ss.str();
    }
public:
    size_t _num_elements;
    std::vector<double> data;
};
#endif
