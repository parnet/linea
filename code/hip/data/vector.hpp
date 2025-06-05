#pragma once
#include <hip/hip_runtime_api.h>
#include "data/vector.hpp"

class HIP_Vector {
public:
    double *data = nullptr;
    hipStream_t stream = nullptr;
    int _num_rows;
public:
    HIP_Vector(Vector &v) {
        this->_num_rows = v.size();

        hipStreamCreate(&stream);
        hipMallocAsync(&data, v._data.size() * sizeof(double), stream);
        hipMemcpyAsync(data, v._data.data(), v._data.size()*sizeof(double), hipMemcpyHostToDevice, stream);
    }

    HIP_Vector(int N) {
        this->_num_rows = N;

        hipStreamCreate(&stream);
        hipMallocAsync(&data, N * sizeof(double), stream);
    }

    void swap(HIP_Vector & other) noexcept {
        std::swap(this->_num_rows, other._num_rows);
        std::swap(this->data, other.data);
        std::swap(this->stream, other.stream);
    }

    Vector get_vector() {

        auto data = std::vector<double>(this->_num_rows, 0.0);


        hipMemcpy(data.data(), this->data, this->_num_rows * sizeof(double), hipMemcpyDeviceToHost);

        Vector result = Vector(data);
        return result;
    }
};

