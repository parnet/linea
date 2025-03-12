find_package(OpenMP QUIET)
if (OpenMP_FOUND)
    message(STATUS "[X]   OpenMP was found version: ${OpenMP_CXX_VERSION}") # was found C version ${OpenMP_C_VERSION}, CXX version
    # todo link against :  -fopenmp -fopenmp-simd  list(APPEND LINK_LIBRARIES OpenMP::OpenMP_CXX)
    set(OpenMP_LIBRARIES "OpenMP::OpenMP_CXX")
else ()
    message(STATUS "[ ]   OpenMP was not found")
endif ()