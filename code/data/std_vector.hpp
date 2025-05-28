#ifndef DATA_STD_VECTOR_HPP
#define DATA_STD_VECTOR_HPP

#include <vector>





class Std_Vector {
public:
    Std_Vector() = default;

    explicit Std_Vector(int num_elements) : _num_elements(num_elements) {
        _data.resize(num_elements);

    }
    explicit Std_Vector(const std::vector<double> &data) : _num_elements(data.size()) {
        _data = data;

    }

    Std_Vector(const Std_Vector &other) :_num_elements(other._num_elements){
        _data.resize(_num_elements);
        for (int i = 0; i < _num_elements; ++i) {
            _data[i] = other._data[i];
        }
    }

    Std_Vector(Std_Vector &&other){
        this->_num_elements = other._num_elements;
        std::swap(_data, other._data);
    }

    Std_Vector &operator=(const Std_Vector &other) {
        _data.resize(_num_elements);
        for (int i = 0; i < _num_elements; ++i) {
            _data[i] = other._data[i];
        }
        return *this;
    }


    Std_Vector &operator=(Std_Vector &&other) noexcept {
        this->_num_elements = other._num_elements;
        std::swap(_data, other._data);
        return *this;
    }

    void swap(Std_Vector &other) noexcept {
        std::swap(_num_elements, other._num_elements);
        std::swap(_data, other._data);

    }

    ~Std_Vector() = default;

    double operator[](size_t index) const {
        return _data[index];
    }

    double & operator[](size_t index) {
        return _data[index];
    }

    int size() const {
        return _num_elements;
    }

    size_t memory_size() {
        size_t total = 0;
        total += sizeof(int)*1;
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
    int _num_elements;
    std::vector<double> _data;
};

#endif
