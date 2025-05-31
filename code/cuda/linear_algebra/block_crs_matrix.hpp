#pragma once
#include <vector>

class Block_CRS_Matrix_Structure {
    public:
    std::vector<int> _row_ptr;
    std::vector<int> _col_ind;
};


class Block_CRS_Matrix_Data {
    public:
    Block_CRS_Matrix_Data();

    std::vector<double> _values;
    Block_CRS_Matrix_Structure _structure;

};

class Block_CRS_Matrix {
    public:
    Block_CRS_Matrix();
    std::vector<int> _row_ptr;
    std::vector<int> _col_ind;
    std::vector<double> _values;
};
