#ifndef UTIL_MODULES_HPP
#define UTIL_MODULES_HPP


inline bool module_avail_blas() {
#ifdef USE_BLAS
    return true;
#endif
    return false;
}

inline bool module_avail_cuda() {
#ifdef USE_CUDA
    return true;
#endif
    return false;
}

inline bool module_avail_hip() {
#ifdef USE_HIP
    return true;
#endif
    return false;
}

inline bool module_avail_kokkos() {
#ifdef USE_KOKKOS
    return true;
#endif
    return false;
}

inline bool module_avail_lapack() {
#ifdef USE_LAPACK
    return true;
#endif
    return false;
}
inline bool module_avail_lua() {
#ifdef USE_LUA
    return true;
#endif
    return false;
}
inline bool module_avail_mkl() {
#ifdef USE_MKL
    return true;
#endif
    return false;
}
inline bool module_avail_openacc() {
#ifdef USE_MKL
    return true;
#endif
    return false;
}

inline bool module_avail_opencl() {
#ifdef USE_OPENCL
    return true;
#endif
    return false;
}

inline bool module_avail_openmp() {
#ifdef USE_OPENMP
    return true;
#endif
    return false;
}

inline bool module_avail_openmp() {
#ifdef USE_OPENMP
    return true;
#endif
    return false;
}

inline bool module_avail_rocm() {
#ifdef USE_ROCM
    return true;
#endif
    return false;
}

inline bool module_avail_sycl() {
#ifdef USE_ROCM
    return true;
#endif
    return false;
}


inline bool module_avail_opencl() {
#ifdef USE_OPENCL
    return true;
#endif
    return false;
}


#endif
