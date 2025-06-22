#include <iostream>

// #include "hdf5/version.hpp"
//#include "benchmark/csrmv.hpp"
//#include "code/util/compiler.hpp"
//#include "code/generator/std_matrix.hpp"
//#include "code/openmp/general.hpp"


//#include "test/std_matrix.hpp"

//#include "problem/heat_equation.hpp"
//#include "benchmark/mv.hpp"
//#include "blas/blas.hpp"
#include "cuda/cusparse/bicgstab.cuh"
#include "libs/cuda.hpp"

#include "util/memory.hpp"

//#include <hip/hip_runtime.h>
#include <iostream>
#include <vector>
#include <cmath>



#include <omp.h>

#include "hip/info/device_property.hpp"
#include "problem/heat_equation.hpp"

int main(int argc, char** argv) {
    omp_set_num_threads(8);
#pragma omp parallel
    {
        int tid = omp_get_thread_num();
        int nthreads = omp_get_num_threads();

#pragma omp critical
        printf("Thread %d out of %d threads\n", tid, nthreads);
    }

    benchmark_heat_equation();
    //hardware_limit();
/*#ifdef USE_HIP
    std::cout << "    using HIP" << std::endl;
    std::string gfx_version = "11.0.0";
    if (argc > 1) {
        gfx_version = argv[1];
    }
    std::string env_var = "HSA_OVERRIDE_GFX_VERSION=" + gfx_version;
    putenv(const_cast<char*>(env_var.c_str()));
    std::cout << " with GFX version " << gfx_version << std::endl;
#endif*/

/*

    int count = 0;
    hipGetDeviceCount(&count);
    printf("HIP Devices: %d\n", count);

    hipDeviceProp_t prop;
    hipGetDeviceProperties(&prop, 0);
    std::cout << "Running on device: "<< prop.name << " (arch "<< prop.gcnArchName << ")\n";

    hipSetDevice(0);




    benchmark_heat_equation();
*/
}


/*
int main(int argc, char** argv)
{
    //test_main();
    //linea::openmp::info();
    // print_arch();
    // auto version = version_hdf5();
    // std::cout << "HDF5: "<< version.str() << std::endl;
    // StdMatrix empty_matrix = StdMatrix(0,0);
    // StdCRSMatrix matrix = StdCRSMatrix(empty_matrix);

    // create_laplacian_2d(matrix,5);
    //std::cout << 7*7*7*7 << std::endl;
    // std::cout << matrix.str() << std::endl;

    // gneral_matrix_test();
    //benchmark_mv();
    //blas_mv_test();
    //double x;
    //std::cin >> x;
    //gneral_matrix_test_blas();
    //benchmark_mv();
    //std::cout <<  2* (1 << 7) +1  << std::endl;
    //memory::info(Giga);
    //check_cuda_device();
    //benchmark_mv();
    //benchmark_heat_equation();
    benchmark_heat_equation();

    return 0;
}*/

/***
 * dense max matrix size gridsize = 257
 * 257 * 257 total grid points
 * 257 * 257 x 257 * 257 matrix size
 * 66'049 x 66'049
 * 4'362'470'401 Einträge insgesamt
 ***/