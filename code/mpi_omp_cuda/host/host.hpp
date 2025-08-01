#ifndef HOST_HPP
#define HOST_HPP

#include "mpi_omp_cuda/data/data.hpp"

inline void create_zero(Vector & defect) {
    const size_t size = defect._num_elements;
    for (size_t i = 0; i < size; ++i) {
        defect._values[i] = 0;
    };
}

inline void extract_diagonal(Vector & diag, CRS_Matrix & matrix, double omega) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::cout << "rank=" << rank << "\t"<< matrix._num_rows << ", " << matrix._num_cols << std::endl;

    for (int row = 0; row < matrix._num_rows; row++) {
        int row_start = matrix._rows_pointer[row];
        int row_end = matrix._rows_pointer[row+1];
        bool diagonal = false;
        for (int col_idx = row_start; col_idx < row_end; col_idx++) {
            int col = matrix._col_index[col_idx];
            if (col == row) {
                diag._values[row] = omega/matrix._values[col_idx];
                diagonal = true;
            }
        }
        if (!diagonal) {
            std::cout << "error - no diagonal element" << std::endl;
        }
    }
}

inline void laplace_rhs_function_2d(Vector & rhs, int Nx, int Ny, double alpha, int y_offset) {

    for (std::size_t i = 0; i < Ny; ++i) {
        double y = static_cast<double>(i+y_offset) / static_cast<double>(Nx-1);
        for (std::size_t j = 0; j < Nx; ++j) {
            double x = static_cast<double>(j) / static_cast<double>(Nx-1);
            rhs._values[i*Nx+j] = 2 * alpha* M_PI*sin(M_PI*x)*sin(M_PI*y);
        }
    }

}

inline CRS_Matrix create_laplacian_2d(int num_x, int num_y) {
        const int global_rows = num_x * num_y;
    const int global_cols = num_x * num_y;  // square matrix

    // std::cout <<"global_rows = " << global_rows << std::endl;

    std::vector<double> data;
    std::vector<int> col_index;
    std::vector<int> row_ptr(global_rows + 1, 0);

    int i,j;
    //std::cout << "gridsize = " << num_x << std::endl;
    //std::cout << "gridsize = " << num_y << std::endl;
    for (int global_row = 0; global_row < global_rows; ++global_row) {

        row_ptr[global_row] = static_cast<int>(data.size());

        i = global_row % num_x;
        j = global_row / num_x;
        //std::cout << "i = " << i << ", j = " << j;

        if (i == 0 || i == num_x - 1 || j == 0 || j == num_y - 1) {
            //std::cout << "t" << std::endl;
            data.push_back(1.0);
            col_index.push_back(global_row);
        } else {
            //std::cout << "f" << std::endl;
            //std::cout << "num_x=" << num_x << " num_y="<<num_y << std::endl;
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



inline CRS_Matrix create_laplacian_3d(int num_x, int num_y, int num_z) {
    const int global_rows = num_x * num_y * num_z;
    const int global_cols = global_rows; // square matrix

    std::vector<double> data;
    std::vector<int> col_index;
    std::vector<int> row_ptr(global_rows + 1, 0);

    int i, j, k;

    int num_xy = num_x * num_y;
    for (int global_row = 0; global_row < global_rows; ++global_row) {
        row_ptr[global_row] = static_cast<int>(data.size());

        i = global_row % num_x;
        j = (global_row / num_x) % num_y;
        k = global_row / num_xy;

        if (i == 0 || i == num_x - 1 ||
            j == 0 || j == num_y - 1 ||
            k == 0 || k == num_z - 1) {
            // Boundary node: Dirichlet condition
            data.push_back(1.0);
            col_index.push_back(global_row);
            } else {
                // Interior node: 7-point stencil
                int center = global_row;
                int left   = center - 1;
                int right  = center + 1;
                int up     = center - num_x;
                int down   = center + num_x;
                int front  = center - num_x * num_y;
                int back   = center + num_x * num_y;

                data.push_back(-1.0); col_index.push_back(front);
                data.push_back(-1.0); col_index.push_back(up);
                data.push_back(-1.0); col_index.push_back(left);
                data.push_back( 6.0); col_index.push_back(center);
                data.push_back(-1.0); col_index.push_back(right);
                data.push_back(-1.0); col_index.push_back(down);
                data.push_back(-1.0); col_index.push_back(back);
            }
    }

    row_ptr[global_rows] = static_cast<int>(data.size());

    return CRS_Matrix(global_rows, global_cols, row_ptr, col_index ,data);
}



inline void laplace_rhs_function_3d(Vector & rhs, int Nx, int Ny, int Nz, double alpha, int z_offset) {
    const int Nxy = Nx * Ny;
    for (std::size_t iz = 0; iz < Nz; ++iz) {
        double z = static_cast<double>(iz+z_offset) / static_cast<double>(Nx-1);

        for (std::size_t iy = 0; iy < Nx; ++iy) {
            double y = static_cast<double>(iy) / static_cast<double>(Ny-1);

            for (std::size_t ix = 0; ix < Nx; ++ix) {
                double x = static_cast<double>(ix) / static_cast<double>(Nx-1);
                rhs._values[iz*Nxy+iy*Nx+ix] = 3 * alpha* M_PI * sin(M_PI*x)*sin(M_PI*y)*sin(M_PI*z);
            }

        }

    }
}




#endif