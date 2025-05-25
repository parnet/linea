
#ifndef GENERATOR_STD_CRS_MATRIX_HPP
#define GENERATOR_STD_CRS_MATRIX_HPP
#include <iostream>

#include "../data/crs_matrix.hpp"

CRS_Matrix create_laplacian_2d(size_t gridsize){ // 5 for 3 inner nodes and two boundary nodes in one direction


    //std::cout << "===== [ Domain ] ===============" << std::endl;
    //std::cout << "Boundary: "<< num_elems_boundary
    //          << " + Inner: " << num_elems_inner
    //          << " = Total: " << num_elems << std::endl;

    // matrix size
    const int rows = gridsize*gridsize;
    const int cols = gridsize*gridsize;
    //std::cout << "===== [ Domain ] ===============" << std::endl;
    //std::cout << "Operator: " << rows << " x " << cols << std::endl;

    // create data structures
    std::vector<double> data;
    std::vector<int> col_index;
    std::vector<int> row_ptr;

    // preparse structures
    row_ptr.resize(rows+1);
    col_index.clear();
    data.clear();

    // first row pointer is always zero
    row_ptr[0] = 0;


    // boundary full row + first following
    // std::cout << "Create upper boundary"<< std::endl;
    row_ptr[0] = 0;
    //std::cout << 0 << " : " <<  0 <<  std::endl;

    int number_of_values = 0;
    int matrix_row;
    for(matrix_row = 0; matrix_row < gridsize; ++matrix_row) {
        //std::cout << "processing row " << matrix_row << " bnd" << std::endl;
        data.push_back(1);
        col_index.push_back(matrix_row);
        ++number_of_values;

        row_ptr[matrix_row+1] = number_of_values;
        //std::cout << matrix_row+1 << " : " <<  number_of_values <<  std::endl;
    }


    // std::cout << "Create boundary and inner"<< std::endl;
    for(size_t index_row = 1; index_row < gridsize - 1; ++index_row) {
        //std::cout << "processing row " << matrix_row  << " bnd" << std::endl;


        data.push_back(1);
        col_index.push_back(matrix_row);
        ++number_of_values;

        row_ptr[matrix_row+1] = number_of_values;
        //std::cout << matrix_row+1 << " : " <<  number_of_values <<  std::endl;
        matrix_row++;

        for(size_t index_col = 1; index_col < gridsize-1; ++index_col) {
            //std::cout << "processing row " << matrix_row << " inner"<< std::endl;


            size_t other_index = (index_row-1) * gridsize + index_col;

            data.push_back(-1);
            col_index.push_back(other_index);

            data.push_back(-1);
            col_index.push_back(matrix_row-1);


            data.push_back(4);
            col_index.push_back(matrix_row);

            data.push_back(-1);
            col_index.push_back(matrix_row+1);

            other_index = (index_row+1) * gridsize + index_col;
            data.push_back(-1);
            col_index.push_back(other_index);
            number_of_values += 5;


            row_ptr[matrix_row+1] = number_of_values;
            //std::cout << matrix_row+1 << " : " <<  number_of_values <<  std::endl;
            matrix_row++;
        }
        //std::cout << "processing row " << matrix_row << " bnd"<< std::endl;

        data.push_back(1);
        col_index.push_back(matrix_row);
        ++number_of_values;


        row_ptr[matrix_row+1] = number_of_values;
        //std::cout << matrix_row+1 << " : " <<  number_of_values <<  std::endl;
        matrix_row++;
    }

    for(size_t index_col = 0; index_col < gridsize; ++index_col) {
        //std::cout << "processing row " << matrix_row << std::endl;

        data.push_back(1);
        col_index.push_back(matrix_row);

        ++number_of_values;

        row_ptr[matrix_row+1] = number_of_values;
        //std::cout << matrix_row+1 << " : " <<  number_of_values <<  std::endl;
        matrix_row++;
    }
    std::cout << "rows: " << rows << std::endl;
    std::cout << "cols: " << cols << std::endl;
    auto matrix = CRS_Matrix(rows, cols, data, col_index, row_ptr);
    /*
    std::cout << "processing row finished" << std::endl;

    std::cout << "row_ptr" << std::endl;
    for (size_t i = 0; i < row_ptr.size(); i++) {
        std::cout << row_ptr[i] << ", ";
    }
    std::cout << std::endl;
    std::cout << "col_idx" << std::endl;
    for (size_t i = 0; i < col_index.size(); i++) {
        std::cout << col_index[i] << ", ";
    }
    std::cout << std::endl;
    std::cout << "values " << std::endl;
    for (size_t i = 0; i < data.size(); i++) {
        std::cout << data[i] << ", ";
    }*/
    return matrix;
}
#endif
