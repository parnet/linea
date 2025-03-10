//
// Created by maro on 2025-03-06.
//

#ifndef DATA_NAIVE_MATRIX_HPP
#define DATA_NAIVE_MATRIX_HPP

class NaiveMatrix {
public:
    NaiveMatrix() {
    }

    explicit NaiveMatrix(size_t num_rows, size_t num_cols) : _num_rows(num_rows), _num_cols(num_cols) {
        //std::cout << "NaiveMatrix::Constructor(size_t,size_t)" << std::endl;
        _num_elements = _num_cols*_num_rows;
        //std::cout << "size(A)="<< _num_elements << std::endl;
        data = new double[_num_elements];

    }

    NaiveMatrix(const NaiveMatrix &other) :_num_elements(other._num_elements){
        //std::cout << "NaiveMatrix::CopyConstructor(const NaiveMatrix&)" << std::endl;
        _num_cols = other._num_cols;
        _num_rows = other._num_rows;
        data = new double[_num_elements];
        //std::cout << "size(A)="<< _num_elements << " # copy"<< std::endl;
        for (size_t i = 0; i < _num_elements; ++i) {
            data[i] = other.data[i];
        }
    }

    NaiveMatrix(NaiveMatrix &&other) {
        std::cout << "NaiveMatrix::NaiveMatrix(const NaiveMatrix&&)" << std::endl;
        if (data != nullptr) {
            delete[] data;
        }
        _num_elements = other._num_elements;
        _num_cols = other._num_cols;
        _num_rows = other._num_rows;

        data = other.data;
        other.data = nullptr;

    }

    NaiveMatrix &operator=(const NaiveMatrix &other) {
        //std::cout << "NaiveMatrix::operator=(const NaiveMatrix&)" << std::endl;
        if (this == &other) {return *this;}

        _num_elements = other._num_elements;

        _num_cols = other._num_cols;
        _num_rows = other._num_rows;
        //std::cout << _num_elements << std::endl;
        if (data != nullptr) {
            delete[] data;
        }
        data = new double[_num_elements];
        //std::cout << "size(A)="<< _num_elements << " # assignment"<< std::endl;
        for (size_t i = 0; i < _num_elements; ++i) {
            data[i] = other.data[i];
        }
        //std::cout << "finished" << std::endl;
        return *this;
    }

    NaiveMatrix &operator=(NaiveMatrix &&other) {
        std::cout << "NaiveMatrix::operator=(const NaiveMatrix&&)" << std::endl;
        if (this == &other) {return *this;}
        if (data != nullptr) {
            delete[] data;
        }
        _num_elements = other._num_elements;
        _num_cols = other._num_cols;
        _num_rows = other._num_rows;

        data = other.data;
        other.data = nullptr;

        return *this;
    }


    ~NaiveMatrix() {
        //std::cout << "Destructor(NaiveMatrix)" << std::endl;
        if (data != nullptr) {
            delete[] data;
        }
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

    size_t memory_size() {
        size_t total = 0;
        total += sizeof(size_t)*3;
        total += sizeof(double)*_num_elements;
        return total;
    }

public:
    size_t _num_elements;
    size_t _num_rows;
    size_t _num_cols;
    double * data = nullptr;
};
#endif
