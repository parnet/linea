find_package(MKL QUIET)

if (MKL_FOUND)
    message(STATUS "[X]   Intel MKL was found version: ${MKL_VERSION}")

else ()
    message(STATUS "[ ]   Intel MKL was not found")
endif ()