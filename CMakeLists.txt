# ===================[ general CMake settings ]=========================================================================
cmake_minimum_required(VERSION 3.18)
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
set(ENABLE_OPENMP "OFF" CACHE STRING "OpenMP library usage")
if (ENABLE_OPENMP)
    include(cmake/openmp.cmake)
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
    include(cmake/hip.cmake)
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
        test/std_matrix.hpp
        benchmark/csrmv.hpp
        code/util/converter.hpp
        code/util/measurement.hpp
        code/generator/std_vector.hpp
        code/converter/std_crs_matrix.hpp
        code/generator/naive_vector.hpp
        code/generator/std_flat_matrix.hpp
        code/generator/std_crs_matrix.hpp
        benchmark/mv.hpp
        code/cpu/mv.hpp
        code/operations_blas.hpp
        test/blas.h
        code/Operations_OpenMP.hpp
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
        code/simd/csrmv.h
        code/operations_cpu.hpp
        code/operations_sse2.hpp
        code/operations_avx.hpp
        code/operations_avx512.hpp
        code/operations_avx512.cpp
        code/operations_avx.cpp
        code/operations_nonopt.hpp
        code/operations_nonopt.cpp
        code/data/c_aligned_matrix.hpp
        code/data/std_flat_matrix.hpp
        code/util/aligned_allocator.hpp
        code/data/c_aligned_vector.hpp
        code/data/crs_matrix.hpp
        code/data/std_vector.hpp
        code/data/std_aligned_vector.hpp
        code/data/std_aligned_matrix.hpp
        code/operations_cpu_nonconst.cpp
        code/operations_cpu_nonconst.hpp
        code/operations_mkl.hpp
        code/Operations_OpenMP.cpp
        code/operations_sse2.cpp
        code/operations_cpu.cpp
        benchmark/datastructures.hpp
        code/util/memory.hpp
        code/util/memory.cpp
        code/operations_cuda_kernel.hpp
        code/cuda/matrix_vector_multiplikation.cu
        code/cuda/matrix_vector_multiplikation.cuh
        code/operations_cublas.hpp
)


message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "CXX flags: ${CMAKE_CXX_FLAGS}")


find_package(CUDAToolkit REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE CUDA::cublas)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES})
target_link_libraries(${PROJECT_NAME} PRIVATE cudart)
set_target_properties(${PROJECT_NAME} PROPERTIES CUDA_SEPARABLE_COMPILATION ON)

#add_executable(${PROJECT_NAME} ${SOURCES} )
#message(VERBOSE "Binary directory is: " ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
#set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
#
#message(STATUS "link-libraries: ${LINK_LIBRARIES}")
#target_link_libraries(${PROJECT_NAME} PRIVATE ${LINK_LIBRARIES})
#target_link_libraries(${PROJECT_NAME} PRIVATE ${HDF5_LIBRARIES})
#target_link_libraries(${PROJECT_NAME} PRIVATE OpenMP::OpenMP_CXX)
#target_link_libraries(${PROJECT_NAME} PRIVATE ${OpenMP_LIBRARIES})
#target_link_libraries(${PROJECT_NAME} PRIVATE ${BLAS_LIBRARIES})


# ===================[ project properties  ]============================================================================

