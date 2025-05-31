
#include "../libs/intrinsics.hpp"
#ifdef USE_AVX512
#include "operations_avx512.hpp"

void Operations_AVX512::matrix_vector_multiplikation(const Matrix &matrix_A, double alpha, const Vector &vektor_x,double beta , Vector &vektor_y) {
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



void Operations_AVX512::matrix_vector_multiplikation(const CRS_Matrix &matrix_A, double alpha, const Vector &vektor_x, double beta, Vector &vektor_y) {
    const size_t r_size = matrix_A._num_rows;

    for (size_t i = 0; i < r_size; i++) {
        vektor_y[i] *= beta;
        __m512d sum_vec = _mm512_setzero_pd(); // AVX-512 accumulator

        for (size_t j = matrix_A._row_ptr[i]; j < matrix_A._row_ptr[i + 1]; j += 8) {
            size_t max_k = std::min(size_t(8), matrix_A._row_ptr[i + 1] - j);

            // Ladda 8 värden från matrix_A._data direkt
            __m512d val_vec = _mm512_loadu_pd(&matrix_A._data[j]);

            // Ladda vektor_x värden individuellt p.g.a. oregelbunden indexering
            double x_array[8];
            for (size_t k = 0; k < max_k; k++) {
                x_array[k] = vektor_x[matrix_A._col_index[j + k]];
            }

            __m512d x_vec = _mm512_loadu_pd(x_array);
            __m512d prod_vec = _mm512_mul_pd(val_vec, x_vec);
            sum_vec = _mm512_fmadd_pd(_mm512_set1_pd(alpha), prod_vec, sum_vec);
        }

        // Summera alla element i sum_vec och lägg till resultatet
        vektor_y[i] += _mm512_reduce_add_pd(sum_vec);
    }
}
#endif