#pragma once
#include <vector>

class BlockVector {
public:
    static constexpr int block_size = 256;
public:
    int size;
    int num_block;
    std::vector<double> _data;

    BlockVector(int size){};
    BlockVector(std::vector<double> _data){};
};
