#include "../libs/intrinsics.hpp"
#ifdef USE_AVX__T // todo
#include "operations_avx.hpp"
void Operations_AVX::matrix_vector_multiplikation(const Matrix &matrix_A, double alpha,
                                                const Vector &vektor_x, double beta, Vector &vektor_y) {
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
#endif