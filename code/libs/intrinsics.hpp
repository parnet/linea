#pragma once

#ifdef USE_INTRINSICS

#if defined(__AVX512F__)
#include <immintrin.h>  // AVX
#define USE_AVX512 1
#define USE_AVX 1
#define USE_SSE3 1
#define USE_SSE2 1
#define USE_SSE 1

#elif defined(__AVX__)
#include <immintrin.h>  // AVX
#define USE_AVX 1
#define USE_SSE3 1
#define USE_SSE2 1
#define USE_SSE 1

#elif defined(__SSE3__)
#include <pmmintrin.h>  // SSE3
#define USE_SSE3 1
#define USE_SSE2 1
#define USE_SSE 1

#elif defined(__SSE2__)
#include <emmintrin.h>  // SSE2
#define USE_SSE2 1
#define USE_SSE 1

#elif defined(__SSE__)
#include <emmintrin.h>
#define USE_SSE 1
#endif
#endif

