#pragma once

#define CUDA_CHECK(cmd) {                                                           \
    cudaError_t error = cmd;                                                        \
    if(error != cudaSuccess) [[unlikely]]                                           \
    {                                                                               \
    std::cerr << "error: " << cudaGetErrorString(error) << " (" << error << ") at " \
              << __FILE__ << ":" << __LINE__ << "\n";                               \
    exit(EXIT_FAILURE);                                                             \
    }                                                                               \
}