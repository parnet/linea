#pragma once
/*
#ifdef USE_CUDA
#include <cuda_runtime.h>
#include "data/crs_matrix.hpp"
#include "data/vector.hpp"

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
*/