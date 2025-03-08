#ifndef STD_CRS_MATRIX_HPP
#define STD_CRS_MATRIX_HPP
#include <utility>
#include <vector>
#include <iostream>

#include "naive_matrix.hpp"
#include "std_matrix.hpp"


class StdCRSMatrix {
public:
    StdCRSMatrix(size_t num_rows, size_t num_cols, std::vector<double> data,
                 std::vector<size_t> col_index,
                 std::vector<size_t> row_ptr) : _num_rows(num_rows),
                                                _num_cols(num_cols),
                                                _data(std::move(data)),
                                                _col_index(std::move(col_index)),
                                                _row_ptr(std::move(row_ptr)) {
    }

    explicit StdCRSMatrix(StdMatrix &matrix) {
        _num_rows = matrix.rows();
        _num_cols = matrix.cols();

        _row_ptr.push_back(0); // Första index är alltid 0

        for (size_t i = 0; i < _num_rows; ++i) {
            for (size_t j = 0; j < matrix.cols(); ++j) {
                if (matrix(i, j) != 0) {
                    _data.push_back(matrix(i, j));
                    _col_index.push_back(j);
                }
            }
            _row_ptr.push_back(_data.size());
        }

    }

    explicit StdCRSMatrix(NaiveMatrix &matrix) {
        _num_rows = matrix.rows();
        _num_cols = matrix.cols();

        _row_ptr.push_back(0); // Första index är alltid 0

        for (size_t i = 0; i < _num_rows; ++i) {
            for (size_t j = 0; j < matrix.cols(); ++j) {
                if (matrix(i, j) != 0) {
                    _data.push_back(matrix(i, j));
                    _col_index.push_back(j);
                }
            }
            _row_ptr.push_back(_data.size());
        }
    }

    std::string str() {
        std::stringstream ss;
        for(size_t i = 0 ; i < _num_rows; ++i) {
            size_t lower = _row_ptr[i];
            size_t upper = _row_ptr[i+1];
            ss << i << "\t lower="<<lower << ", upper=" << upper << std::endl;
            for( size_t j = lower; j < upper; ++j) {
                ss  << i << "\t "<<i<<","<<this->_col_index[j] << ": " << this->_data[j] << ", " << std::endl;
            }
            ss << "============" << std::endl;
        }
        return  ss.str();
    }

private:
    size_t _num_rows;
    size_t _num_cols;
    std::vector<double> _data;
    std::vector<size_t> _col_index;
    std::vector<size_t> _row_ptr;
};

#endif
