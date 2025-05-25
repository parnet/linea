
#ifndef CUDA_DATASTRUCTURES_HPP
#define CUDA_DATASTRUCTURES_HPP

#include <cuda_runtime.h>
#include "data/crs_matrix.hpp"
#include "data/vector.hpp"


class CUDA_CRS_Matrix {
    public:

    double *data = nullptr;
    int *d_row_ptr = nullptr;
    int *d_col_idx = nullptr;
    int _num_rows;
    int _num_cols;

    cudaStream_t stream = nullptr;


    CUDA_CRS_Matrix(CRS_Matrix &A) {
        cudaStreamCreate(&stream);
        this->_num_rows = A._num_rows;
        this->_num_cols = A._num_cols;

        cudaMallocAsync(&d_row_ptr, A._row_ptr.size() * sizeof(int), stream);
        cudaMallocAsync(&d_col_idx, A._col_index.size() * sizeof(int), stream);
        cudaMallocAsync(&data, A._data.size() * sizeof(double), stream);

        cudaMemcpyAsync(d_row_ptr, A._row_ptr.data(), A._row_ptr.size()*sizeof(int), cudaMemcpyHostToDevice, stream);
        cudaMemcpyAsync(d_col_idx, A._col_index.data(), A._col_index.size()*sizeof(int), cudaMemcpyHostToDevice, stream);
        cudaMemcpyAsync(data, A._data.data(), A._data.size()*sizeof(double), cudaMemcpyHostToDevice, stream);
    }

    CRS_Matrix get_matrix() {

        auto data = std::vector<double>(this->_num_rows, 0.0);
        auto col_idx = std::vector<int>(this->_num_rows, 0);
        auto row_ptr = std::vector<int>(this->_num_rows, 0);

        cudaMemcpy(row_ptr.data(), d_row_ptr, (this->_num_rows + 1) * sizeof(int), cudaMemcpyDeviceToHost);
        cudaMemcpy(col_idx.data(), d_col_idx, this->_num_rows * sizeof(int), cudaMemcpyDeviceToHost);
        cudaMemcpy(data.data(), this->data, this->_num_rows * sizeof(double), cudaMemcpyDeviceToHost);

        CRS_Matrix result = CRS_Matrix(this->_num_rows,this->_num_cols,data,col_idx,row_ptr);
        return result;
    }
};

class CUDA_Vector {
public:
    double *data = nullptr;
    cudaStream_t stream = nullptr;
    int _num_rows;
public:
    CUDA_Vector(Vector &v) {
        this->_num_rows = v.size();

        cudaStreamCreate(&stream);
        cudaMallocAsync(&data, v._data.size() * sizeof(double), stream);
        cudaMemcpyAsync(data, v._data.data(), v._data.size()*sizeof(double), cudaMemcpyHostToDevice, stream);
    }

    CUDA_Vector(int N) {
        this->_num_rows = N;

        cudaStreamCreate(&stream);
        cudaMallocAsync(&data, N * sizeof(double), stream);
    }

    void swap(CUDA_Vector & other) noexcept {
        std::swap(this->_num_rows, other._num_rows);
        std::swap(this->data, other.data);
        std::swap(this->stream, other.stream);
    }

    Vector get_vector() {

        auto data = std::vector<double>(this->_num_rows, 0.0);


        cudaMemcpy(data.data(), this->data, this->_num_rows * sizeof(double), cudaMemcpyDeviceToHost);

        Vector result = Vector(data);
        return result;
    }
};

#endif