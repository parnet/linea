find_package(HDF5 QUIET COMPONENTS C CXX)
# find_package(HDF5)
if(HDF5_FOUND)
    message(STATUS "[X]   HDF5_VERSION was found version " ${HDF5_VERSION})
    include_directories(${HDF5_INCLUDE_DIR})
    # todo link against : hdf5 hdf5_cpp
else ()
    message(STATUS "[ ]   HDF5 was not found")
endif ()