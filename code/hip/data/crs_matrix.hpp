#pragma once
#ifdef USE_HIP

#include <hip/hip_runtime_api.h>

#include "data/crs_matrix.hpp"
#include "data/vector.hpp"

class HIP_CRS_Matrix {
public:

    double *data = nullptr;
    int *d_row_ptr = nullptr;
    int *d_col_idx = nullptr;
    int _num_rows;
    int _num_cols;

    hipStream_t stream = nullptr;


    HIP_CRS_Matrix(CRS_Matrix &A) {
        hipStreamCreate(&stream);
        this->_num_rows = A._num_rows;
        this->_num_cols = A._num_cols;

        hipMallocAsync(&d_row_ptr, A._row_ptr.size() * sizeof(int), stream);
        hipMallocAsync(&d_col_idx, A._col_index.size() * sizeof(int), stream);
        hipMallocAsync(&data, A._data.size() * sizeof(double), stream);

        hipMemcpyAsync(d_row_ptr, A._row_ptr.data(), A._row_ptr.size()*sizeof(int), hipMemcpyHostToDevice, stream);
        hipMemcpyAsync(d_col_idx, A._col_index.data(), A._col_index.size()*sizeof(int), hipMemcpyHostToDevice, stream);
        hipMemcpyAsync(data, A._data.data(), A._data.size()*sizeof(double), hipMemcpyHostToDevice, stream);
    }

    CRS_Matrix get_matrix() {

        auto data = std::vector<double>(this->_num_rows, 0.0);
        auto col_idx = std::vector<int>(this->_num_rows, 0);
        auto row_ptr = std::vector<int>(this->_num_rows, 0);

        hipMemcpy(row_ptr.data(), d_row_ptr, (this->_num_rows + 1) * sizeof(int), hipMemcpyDeviceToHost);
        hipMemcpy(col_idx.data(), d_col_idx, this->_num_rows * sizeof(int), hipMemcpyDeviceToHost);
        hipMemcpy(data.data(), this->data, this->_num_rows * sizeof(double), hipMemcpyDeviceToHost);

        CRS_Matrix result = CRS_Matrix(this->_num_rows,this->_num_cols,data,col_idx,row_ptr);
        return result;
    }
};

#endif