if (EXISTS "/opt/rocm")
    list(APPEND CMAKE_PREFIX_PATH "/opt/rocm")
endif()

set(CMAKE_HIP_ARCHITECTURES "gfx1103")


find_package(hip QUIET)
find_package(hipblas QUIET)
find_package(hipsparse QUIET)
#find_package(hipsolver QUIET)


if (hip_FOUND)
    enable_language(HIP)
    execute_process(
            COMMAND hipconfig --version
            OUTPUT_VARIABLE HIP_VERSION
            ERROR_VARIABLE HIP_VERSION_ERROR
            RESULT_VARIABLE HIP_VERSION_RESULT
    )

    # Check if the command was successful
    if (NOT HIP_VERSION_RESULT EQUAL 0)
        message(FATAL_ERROR "[X]   Failed to get HIP version. Error: ${HIP_VERSION_ERROR}")
    else()
        message(STATUS "[X]   HIP was found: ${HIP_VERSION}")
    endif()

    message(STATUS " |    HIP target - ${HIP_LIBRARIES}")
    message(STATUS " |    HIP compiler: ${HIP_HIPCC_EXECUTABLE}")
    set(CMAKE_CXX_COMPILER "${HIP_HIPCC_EXECUTABLE}")
    add_compile_definitions(USE_HIP)
    list(APPEND LINK_LIBRARIES ${HIP_LIBRARIES})
else ()
    message(STATUS "[ ]   HIP not found on the system")
endif ()

if (hipblas_FOUND)
    message(STATUS "[X]   hipBLAS was found: ${HIPBLAS_LIBRARIES}")
    list(APPEND LINK_LIBRARIES ${HIPBLAS_LIBRARIES})
else ()
    message(STATUS "[ ]   hipBLAS not found on the system")

endif ()


if (hipsparse_FOUND)
    message(STATUS "[X]   hipSparse was found: ${HIPSPARSE_LIBRARIES}")
    list(APPEND LINK_LIBRARIES ${HIPSPARSE_LIBRARIES})
else ()
    message(STATUS "[ ]   hipsparse not found on the system")
endif ()

message(STATUS " |    hip include directory: ${HIP_INCLUDE_DIRS}")
message(STATUS " |    hip library directory: ${HIP_LIBRARIES}")
message(STATUS " |    HIP compiler: ${HIP_HIPCC_EXECUTABLE}")
message(STATUS " |    HIP architecture: ${CMAKE_HIP_ARCHITECTURES}")

get_target_property(TARGETS hip::host IMPORTED_LOCATION)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  --offload-arch=gfx1103") # --offload-arch=gfx1103
set(CMAKE_CXX_COMPILER "${HIP_HIPCC_EXECUTABLE}")
