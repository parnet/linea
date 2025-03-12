#ifndef LINEA_LIB_INTRINSICS_HPP
#define LINEA_LIB_INTRINSICS_HPP

#ifdef USE_INTRINSICS
#if defined(__AVX512F__)
#include <immintrin.h>  // AVX
#elif defined(__SSE3__)
#include <pmmintrin.h>  // SSE3
#elif defined(__SSE2__)
#include <emmintrin.h>  // SSE2
#endif
#endif

#endif