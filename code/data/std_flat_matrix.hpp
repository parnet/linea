#pragma once

#include <sstream>
#include <string>
#include <vector>



class Std_Flat_Matrix {
public:
    Std_Flat_Matrix() = default;

    explicit Std_Flat_Matrix(size_t num_rows, size_t num_cols) : _num_rows(num_rows), _num_cols(num_cols) {
        _num_elements = _num_cols*num_rows;
        _data.resize(_num_elements);
    }

    Std_Flat_Matrix(const Std_Flat_Matrix &other) :_num_elements(other._num_elements){
        _num_elements = other._num_elements;
        _num_rows = other._num_rows;
        _num_cols = other._num_cols;
        _data.resize(_num_elements);
        for (size_t i = 0; i < _num_elements; ++i) {
            _data[i] = other._data[i];
        }
    }

    Std_Flat_Matrix(Std_Flat_Matrix &&other) noexcept {
        _num_elements = other._num_elements;
        _num_rows = other._num_rows;
        _num_cols = other._num_cols;
        std::swap(_data,other._data);
    }

    Std_Flat_Matrix &operator=(const Std_Flat_Matrix &other) {
        _num_elements = other._num_elements;
        _num_rows = other._num_rows;
        _num_cols = other._num_cols;
        _data.resize(_num_elements);
        for (size_t i = 0; i < _num_elements; ++i) {
            _data[i] = other._data[i];
        }
        return *this;
    }

    Std_Flat_Matrix &operator=(Std_Flat_Matrix &&other) noexcept {
        _num_elements = other._num_elements;
        _num_rows = other._num_rows;
        _num_cols = other._num_cols;
        std::swap(_data,other._data);
        return *this;
    }

    ~Std_Flat_Matrix() = default;

    double operator()(size_t row_index, size_t col_index) const {
        return _data[row_index * this->_num_rows+col_index];
    }

    double & operator()(size_t row_index, size_t col_index) {
        return _data[row_index * this->_num_rows+col_index];
    }

    size_t rows() const {
        return _num_rows;
    }

    size_t cols() const {
        return _num_cols;
    }

    size_t memory_size() const {
        size_t total = 0;
        total += sizeof(size_t)*3;
        total += sizeof(double)*_num_elements; // todo plus costs for std::linear_algebra?
        return total;
    }


    std::string str() {
        std::stringstream ss;
        for(size_t i = 0 ; i < _num_rows; ++i) {
            for( size_t j = 0; j < _num_cols; ++j) {
                ss << " " << this->operator()(i,j) ;
            }
            ss << std::endl;
        }
        return  ss.str();
    }

public:
    size_t _num_elements;
    size_t _num_rows;
    size_t _num_cols;
    std::vector<double> _data;
};
