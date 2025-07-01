#ifndef HOST_HPP
#define HOST_HPP
#include "data/vector.hpp"
#include "mpi_omp_cuda/data/crs_matrix.hpp"

inline void create_zero(Vector & defect) {
    std::cout << "todo - implement" << std::endl;
}

inline void extract_diagonal(Vector & diag, CRS_Matrix & matrix, double omega) {
    std::cout << "todo - implement" << std::endl;

}

inline void laplace_rhs_function(Vector & rhs, int Nx, int Nx, double alpha) {
    std::cout << "todo - implement" << std::endl;

}

inline CRS_Matrix create_laplacian_2d(int num_x, int num_y) {
    const int global_rows = num_x * num_y;
    const int global_cols = num_x * num_y;  // square matrix

    std::cout <<"global_rows = " << global_rows << std::endl;

    std::vector<double> data;
    std::vector<int> col_index;
    std::vector<int> row_ptr(global_rows + 1, 0);

    int i,j;
    std::cout << "gridsize = " << num_x << std::endl;
    std::cout << "gridsize = " << num_y << std::endl;
    for (int global_row = 0; global_row < global_rows; ++global_row) {

        row_ptr[global_row] = static_cast<int>(data.size());

        i = global_row / num_x;
        j = global_row % num_x;
        std::cout << "i = " << i << ", j = " << j;

        if (i == 0 || i == num_x - 1 || j == 0 || j == num_y - 1) {
            std::cout << "t" << std::endl;
            data.push_back(1.0);
            col_index.push_back(global_row);
        } else {
            std::cout << "f" << std::endl;
            // Interior node: 5-point stencil
            int center = global_row;
            int left   = center - 1;
            int right  = center + 1;
            int up     = center - num_x;
            int down   = center + num_x;

            data.push_back(-1.0); col_index.push_back(up);
            data.push_back(-1.0); col_index.push_back(left);
            data.push_back( 4.0); col_index.push_back(center);
            data.push_back(-1.0); col_index.push_back(right);
            data.push_back(-1.0); col_index.push_back(down);
        }
    }


    row_ptr[global_rows] = static_cast<int>(data.size());
    return CRS_Matrix(global_rows, global_cols,row_ptr,col_index, data );
}



#endif