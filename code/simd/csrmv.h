
#ifndef CSRMV_H
#define CSRMV_H

#include "data/std_crs_matrix.hpp"
#include "data/vector.hpp"
#include "libs/intrinsics.hpp"


void MatrixVectorMultiplikation_AVX512(const CRS_Matrix &matrix_A, double alpha, const Vector &vektor_x, double beta, Vector &vektor_y) {
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
