

#define BLOCK_SIZE 256
#define NUM_STREAMS 8
#include <cstdio>


__global__ void zero_row_kernel(double* __restrict__ data, int cols, int target_row) {
    unsigned int tid = threadIdx.x + blockIdx.x * blockDim.x;
    int start = target_row * cols;

    if (tid < cols) {
        data[start + tid] = 0.0;
    }
}

void zero_row_kernel_launch(
    double* d_data, int cols, int target_row,
    cudaStream_t stream = nullptr){
    int threadsPerBlock = 256;
    int blocksPerGrid = (cols + threadsPerBlock - 1) / threadsPerBlock;

    zero_row_kernel<<<blocksPerGrid, threadsPerBlock,0 , stream>>>(d_data, cols, target_row);


    cudaError_t err = cudaGetLastError();
    if (err != cudaSuccess) {
        printf("CUDA kernel launch error: %s\n", cudaGetErrorString(err));
    }
}

__global__ void residual_crs_kernel(
    double* __restrict__ defect_values,
    int num_rows,
    const int* __restrict__ row_ptr,
    const int* __restrict__ col_idx,
    const double* __restrict__ values,
    const double* __restrict__ x,
    const double* __restrict__ rhs_values
    ) {
    const unsigned int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i >= num_rows) return;

    double Ax_i = 0.0;
    int row_start = row_ptr[i];
    int row_end = row_ptr[i+1];

    for (int j = row_start; j < row_end; j++) {
        Ax_i += values[j] * x[col_idx[j]];
    }

    defect_values[i] = rhs_values[i] - Ax_i;

}

 void residual_crs_kernel_launch(
    double* defect_values,
    int num_rows,
    const int* matrix_row_ptr,
    const int* matrix_col_idx,
    const double* matrix_a_values,
    const double* x_values,
    const double* rhs_values,
    cudaStream_t stream = nullptr){
    int threadsPerBlock = 256;
    int blocksPerGrid = (num_rows + threadsPerBlock - 1) / threadsPerBlock;

    residual_crs_kernel<<<blocksPerGrid, threadsPerBlock,0,stream>>>(defect_values,num_rows, matrix_row_ptr, matrix_col_idx, matrix_a_values, x_values, rhs_values);

    cudaError_t err = cudaGetLastError();
    if (err != cudaSuccess) {
        printf("CUDA kernel launch error: %s\n", cudaGetErrorString(err));
    }
}





__global__ void norm2_partial_sum(const double* x, double* block_sums, int N) {
    __shared__ double sdata[BLOCK_SIZE];
    int tid = threadIdx.x;
    int idx = blockIdx.x * blockDim.x + tid;

    double val = 0.0;
    if (idx < N) val = x[idx] * x[idx];
    sdata[tid] = val;
    __syncthreads();

    for (int s = blockDim.x / 2; s > 0; s >>= 1) {
        if (tid < s) sdata[tid] += sdata[tid + s];
        __syncthreads();
    }

    if (tid == 0) block_sums[blockIdx.x] = sdata[0];
}


void reduce_norm_stream(const double* d_x, int N, double* d_result, cudaStream_t stream) {
    int n = N;
    const double* input = d_x;

    // Buffertar för partial sums (ping-pong)
    int max_blocks = (N + BLOCK_SIZE - 1) / BLOCK_SIZE;

    double* buffer_a = nullptr;
    double* buffer_b = nullptr;
    cudaMallocAsync(&buffer_a, max_blocks * sizeof(double), stream);
    cudaMallocAsync(&buffer_b, max_blocks * sizeof(double), stream);

    bool toggle = true; // styr vilken buffert som är output

    while (n > 1) {
        int blocks = (n + BLOCK_SIZE - 1) / BLOCK_SIZE;
        double* output = toggle ? buffer_b : buffer_a;

        // Kör kernel
        norm2_partial_sum<<<blocks, BLOCK_SIZE, 0, stream>>>(input, output, n);

        // Nästa steg använder output som input
        n = blocks;
        input = output;

        // Växla buffert
        toggle = !toggle;
    }



    // Kopiera slutresultatet (en scalar) till d_result
    cudaMemcpyAsync(d_result, input, sizeof(double), cudaMemcpyDeviceToDevice, stream);

    cudaFreeAsync(buffer_b, stream);
    cudaFreeAsync(buffer_a, stream);
}



void parallel_norm(double &norm, double *x, const int num_elem, cudaStream_t stream = nullptr) {
    double* d_partial_results[NUM_STREAMS];
    cudaStream_t streams[NUM_STREAMS];
    double h_partial_results[NUM_STREAMS];

    int chunk_size = (num_elem + NUM_STREAMS - 1) / NUM_STREAMS;
    //size=%d\n", num_elem, chunk_size);
    // Skapa streams och allokera minne för partial results
    for (int i = 0; i < NUM_STREAMS; i++) {
        cudaStreamCreate(&streams[i]);
        cudaMalloc(&d_partial_results[i], sizeof(double));
    }

    // Kör parallella reduktioner på delar av x
    for (int i = 0; i < NUM_STREAMS; i++) {
        int offset = i * chunk_size;
        int current_chunk = std::min(chunk_size, num_elem - offset);

        if (current_chunk > 0) {
            //printf("chunk=%d offset=%d\n", current_chunk, offset);
            reduce_norm_stream(x + offset, current_chunk, d_partial_results[i], streams[i]);

        } else {
            printf("norm memset case");
            // säkerhetskopiering om vi gått över gränsen
            cudaMemsetAsync(d_partial_results[i], 0, sizeof(double), streams[i]);
        }
    }

    // Synkronisera och samla resultat
    for (int i = 0; i < NUM_STREAMS; i++) {
        cudaStreamSynchronize(streams[i]);
        cudaMemcpy(&h_partial_results[i], d_partial_results[i], sizeof(double), cudaMemcpyDeviceToHost);
    }

    // Summera och ta roten
    double sum = 0;
    for (int i = 0; i < NUM_STREAMS; i++) {
        sum += h_partial_results[i];
    }
    norm = sqrt(sum);

    // Rensa upp
    for (int i = 0; i < NUM_STREAMS; i++) {
        cudaFree(d_partial_results[i]);
        cudaStreamDestroy(streams[i]);
    }
}



__global__ void jacobi_step_kernel(
    double* __restrict__ x_new,
    const double* __restrict__ x_old,
    const double* __restrict__ omega_D_inv,// omega * D^{-1} precomputed
    const double* __restrict__ defect,
    int N)
{
    unsigned int row = blockIdx.x * blockDim.x + threadIdx.x;
    if (row >= N) return;

    x_new[row] = x_old[row] + (omega_D_inv[row]*defect[row]);
}


void jacobi_step_launch(
    double* x_new,
    const double* x_old,
    const double* omega_D_inv,
    const double* defect,
    int num_rows,
    cudaStream_t stream  // default: stream 0 (default stream)
) {
    constexpr int blockSize = 256;
    const int gridSize = (num_rows + blockSize - 1) / blockSize;

    jacobi_step_kernel<<<gridSize, blockSize, 0, stream>>>(x_new, x_old, omega_D_inv, defect, num_rows);
}
