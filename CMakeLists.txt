# ===================[ general CMake settings ]=========================================================================
cmake_minimum_required(VERSION 3.15)
set(PROJECT_NAME "Linea")

project(${PROJECT_NAME})

if(${CMAKE_BINARY_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
    message(FATAL_ERROR "Inplace builds not allowed! Create a build directory")
endif ()

# ===================[ parse parameter ]================================================================================


# todo add_compile_definitions(USE_LAPACK)
# todo add_compile_definitions(USE_MKL)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native -O3 -Wall -Wextra -DNDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -Wextra")


# ===================[ MPI ]============================================================================================
find_package(PkgConfig QUIET)

find_package(MPI QUIET)
if (MPI_FOUND)
    message(STATUS "MPI was found version: ${MPI_CXX_VERSION}")
    #message(STATUS "MPI found: ${MPI_CXX_COMPILER}")
    #message(STATUS "MPI include path: ${MPI_INCLUDE_PATH}")
    #message(STATUS "MPI libraries: ${MPI_LIBRARIES}")
    #list(APPEND LINK_LIBRARIES ${MPI_LIBRARIES})
    #if(CMAKE_CXX_COMPILER)
    #    message(STATUS ${CMAKE_CXX_COMPILER})
    #else ()
    #    message(STATUS "MPICXX set as compiler")
    #    #set(CMAKE_CXX_COMPILER mpicxx)
    #endif()

    # -DMPI_C_COMPILER=/path/to/mpicc -DMPI_CXX_COMPILER=/path/to/mpicxx ..
else()
    message(STATUS "MPI was ot found")
endif()
# ----------------------------------------------------------------------------------------------------------------------
find_package(OpenMP QUIET)
if (OpenMP_FOUND)
    message(STATUS "OpenMP was found version: ${OpenMP_CXX_VERSION}") # was found C version ${OpenMP_C_VERSION}, CXX version
    # todo link against :  -fopenmp -fopenmp-simd  list(APPEND LINK_LIBRARIES OpenMP::OpenMP_CXX)
    set(OpenMP_LIBRARIES "OpenMP::OpenMP_CXX")
else ()
    message(STATUS "OpenMP was not found")
endif ()
# ----------------------------------------------------------------------------------------------------------------------
# todo HIP
# ----------------------------------------------------------------------------------------------------------------------
# todo ROCm
# ----------------------------------------------------------------------------------------------------------------------
# todo CUDA
# ----------------------------------------------------------------------------------------------------------------------
# todo Kokkos
# ----------------------------------------------------------------------------------------------------------------------
# todo OpenCL
# ----------------------------------------------------------------------------------------------------------------------
find_package(HDF5 QUIET COMPONENTS C CXX)
# find_package(HDF5)
if(HDF5_FOUND)
    message(STATUS "HDF5_VERSION was found version " ${HDF5_VERSION})
    include_directories(${HDF5_INCLUDE_DIR})
    # todo link against : hdf5 hdf5_cpp
else ()
    message(STATUS "HDF5 was not found")
endif ()
# ----------------------------------------------------------------------------------------------------------------------
find_package(LAPACK QUIET)
if(LAPACK_FOUND)
    if (LAPACK_VERSION STREQUAL "")
        message(STATUS "LAPACK was found: ${LAPACK_VERSION}")
    else ()
        pkg_check_modules(LAPACK_PKG QUIET lapack)
        if(LAPACK_PKG_FOUND)
            message(STATUS "LAPACK version: ${LAPACK_PKG_VERSION}")
        endif ()
    endif ()
    # todo link against : lapack

else ()
    message(STATUS "LAPCK was not found")
endif ()



# ----------------------------------------------------------------------------------------------------------------------
find_package(BLAS QUIET)

if(BLAS_FOUND)
    if (OPENBLAS_VERSION STREQUAL "")
        message(STATUS "LAPACK was found: ${OPENBLAS_VERSION}")
    else ()
        pkg_check_modules(BLAS_PKG QUIET code/blas)
        if(BLAS_PKG_FOUND)
            message(STATUS "BLAS version: ${BLAS_PKG_VERSION}")
        endif ()
    endif ()
    # message(STATUS ${BLAS_LIBRARIES})
    # target_compile_definitions(Linea PRIVATE -DUSE_BLAS=1)
    # list(APPEND ${LINK_LIBRARIES} ${BLAS_LIBRARIES})
endif ()
# ----------------------------------------------------------------------------------------------------------------------
find_package(IntelMKL QUIET)

if (IntelMKL_FOUND)
    message(STATUS "Intel MKL was found version: ${IntelMKL_VERSION}")

else ()
    message(STATUS "Intel MKL was not found")
endif ()
# ----------------------------------------------------------------------------------------------------------------------

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME}
        code/main.cpp
        code/data/naive_vector.hpp
        code/util/compiler.hpp
        code/hdf5/parallel_file.hpp
        code/hdf5/version.hpp
        code/util/version.hpp
        code/data/naive_matrix.hpp
        code/data/std_crs_matrix.hpp
        code/data/std_flat_matrix.hpp
        code/data/std_vector.hpp
        code/data/std_matrix.hpp
        code/generator/std_matrix.hpp
        code/generator/naive_matrix.h
        code/cpu/csrmv.hpp
        test/std_matrix.hpp
        code/openmp/csrmv.hpp
        code/openmp/general.hpp
        code/cpu/dot.hpp
)


message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "CXX flags: ${CMAKE_CXX_FLAGS}")

#add_executable(${PROJECT_NAME} ${SOURCES} )
#message(VERBOSE "Binary directory is: " ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
#set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
#
#message(STATUS "link-libraries: ${LINK_LIBRARIES}")
#target_link_libraries(${PROJECT_NAME} PRIVATE ${LINK_LIBRARIES})
#target_link_libraries(${PROJECT_NAME} PRIVATE ${HDF5_LIBRARIES})
MESSAGE(STATUS "${OpenMP_LIBRARIES}")
#target_link_libraries(${PROJECT_NAME} PRIVATE OpenMP::OpenMP_CXX)
target_link_libraries(${PROJECT_NAME} PRIVATE ${OpenMP_LIBRARIES})


# ===================[ project properties  ]============================================================================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)