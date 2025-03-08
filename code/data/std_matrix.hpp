#ifndef DATA_STD_MATRIX_HPP
#define DATA_STD_MATRIX_HPP
#include <vector>
#include <sstream>

class StdMatrix {
public:
    explicit StdMatrix(size_t num_rows, size_t num_cols) : _num_rows(num_rows), _num_cols(num_cols) {
        _num_elements = _num_cols*num_rows;
        _data.resize(_num_rows);
        for(size_t i = 0 ; i < _num_rows; ++i) {
            _data.emplace_back();
            _data[i].resize(_num_cols);
        }

    }

    StdMatrix(const StdMatrix &other) :_num_elements(other._num_elements){
        _num_elements = other._num_elements;
        _num_rows = other._num_rows;
        _num_cols = other._num_cols;
        _data.resize(_num_elements);
        for (size_t i = 0; i < _num_elements; ++i) {
            _data[i] = other._data[i];
        }
    }

    ~StdMatrix() = default;

    //double operator()(size_t row_index, size_t col_index) const {
    //    return data[row_index][col_index];
    //}

    double & operator()(size_t row_index, size_t col_index) {
        return _data[row_index][col_index];
    }

    double operator()(size_t row_index, size_t col_index) const {
        return _data[row_index][col_index];
    }
    size_t rows() {
        return _num_rows;
    }

    size_t cols() {
        return _num_cols;
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
    std::vector<std::vector<double>> _data;
};


#endif
