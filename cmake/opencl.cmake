find_package(OpenCL QUIET)


#set(OpenCL_INCLUDE_DIRS "/opt/rocm-6.2.1/opencl/include")
#set(OpenCL_LIBRARIES "/opt/rocm-6.2.1/opencl/lib/libOpenCL.so")

if (DEFINED ENV{OPENCL_PATH})
    list(APPEND CMAKE_PREFIX_PATH "$ENV{OPENCL_PATH}")
else ()
    # Fallback to standard path
    set(OpenCL_INCLUDE_DIRS "/opt/rocm/include")
    set(OpenCL_LIBRARIES "/opt/rocm/lib/libOpenCL.so")
    set(OpenCL_FOUND 1)
endif()

if(OpenCL_FOUND)
    message(STATUS "OpenCL was found")
    include_directories(${OpenCL_INCLUDE_DIRS})
    list(APPEND LINK_LIBRARIES ${OpenCL_LIBRARIES})
    add_compile_definitions(USE_OPENCL)
else ()
    message(STATUS "OpenCL was not found")
endif ()
