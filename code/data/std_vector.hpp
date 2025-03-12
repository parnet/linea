#ifndef DATA_STD_VECTOR_HPP
#define DATA_STD_VECTOR_HPP
#include <vector>

class StdVector {
public:
    StdVector() = default;

    explicit StdVector(size_t num_elements) : _num_elements(num_elements) {
        _data.resize(num_elements);

    }

    StdVector(const StdVector &other) :_num_elements(other._num_elements){
        _data.resize(_num_elements);
        for (size_t i = 0; i < _num_elements; ++i) {
            _data[i] = other._data[i];
        }
    }

    StdVector(StdVector &&other){
        this->_num_elements = other._num_elements;
        std::swap(_data, other._data);
    }

    StdVector &operator=(const StdVector &other) {
        _data.resize(_num_elements);
        for (size_t i = 0; i < _num_elements; ++i) {
            _data[i] = other._data[i];
        }
        return *this;
    }


    StdVector &operator=(StdVector &&other) noexcept {
        this->_num_elements = other._num_elements;
        std::swap(_data, other._data);
        return *this;
    }

    ~StdVector() = default;

    double operator[](size_t index) const {
        return _data[index];
    }

    double & operator[](size_t index) {
        return _data[index];
    }

    size_t size() const {
        return _num_elements;
    }

    size_t memory_size() {
        size_t total = 0;
        total += sizeof(size_t)*1;
        total += sizeof(double)*_num_elements;
        return total;
    }

    std::string str() const {
        std::stringstream ss;
        for (size_t i = 0; i < _num_elements -1; ++i) {
            ss << _data[i] << ", ";
        }
        ss << _data[_num_elements-1];
        return ss.str();
    }
public:
    size_t _num_elements;
    std::vector<double> _data;
};

#endif
