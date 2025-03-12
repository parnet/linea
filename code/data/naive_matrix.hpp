//
// Created by maro on 2025-03-06.
//

#ifndef DATA_NAIVE_MATRIX_HPP
#define DATA_NAIVE_MATRIX_HPP
#include <sstream>

class NaiveMatrix {
public:
    NaiveMatrix() = default;

    explicit NaiveMatrix(size_t num_rows, size_t num_cols) : _num_rows(num_rows), _num_cols(num_cols) {
        //std::cout << "NaiveMatrix::Constructor(size_t,size_t)" << std::endl;
        _num_elements = _num_cols*_num_rows;
        //std::cout << "size(A)="<< _num_elements << std::endl;
        //_data = new double[_num_elements];
        //_data = static_cast<double *>(malloc(_num_elements * sizeof(double)));
        size_t alignment = 64;
        _data  = static_cast<double*>(std::aligned_alloc(alignment, _num_elements * sizeof(double)));

    }

    NaiveMatrix(const NaiveMatrix &other) :_num_elements(other._num_elements){
        //std::cout << "NaiveMatrix::CopyConstructor(const NaiveMatrix&)" << std::endl;
        size_t alignment =  64;
        _num_cols = other._num_cols;
        _num_rows = other._num_rows;
        //_data = new double[_num_elements];
        // _data  = static_cast<double *>(malloc(_num_elements * sizeof(double)));
        _data  = static_cast<double*>(std::aligned_alloc(alignment, _num_elements * sizeof(double)));
        //std::cout << "size(A)="<< _num_elements << " # copy"<< std::endl;
        for (size_t i = 0; i < _num_elements; ++i) {
            _data[i] = other._data[i];
        }
    }

    NaiveMatrix(NaiveMatrix &&other) {
        //std::cout << "NaiveMatrix::NaiveMatrix(const NaiveMatrix&&)" << std::endl;
        if (_data != nullptr) {
            delete[] _data;
        }
        _num_elements = other._num_elements;
        _num_cols = other._num_cols;
        _num_rows = other._num_rows;

        _data = other._data;
        other._data = nullptr;

    }

    NaiveMatrix &operator=(const NaiveMatrix &other) {
        size_t alignment = 64;
        //std::cout << "NaiveMatrix::operator=(const NaiveMatrix&)" << std::endl;
        if (this == &other) {return *this;}

        _num_elements = other._num_elements;

        _num_cols = other._num_cols;
        _num_rows = other._num_rows;
        //std::cout << _num_elements << std::endl;

        delete[] _data;

        //_data = new double[_num_elements];
        //_data = static_cast<double *>(malloc(_num_elements * sizeof(double)));
        _data  = static_cast<double*>(std::aligned_alloc(alignment, _num_elements * sizeof(double)));
        //std::cout << "size(A)="<< _num_elements << " # assignment"<< std::endl;
        for (size_t i = 0; i < _num_elements; ++i) {
            _data[i] = other._data[i];
        }
        //std::cout << "finished" << std::endl;
        return *this;
    }

    NaiveMatrix &operator=(NaiveMatrix &&other) noexcept{
        //std::cout << "NaiveMatrix::operator=(const NaiveMatrix&&)" << std::endl;
        if (this == &other) {return *this;}
        if (_data != nullptr) {
            delete[] _data;
        }
        _num_elements = other._num_elements;
        _num_cols = other._num_cols;
        _num_rows = other._num_rows;

        _data = other._data;
        other._data = nullptr;

        return *this;
    }

    ~NaiveMatrix() {
        //std::cout << "Destructor(NaiveMatrix)" << std::endl;

        delete[] _data;

    }



    double operator()(size_t row_index, size_t col_index) const {
        return _data[row_index * this->_num_rows+col_index];
    }

    double & operator()(size_t row_index, size_t col_index) {
        return _data[row_index * this->_num_rows+col_index];
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
    double * _data = nullptr;
};
#endif
