
#include <CL/cl.h>


const char* kernel_axpy = R"(
        __kernel void axpy(
            __global const float* A,
            __global float* x,
            __global const float* y,
            const int N)
        {
            int i = get_global_id(0);
            float sum = 0.0;
            for (int j = 0; j < N; j++) {
                sum += A[i * N + j] * x[j];
            }
            x[i] = sum + y[i];
        }
    )";

// Helper function to check OpenCL errors
#define CHECK_ERROR(err, msg) \
    if (err != CL_SUCCESS) { \
        std::cerr << msg << " (Error code: " << err << ")" << std::endl; \
        exit(EXIT_FAILURE); \
    }

std::string loadKernelSource(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open kernel file: " << filename << std::endl;
        exit(EXIT_FAILURE);
    }

    std::ostringstream oss;
    oss << file.rdbuf();
    return oss.str();
}

int opencl_axpy_example() {
    // Example size of the matrix and vectors
    const int N = 3;
    std::vector<float> A = {1.0, 2.0, 3.0,
                            4.0, 5.0, 6.0,
                            7.0, 8.0, 9.0}; // 3x3 matrix
    std::vector<float> x = {1.0, 1.0, 1.0};  // Vector x
    std::vector<float> y = {1.0, 2.0, 3.0};  // Vector y
    std::vector<float> result(N);

    // Load the OpenCL platform
    cl_int err;
    cl_platform_id platform;
    err = clGetPlatformIDs(1, &platform, nullptr);
    CHECK_ERROR(err, "Failed to find OpenCL platform");

    // Get the device
    cl_device_id device;
    err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, nullptr);
    CHECK_ERROR(err, "Failed to find OpenCL device");

    // Create context and command queue
    cl_context context = clCreateContext(nullptr, 1, &device, nullptr, nullptr, &err);
    CHECK_ERROR(err, "Failed to create OpenCL context");
    cl_command_queue queue = clCreateCommandQueue(context, device, 0, &err);
    CHECK_ERROR(err, "Failed to create command queue");

    // Create program from kernel source
    std::string kernelSource = loadKernelSource("axpy_kernel.cl");
    const char* kernelSourceCStr = kernelSource.c_str();
    cl_program program = clCreateProgramWithSource(context, 1, &kernelSourceCStr, nullptr, &err);
    CHECK_ERROR(err, "Failed to create OpenCL program");
    err = clBuildProgram(program, 1, &device, nullptr, nullptr, nullptr);
    CHECK_ERROR(err, "Failed to build OpenCL program");

    // Create kernel
    cl_kernel kernel = clCreateKernel(program, "axpy", &err);
    CHECK_ERROR(err, "Failed to create kernel");

    // Create memory buffers
    cl_mem bufferA = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(float) * N * N, A.data(), &err);
    CHECK_ERROR(err, "Failed to create buffer for A");
    cl_mem bufferX = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(float) * N, x.data(), &err);
    CHECK_ERROR(err, "Failed to create buffer for x");
    cl_mem bufferY = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(float) * N, y.data(), &err);
    CHECK_ERROR(err, "Failed to create buffer for y");

    // Set kernel arguments
    clSetKernelArg(kernel, 0, sizeof(cl_mem), &bufferA);
    clSetKernelArg(kernel, 1, sizeof(cl_mem), &bufferX);
    clSetKernelArg(kernel, 2, sizeof(cl_mem), &bufferY);
    clSetKernelArg(kernel, 3, sizeof(int), &N);

    // Execute the kernel
    size_t globalWorkSize = N;
    err = clEnqueueNDRangeKernel(queue, kernel, 1, nullptr, &globalWorkSize, nullptr, 0, nullptr, nullptr);
    CHECK_ERROR(err, "Failed to enqueue kernel");

    // Read the result back to host
    err = clEnqueueReadBuffer(queue, bufferX, CL_TRUE, 0, sizeof(float) * N, result.data(), 0, nullptr, nullptr);
    CHECK_ERROR(err, "Failed to read buffer");

    // Print the result
    std::cout << "Result vector x: ";
    for (float val : result) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    // Clean up
    clReleaseMemObject(bufferA);
    clReleaseMemObject(bufferX);
    clReleaseMemObject(bufferY);
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);

    return 0;
}


int opencl_main() {
    // Get platform information
    cl_uint platformCount;
    clGetPlatformIDs(0, nullptr, &platformCount);

    std::cout << "Number of OpenCL platforms found: " << platformCount << std::endl;
    std::cout << std::endl << std::endl;

    opencl_axpy_example();
    return 0;
}
