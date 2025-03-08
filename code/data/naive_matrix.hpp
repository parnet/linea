//
// Created by maro on 2025-03-06.
//

#ifndef DATA_NAIVE_MATRIX_HPP
#define DATA_NAIVE_MATRIX_HPP

class NaiveMatrix {
public:
    explicit NaiveMatrix(size_t num_rows, size_t num_cols) : _num_rows(num_rows), _num_cols(num_cols) {
        _num_elements = _num_cols*num_rows;
        data = new double[_num_elements];

    }

    NaiveMatrix(const NaiveMatrix &other) :_num_elements(other._num_elements){
        _num_cols = other._num_cols;
        _num_rows = other._num_rows;
        data = new double[_num_elements];
        for (size_t i = 0; i < _num_elements; ++i) {
            data[i] = other.data[i];
        }
    }

    ~NaiveMatrix() {
        delete[] data;
    }

    double operator()(size_t row_index, size_t col_index) const {
        return data[row_index * this->_num_rows+col_index];
    }

    double & operator()(size_t row_index, size_t col_index) {
        return data[row_index * this->_num_rows+col_index];
    }

    size_t rows() {
        return _num_rows;
    }

    size_t cols() {
        return _num_cols;
    }
private:
    size_t _num_elements;
    size_t _num_rows;
    size_t _num_cols;
    double * data;
};
#endif
