
#ifndef CSRMV_H
#define CSRMV_H

#include "data/std_crs_matrix.hpp"
#include "data/std_vector.hpp"
#include "libs/intrinsics.hpp"


void MatrixVectorMultiplikation_AVX512_nonalignas(const StdCRSMatrix &matrix_A, double alpha, const StdVector &vektor_x, double beta, StdVector &vektor_y) {
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

void MatrixVectorMultiplikation_AVX512(const StdCRSMatrix &matrix_A, double alpha, const StdVector &vektor_x, double beta, StdVector &vektor_y) {
    const size_t r_size = matrix_A._num_rows;

    for (size_t i = 0; i < r_size; i++) {
        vektor_y[i] *= beta;
        __m512d sum_vec = _mm512_setzero_pd();

        for (size_t j = matrix_A._row_ptr[i]; j < matrix_A._row_ptr[i + 1]; j += 8) {
            // Ladda 8 värden från `matrix_A._data`
            __m512d val_vec = _mm512_load_pd(&matrix_A._data[j]); // Alignerad laddning för bättre prestanda

            // Ladda 8 index för vektor_x
            __m256i index_vec = _mm256_load_si256(reinterpret_cast<const __m256i*>(&matrix_A._col_index[j]));

            // Gather-laddning från vektor_x
            __m512d x_vec = _mm512_i32gather_pd(index_vec, vektor_x.data(), sizeof(double));

            // Beräkning
            __m512d prod_vec = _mm512_mul_pd(val_vec, x_vec);
            sum_vec = _mm512_fmadd_pd(_mm512_set1_pd(alpha), prod_vec, sum_vec);
        }

        // Summera alla element i `sum_vec`
        vektor_y[i] += _mm512_reduce_add_pd(sum_vec);
    }
}
#endif //CSRMV_H
