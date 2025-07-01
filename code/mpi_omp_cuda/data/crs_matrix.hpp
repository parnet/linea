#ifndef CRS_MATRIX_HPP
#define CRS_MATRIX_HPP
#include <cuda_runtime_api.h>
#include <iostream>
#include <mpi.h>
#include <vector>

#include "data/crs_matrix.hpp"
#include "../distribution.hpp"


class CRS_Matrix {
public:
    std::vector<int> _rows_pointer;
    std::vector<int> _col_index;
    std::vector<double> _values;

    int _num_rows;
    int _num_cols;


    CRS_Matrix( int num_rows, int num_cols, const std::vector<int>& rows_pointer,
            const std::vector<int> &col_index,
            const std::vector<double> &values) {

        this->_num_rows = num_rows;
        this->_num_cols = num_cols;
        this->_rows_pointer = rows_pointer;
        this->_col_index = col_index;
        this->_values = values;
    }
};


class Global_CRS_Matrix {
public:
    std::vector<int> _rows_pointer;
    std::vector<int> _col_index;
    std::vector<double> _values;

    int _num_rows;
    int _num_cols;


    Global_CRS_Matrix( int num_rows, int num_cols, const std::vector<int>& rows_pointer,
            const std::vector<int> &col_index,
            const std::vector<double> &values) {

        this->_num_rows = num_rows;
        this->_num_cols = num_cols;
        this->_rows_pointer = rows_pointer;
        this->_col_index = col_index;
        this->_values = values;
    }
};


class Local_CRS_Matrix {
public:
    std::vector<int> _rows_pointer;
    std::vector<int> _col_index;
    std::vector<double> _values;

    int _num_rows; // global
    int _num_cols; // global

    int _index_offset; // local
    int _num_local_rows; // local

    int _comm_rank;
    int _comm_size;

    Local_CRS_Matrix(int num_rows, int num_cols, const std::vector<int>& rows_pointer,
            const std::vector<int> &col_index,
            const std::vector<double> &values, int comm_rank, int comm_size,
            int index_offset, int num_local_rows) {

        this->_num_rows = num_rows;
        this->_num_cols = num_cols;
        this->_rows_pointer = rows_pointer;
        this->_col_index = col_index;
        this->_values = values;

        this->_comm_rank = comm_rank;
        this->_comm_size = comm_size;

        this->_index_offset = index_offset;
        this->_num_local_rows = num_local_rows;
    }

    Local_CRS_Matrix(Global_CRS_Matrix & A, int comm_rank, int comm_size) {
        this->_num_rows = A._num_rows;
        this->_num_cols = A._num_cols;

        this->_comm_rank = comm_rank;
        this->_comm_size = comm_size;

        this->_index_offset = 0;
        this->_num_local_rows = 0;
        distribution_slice(this->_index_offset, this->_num_local_rows, A._num_rows, comm_rank, comm_size);
        std::cout << "rank: " << comm_rank <<  " from: " << this->_index_offset << " to: " << this->_index_offset+this->_num_local_rows-1 << std::endl;

        this->_rows_pointer = std::vector<int>();
        this->_col_index = std::vector<int>();
        this->_values = std::vector<double>();


        // extract matrix:
        this->_rows_pointer.push_back(0);
        int number_of_elements = 0;
        for (int i = this->_index_offset; i < this->_index_offset + this->_num_local_rows; i++) {
            std::cout << "row="<< i << std::endl;
            int col_start = A._rows_pointer[i];
            int col_stop = A._rows_pointer[i+1];

            for (int j = col_start; j < col_stop; j++) {
                number_of_elements += 1;
                this->_col_index.push_back(A._col_index[j]);
                this->_values.push_back(A._values[j]);
            }

            this->_rows_pointer.push_back(number_of_elements);
        }
    }
};


class Distributed_CRS_Matrix {
public:
};



class X_Global_CRS_Matrix {
public:
};


class X_Local_CRS_Matrix {
public:
};


class X_Distributed_CRS_Matrix {
public:
};


#endif
