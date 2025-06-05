#pragma once
#ifdef USE_CUDA
#include <cuda_runtime_api.h>
#include <iostream>

void check_cuda_device() {
    int device;
    cudaGetDevice(&device);

    cudaDeviceProp prop;
    cudaGetDeviceProperties(&prop, device);

    std::cout << "CUDA Device: " << prop.name << std::endl;
    std::cout << "Compute Capability: " << prop.major << "." << prop.minor << std::endl;
}

#endif