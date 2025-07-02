#ifndef CRS_MATRIX_HPP
#define CRS_MATRIX_HPP
#include <cuda_runtime_api.h>
#include <iostream>
#include <mpi.h>
#include <utility>
#include <vector>

#include "data/data.hpp"
#include "../distribution.hpp"


class Vector {
public:
    int _num_elements;
    std::vector<double> _values;

    Vector() { }
    Vector(int num_elements) : _num_elements(num_elements) {
        _values.resize(num_elements);
    }

    Vector(std::vector<double> values) {
        _num_elements = _values.size();
        _values = std::move(values);
    }
    Vector(const Vector &other)  :_num_elements(other._num_elements) {
        _values.resize(_num_elements);
        for (int i = 0; i < _num_elements; ++i) {
            _values[i] = other._values[i];
        }
    }

    void swap(Vector &other){
        std::cout << "todo implement swap" << std::endl;
    }

    size_t memory_size() {
        size_t total = 0;
        total += sizeof(int)*1;
        total += sizeof(double)*_num_elements;
        return total;
    }

};

class X_Vector {
public:
    int _num_rows;
    double * _values;
    cudaStream_t stream = nullptr;

    X_Vector() {
        std::cout << "todo implement X_Vector " << std::endl;
    }

    X_Vector(const Vector &vector) {
        this->_num_rows = vector._num_elements;

        cudaStreamCreate(&stream);
        cudaMallocAsync((void**)&_values, vector._values.size() * sizeof(double), stream);
        cudaMemcpyAsync(_values, vector._values.data(), vector._values.size()*sizeof(double), cudaMemcpyHostToDevice, stream);
    }

    void swap(X_Vector &other) {
        std::swap(this->_num_rows, other._num_rows);
        std::swap(this->_values, other._values);
        std::swap(this->stream, other.stream);
    }

    size_t memory_size() {
        size_t total = 0;
        total += sizeof(int)*1;
        total += sizeof(double)*_num_rows;
        return total;
    }

    Vector get_vector() {
        cudaStreamSynchronize(this->stream);
        auto data = std::vector<double>(this->_num_rows, 0.0);
        cudaMemcpy(data.data(), this->_values, this->_num_rows * sizeof(double), cudaMemcpyDeviceToHost);

        Vector result = Vector(data);
        return result;
    }
};


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
        this->_col_index = col_index ;
        this->_values = values;
    }
    size_t memory_size() {
        size_t total = 0;
        total += sizeof(int)*2;
        total += _values.size() * sizeof(double);
        total += _col_index.size() * sizeof(int);
        total += _rows_pointer.size() * sizeof(int);
        return total;
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

class X_CRS_Matrix {
public:
    double *_d_values = nullptr;
    int *_d_row_ptr = nullptr;
    int *_d_col_idx = nullptr;
    int _num_rows;
    int _num_cols;

    cudaStream_t stream = nullptr;


    X_CRS_Matrix(CRS_Matrix &A) {
        cudaStreamCreate(&stream);
        this->_num_rows = A._num_rows;
        this->_num_cols = A._num_cols;

        cudaMallocAsync((void**)&_d_row_ptr, A._rows_pointer.size() * sizeof(int), stream);
        cudaMallocAsync((void**)&_d_col_idx, A._col_index.size() * sizeof(int), stream);
        cudaMallocAsync((void**)&_d_values, A._values.size() * sizeof(double), stream);

        cudaMemcpyAsync(_d_row_ptr, A._rows_pointer.data(), A._rows_pointer.size()*sizeof(int), cudaMemcpyHostToDevice, stream);
        cudaMemcpyAsync(_d_col_idx, A._col_index.data(), A._col_index.size()*sizeof(int), cudaMemcpyHostToDevice, stream);
        cudaMemcpyAsync(_d_values, A._values.data(), A._values.size()*sizeof(double), cudaMemcpyHostToDevice, stream);
    }

    CRS_Matrix get_matrix() {

        auto values = std::vector<double>(this->_num_rows, 0.0);
        auto col_idx = std::vector<int>(this->_num_rows, 0);
        auto row_ptr = std::vector<int>(this->_num_rows, 0);

        cudaMemcpy(row_ptr.data(), _d_row_ptr, (this->_num_rows + 1) * sizeof(int), cudaMemcpyDeviceToHost);
        cudaMemcpy(col_idx.data(), _d_col_idx, this->_num_rows * sizeof(int), cudaMemcpyDeviceToHost);
        cudaMemcpy(values.data(), this->_d_values, this->_num_rows * sizeof(double), cudaMemcpyDeviceToHost);

        CRS_Matrix result = CRS_Matrix(this->_num_rows, this->_num_cols,
                                     row_ptr, col_idx, values);
        return result;
    }

    size_t memory_size() {
        std::cout << "todo implement memory_size " << std::endl;
    }
};


class X_Local_CRS_Matrix {
public:
};


class X_Distributed_CRS_Matrix {
public:
};


#endif
