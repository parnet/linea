#ifndef SIMD_MV_HPP
#define SIMD_MV_HPP

#include "libs/intrinsics.hpp"  // AVX-512

#include "../data/std_flat_matrix.hpp"
#include "../data/std_vector.hpp"



void MatrixVectorMultiplikationSSE2(const StdFlatMatrix &matrix_A, double alpha,
                                    const StdVector &vektor_x, double beta, StdVector &vektor_y) {
    const size_t mr = matrix_A._num_rows;
    const size_t mc = matrix_A._num_cols;

    // Steg 1: Skalera vektor_y med beta med SSE3
    for (size_t i = 0; i < mr; i += 2) {  // SSE hanterar 2 doubles per register
        __m128d beta_vec = _mm_set1_pd(beta);
        __m128d y_vec = _mm_loadu_pd(&vektor_y._data[i]);  // Ladda 2 double-värden
        y_vec = _mm_mul_pd(y_vec, beta_vec);               // y = y * beta (SIMD)
        _mm_storeu_pd(&vektor_y._data[i], y_vec);          // Spara resultatet
    }

    // Steg 2: Matris-vektor-multiplikation (A * x)
    for (size_t i = 0; i < mr; ++i) {
        __m128d sum_vec = _mm_setzero_pd();  // Initiera SIMD-summa till 0

        for (size_t j = 0; j < mc; j += 2) {  // Bearbeta 2 element i taget
            __m128d a_vec = _mm_loadu_pd(&matrix_A._data[i * mc + j]); // Ladda 2 element från matrisen
            __m128d x_vec = _mm_loadu_pd(&vektor_x._data[j]);          // Ladda 2 element från vektor_x
            __m128d mul_vec = _mm_mul_pd(a_vec, x_vec);                // Elementvis multiplikation
            sum_vec = _mm_add_pd(sum_vec, mul_vec);                    // Summera resultaten
        }

        // Horisontell summering av sum_vec (2 element till en skalar)
        double sum[2];
        _mm_storeu_pd(sum, sum_vec);
        double total_sum = sum[0] + sum[1];

        // Uppdatera vektor_y
        vektor_y._data[i] += alpha * total_sum;
    }
}

void MatrixVectorMultiplikationAVX(const StdFlatMatrix &matrix_A, double alpha,
                                   const StdVector &vektor_x, double beta, StdVector &vektor_y) {
    const size_t mr = matrix_A._num_rows;
    const size_t mc = matrix_A._num_cols;

    // Steg 1: Skalera vektor_y med beta med AVX
    for (size_t i = 0; i < mr; i += 4) {  // AVX hanterar 4 doubles per register
        __m256d beta_vec = _mm256_set1_pd(beta);
        __m256d y_vec = _mm256_loadu_pd(&vektor_y._data[i]);  // Ladda 4 double-värden
        y_vec = _mm256_mul_pd(y_vec, beta_vec);               // y = y * beta (SIMD)
        _mm256_storeu_pd(&vektor_y._data[i], y_vec);          // Spara resultatet
    }

    // Steg 2: Matris-vektor-multiplikation (A * x)
    for (size_t i = 0; i < mr; ++i) {
        __m256d sum_vec = _mm256_setzero_pd();  // Initiera SIMD-summa till 0

        for (size_t j = 0; j < mc; j += 4) {  // Bearbeta 4 element i taget
            __m256d a_vec = _mm256_loadu_pd(&matrix_A._data[i * mc + j]); // Ladda 4 element från matrisen
            __m256d x_vec = _mm256_loadu_pd(&vektor_x._data[j]);          // Ladda 4 element från vektor_x
            __m256d mul_vec = _mm256_mul_pd(a_vec, x_vec);                // Elementvis multiplikation
            sum_vec = _mm256_add_pd(sum_vec, mul_vec);                    // Summera resultaten
        }

        // Horisontell summering av sum_vec (sammanfoga de 4 elementen till en skalar)
        double sum[4];
        _mm256_storeu_pd(sum, sum_vec);
        double total_sum = sum[0] + sum[1] + sum[2] + sum[3];

        // Uppdatera vektor_y
        vektor_y._data[i] += alpha * total_sum;
    }
}


void MatrixVectorMultiplikationAVX512(const StdFlatMatrix &matrix_A, double alpha,
                                      const StdVector &vektor_x,double beta , StdVector &vektor_y) {
    const size_t mr = matrix_A._num_rows;
    const size_t mc = matrix_A._num_cols;

    // Steg 1: Skalera vektor_y med beta med AVX-512
    for (size_t i = 0; i < mr; i += 8) {  // AVX-512 hanterar 8 doubles per register
        __m512d beta_vec = _mm512_set1_pd(beta);
        __m512d y_vec = _mm512_loadu_pd(&vektor_y._data[i]);  // Ladda 8 double-värden
        y_vec = _mm512_mul_pd(y_vec, beta_vec);               // y = y * beta (SIMD)
        _mm512_storeu_pd(&vektor_y._data[i], y_vec);          // Spara resultatet
    }

    // Steg 2: Matris-vektor-multiplikation (A * x)
    for (size_t i = 0; i < mr; ++i) {
        __m512d sum_vec = _mm512_setzero_pd();  // Initiera SIMD-summa till 0

        for (size_t j = 0; j < mc; j += 8) {  // Bearbeta 8 element i taget
            __m512d a_vec = _mm512_loadu_pd(&matrix_A._data[i * mc + j]); // Ladda 8 element från matrisen
            __m512d x_vec = _mm512_loadu_pd(&vektor_x._data[j]);          // Ladda 8 element från vektor_x
            __m512d mul_vec = _mm512_mul_pd(a_vec, x_vec);              // elementvis multiplikation
            sum_vec = _mm512_add_pd(sum_vec, mul_vec);                // summera resultaten
        }

        // Horisontell summering av sum_vec (sammanfoga de 8 elementen till en skalar)
        double sum = _mm512_reduce_add_pd(sum_vec);

        // Uppdatera vektor_y
        vektor_y._data[i] += alpha * sum;
    }
}
#endif
