#pragma once
#include <cstdlib>





class C_Aligned_Vector {
public:
    C_Aligned_Vector() = default;

    constexpr size_t alignment = 64;

    explicit C_Aligned_Vector(size_t num_elements) : _num_elements(num_elements) {
        _data  = static_cast<double*>(std::aligned_alloc(alignment, _num_elements * sizeof(double)));


    }

    C_Aligned_Vector(const C_Aligned_Vector &other) :_num_elements(other._num_elements){
        _data  = static_cast<double*>(std::aligned_alloc(alignment, _num_elements * sizeof(double)));

        for (size_t i = 0; i < _num_elements; ++i) {
            _data[i] = other._data[i];
        }
    }


    C_Aligned_Vector &operator=(const C_Aligned_Vector &other) {
        if(this == &other) {
            return *this;
        }
        delete[] _data;
        _data = static_cast<double*>(std::aligned_alloc(alignment, _num_elements * sizeof(double)));;
        _num_elements = other._num_elements;
        for (size_t i = 0; i < _num_elements; ++i) {
            _data[i] = other._data[i];
        }
        return *this;
    }


    C_Aligned_Vector &operator=(C_Aligned_Vector &&other) noexcept {
        if(this == &other) {
            return *this;
        }
        this->_num_elements = other._num_elements;
        std::swap(_data, other._data);
        return *this;
    }

    ~C_Aligned_Vector() {
        delete[] _data;
    }



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
    double * _data;
};
