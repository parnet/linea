#pragma once
/*
#include <vector>

#include "distribution.hpp"
#include "data/data.hpp"

Local_CRS_Matrix create_laplacian_2d(int gridsize, int rank, int size) {
    const int global_rows = gridsize * gridsize;
    const int global_cols = global_rows;  // square matrix

    int local_start = 0;
    int local_size = 0;
    distribution_slice(local_start, local_size, global_rows, rank, size);
    std::cout <<"global_rows = " << global_rows << std::endl;
    std::cout <<"local_size = " << local_size << std::endl;
    int local_end = local_start + local_size;

    std::vector<double> data;
    std::vector<int> col_index;
    std::vector<int> row_ptr(local_size + 1, 0);

    int i,j;
    std::cout << "gridsize = " << gridsize << std::endl;
    std::cout << "end = " << local_end << std::endl;
    for (int global_row = local_start; global_row < local_end; ++global_row) {
        int local_row = global_row - local_start;
        row_ptr[local_row] = static_cast<int>(data.size());

        i = global_row / gridsize;
        j = global_row % gridsize;
        std::cout << "i = " << i << ", j = " << j;

        if (i == 0 || i == gridsize - 1 || j == 0 || j == gridsize - 1) {
            std::cout << "t" << std::endl;
            data.push_back(1.0);
            col_index.push_back(global_row);
        } else {
            std::cout << "f" << std::endl;
            // Interior node: 5-point stencil
            int center = global_row;
            int left   = center - 1;
            int right  = center + 1;
            int up     = center - gridsize;
            int down   = center + gridsize;

            data.push_back(-1.0); col_index.push_back(up);
            data.push_back(-1.0); col_index.push_back(left);
            data.push_back( 4.0); col_index.push_back(center);
            data.push_back(-1.0); col_index.push_back(right);
            data.push_back(-1.0); col_index.push_back(down);
        }
    }
}


*/