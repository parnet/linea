#pragma once

#include <iostream>

#include "data/crs_matrix.hpp"
#include "data/vector.hpp"

void crs_to_dense(
    std::vector<std::vector<double>>& dense_matrix,
    const std::vector<double>& values,
    const std::vector<int>& col_idx,
    const std::vector<int>& row_ptr,

    int num_rows,
    int num_cols
)
{
    // Initiera med nollor
    dense_matrix.assign(num_rows, std::vector<double>(num_cols, 0.0));

    for (int row = 0; row < num_rows; ++row) {
        int row_start = row_ptr[row];
        int row_end   = row_ptr[row + 1];

        for (int i = row_start; i < row_end; ++i) {
            int col = col_idx[i];
            dense_matrix[row][col] = values[i];
        }
    }
}



inline void extract_diagonal(Vector & vec, const CRS_Matrix & matrix,double omega = 1.0) {
    std::cout << matrix._num_rows << ", " << matrix._num_cols << std::endl;


    for (int row = 0; row < matrix._num_rows; row++) {
        int row_start = matrix._row_ptr[row];
        int row_end = matrix._row_ptr[row+1];
        bool diagonal = false;
        for (int col_idx = row_start; col_idx < row_end; col_idx++) {
            int col = matrix._col_index[col_idx];
            if (col == row) {
                vec._data[row] = omega/matrix._data[col_idx];
                diagonal = true;
            }
        }
        if (!diagonal) {
            std::cout << "error - no diagonal element" << std::endl;
        }
    }

}
