#pragma once
#include <utility>
#include <vector>
#include "std_matrix.hpp"



class Std_CRS_Matrix {
public:
    Std_CRS_Matrix() = default;

    // todo copy constructor
    // todo copy operator
    // todo move constructor
    // todo move operator
    // todo destructor

    Std_CRS_Matrix(int num_rows, int num_cols, std::vector<double> data,
                 std::vector<int> col_index,
                 std::vector<int> row_ptr) : _num_rows(num_rows),
                                                _num_cols(num_cols),
                                                _data(std::move(data)),
                                                _col_index(std::move(col_index)),
                                                _row_ptr(std::move(row_ptr)) {
    }

    size_t memory_size() {
        size_t total = 0;
        total += sizeof(int)*2;
        total += _data.size() * sizeof(double);
        total += _col_index.size() * sizeof(int);
        total += _row_ptr.size() * sizeof(int);
        return total;
    }


    std::string str() {
        std::stringstream ss;
        for(int i = 0 ; i < _num_rows; ++i) {
            int lower = _row_ptr[i];
            int upper = _row_ptr[i+1];
            ss << i << "\t lower="<<lower << ", upper=" << upper << std::endl;
            for( int j = lower; j < upper; ++j) {
                ss  << i << "\t "<<","<<this->_col_index[j] << ": " << this->_data[j] << ", " << std::endl;
            }
        }
        return  ss.str();
    }

public:
    int _num_rows;
    int _num_cols;
    std::vector<double> _data;
    std::vector<int> _col_index;
    std::vector<int> _row_ptr;
};

