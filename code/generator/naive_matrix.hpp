#ifndef GENERATOR_NAIVE_MATRIX_HPP
#define GENERATOR_NAIVE_MATRIX_HPP

#include <cmath>
#include <functional>


#include "../data/c_matrix.hpp"

void create_laplacian_2d(C_Matrix &matrix,size_t gridsize){ // 5 for 3 inner nodes and two boundary nodes in one direction
    //std::cout << "create_laplacian_2d(NaiveMatrix&)" << std::endl;
    matrix = C_Matrix(gridsize*gridsize,gridsize*gridsize);
    const size_t rows = matrix.rows();
    const size_t cols = matrix.cols();
    //std::cout << "rows = " << rows << std::endl;
    //std::cout << "cols " << cols << std::endl;

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

#endif
