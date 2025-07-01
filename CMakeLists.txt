# ===================[ general CMake settings ]=========================================================================
cmake_minimum_required(VERSION 3.10)
set(PROJECT_NAME "Linea" LANGUAGES CXX)

project(${PROJECT_NAME})

if(${CMAKE_BINARY_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
    message(FATAL_ERROR "Inplace builds not allowed! Create a build directory")
endif ()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)



set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED OFF)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(code)


# ===================[ parse parameter ]================================================================================


# todo add_compile_definitions(USE_LAPACK)
# todo add_compile_definitions(USE_MKL)

set(OPT_LVL "3" CACHE STRING "Optimization level")


# not for nvcc :::  -march=native  -Wall  -Wextra -funroll-loops -funroll-loops -ftree-vectorize  -DNDEBUG
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O${OPT_LVL}   ")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O${OPT_LVL}    ")

set(CMAKE_C_FLAGS_DEBUG "-g -Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -Wextra")

#set(CMAKE_CXX_FLAGS  ${CMAKE_CXX_FLAGS_RELEASE})

find_package(PkgConfig QUIET)
# ===================[ Intrinsics ]=====================================================================================
set(ENABLE_INTRINSICS "OFF" CACHE STRING "INTRINSICS library usage")
add_definitions(-DUSE_INTRINSICS=${ENABLE_INTRINSICS})
# ===================[ MPI ]============================================================================================
set(ENABLE_MPI "OFF" CACHE STRING "MPI library usage")
if (ENABLE_MPI)
    include(cmake/mpi.cmake)
endif ()

# ----------------------------------------------------------------------------------------------------------------------

set(ENABLE_CUDA "OFF" CACHE STRING "CUDA library usage")
if (ENABLE_CUDA)
    include(cmake/cuda.cmake)
endif ()
# ----------------------------------------------------------------------------------------------------------------------
set(ENABLE_ROCM "OFF" CACHE STRING "ROCm library usage")
if (ENABLE_ROCM)
    include(cmake/rocm.cmake)
endif ()
# ----------------------------------------------------------------------------------------------------------------------
set(ENABLE_HIP "OFF" CACHE STRING "HIP library usage")
if (ENABLE_HIP)
    set(CMAKE_HIP_ARCHITECTURES gfx1103)
    include(cmake/hip.cmake)
endif ()
# ----------------------------------------------------------------------------------------------------------------------
set(ENABLE_OPENMP "OFF" CACHE STRING "OpenMP library usage")
if (ENABLE_OPENMP)
    include(cmake/openmp.cmake)
endif ()
# ----------------------------------------------------------------------------------------------------------------------
set(ENABLE_OFFLOADING "OFF" CACHE STRING "Offloading usage")
if (ENABLE_OFFLOADING)
    include(cmake/offloading.cmake)
endif ()
# ----------------------------------------------------------------------------------------------------------------------
set(ENABLE_KOKKOS "OFF" CACHE STRING "KOKKOS library usage")
if (ENABLE_KOKKOS)
    include(cmake/kokkos.cmake)
endif ()
# ----------------------------------------------------------------------------------------------------------------------
set(ENABLE_OPENCL "OFF" CACHE STRING "OPENCL library usage")
if (ENABLE_OPENCL)
    include(cmake/opencl.cmake)
endif ()
# ----------------------------------------------------------------------------------------------------------------------
set(ENABLE_EIGEN "OFF" CACHE STRING "EIGEN library usage")
if (ENABLE_EIGEN)
    include(cmake/eigen.cmake)
endif ()
# ----------------------------------------------------------------------------------------------------------------------
set(ENABLE_BLAS "OFF" CACHE STRING "BLAS library usage")
if (ENABLE_BLAS)
    include(cmake/blas.cmake)
endif ()
# ----------------------------------------------------------------------------------------------------------------------
set(ENABLE_LAPACK "OFF" CACHE STRING "LAPACK library usage")
if (ENABLE_LAPACK)
    include(cmake/lapack.cmake)
endif ()
# ----------------------------------------------------------------------------------------------------------------------
set(ENABLE_MKL "OFF" CACHE STRING "MKL library usage")
if (ENABLE_MKL)
    include(cmake/mkl.cmake)
endif ()
# ----------------------------------------------------------------------------------------------------------------------
set(ENABLE_HDF5 "OFF" CACHE STRING "HDF5 library usage")
if (ENABLE_HDF5)
    include(cmake/hdf5.cmake)
endif ()
# ----------------------------------------------------------------------------------------------------------------------
set(ENABLE_LUA "OFF" CACHE STRING "LUA library usage")
if (ENABLE_LUA)
    include(cmake/lua.cmake)
endif ()
# ----------------------------------------------------------------------------------------------------------------------

if(ENABLE_CUDA)
    include_directories(${CUDA_INCLUDE_DIRS})
    include_directories(${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES})
    include_directories(${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRS})
endif ()

add_executable(${PROJECT_NAME}
        main.cpp
        code/data/c_vector.hpp
        code/util/compiler.hpp
        code/hdf5/parallel_file.hpp
        code/hdf5/version.hpp
        code/util/version.hpp
        code/data/c_matrix.hpp
        code/data/std_crs_matrix.hpp
        code/data/matrix.hpp
        code/data/vector.hpp
        code/data/std_matrix.hpp
        code/generator/std_matrix.hpp
        code/generator/naive_matrix.hpp
        code/util/converter.hpp
        code/util/measurement.hpp
        code/generator/std_vector.hpp
        code/converter/std_crs_matrix.hpp
        code/generator/naive_vector.hpp
        code/generator/std_flat_matrix.hpp
        code/generator/std_crs_matrix.hpp
        benchmark/mv.hpp
        code/cpu/mv.hpp

        code/util/modules.hpp
        code/libs/openmp.hpp
        code/libs/sycl.hpp
        code/libs/rocm.hpp
        code/libs/opencl.hpp
        code/libs/openacc.hpp
        code/libs/mpi.hpp
        code/libs/mkl.hpp
        code/libs/lapack.hpp
        code/libs/lua.hpp
        code/libs/kokkos.hpp
        code/libs/hip.hpp
        code/libs/hdf5.hpp
        code/libs/cuda.hpp
        code/libs/blas.hpp
        code/libs/intrinsics.hpp
        code/libs/__lib_blas.h
        code/simd/csr_mv.h
        code/data/c_aligned_matrix.hpp
        code/data/std_flat_matrix.hpp
        code/util/aligned_allocator.hpp
        code/data/c_aligned_vector.hpp
        code/data/crs_matrix.hpp
        code/data/std_vector.hpp
        code/data/std_aligned_vector.hpp
        code/data/std_aligned_matrix.hpp
        code/util/memory.hpp
        code/util/memory.cpp
        code/compile_time/suffix.hpp
        code/libs/openmp.cpp
        code/data/gpu_matrix.hpp
        todo/integrate.hpp
        todo/polynom.hpp
        todo/vector.hpp
        problem/cuda_heat_equation.hpp
        code/generator/extract_diag.hpp
        code/cuda/cusparse/bicgstab.cu
        code/cuda/cusparse/bicgstab.cuh
        code/compile_time/complexity.hpp

        code/blas/blas.hpp
        code/openmp/openmp.hpp
        code/cpu/cpu_nonconst.cpp
        code/cpu/cpu_nonconst.hpp
        code/mkl/mkl.hpp
        code/openmp/openmp.cpp
        code/simd/sse2.cpp
        code/cpu/cpu.cpp
        code/cuda/cuda.hpp
        code/cuda/cublas.hpp
        code/offloading/openmp_offload.cpp
        code/offloading/openmp_offload.hpp
        code/opencl/opencl.cpp
        code/opencl/opencl.hpp
        code/cpu/cpu.hpp
        code/simd/sse2.hpp
        code/simd/avx.hpp
        code/simd/avx512.hpp
        code/simd/avx512.cpp
        code/simd/avx.cpp
        code/cpu/nonopt.hpp
        code/cpu/nonopt.cpp
        code/cuda/cusparse/bicgstab.cuh
        code/cpu/linear_algebra/vector.hpp
        code/cpu/linear_algebra/matrix.hpp
        code/cpu/linear_algebra/crs_matrix.hpp
        code/cuda/linear_algebra/crs_matrix.hpp
        code/cuda/linear_algebra/matrix.hpp
        code/cuda/linear_algebra/vector.hpp
        code/cpu/definitions.hpp
        code/cuda/data/vector.hpp
        code/cuda/data/crs_matrix.hpp
        code/cuda/linear_algebra/block_crs_matrix.hpp
        code/cuda/linear_algebra/kernel/sum_of_squares.cuh
        code/cuda/linear_algebra/kernel/sum_of_squares.cu
        code/cuda/linear_algebra/kernel/crs_residual.cu
        code/cuda/linear_algebra/kernel/crs_residual.cuh
        code/cuda/linear_algebra/kernel/vector_div_scalar.cu
        code/cuda/linear_algebra/kernel/vector_div_scalar.cuh
        code/cuda/linear_algebra/kernel/matrix_vector_multiplication.cu
        code/cuda/linear_algebra/kernel/matrix_vector_multiplication.cuh
        code/cuda/linear_algebra/kernel/block_matrix_vector_multitplication.cu
        code/cuda/linear_algebra/kernel/block_matrix_vector_multitplication.cuh
        code/cuda/linear_algebra/vector.cpp
        code/cuda/linear_algebra/kernel/jacobi_step.cu
        code/cuda/linear_algebra/kernel/jacobi_step.cuh
        code/cuda/data/matrix.hpp
        code/cuda/data/block_vector.hpp
        code/cuda/data/block_crs_matrix.hpp
        code/cuda/data/block_matrix.hpp
        code/data/block_vector.hpp
        code/data/block_matrix.hpp
        code/data/block_crs_matrix.hpp
        code/cuda/linear_algebra/block_vector.hpp
        code/cuda/linear_algebra/block_matrix.hpp
        code/cpu/linear_algebra/block_crs_matrix.hpp
        code/cpu/linear_algebra/block_vector.hpp
        code/cpu/linear_algebra/block_matrix.hpp
        code/sys_info.hpp
        code/cpu/block_vector.cpp
        code/cpu/block_vector.hpp
        code/hip/data/block_crs_matrix.hpp
        code/hip/data/block_matrix.hpp
        code/hip/data/block_vector.hpp
        code/hip/data/crs_matrix.hpp
        code/hip/data/matrix.hpp
        code/hip/data/vector.hpp
        code/hip/linear_algebra/block_crs_matrix.hpp
        code/hip/linear_algebra/block_matrix.hpp
        code/hip/linear_algebra/block_vector.hpp
        code/hip/linear_algebra/crs_matrix.hpp
        code/hip/linear_algebra/matrix.hpp
        code/hip/linear_algebra/vector.hpp
        code/hip/linear_algebra/kernel/block_matrix_vector_multitplication.hip
        code/hip/linear_algebra/kernel/block_matrix_vector_multitplication.hpp
        problem/hip_heat_equation.hpp
        problem/heat_equation.hpp
        code/hip/linear_algebra/kernel/sum_of_squares.hip
        code/hip/linear_algebra/kernel/sum_of_squares.hpp
        code/hip/linear_algebra/kernel/jacobi_step.hip
        code/hip/linear_algebra/kernel/jacobi_step.hpp
        code/hip/linear_algebra/kernel/crs_residual.hip
        code/hip/linear_algebra/kernel/crs_residual.hpp
        code/hip/definitions.hpp
        code/macro/cuda_error.hpp
        code/macro/hip_error.hpp
        code/hip/info/device_property.hpp
        problem/cpu_heat_equation.hpp
        problem/hipsparse_heat_equation.hpp
        problem/vec_print.hpp
        code/mpi_omp_cuda/data/crs_matrix.hpp
        code/mpi_omp_cuda/distribution.hpp
        code/mpi_omp_cuda/laplacian.hpp
        code/mpi_omp_cuda/create_laplacian.hpp
        code/mpi_omp_cuda/kernel/kernel.cuh
        code/mpi_omp_cuda/kernel/kernel.cu
        code/mpi_omp_cuda/kernel/bridge.hpp
        code/mpi_omp_cuda/host/host.hpp
)

if(ENABLE_CUDA)
    find_package(CUDAToolkit REQUIRED)
endif ()


message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "CXX flags: ${CMAKE_CXX_FLAGS}")


# find_package(CUDAToolkit REQUIRED)
if (ENABLE_CUDA)
    target_link_libraries(${PROJECT_NAME} PRIVATE CUDA::cublas)

    target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES})
    target_link_libraries(${PROJECT_NAME} PRIVATE cudart)
    set_target_properties(${PROJECT_NAME} PROPERTIES CUDA_SEPARABLE_COMPILATION ON)

    target_link_libraries(${PROJECT_NAME} PRIVATE CUDA::cublas CUDA::cusparse CUDA::cudart)
endif ()

if (ENABLE_HIP)
    # target_link_libraries(${PROJECT_NAME} PRIVATE hip::host hip::hipBLAS)

    target_link_libraries(${PROJECT_NAME} PRIVATE hip::host)
    #target_link_libraries(${PROJECT_NAME} PRIVATE hip::device /opt/rocm/lib/libhipsparse.so)

endif ()

if (ENABLE_MPI)
    target_include_directories(${PROJECT_NAME} PRIVATE ${MPI_INCLUDE_PATH})
    target_link_libraries(${PROJECT_NAME} PRIVATE MPI::MPI_CXX)
endif ()
#add_executable(${PROJECT_NAME} ${SOURCES} )
#message(VERBOSE "Binary directory is: " ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
#set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
#
#message(STATUS "link-libraries: ${LINK_LIBRARIES}")
#target_link_libraries(${PROJECT_NAME} PRIVATE ${LINK_LIBRARIES})
#target_link_libraries(${PROJECT_NAME} PRIVATE ${HDF5_LIBRARIES})
#target_link_libraries(${PROJECT_NAME} PRIVATE OpenMP::OpenMP_CXX)
message(STATUS ${OpenMP_LIBRARIES})
if(OpenMP_LIBRARIES)
    #target_link_libraries(${PROJECT_NAME} PRIVATE ${OpenMP_LIBRARIES})
    #target_link_libraries(${PROJECT_NAME} PRIVATE OpenMP::OpenMP_CXX)
    message(STATUS "linking openmp")
    target_link_libraries(${PROJECT_NAME} PRIVATE ${OpenMP_LIBRARIES})
endif ()
if(OpenCL_LIBRARIES)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${OpenCL_LIBRARIES})
endif ()

if(Eigen3_VERSION)
    message(STATUS "Eigen3::Eigen")
    target_link_libraries(${PROJECT_NAME} PRIVATE Eigen3::Eigen)
endif ()

if(Kokkos_LIBRARIES)
    get_property(targets GLOBAL PROPERTY TARGETS)
    message(STATUS "Available targets: ${targets}")
endif ()
#target_link_libraries(${PROJECT_NAME} PRIVATE ${BLAS_LIBRARIES})

target_compile_options(${PROJECT_NAME} PRIVATE -march=native)

# ===================[ project properties  ]============================================================================

