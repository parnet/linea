#include <rocsparse/rocsparse.h>
#include <hipsparse/hipsparse.h>
#include <hip/hip_runtime.h>
#include <hipsparse.h>


#include <omp.h>

using namespace std;
/// \brief Checks if the provided error code is \p hipSuccess and if not,
/// prints an error message to the standard error output and terminates the program
/// with an error code.
#define HIP_CHECK(condition)                                                                \
    {                                                                                       \
        const hipError_t error = condition;                                                 \
        if(error != hipSuccess)                                                             \
        {                                                                                   \
            std::cerr << "An error encountered: \"" << hipGetErrorString(error) << "\" at " \
                      << __FILE__ << ':' << __LINE__ << std::endl;                          \
            std::exit(-1);                                                                  \
        }                                                                                   \
    }


int main() {
    rocsparse_handle handle;
    rocsparse_create_handle(&handle);
    int version;
    rocsparse_get_version(handle, &version);
    std::cout << "version " << version << std::endl;
    rocsparse_set_pointer_mode(handle,rocsparse_pointer_mode_host);
    // alpha * ( 1.0  0.0  2.0 ) * ( 1.0 ) + beta * ( 4.0 ) = (  31.1 )
    //         ( 3.0  0.0  4.0 ) * ( 2.0 )          ( 5.0 ) = (  62.0 )
    //         ( 5.0  6.0  0.0 ) * ( 3.0 )          ( 6.0 ) = (  70.7 )
    //         ( 7.0  0.0  8.0 ) *                  ( 7.0 ) = ( 123.8 )

    int m = 4;
    int n = 3;
    int nnz = 8;

    // CSR row pointers
    int hcsr_row_ptr[5] = {0, 2, 4, 6, 8};

    // CSR column indices
    int hcsr_col_ind[8] = {0, 2, 0, 2, 0, 1, 0, 2};

    // CSR values
    float hcsr_val[8] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0};

    // Transposition of the matrix

    rocsparse_operation trans = rocsparse_operation_none;

    // Scalar alpha and beta
    float alpha = 3.7;
    float beta = 1.3;

    // x and y
    float hx[3] = {1.0, 2.0, 3.0};
    float hy[4] = {4.0, 5.0, 6.0, 7.0};

    // Matrix descriptor
    rocsparse_mat_descr descr;
    rocsparse_create_mat_descr(&descr);


    // Offload data to device
    int *dcsr_row_ptr;
    int *dcsr_col_ind;
    float *dcsr_val;
    float *dx;
    float *dy;


    HIP_CHECK(hipMalloc((void **)&dcsr_row_ptr, sizeof(int) * (m + 1)));
    HIP_CHECK(hipMalloc((void **)&dcsr_col_ind, sizeof(int) * nnz));
    HIP_CHECK(hipMalloc((void **)&dcsr_val, sizeof(float) * nnz));
    HIP_CHECK(hipMalloc((void **)&dx, sizeof(float) * n));
    HIP_CHECK(hipMalloc((void **)&dy, sizeof(float) * m));

    HIP_CHECK(hipMemcpy(dcsr_row_ptr, hcsr_row_ptr, sizeof(int) * (m + 1), hipMemcpyHostToDevice));
    HIP_CHECK(hipMemcpy(dcsr_col_ind, hcsr_col_ind, sizeof(int) * nnz, hipMemcpyHostToDevice));
    HIP_CHECK(hipMemcpy(dcsr_val, hcsr_val, sizeof(float) * nnz, hipMemcpyHostToDevice));
    HIP_CHECK(hipMemcpy(dx, hx, sizeof(float) * n, hipMemcpyHostToDevice));
    HIP_CHECK(hipMemcpy(dy, hy, sizeof(float) * m, hipMemcpyHostToDevice));
    for (int i = 0; i < 4; ++i) {
        std::cout << dy[i] << ' ';
    }
    // Call dcsrmv to perform y = alpha * A x + beta * y

  rocsparse_mat_info info;
  rocsparse_create_mat_info(&info);
  /*rocsparse_scsrmv_analysis(handle,
                             trans,
                             m,
                             n,
                             nnz,
                             descr,
                             dcsr_val[0],
                             dcsr_row_ptr[0],
                             dcsr_col_ind[0],
                             info);*/

  auto val = rocsparse_scsrmv(handle,
                             trans,
                             m,
                             n,
                             nnz,
                             &alpha,
                             descr,
                             dcsr_val,
                             dcsr_row_ptr,
                             dcsr_col_ind,
                             info,
                             dx,
                             &beta,
                             dy);

    std::cout << " status: " << val << std::endl;


    // Copy result back to host
    HIP_CHECK(hipMemcpy(hy, dy, sizeof(float) * m, hipMemcpyDeviceToHost));

    for (int i = 0; i < 4; ++i) {
        std::cout << dy[i] << ' ';
    }
    std::cout << std::endl;

    // Clear hipSPARSE
    //hipsparseDestroyMatDescr(descr);
    rocsparse_destroy_mat_descr(descr);
    //hipsparseDestroy(handle);
    rocsparse_destroy_handle(handle);

    // Clear device memory
    HIP_CHECK(hipFree(dcsr_row_ptr));
    HIP_CHECK(hipFree(dcsr_col_ind));
    HIP_CHECK(hipFree(dcsr_val));
    HIP_CHECK(hipFree(dx));
    HIP_CHECK(hipFree(dy));
    return 0;
}

int main()
  {
    int N           = 10240;
    int nnz         = 256;
    float alpha     = 10.0f;
    float tolerance = 1e-8f;

    std::vector<int> hx_ind(nnz);
    std::vector<float> hx_val(nnz);
    std::vector<float> hy(N);

    // Allocate memory on the device
    int* dx_ind;
    float* dx_val;
    float* dy;

    hipMalloc(&dx_ind, nnz * sizeof(int));
    hipMalloc(&dx_val, nnz * sizeof(float));
    hipMalloc(&dy, N * sizeof(float));

    // Initial Data on CPU,
    srand(1);

    for(int i = 0; i < nnz; ++i)
      {
        hx_ind[i] = i * 40;
        hx_val[i] = rand() % 10 + 1; // Generate an integer number between [1, 10]
      }

    for(int i = 0; i < N; ++i)
      {
        hy[i] = rand() % 10 + 1; // Generate an integer number between [1, 10]
      }

     // Copy data to device
     hipMemcpy(dx_ind, hx_ind.data(), sizeof(int) * nnz, hipMemcpyHostToDevice);
    hipMemcpy(dx_val, hx_val.data(), sizeof(float) * nnz, hipMemcpyHostToDevice);
    hipMemcpy(dy, hy.data(), sizeof(float) * N, hipMemcpyHostToDevice);

    // Initialize rocSPARSE
    hipsparseHandle_t handle;
    hipsparseCreate(&handle);

    // Run saxpyi on device
    hipsparseSaxpyi(handle, nnz, &alpha, dx_val, dx_ind, dy, HIPSPARSE_INDEX_BASE_ZERO);

    // Copy output from device memory to host memory
    std::vector<float> result(N);
    hipMemcpy(result.data(), dy, sizeof(float) * N, hipMemcpyDeviceToHost);

    // Verify hipsparseSaxpyi result
    for(int i = 0; i < nnz; ++i)
      {
        hy[hx_ind[i]] += alpha * hx_val[i];
      }

    float error;
    for(int i = 0; i < N; ++i)
      {
        error = fabs(hy[i] - result[i]);
        if(error > tolerance)
         {
           fprintf(stderr, "Error in element %d: CPU=%f, GPU=%f\n", i, hy[i], result[i]);
           break;
         }
      }

    if(error > tolerance)
      {
       printf("axpyi test failed!\n");
      }
    else
      {
      printf("axpyi test passed!\n");
      }

    hipFree(dx_ind);
    hipFree(dx_val);
    hipFree(dy);

    hipsparseDestroy(handle);

    return 0;
   }


#include <hip/hip_runtime.h>
#include <hipsparse/hipsparse.h>


int main(int argc, char *argv[]) {
    // Handle version override through command line argument
    std::string gfx_version = "11.0.0"; // Default
    if (argc > 1) {
        gfx_version = argv[1];
    }
    // Set the environment variable for HSA
    std::string env_var = "HSA_OVERRIDE_GFX_VERSION=" + gfx_version;
    putenv(const_cast<char*>(env_var.c_str())); // Set the environment variable


    int numRows = 4;
    int numCols = 3;
    int numNonZeros = 8;
    float alpha = 3.7f;
    float beta = 1.3f;

    std::vector<int> h_rowOffsets = {0, 2, 4, 6, 8}; //{0, 2, 3, 4};
    std::vector<int> h_colIndices = {0, 2, 0, 2, 0, 1, 0, 2}; //{0, 2, 1, 2};
    std::vector<float> h_values =  {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0};// {1.0f, 2.0f, 3.0f, 4.0f};
    std::vector<float> h_x = {1.0, 2.0, 3.0};
    std::vector<float> h_y = {4.0, 5.0, 6.0, 7.0};

    int *d_rowOffsets, *d_colIndices;
    float *d_values, *d_x, *d_y;

    if (hipMalloc(&d_rowOffsets, h_rowOffsets.size() * sizeof(int)) != hipSuccess ||
        hipMalloc(&d_colIndices, h_colIndices.size() * sizeof(int)) != hipSuccess ||
        hipMalloc(&d_values, h_values.size() * sizeof(float)) != hipSuccess ||
        hipMalloc(&d_x, h_x.size() * sizeof(float)) != hipSuccess ||
        hipMalloc(&d_y, h_y.size() * sizeof(float)) != hipSuccess) {
        std::cerr << "hipMalloc failed!" << std::endl;
        return -1;
    }

    if (hipMemcpy(d_rowOffsets, h_rowOffsets.data(), h_rowOffsets.size() * sizeof(int), hipMemcpyHostToDevice) != hipSuccess ||
        hipMemcpy(d_colIndices, h_colIndices.data(), h_colIndices.size() * sizeof(int), hipMemcpyHostToDevice) != hipSuccess ||
        hipMemcpy(d_values, h_values.data(), h_values.size() * sizeof(float), hipMemcpyHostToDevice) != hipSuccess ||
        hipMemcpy(d_x, h_x.data(), h_x.size() * sizeof(float), hipMemcpyHostToDevice) != hipSuccess ||
        hipMemcpy(d_y, h_y.data(), h_y.size() * sizeof(float), hipMemcpyHostToDevice) != hipSuccess) {
        std::cerr << "hipMemcpy failed!" << std::endl;
        return -1;
    }

    hipsparseHandle_t handle;
    hipsparseStatus_t status = hipsparseCreate(&handle);
    if (status != HIPSPARSE_STATUS_SUCCESS) {
        std::cerr << "hipsparseCreate failed!" << std::endl;
        return -1;
    }

    hipsparseMatDescr_t descr;

    status = hipsparseCreateMatDescr(&descr);
    //hipsparseSetMatType(descr, HIPSPARSE_MATRIX_TYPE_GENERAL);
    //hipsparseSetMatIndexBase(descr, HIPSPARSE_INDEX_BASE_ZERO);
    if (status != HIPSPARSE_STATUS_SUCCESS) {
        std::cerr << "hipsparseCreateMatDescr failed!" << std::endl;
        hipsparseDestroy(handle);
        return -1;
    }

    status = hipsparseScsrmv(handle, HIPSPARSE_OPERATION_NON_TRANSPOSE,
                             numRows, numCols, numNonZeros, &alpha,
                             descr, d_values, d_rowOffsets, d_colIndices,
                             d_x, &beta, d_y);
    if (status != HIPSPARSE_STATUS_SUCCESS) {
        std::cerr << "hipsparseScsrmv failed!" << std::endl;
        hipsparseDestroyMatDescr(descr);
        hipsparseDestroy(handle);
        return -1;
    }

    hipDeviceSynchronize();

    hipMemcpy(h_y.data(), d_y, h_y.size() * sizeof(float), hipMemcpyDeviceToHost);

    std::cout << "Resulting linear_algebra y:" << std::endl;
    for (auto val : h_y) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    hipsparseDestroyMatDescr(descr);
    hipsparseDestroy(handle);
    hipFree(d_rowOffsets);
    hipFree(d_colIndices);
    hipFree(d_values);
    hipFree(d_x);
    hipFree(d_y);

    return 0;
}


// main.cpp
#include <iostream>
#include <vector>
#include <hip/hip_runtime.h>
#include <hipsparse/hipsparse.h>
#include <iostream>
#include <vector>


int main(int argc, char *argv[]) {
    // Handle version override through command line argument
    std::string gfx_version = "11.0.0"; // Default
    if (argc > 1) {
        gfx_version = argv[1];
    }
    // Set the environment variable for HSA
    //std::string env_var = "HSA_OVERRIDE_GFX_VERSION=" + gfx_version;
    //putenv(const_cast<char*>(env_var.c_str())); // Set the environment variable


    int numRows = 4;
    int numCols = 3;
    int numNonZeros = 8;
    float alpha = 3.7f;
    float beta = 1.3f;

    std::vector<int> h_rowOffsets = {0, 2, 4, 6, 8}; //{0, 2, 3, 4};
    std::vector<int> h_colIndices = {0, 2, 0, 2, 0, 1, 0, 2}; //{0, 2, 1, 2};
    std::vector<float> h_values =  {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0};// {1.0f, 2.0f, 3.0f, 4.0f};
    std::vector<float> h_x = {1.0, 2.0, 3.0};
    std::vector<float> h_y = {4.0, 5.0, 6.0, 7.0};

    int *d_rowOffsets, *d_colIndices;
    float *d_values, *d_x, *d_y;

    if (hipMalloc(&d_rowOffsets, h_rowOffsets.size() * sizeof(int)) != hipSuccess ||
        hipMalloc(&d_colIndices, h_colIndices.size() * sizeof(int)) != hipSuccess ||
        hipMalloc(&d_values, h_values.size() * sizeof(float)) != hipSuccess ||
        hipMalloc(&d_x, h_x.size() * sizeof(float)) != hipSuccess ||
        hipMalloc(&d_y, h_y.size() * sizeof(float)) != hipSuccess) {
        std::cerr << "hipMalloc failed!" << std::endl;
        return -1;
    }

    if (hipMemcpy(d_rowOffsets, h_rowOffsets.data(), h_rowOffsets.size() * sizeof(int), hipMemcpyHostToDevice) != hipSuccess ||
        hipMemcpy(d_colIndices, h_colIndices.data(), h_colIndices.size() * sizeof(int), hipMemcpyHostToDevice) != hipSuccess ||
        hipMemcpy(d_values, h_values.data(), h_values.size() * sizeof(float), hipMemcpyHostToDevice) != hipSuccess ||
        hipMemcpy(d_x, h_x.data(), h_x.size() * sizeof(float), hipMemcpyHostToDevice) != hipSuccess ||
        hipMemcpy(d_y, h_y.data(), h_y.size() * sizeof(float), hipMemcpyHostToDevice) != hipSuccess) {
        std::cerr << "hipMemcpy failed!" << std::endl;
        return -1;
    }

    hipsparseHandle_t handle;
    hipsparseStatus_t status = hipsparseCreate(&handle);
    if (status != HIPSPARSE_STATUS_SUCCESS) {
        std::cerr << "hipsparseCreate failed!" << std::endl;
        return -1;
    }

    hipsparseMatDescr_t descr;

    status = hipsparseCreateMatDescr(&descr);
    //hipsparseSetMatType(descr, HIPSPARSE_MATRIX_TYPE_GENERAL);
    //hipsparseSetMatIndexBase(descr, HIPSPARSE_INDEX_BASE_ZERO);
    if (status != HIPSPARSE_STATUS_SUCCESS) {
        std::cerr << "hipsparseCreateMatDescr failed!" << std::endl;
        hipsparseDestroy(handle);
        return -1;
    }

    status = hipsparseScsrmv(handle, HIPSPARSE_OPERATION_NON_TRANSPOSE,
                             numRows, numCols, numNonZeros, &alpha,
                             descr, d_values, d_rowOffsets, d_colIndices,
                             d_x, &beta, d_y);
    if (status != HIPSPARSE_STATUS_SUCCESS) {
        std::cerr << "hipsparseScsrmv failed!" << std::endl;
        hipsparseDestroyMatDescr(descr);
        hipsparseDestroy(handle);
        return -1;
    }

    hipDeviceSynchronize();

    hipMemcpy(h_y.data(), d_y, h_y.size() * sizeof(float), hipMemcpyDeviceToHost);

    std::cout << "Resulting linear_algebra y:" << std::endl;
    for (auto val : h_y) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    hipsparseDestroyMatDescr(descr);
    hipsparseDestroy(handle);
    hipFree(d_rowOffsets);
    hipFree(d_colIndices);
    hipFree(d_values);
    hipFree(d_x);
    hipFree(d_y);

    return 0;
}



#include <fstream>
#include <hip/hip_runtime.h>
#include <iostream>
#include <vector>

#include "util/timer.h"

constexpr int N = 8192; // Grid size (N x N)
constexpr int max_levels = 6; // Maximum number of levels for multigrid
constexpr int smoothing_steps = 3; // Number of smoothing steps (Jacobi)

__global__ void jacobi_relaxation(float* u, float* u_new, float* f, int N) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    int j = blockIdx.y * blockDim.y + threadIdx.y;

    if (i > 0 && i < N-1 && j > 0 && j < N-1) {
        int idx = i * N + j;
        u_new[idx] = 0.25 * (u[idx - 1] + u[idx + 1] + u[idx - N] + u[idx + N] - f[idx]);
    }
}

__global__ void compute_residual(float* u, float* f, float* residual, int N) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    int j = blockIdx.y * blockDim.y + threadIdx.y;

    if (i > 0 && i < N-1 && j > 0 && j < N-1) {
        int idx = i * N + j;
        residual[idx] = f[idx] - (4 * u[idx] - u[idx - 1] - u[idx + 1] - u[idx - N] - u[idx + N]);
    }
}

__global__ void restrict_grid(float* fine, float* coarse, int N_fine) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    int j = blockIdx.y * blockDim.y + threadIdx.y;
    int N_coarse = N_fine / 2;

    if (i > 0 && i < N_coarse - 1 && j > 0 && j < N_coarse - 1) {
        int fine_idx = 2 * i * N_fine + 2 * j;
        int coarse_idx = i * N_coarse + j;
        coarse[coarse_idx] = 0.25f * (fine[fine_idx] + fine[fine_idx + 1] +
                                     fine[fine_idx + N_fine] + fine[fine_idx + N_fine + 1]);
    }
}

__global__ void prolong_grid(float* coarse, float* fine, int N_fine) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    int j = blockIdx.y * blockDim.y + threadIdx.y;
    int N_coarse = N_fine / 2;

    if (i < N_fine && j < N_fine) {
        int fine_idx = i * N_fine + j;
        int coarse_i = i / 2, coarse_j = j / 2;
        int coarse_idx = coarse_i * N_coarse + coarse_j;
        fine[fine_idx] = coarse[coarse_idx];
    }
}

// Apply Jacobi relaxation
void apply_jacobi(float* u, float* u_new, float* f, int N) {
    dim3 dimBlock(16, 16);
    dim3 dimGrid((N + dimBlock.x - 1) / dimBlock.x, (N + dimBlock.y - 1) / dimBlock.y);

    for (int i = 0; i < smoothing_steps; ++i) {
        jacobi_relaxation<<<dimGrid, dimBlock>>>(u, u_new, f, N);
        std::swap(u, u_new);
    }
}
// Multigrid V-cycle with updated grid and block definitions
void multigrid_vcycle(float* u, float* f, float* residual, int level) {
    int N = ::N >> level;
    if (level == max_levels - 1) {
        apply_jacobi(u, u, f, N);
        return;
    }

    float *d_u_coarse, *d_f_coarse;
    int N_coarse = N / 2;
    std::cout << hipMalloc(&d_u_coarse, N_coarse * N_coarse * sizeof(float));
    std::cout << hipMalloc(&d_f_coarse, N_coarse * N_coarse * sizeof(float));

    dim3 dimBlock(16, 16);
    dim3 dimGrid((N + dimBlock.x - 1) / dimBlock.x, (N + dimBlock.y - 1) / dimBlock.y);

    apply_jacobi(u, u, f, N);
    compute_residual<<<dimGrid, dimBlock>>>(u, f, residual, N);

    // Adjust the grid dimensions for the coarse level
    dim3 dimGridCoarse((N_coarse + dimBlock.x - 1) / dimBlock.x, (N_coarse + dimBlock.y - 1) / dimBlock.y);
    restrict_grid<<<dimGridCoarse, dimBlock>>>(residual, d_f_coarse, N);

    std::cout << hipMemset(d_u_coarse, 0, N_coarse * N_coarse * sizeof(float));
    multigrid_vcycle(d_u_coarse, d_f_coarse, residual, level + 1);

    prolong_grid<<<dimGrid, dimBlock>>>(d_u_coarse, u, N);
    apply_jacobi(u, u, f, N);

    std::cout <<  hipFree(d_u_coarse);
    std::cout <<  hipFree(d_f_coarse);
}
void save_solution_to_file(const std::vector<float>& solution, int N) {
    std::ofstream file("solution.csv");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            file << solution[i * N + j];
            if (j < N - 1) file << ", ";
        }
        file << "\n";
    }
    file.close();
}


int gmg_main() {
    auto k_1 = 3.0;
    auto k_2 = 2.0;

    auto k_3 = 5.0;
    auto k_4 = 3.0;

    auto k_5 = 4.0;
    auto k_6 = 7.0;

    auto alpha = 1.0;
    auto alpha_2 = 1.7;
    auto p = 1.0;
    auto t = 0.5;

    std::vector<float> u_host(N * N);
    for(int i = 0; i < N; ++i) {
        for(int j = 0; j < N ; ++j) {
            auto x_1 = float(i)/float(N-1);
            auto x_2 = float(j)/float(N-1);

            u_host[i*N + j ] = (M_PI*p*sin(p*M_PI*t) - alpha*(k_1*k_1 + k_2*k_2)*M_PI*M_PI*cos(p*M_PI*t))*sin(k_1*M_PI*x_1)*sin(k_2*M_PI*x_2);
        }
    }

    Timer timer = Timer();
    timer.start();
    float *u, *f, *residual;

    std::cout <<  hipMalloc(&u, N * N * sizeof(float));
    std::cout <<  hipMalloc(&f, N * N * sizeof(float));
    std::cout <<  hipMalloc(&residual, N * N * sizeof(float));

    // Initialize f with the source term, and u as the initial guess.
    // Here, we'll use zero-initialization for simplicity.
    std::cout <<  hipMemset(u, 0, N * N * sizeof(float));
    //std::cout <<  hipMemset(f, 0, N * N * sizeof(float));
    std::cout << hipMemcpy( u, u_host.data(), N * N * sizeof(float),  hipMemcpyHostToDevice);

    // Apply the multigrid V-cycle
    multigrid_vcycle(u, f, residual, 0);


    std::cout << hipMemcpy(u_host.data(), u, N * N * sizeof(float), hipMemcpyDeviceToHost);
    save_solution_to_file(u_host, N);
    auto freeu = hipFree(u);
    auto freef = hipFree(f);
    auto freer = hipFree(residual);
    timer.stop();

    auto difference = timer.get();
    std::cout << std::endl;
    std::cout << N << " ; Multigrid solution completed in ; " << difference << std::endl;
    return 0;
}




void spmv_hipsparse(int num_rows, int num_cols, int nnz, const int *h_row_ptr,
                     const int *h_col_idx, const float *h_values, const float *h_x, float *h_y) {
    // HIP sparse handle
    hipsparseHandle_t handle;
    hipsparseCreate(&handle);

    // Allocate device memory
    int *d_row_ptr, *d_col_idx;
    float *d_values, *d_x, *d_y;
    hipMalloc(&d_row_ptr, (num_rows + 1) * sizeof(int));
    hipMalloc(&d_col_idx, nnz * sizeof(int));
    hipMalloc(&d_values, nnz * sizeof(float));
    hipMalloc(&d_x, num_cols * sizeof(float));
    hipMalloc(&d_y, num_rows * sizeof(float));

    // Copy data to device
    hipMemcpy(d_row_ptr, h_row_ptr, (num_rows + 1) * sizeof(int), hipMemcpyHostToDevice);
    hipMemcpy(d_col_idx, h_col_idx, nnz * sizeof(int), hipMemcpyHostToDevice);
    hipMemcpy(d_values, h_values, nnz * sizeof(float), hipMemcpyHostToDevice);
    hipMemcpy(d_x, h_x, num_cols * sizeof(float), hipMemcpyHostToDevice);
    hipMemset(d_y, 0, num_rows * sizeof(float));

    // Define descriptor
    hipsparseMatDescr_t descr;
    hipsparseCreateMatDescr(&descr);
    hipsparseSetMatType(descr, HIPSPARSE_MATRIX_TYPE_GENERAL);
    hipsparseSetMatIndexBase(descr, HIPSPARSE_INDEX_BASE_ZERO);

    // Perform SpMV
    float alpha = 1.0f, beta = 0.0f;
    hipsparseScsrmv(handle, HIPSPARSE_OPERATION_NON_TRANSPOSE, num_rows, num_cols, nnz,
                    &alpha, descr, d_values, d_row_ptr, d_col_idx, d_x, &beta, d_y);

    // Copy result back
    hipMemcpy(h_y, d_y, num_rows * sizeof(float), hipMemcpyDeviceToHost);

    // Clean up
    hipsparseDestroyMatDescr(descr);
    hipsparseDestroy(handle);
    hipFree(d_row_ptr);
    hipFree(d_col_idx);
    hipFree(d_values);
    hipFree(d_x);
    hipFree(d_y);
}

int main() {
    // Example data in CSR format
    int num_rows = 3, num_cols = 3, nnz = 4;
    int h_row_ptr[] = {0, 2, 3, 4};
    int h_col_idx[] = {0, 2, 1, 2};
    float h_values[] = {1.0, 2.0, 3.0, 4.0};
    float h_x[] = {1.0, 2.0, 3.0};
    float h_y[3] = {0};

    // Run SpMV using hipsparse
    spmv_hipsparse(num_rows, num_cols, nnz, h_row_ptr, h_col_idx, h_values, h_x, h_y);

    // Print result
    std::cout << "Result: ";
    for (int i = 0; i < num_rows; i++) {
        std::cout << h_y[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}



#define THREADS_PER_BLOCK 256

// HIP kernel för SpMV i CSR-format
__global__ void spmv_csr(int num_rows, const int *row_ptr, const int *col_idx,
                         const float *values, const float *x, float *y) {
    int row = blockIdx.x * blockDim.x + threadIdx.x;
    if (row < num_rows) {
        float sum = 0.0f;
        for (int j = row_ptr[row]; j < row_ptr[row + 1]; j++) {
            sum += values[j] * x[col_idx[j]];
        }
        y[row] = sum;
    }
}

void spmv_hip(int num_rows, int num_cols, int nnz, const int *h_row_ptr,
              const int *h_col_idx, const float *h_values, const float *h_x, float *h_y) {
    int *d_row_ptr, *d_col_idx;
    float *d_values, *d_x, *d_y;

    // Allokera minne på GPU
    hipMalloc(&d_row_ptr, (num_rows + 1) * sizeof(int));
    hipMalloc(&d_col_idx, nnz * sizeof(int));
    hipMalloc(&d_values, nnz * sizeof(float));
    hipMalloc(&d_x, num_cols * sizeof(float));
    hipMalloc(&d_y, num_rows * sizeof(float));

    // Kopiera data till GPU
    hipMemcpy(d_row_ptr, h_row_ptr, (num_rows + 1) * sizeof(int), hipMemcpyHostToDevice);
    hipMemcpy(d_col_idx, h_col_idx, nnz * sizeof(int), hipMemcpyHostToDevice);
    hipMemcpy(d_values, h_values, nnz * sizeof(float), hipMemcpyHostToDevice);
    hipMemcpy(d_x, h_x, num_cols * sizeof(float), hipMemcpyHostToDevice);

    // Starta kernel
    int num_blocks = (num_rows + THREADS_PER_BLOCK - 1) / THREADS_PER_BLOCK;
    hipLaunchKernelGGL(spmv_csr, dim3(num_blocks), dim3(THREADS_PER_BLOCK), 0, 0,
                       num_rows, d_row_ptr, d_col_idx, d_values, d_x, d_y);

    // Kopiera resultat tillbaka
    hipMemcpy(h_y, d_y, num_rows * sizeof(float), hipMemcpyDeviceToHost);

    // Frigör GPU-minne
    hipFree(d_row_ptr);
    hipFree(d_col_idx);
    hipFree(d_values);
    hipFree(d_x);
    hipFree(d_y);
}

int main() {
    // Exempeldata i CSR-format
    int num_rows = 3, num_cols = 3, nnz = 4;
    int h_row_ptr[] = {0, 2, 3, 4};
    int h_col_idx[] = {0, 2, 1, 2};
    float h_values[] = {1.0, 2.0, 3.0, 4.0};
    float h_x[] = {1.0, 2.0, 3.0};
    float h_y[3] = {0};

    // Kör SpMV
    spmv_hip(num_rows, num_cols, nnz, h_row_ptr, h_col_idx, h_values, h_x, h_y);

    // Skriv ut resultat
    std::cout << "Result: ";
    for (int i = 0; i < num_rows; i++) {
        std::cout << h_y[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}

#include <iostream>
#include <hip/hip_runtime.h>
#include <rocblas.h>
#include <rocsparse.h>

int main() {
    // Initialize rocBLAS and rocSPARSE handles
    rocblas_handle blas_handle;
    rocsparse_handle sparse_handle;
    rocblas_create_handle(&blas_handle);
    rocsparse_create_handle(&sparse_handle);

    // Define the size of vectors and matrix
    const int N = 5;  // Number of rows
    const int M = 5;  // Number of columns
    const int nnz = 3;  // Number of non-zero elements in sparse matrix

    // Host data for sparse matrix A in CSR format
    float h_A_values[] = {1.0f, 2.0f, 3.0f};   // Non-zero values of A
    int h_A_columns[] = {0, 2, 4};             // Column indices for A
    int h_A_row_ptr[] = {0, 1, 1, 2, 3, 3};    // Row pointers for A

    // Host data for vectors x and y
    float h_x[] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f};
    float h_y[] = {5.0f, 4.0f, 3.0f, 2.0f, 1.0f};
    float alpha = 2.0f;

    // Allocate device memory
    float *d_A_values, *d_x, *d_y;
    int *d_A_columns, *d_A_row_ptr;
    hipMalloc(&d_A_values, nnz * sizeof(float));
    hipMalloc(&d_A_columns, nnz * sizeof(int));
    hipMalloc(&d_A_row_ptr, (N + 1) * sizeof(int));
    hipMalloc(&d_x, M * sizeof(float));
    hipMalloc(&d_y, N * sizeof(float));

    // Copy data to device
    hipMemcpy(d_A_values, h_A_values, nnz * sizeof(float), hipMemcpyHostToDevice);
    hipMemcpy(d_A_columns, h_A_columns, nnz * sizeof(int), hipMemcpyHostToDevice);
    hipMemcpy(d_A_row_ptr, h_A_row_ptr, (N + 1) * sizeof(int), hipMemcpyHostToDevice);
    hipMemcpy(d_x, h_x, M * sizeof(float), hipMemcpyHostToDevice);
    hipMemcpy(d_y, h_y, N * sizeof(float), hipMemcpyHostToDevice);

    // Prepare the AXPY operation: y = alpha * A * x + y
    rocsparse_mat_descr descr;
    rocsparse_create_mat_descr(&descr);
    rocsparse_set_mat_type(descr, rocsparse_matrix_type_general);
    rocsparse_set_mat_index_base(descr, rocsparse_index_base_zero);

    // Perform sparse matrix-linear_algebra multiplication: A * x -> temp_y
    rocsparse_scsrmv(sparse_handle, rocsparse_operation_none, N, M, nnz,
                     &alpha, descr, d_A_values, d_A_row_ptr, d_A_columns,
                     d_x, &alpha, d_y);

    // Copy the result back to host
    hipMemcpy(h_y, d_y, N * sizeof(float), hipMemcpyDeviceToHost);

    // Print result
    std::cout << "Result y after AXPY operation: ";
    for (int i = 0; i < N; ++i) {
        std::cout << h_y[i] << " ";
    }
    std::cout << std::endl;

    // Clean up
    hipFree(d_A_values);
    hipFree(d_A_columns);
    hipFree(d_A_row_ptr);
    hipFree(d_x);
    hipFree(d_y);
    rocsparse_destroy_mat_descr(descr);
    rocblas_destroy_handle(blas_handle);
    rocsparse_destroy_handle(sparse_handle);

    return 0;
}

#include <rocsparse/rocsparse.h>
#include <hipsparse/hipsparse.h>

/// \brief Checks if the provided error code is \p hipSuccess and if not,
/// prints an error message to the standard error output and terminates the program
/// with an error code.
#define HIP_CHECK(condition)                                                                \
    {                                                                                       \
        const hipError_t error = condition;                                                 \
        if(error != hipSuccess)                                                             \
        {                                                                                   \
            std::cerr << "An error encountered: \"" << hipGetErrorString(error) << "\" at " \
                      << __FILE__ << ':' << __LINE__ << std::endl;                          \
            std::exit(-1);                                                                  \
        }                                                                                   \
    }


int main() {
    rocsparse_handle handle;
    rocsparse_create_handle(&handle);
    int version;
    rocsparse_get_version(handle, &version);
    std::cout << "version " << version << std::endl;
    rocsparse_set_pointer_mode(handle,rocsparse_pointer_mode_host);
    // alpha * ( 1.0  0.0  2.0 ) * ( 1.0 ) + beta * ( 4.0 ) = (  31.1 )
    //         ( 3.0  0.0  4.0 ) * ( 2.0 )          ( 5.0 ) = (  62.0 )
    //         ( 5.0  6.0  0.0 ) * ( 3.0 )          ( 6.0 ) = (  70.7 )
    //         ( 7.0  0.0  8.0 ) *                  ( 7.0 ) = ( 123.8 )

    int m = 4;
    int n = 3;
    int nnz = 8;

    // CSR row pointers
    int hcsr_row_ptr[5] = {0, 2, 4, 6, 8};

    // CSR column indices
    int hcsr_col_ind[8] = {0, 2, 0, 2, 0, 1, 0, 2};

    // CSR values
    float hcsr_val[8] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0};

    // Transposition of the matrix

    rocsparse_operation trans = rocsparse_operation_none;

    // Scalar alpha and beta
    float alpha = 3.7;
    float beta = 1.3;

    // x and y
    float hx[3] = {1.0, 2.0, 3.0};
    float hy[4] = {4.0, 5.0, 6.0, 7.0};

    // Matrix descriptor
    rocsparse_mat_descr descr;
    rocsparse_create_mat_descr(&descr);


    // Offload data to device
    int *dcsr_row_ptr;
    int *dcsr_col_ind;
    float *dcsr_val;
    float *dx;
    float *dy;


    HIP_CHECK(hipMalloc((void **)&dcsr_row_ptr, sizeof(int) * (m + 1)));
    HIP_CHECK(hipMalloc((void **)&dcsr_col_ind, sizeof(int) * nnz));
    HIP_CHECK(hipMalloc((void **)&dcsr_val, sizeof(float) * nnz));
    HIP_CHECK(hipMalloc((void **)&dx, sizeof(float) * n));
    HIP_CHECK(hipMalloc((void **)&dy, sizeof(float) * m));

    HIP_CHECK(hipMemcpy(dcsr_row_ptr, hcsr_row_ptr, sizeof(int) * (m + 1), hipMemcpyHostToDevice));
    HIP_CHECK(hipMemcpy(dcsr_col_ind, hcsr_col_ind, sizeof(int) * nnz, hipMemcpyHostToDevice));
    HIP_CHECK(hipMemcpy(dcsr_val, hcsr_val, sizeof(float) * nnz, hipMemcpyHostToDevice));
    HIP_CHECK(hipMemcpy(dx, hx, sizeof(float) * n, hipMemcpyHostToDevice));
    HIP_CHECK(hipMemcpy(dy, hy, sizeof(float) * m, hipMemcpyHostToDevice));
    for (int i = 0; i < 4; ++i) {
        std::cout << dy[i] << ' ';
    }
    // Call dcsrmv to perform y = alpha * A x + beta * y

  rocsparse_mat_info info;
  rocsparse_create_mat_info(&info);
  /*rocsparse_scsrmv_analysis(handle,
                             trans,
                             m,
                             n,
                             nnz,
                             descr,
                             dcsr_val[0],
                             dcsr_row_ptr[0],
                             dcsr_col_ind[0],
                             info);*/

  auto val = rocsparse_scsrmv(handle,
                             trans,
                             m,
                             n,
                             nnz,
                             &alpha,
                             descr,
                             dcsr_val,
                             dcsr_row_ptr,
                             dcsr_col_ind,
                             info,
                             dx,
                             &beta,
                             dy);

    std::cout << " status: " << val << std::endl;


    // Copy result back to host
    HIP_CHECK(hipMemcpy(hy, dy, sizeof(float) * m, hipMemcpyDeviceToHost));

    for (int i = 0; i < 4; ++i) {
        std::cout << dy[i] << ' ';
    }
    std::cout << std::endl;

    // Clear hipSPARSE
    //hipsparseDestroyMatDescr(descr);
    rocsparse_destroy_mat_descr(descr);
    //hipsparseDestroy(handle);
    rocsparse_destroy_handle(handle);

    // Clear device memory
    HIP_CHECK(hipFree(dcsr_row_ptr));
    HIP_CHECK(hipFree(dcsr_col_ind));
    HIP_CHECK(hipFree(dcsr_val));
    HIP_CHECK(hipFree(dx));
    HIP_CHECK(hipFree(dy));
    return 0;
}


int main()
  {
    int N           = 10240;
    int nnz         = 256;
    float alpha     = 10.0f;
    float tolerance = 1e-8f;

    std::vector<int> hx_ind(nnz);
    std::vector<float> hx_val(nnz);
    std::vector<float> hy(N);

    // Allocate memory on the device
    int* dx_ind;
    float* dx_val;
    float* dy;

    hipMalloc(&dx_ind, nnz * sizeof(int));
    hipMalloc(&dx_val, nnz * sizeof(float));
    hipMalloc(&dy, N * sizeof(float));

    // Initial Data on CPU,
    srand(1);

    for(int i = 0; i < nnz; ++i)
      {
        hx_ind[i] = i * 40;
        hx_val[i] = rand() % 10 + 1; // Generate an integer number between [1, 10]
      }

    for(int i = 0; i < N; ++i)
      {
        hy[i] = rand() % 10 + 1; // Generate an integer number between [1, 10]
      }

     // Copy data to device
     hipMemcpy(dx_ind, hx_ind.data(), sizeof(int) * nnz, hipMemcpyHostToDevice);
    hipMemcpy(dx_val, hx_val.data(), sizeof(float) * nnz, hipMemcpyHostToDevice);
    hipMemcpy(dy, hy.data(), sizeof(float) * N, hipMemcpyHostToDevice);

    // Initialize rocSPARSE
    hipsparseHandle_t handle;
    hipsparseCreate(&handle);

    // Run saxpyi on device
    hipsparseSaxpyi(handle, nnz, &alpha, dx_val, dx_ind, dy, HIPSPARSE_INDEX_BASE_ZERO);

    // Copy output from device memory to host memory
    std::vector<float> result(N);
    hipMemcpy(result.data(), dy, sizeof(float) * N, hipMemcpyDeviceToHost);

    // Verify hipsparseSaxpyi result
    for(int i = 0; i < nnz; ++i)
      {
        hy[hx_ind[i]] += alpha * hx_val[i];
      }

    float error;
    for(int i = 0; i < N; ++i)
      {
        error = fabs(hy[i] - result[i]);
        if(error > tolerance)
         {
           fprintf(stderr, "Error in element %d: CPU=%f, GPU=%f\n", i, hy[i], result[i]);
           break;
         }
      }

    if(error > tolerance)
      {
       printf("axpyi test failed!\n");
      }
    else
      {
      printf("axpyi test passed!\n");
      }

    hipFree(dx_ind);
    hipFree(dx_val);
    hipFree(dy);

    hipsparseDestroy(handle);

    return 0;
   }


#pragma omp requires unified_shared_memory

void saxpy() {
   int N = 64'000'0000;
   float a;
   double t;
   double tb, te;


   float* x = new (std::align_val_t(128)) float[N];
   float* y = new (std::align_val_t(128)) float[N];

   #pragma omp target teams distribute parallel for
   for (int i = 0; i < N; ++i) {
      x[i] = 1.0f;
      y[i] = 2.0f;
   }

   a = 2.0f;

   tb = omp_get_wtime();

   #pragma omp target teams distribute parallel for thread_limit(4) num_teams(8) dist_schedule(static)
   for (int i = 0; i < N; i++) {
      y[i] = a * x[i] + y[i];
   }

   te = omp_get_wtime();
   t = te - tb;

   printf("Time of kernel: %lf\n", t);

   printf("plausibility check output:\n");
   printf("y[0] %lf\n",y[0]);
   printf("y[N-1] %lf\n",y[N-1]);

}
int main(int argc, char *argv[]){
   std::string gfx_version = "11.0.0";

   std::string env_var = "HSA_OVERRIDE_GFX_VERSION=" + gfx_version;
   putenv(const_cast<char*>(env_var.c_str()));
   std::cout << " with GFX version " << gfx_version << std::endl;
   saxpy();
}