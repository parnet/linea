#pragma once

#include "../data/c_matrix.hpp"
#include "../data/std_matrix.hpp"
#include "../data/matrix.hpp"
#include "../data/std_crs_matrix.hpp"


CRS_Matrix std_crs_from_matrix(StdMatrix &matrix) {
    size_t _num_rows = matrix.rows();
    size_t _num_cols = matrix.cols();
    std::vector<size_t> _row_ptr;
    std::vector<size_t> _col_index;
    std::vector<double > _data;

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
    return CRS_Matrix(_num_rows,_num_cols,_data,_col_index,_row_ptr);
}

CRS_Matrix std_crs_from_matrix(NaiveMatrix &matrix) {
    size_t _num_rows = matrix.rows();
    size_t _num_cols = matrix.cols();
    std::vector<size_t> _row_ptr;
    std::vector<size_t> _col_index;
    std::vector<double > _data;
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
    return CRS_Matrix(_num_rows,_num_cols,_data,_col_index,_row_ptr);
}


CRS_Matrix std_crs_from_matrix(Matrix &matrix) {
    size_t _num_rows = matrix.rows();
    size_t _num_cols = matrix.cols();
    std::vector<size_t> _row_ptr;
    std::vector<size_t> _col_index;
    std::vector<double > _data;

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
    return CRS_Matrix(_num_rows,_num_cols,_data,_col_index,_row_ptr);
}

