#include "vector_div_scalar.cuh"

__global__ void vector_div_scalar(double* D, int N, float omega) {
    unsigned int row = blockIdx.x * blockDim.x + threadIdx.x;
    if (row >= N) {
        return;
    }
    D[row] = omega / D[row];
}
