#ifndef GENERATOR_NAIVE_MATRIX_HPP
#define GENERATOR_NAIVE_MATRIX_HPP

#include <cmath>
#include <functional>

#include "../data/std_crs_matrix.hpp"
#include "../data/std_vector.hpp"
#include "../data/std_matrix.hpp"

void create_laplacian_1d(StdMatrix &matrix,size_t gridsize){ // 5 for 3 inner nodes and two boundary nodes
    matrix = StdMatrix(gridsize,gridsize);
    const size_t rows = matrix.rows();
    const size_t cols = matrix.cols();

    matrix(0,0) = 1;
    for(int index = 1; index < gridsize-1; ++index) {
        matrix(index,index+1) = -1;
        matrix(index,index  ) =  2;
        matrix(index,index-1) = -1;
    }
    matrix(gridsize-1,gridsize-1) = 1;
}

void create_laplacian_2d(StdMatrix &matrix,size_t gridsize){ // 5 for 3 inner nodes and two boundary nodes in one direction
    matrix = StdMatrix(gridsize*gridsize,gridsize*gridsize);
    const size_t rows = matrix.rows();
    const size_t cols = matrix.cols();

    matrix(0,0) = 1;
    for(size_t index_x = 1; index_x < gridsize-1; ++index_x) {
        size_t index_target = index_x*gridsize ;
        matrix(index_target,index_target) = 1;
        index_target = index_x*gridsize + gridsize-1;
        matrix(index_target,index_target) = 1;
    }

    for(size_t index_y = 0; index_y < gridsize; ++index_y) {
        size_t index_target = 0*gridsize +index_y ;
        matrix(index_target,index_target) = 1;

        index_target = (gridsize-1)*gridsize + index_y;
        matrix(index_target,index_target) = 1;
    }

    for(size_t index_x = 1; index_x < gridsize-1; ++index_x) {
        for(size_t index_y = 1; index_y < gridsize-1; ++index_y) {
            size_t index_target = index_x*gridsize + index_y;


            matrix(index_target,index_target-1) = -1;
            matrix(index_target,index_target  ) =  4;
            matrix(index_target,index_target+1) = -1;
            matrix(index_target,index_target-gridsize) = -1;
            matrix(index_target,index_target+gridsize) = -1;

        }
    }
    matrix(rows-1,cols-1) = 1;
}

void create_laplacian_2d(StdCRSMatrix &matrix,size_t gridsize){ // 5 for 3 inner nodes and two boundary nodes in one direction

    const size_t rows = gridsize*gridsize;
    const size_t cols = gridsize*gridsize;

    std::vector<double> data;
    std::vector<size_t> col_index;
    std::vector<size_t> row_ptr;
    row_ptr.resize(rows+1);
    col_index.clear();
    data.clear();
    row_ptr[0] = 0;


    size_t num_elems_boundary = 2*gridsize + 2*(gridsize-2);
    size_t num_elems_inner = (gridsize-2)*(gridsize-2)*5;
    size_t num_elems = num_elems_boundary + num_elems_inner;
    std::cout << num_elems_boundary << " + " << num_elems_inner << " = " << num_elems << std::endl;
    // boundary full row + first following
    std::cout << "Create upper boundary"<< std::endl;
    size_t index=0;
    for(size_t index_col = 0; index_col < gridsize; ++index_col) {
        data.push_back(1);
        col_index.push_back(index_col);
        std::cout << index_col << ", " << index_col << ": " << 1<<std::endl;        ++index;
        row_ptr[index] = index;
    }

    std::cout << "Create boundary and inner"<< std::endl;
    for(size_t index_row = 1; index_row < gridsize - 1; ++index_row) {
        std::cout << "elem=" << index << std::endl;
        data.push_back(1);
        col_index.push_back(index);
        std::cout << index << ", " << index << ": " << 1<<std::endl;
        ++index;
        row_ptr[index] = row_ptr[index-1]+1;

        for(size_t index_col = 1; index_col < gridsize-1; ++index_col) {


            size_t other_index = (index_row-1) * gridsize + index_col;
            data.push_back(-1);
            col_index.push_back(other_index);
            std::cout << index << ", " << other_index << ": " << -1<<std::endl;

            data.push_back(-1);
            col_index.push_back(index-1);
            std::cout << index << ", " << index-1 << ": " << -1<<std::endl;

            data.push_back(4);
            col_index.push_back(index);
            std::cout << index << ", " << index << ": " << 4<<std::endl;

            data.push_back(-1);
            col_index.push_back(index+1);
            std::cout << index << ", " << index+1 << ": " << -1<<std::endl;

            other_index = (index_row+1) * gridsize + index_col;
            data.push_back(-1);
            col_index.push_back(other_index);
            std::cout << index << ", " << other_index << ": " << -1<<std::endl;
            ++index;
            row_ptr[index] = row_ptr[index-1]+5;
        }

        data.push_back(1);
        col_index.push_back(index);
        std::cout << index << ", " << index << ": " << 1<<std::endl;
        ++index;
        row_ptr[index] = row_ptr[index-1]+1;
    }
    std::cout << "Create lower boundary"<< std::endl;
    // boundary full row
    for(size_t index_col = 0; index_col < gridsize; ++index_col) {

        data.push_back(1);
        col_index.push_back(index);
        std::cout << index << ", " << index << ": " << 1<<std::endl;
        ++index;
        row_ptr[index] = row_ptr[index-1]+1;
    }
    std::cout << std::endl;
    std::cout << std::endl;

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
    matrix = StdCRSMatrix(rows, cols, data, col_index, row_ptr);
}

inline void create_sin_1d(StdVector &v) {
    const size_t size = v.size();
    const auto dsize = static_cast<double>(size);
    for (size_t i = 0; i < size; ++i) {
        v.operator[](i) = sin(2 * M_PI * (static_cast<double>(i) / (dsize)));
    };
}

inline void create_cos_1d(StdVector &v) {
    const size_t size = v.size();
    const auto dsize = static_cast<double>(size) -2 ;
    for (size_t i = 0; i < size; ++i) {
        v.operator[](i) = cos(2 * M_PI * (static_cast<double>(i) / (dsize)));
    };
}

inline void create_fun(StdVector &v,const std::function<double (size_t)>& fun) {
    const size_t size = v.size();
    for (size_t i = 0; i < size; ++i) {
        v.operator[](i) = fun(i);
    };
}
#endif //GENERATOR_HPP

#endif //NAIVE_MATRIX_H
