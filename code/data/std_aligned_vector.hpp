#pragma once
#include <string>
#include <vector>




class Std_Aligned_Vector {
public:
    Std_Aligned_Vector() = default;

    explicit Std_Aligned_Vector(size_t num_elements) : _num_elements(num_elements) {
        _data.resize(num_elements);

    }

    Std_Aligned_Vector(const Std_Aligned_Vector &other) :_num_elements(other._num_elements){
        _data.resize(_num_elements);
        for (size_t i = 0; i < _num_elements; ++i) {
            _data[i] = other._data[i];
        }
    }

    Std_Aligned_Vector(Std_Aligned_Vector &&other){
        this->_num_elements = other._num_elements;
        std::swap(_data, other._data);
    }

    Std_Aligned_Vector &operator=(const Std_Aligned_Vector &other) {
        _data.resize(_num_elements);
        for (size_t i = 0; i < _num_elements; ++i) {
            _data[i] = other._data[i];
        }
        return *this;
    }


    Std_Aligned_Vector &operator=(Std_Aligned_Vector &&other) noexcept {
        this->_num_elements = other._num_elements;
        std::swap(_data, other._data);
        return *this;
    }

    ~Std_Aligned_Vector() = default;

    double operator[](size_t index) const {
        return _data[index];
    }

    double & operator[](size_t index) {
        return _data[index];
    }

    size_t size() const {
        return _num_elements;
    }

    size_t memory_size() const {
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
    size_t _num_elements{};
    std::vector<double> _data;
};

