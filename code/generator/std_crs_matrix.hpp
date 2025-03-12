
#ifndef GENERATOR_STD_CRS_MATRIX_HPP
#define GENERATOR_STD_CRS_MATRIX_HPP
#include <iostream>

#include "../data/std_crs_matrix.hpp"

StdCRSMatrix create_laplacian_2d(size_t gridsize){ // 5 for 3 inner nodes and two boundary nodes in one direction

    const size_t num_elems_boundary = 2*gridsize + 2*(gridsize-2);
    const size_t num_elems_inner = (gridsize-2)*(gridsize-2)*5;
    const size_t num_elems = gridsize*gridsize;

    //std::cout << "===== [ Domain ] ===============" << std::endl;
    //std::cout << "Boundary: "<< num_elems_boundary
    //          << " + Inner: " << num_elems_inner
    //          << " = Total: " << num_elems << std::endl;

    // matrix size
    const size_t rows = gridsize*gridsize;
    const size_t cols = gridsize*gridsize;
    //std::cout << "===== [ Domain ] ===============" << std::endl;
    //std::cout << "Operator: " << rows << " x " << cols << std::endl;

    // create data structures
    std::vector<double> data;
    std::vector<size_t> col_index;
    std::vector<size_t> row_ptr;

    // preparse structures
    row_ptr.resize(rows+1);
    col_index.clear();
    data.clear();

    // first row pointer is always zero
    row_ptr[0] = 0;


    // boundary full row + first following
    // std::cout << "Create upper boundary"<< std::endl;
    size_t index = 0;
    for(size_t index_col = 0; index_col < gridsize; ++index_col) {
        data.push_back(1);
        col_index.push_back(index_col);
        //std::cout << index_col << ", " << index_col << ": " << 1<<std::endl;        ++index;
        row_ptr[index] = index;
    }

    // std::cout << "Create boundary and inner"<< std::endl;
    for(size_t index_row = 1; index_row < gridsize - 1; ++index_row) {
        // std::cout << "elem=" << index << std::endl;
        data.push_back(1);
        col_index.push_back(index);
        //std::cout << index << ", " << index << ": " << 1<<std::endl;
        ++index;
        row_ptr[index] = row_ptr[index-1]+1;

        for(size_t index_col = 1; index_col < gridsize-1; ++index_col) {

            size_t other_index = (index_row-1) * gridsize + index_col;
            data.push_back(-1);
            col_index.push_back(other_index);
            //std::cout << index << ", " << other_index << ": " << -1<<std::endl;

            data.push_back(-1);
            col_index.push_back(index-1);
            //std::cout << index << ", " << index-1 << ": " << -1<<std::endl;

            data.push_back(4);
            col_index.push_back(index);
            //std::cout << index << ", " << index << ": " << 4<<std::endl;

            data.push_back(-1);
            col_index.push_back(index+1);
            //std::cout << index << ", " << index+1 << ": " << -1<<std::endl;

            other_index = (index_row+1) * gridsize + index_col;
            data.push_back(-1);
            col_index.push_back(other_index);
            // std::cout << index << ", " << other_index << ": " << -1<<std::endl;
            ++index;
            row_ptr[index] = row_ptr[index-1]+5;
        }

        data.push_back(1);
        col_index.push_back(index);
        //std::cout << index << ", " << index << ": " << 1<<std::endl;
        ++index;
        row_ptr[index] = row_ptr[index-1]+1;
    }
    //std::cout << "Create lower boundary"<< std::endl;
    // boundary full row
    for(size_t index_col = 0; index_col < gridsize; ++index_col) {

        data.push_back(1);
        col_index.push_back(index);
        //std::cout << index << ", " << index << ": " << 1<<std::endl;
        ++index;
        row_ptr[index] = row_ptr[index-1]+1;
    }

    matrix = StdCRSMatrix(rows, cols, data, col_index, row_ptr);


    //std::cout << std::endl;
    //std::cout << std::endl;

    //row_ptr[index] = row_ptr[index-1]+1;

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
    }
    std::cout << std::endl;
    std::cout << index << "\t" << row_ptr.size() <<  std::endl;


    std::cout << std::endl;
    std::cout << std::endl;
    return matrix;
}
#endif
