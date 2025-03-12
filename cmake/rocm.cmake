if (DEFINED ENV{ROCM_PATH})
    list(APPEND CMAKE_PREFIX_PATH "$ENV{ROCM_PATH}")
elseif (EXISTS "/opt/rocm")
    # Fallback to standard path
    list(APPEND CMAKE_PREFIX_PATH "/opt/rocm")
endif()

# set(ROCM_DIR "/opt/rocm" CACHE PATH "Path to ROCm installation")
# list(APPEND CMAKE_PREFIX_PATH "${ROCM_DIR}")
find_package(hip QUIET)
find_package(ROCM QUIET)
find_package(rocblas QUIET)

find_package(rocsparse QUIET)
find_package(ROCSPARSE REQUIRED)

add_compile_definitions(USE_ROCM)
# Check if rocBLAS and rocSPARSE were found
if (NOT rocblas_FOUND)
    message(FATAL_ERROR "rocBLAS not found! Please install ROCm with rocBLAS.")
endif()

if (NOT rocsparse_FOUND)
    message(FATAL_ERROR "rocSPARSE not found! Please install ROCm with rocSPARSE.")
endif()

if (DEFINED ENV{ROCM_PATH})
    list(APPEND CMAKE_PREFIX_PATH "$ENV{ROCM_PATH}")
elseif (EXISTS "/opt/rocm")
    # Fallback to standard path

    # Ställ in ROCm:s installationsväg (om det behövs explicit)
    set(ROCM_PATH "/opt/rocm")

    # Lägg till sökväg för CMake att hitta ROCm-paketen
    list(APPEND CMAKE_PREFIX_PATH "${ROCM_PATH}")

    include_directories("/opt/rocm/include")
    link_directories("/opt/rocm/lib")


# Add custom paths for tools and libraries
set(CMAKE_PROGRAM_PATH "/opt/rocm/bin" CACHE PATH "Path to custom tools")
set(CMAKE_PREFIX_PATH "/opt/rocm" CACHE PATH "Path to ROCm libraries")

# Set environment variables for the build process
set(ENV{PATH} "/opt/rocm/bin:$ENV{PATH}")
set(ENV{LD_LIBRARY_PATH} "/opt/rocm/lib:$ENV{LD_LIBRARY_PATH}")