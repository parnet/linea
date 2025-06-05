

if (EXISTS "/opt/rocm")
    list(APPEND CMAKE_PREFIX_PATH "/opt/rocm")
endif()


#--rocm-device-lib-path=/usr/lib64/rocm/llvm/lib/clang/18/amdgcn/bitcode
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp -fopenmp-targets=amdgcn-amd-amdhsa -Xopenmp-target=amdgcn-amd-amdhsa -march=gfx1100 --rocm-device-lib-path=/usr/lib64/rocm/llvm/lib/clang/18/amdgcn/bitcode")
# include_directories(/usr/lib64/rocm/llvm/lib/clang/18/include)

message(STATUS "set----------|||-")