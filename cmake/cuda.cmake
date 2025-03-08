# Enable CUDA
set(CMAKE_CUDA_ARCHITECTURES "86") # for rtx3070Ti, 3090
include(CheckLanguage)
enable_language(CUDA)

# set(CMAKE_CUDA_ARCHITECTURES "90") for hopper !

find_package(CUDAToolkit QUIET)
check_language(CUDA)

if (CMAKE_CUDA_COMPILER)
    enable_language(CUDA)
    message(STATUS "CUDA found: ${CUDA_VERSION}")
    target_compile_definitions(Linea PRIVATE -DUSE_CUDA=1)
    message(STATUS "CUDA was not found")
    set(CMAKE_CXX_COMPILER nvcc)
elseif ()
endif ()
list(APPEND LINK_LIBRARIES ${CUDA_LIBRARIES})