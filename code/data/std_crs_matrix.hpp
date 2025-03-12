#ifndef DATA_STD_CRS_MATRIX_HPP
#define DATA_STD_CRS_MATRIX_HPP

#include <utility>
#include <vector>
#include "naive_matrix.hpp"
#include "std_matrix.hpp"


class StdCRSMatrix {
public:
    StdCRSMatrix() = default;

    StdCRSMatrix(size_t num_rows, size_t num_cols, std::vector<double> data,
                 std::vector<size_t> col_index,
                 std::vector<size_t> row_ptr) : _num_rows(num_rows),
                                                _num_cols(num_cols),
                                                _data(std::move(data)),
                                                _col_index(std::move(col_index)),
                                                _row_ptr(std::move(row_ptr)) {
    }

    size_t memory_size() {
        size_t total = 0;
        total += sizeof(size_t)*2;
        total += _data.size() * sizeof(double);
        total += _col_index.size() * sizeof(size_t);
        total += _row_ptr.size() * sizeof(size_t);
        return total;
    }


    std::string str() {
        std::stringstream ss;
        for(size_t i = 0 ; i < _num_rows; ++i) {
            size_t lower = _row_ptr[i];
            size_t upper = _row_ptr[i+1];
            ss << i << "\t lower="<<lower << ", upper=" << upper << std::endl;
            for( size_t j = lower; j < upper; ++j) {
                ss  << i << "\t "<<","<<this->_col_index[j] << ": " << this->_data[j] << ", " << std::endl;
            }
        }
        return  ss.str();
    }

public:
    size_t _num_rows;
    size_t _num_cols;
    std::vector<double> _data;
    std::vector<size_t> _col_index;
    std::vector<size_t> _row_ptr;
};

#endif
