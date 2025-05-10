find_package(Eigen3 REQUIRED)

if (Eigen3_FOUND)
    message(STATUS "[X]   Eigen was found version: ${Eigen3_VERSION}")

else ()
    message(STATUS "[ ]   Eigen was not found")
endif ()
