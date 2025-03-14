#ifndef DATA_C_ALIGNED_MATRIX_HPP
#define DATA_C_ALIGNED_MATRIX_HPP

#include <sstream>





class C_Aligned_Matrix {
public:
    C_Aligned_Matrix() = default;

    constexpr size_t alignment = 64;

    explicit C_Aligned_Matrix(size_t num_rows, size_t num_cols) : _num_rows(num_rows), _num_cols(num_cols) {
        _num_elements = _num_cols*_num_rows;
        _data  = static_cast<double*>(std::aligned_alloc(alignment, _num_elements * sizeof(double)));

    }

    C_Aligned_Matrix(const C_Aligned_Matrix &other) :_num_elements(other._num_elements){
        if (_data != nullptr) {
            delete[] _data;
        }
        _num_cols = other._num_cols;
        _num_rows = other._num_rows;
        _data  = static_cast<double*>(std::aligned_alloc(alignment, _num_elements * sizeof(double)));
        for (size_t i = 0; i < _num_elements; ++i) {
            _data[i] = other._data[i];
        }
    }

    C_Aligned_Matrix(C_Aligned_Matrix &&other) noexcept {
        if (_data != nullptr) {
            delete[] _data;
        }
        _num_elements = other._num_elements;
        _num_cols = other._num_cols;
        _num_rows = other._num_rows;

        _data = other._data;
        other._data = nullptr;

    }

    C_Aligned_Matrix &operator=(const C_Aligned_Matrix &other) {
        if (this == &other) {return *this;}

        _num_elements = other._num_elements;

        _num_cols = other._num_cols;
        _num_rows = other._num_rows;

        delete[] _data;

        _data  = static_cast<double*>(std::aligned_alloc(alignment, _num_elements * sizeof(double)));
        for (size_t i = 0; i < _num_elements; ++i) {
            _data[i] = other._data[i];
        }
        //std::cout << "finished" << std::endl;
        return *this;
    }

    C_Aligned_Matrix &operator=(C_Aligned_Matrix &&other) noexcept{
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


    ~C_Aligned_Matrix() {
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
    size_t _num_elements{};
    size_t _num_rows{};
    size_t _num_cols{};
    double * _data = nullptr;
};
#endif
