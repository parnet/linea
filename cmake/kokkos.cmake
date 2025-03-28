
#set(Kokkos_DIR "/storage/libs/kokkos/bin")
#list(APPEND CMAKE_PREFIX_PATH "/storage/libs/kokkos/cmake")

find_package(Kokkos QUIET)

if (Kokkos_FOUND)
    set(Kokkos_LIBRARIES "Kokkos::Kokkos")
    message(STATUS "[X]   Kokkos was found version: ${Kokkos_VERSION}")
    message(STATUS "Kokkos found version: ${Kokkos_VERSION}")
    message(STATUS "Kokkos libraries: ${Kokkos_LIBRARIES}")
    get_property(targets GLOBAL PROPERTY TARGETS)
    message(STATUS "Available targets: ${targets}")

else ()
    message(STATUS "[ ]   Kokkos was not found")
endif ()