

#include "operations_opencl.hpp"

#include <CL/cl.hpp>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>






#define CHECK_ERR(err, msg) if (err != CL_SUCCESS) { std::cerr << msg << " Error code: " << err << std::endl; exit(1); }


std::string loadKernel(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Kunde inte öppna filen: " << filename << std::endl;
        exit(1);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

/*
__kernel void matvec_mult(
    __global const float* A, // Matris A (M x N)
    __global const float* x, // Vektor x (N)
    __global float* y,       // Vektor y (M)
    const float alpha,       // Skalär alpha
    const float beta,        // Skalär beta
    const int M,             // Antal rader i A
    const int N)             // Antal kolumner i A
{
    // Global tråd-ID motsvarar radindex i matrisen A
    int row = get_global_id(0);

    if (row < M) {
        float dot_product = 0.0f;
        for (int col = 0; col < N; col++) {
            dot_product += A[row * N + col] * x[col];
        }
        y[row] = alpha * dot_product + beta * y[row];
    }
}
*/
const char* kernel_matrix_vector_multiplication = R"CLC(
__kernel void matvec_mult(
    __global const double* A,
    __global const double* x,
    __global double* y,
    const double alpha,
    const double beta,
    const int M,
    const int N)
{
    int row = get_global_id(0);
    if (row < M) {
        double dot_product = 0.0;
        for (int col = 0; col < N; col++) {
            dot_product += A[row * N + col] * x[col];
        }
        y[row] = alpha * dot_product + beta * y[row];
    }
}
)CLC";

void Operations_OpenCL::matrix_vector_multiplikation(const Matrix & matrix_A, double alpha,  const Vector & vektor_x, double beta, Vector & vektor_y){

    const int M = matrix_A.rows();
    const int N = matrix_A.cols();

    cl_int err;

    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);
    cl::Platform platform = platforms[0]; // first device

    std::vector<cl::Device> devices;
    platform.getDevices(CL_DEVICE_TYPE_GPU, &devices);
    cl::Device device = devices[0];

    // 2. Skapa kontext och kommandokö
    cl::Context context({device});
    cl::CommandQueue queue(context, device, CL_QUEUE_PROFILING_ENABLE, &err);
    CHECK_ERR(err, "Kunde inte skapa kommandokö");

    // 3. Bygg program
    cl::Program program(context, kernel_matrix_vector_multiplication, false, &err);
    CHECK_ERR(err, "Kunde inte skapa OpenCL-program");

    err = program.build({device});
    CHECK_ERR(err, "Misslyckades att bygga OpenCL-program");

    // 4. Skapa buffertar
    cl::Buffer bufferA(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(double) * M * N, const_cast<double*>(matrix_A._data.data()), &err);
    CHECK_ERR(err, "Kunde inte skapa bufferA");

    cl::Buffer bufferX(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(double) * N, const_cast<double*>(vektor_x._data.data()), &err);
    CHECK_ERR(err, "Kunde inte skapa bufferX");

    cl::Buffer bufferY(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(double) * M, vektor_y._data.data(), &err);
    CHECK_ERR(err, "Kunde inte skapa bufferY");

    // 5. Skapa och sätt kernel
    cl::Kernel kernel(program, "matvec_mult", &err);
    CHECK_ERR(err, "Misslyckades att skapa kernel");

    kernel.setArg(0, bufferA);
    kernel.setArg(1, bufferX);
    kernel.setArg(2, bufferY);
    kernel.setArg(3, alpha);
    kernel.setArg(4, beta);
    kernel.setArg(5, M);
    kernel.setArg(6, N);

    // 6. Utför beräkning
    cl::NDRange globalSize(M);
    err = queue.enqueueNDRangeKernel(kernel, cl::NullRange, globalSize, cl::NullRange);
    CHECK_ERR(err, "Misslyckades att köra kernel");

    queue.finish(); // syncroniseringspunkt

    // 7. Läs tillbaka resultatet
    queue.enqueueReadBuffer(bufferY, CL_TRUE, 0, sizeof(double) * M, vektor_y._data.data());

}