

#define BLOCK_SIZE 256
#define NUM_STREAMS 8


__global__ void zero_row_kernel(double* x, int row_idx, int cols) {
    int col = blockIdx.x * blockDim.x + threadIdx.x;
    if (col < cols) {
        x[row_idx * cols + col] = 0.0;
    }
}


__global__ void residual_crs_kernel(
    int local_rows,
    const int* __restrict__ row_ptr,
    const int* __restrict__ col_idx,
    const double* __restrict__ values,
    const double* __restrict__ x,
    const double* __restrict__ b,
    double* d)
{
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i >= local_rows) return;

    double Ax_i = 0.0;
    int row_start = row_ptr[i];
    int row_end = row_ptr[i+1];

    for (int j = row_start; j < row_end; j++) {
        Ax_i += values[j] * x[col_idx[j]];
    }

    d[i] = b[i] - Ax_i;

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
    double* partial_sums = nullptr;

    // Allokera max blocks
    int max_blocks = (N + BLOCK_SIZE - 1) / BLOCK_SIZE;
    cudaMallocAsync(&partial_sums, max_blocks * sizeof(double), stream);

    while (n > 1) {
        int blocks = (n + BLOCK_SIZE - 1) / BLOCK_SIZE;
        norm2_partial_sum<<<blocks, BLOCK_SIZE, 0, stream>>>(input, partial_sums, n);
        cudaStreamSynchronize(stream);

        n = blocks;
        input = partial_sums;
    }

    // Kopiera slutresultatet (en scalar) till d_result
    cudaMemcpyAsync(d_result, input, sizeof(double), cudaMemcpyDeviceToDevice, stream);

    cudaFreeAsync(partial_sums, stream);
}




void cu_parallel_norm(double &norm, const int N) {
    double* d_x[NUM_STREAMS];
    double* d_partial_results[NUM_STREAMS];
    cudaStream_t streams[NUM_STREAMS];
    double h_partial_results[NUM_STREAMS];

    // Initiera streams och allokera minne
    for (int i = 0; i < NUM_STREAMS; i++) {
        cudaStreamCreate(&streams[i]);
        cudaMalloc(&d_x[i], N * sizeof(double));
        cudaMalloc(&d_partial_results[i], sizeof(double));
        // TODO: kopiera data till d_x[i] ...
    }

    // Kör parallella reduktioner
    for (int i = 0; i < NUM_STREAMS; i++) {
        reduce_norm_stream(d_x[i], N, d_partial_results[i], streams[i]);
    }

    // Synkronisera alla streams
    for (int i = 0; i < NUM_STREAMS; i++) {
        cudaStreamSynchronize(streams[i]);
    }

    // Kopiera partial sums till host
    for (int i = 0; i < NUM_STREAMS; i++) {
        cudaMemcpy(&h_partial_results[i], d_partial_results[i], sizeof(double), cudaMemcpyDeviceToHost);
    }

    // Slutlig norm
    double sum = 0;
    for (int i = 0; i < NUM_STREAMS; i++) {
        sum += h_partial_results[i];
    }
    double norm = sqrt(sum);


    // Frigör resurser
    for (int i = 0; i < NUM_STREAMS; i++) {
        cudaFree(d_x[i]);
        cudaFree(d_partial_results[i]);
        cudaStreamDestroy(streams[i]);
    }

}

