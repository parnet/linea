#include "sse2.hpp"
#include "../libs/intrinsics.hpp"

void Operations_SSE2::matrix_vector_multiplikation(const Matrix &matrix_A, double alpha,
                                                   const Vector &vektor_x, double beta, Vector &vektor_y) {
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
