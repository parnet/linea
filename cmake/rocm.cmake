if (DEFINED ENV{ROCM_PATH})
    list(APPEND CMAKE_PREFIX_PATH "$ENV{ROCM_PATH}")
elseif (EXISTS "/opt/rocm")
    # Fallback to standard path
    list(APPEND CMAKE_PREFIX_PATH "/opt/rocm")
endif()

# set(ROCM_DIR "/opt/rocm" CACHE PATH "Path to ROCm installation")
# list(APPEND CMAKE_PREFIX_PATH "${ROCM_DIR}")
find_package(hip QUIET)
find_package(rocblas QUIET)
find_package(rocsparse QUIET)


add_compile_definitions(USE_ROCM)
# Check if rocBLAS and rocSPARSE were found
if (NOT rocblas_FOUND)
    message(FATAL_ERROR "rocBLAS not found! Please install ROCm with rocBLAS.")
endif()

if (NOT rocsparse_FOUND)
    message(FATAL_ERROR "rocSPARSE not found! Please install ROCm with rocSPARSE.")
endif()