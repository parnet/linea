
set(Kokkos_DIR "/storage/libs/kokkos/bin")
find_package(Kokkos QUIET)

if (Kokkos_FOUND)
    message(STATUS "[X]   Kokkos was found version: ${Kokkos_VERSION}")

else ()
    message(STATUS "[ ]   Kokkos was not found")
endif ()