
# Enable CUDA

#include(CheckLanguage)
enable_language(CUDA)

# set(CMAKE_CUDA_ARCHITECTURES "90") for hopper !

#find_package(CUDAToolkit QUIET)
#check_language(CUDA)

if (CMAKE_CUDA_COMPILER)
    message(STATUS "CUDA Aktiverades" )
    set(CMAKE_CUDA_ARCHITECTURES "86") # for rtx3070Ti, 3090
    set(CMAKE_CUDA_STANDARD 17)
#    enable_language(CUDA)
#    message(STATUS "CUDA found: ${CUDA_VERSION}")
    add_definitions(-DUSE_CUDA=1)
#    target_compile_definitions(Linea PRIVATE -DUSE_CUDA=1)
#    message(STATUS "CUDA was not found")
#    set(CMAKE_CXX_COMPILER nvcc)

    #elseif ()
endif ()
#list(APPEND LINK_LIBRARIES ${CUDA_LIBRARIES})