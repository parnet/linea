#pragma once
#include "data/block_vector.hpp"
#include <cmath>

void b_sin(BlockVector & vector, int block) {
    const int offset = block * BlockVector::block_size;
    const int limit = offset + BlockVector::block_size;
    for(int i = offset; i < limit; ++i) {
        vector._data[i] = sin(vector._data[i]);
    }
}

void b_sin(BlockVector & vector) {
    int offset = vector.size / BlockVector::block_size;
    for(int i = offset; i < vector.size; ++i) {
        vector._data[i] = sin(vector._data[i]);
    }
}

void b_cos(BlockVector & vector, int block) {
    const int offset = block * BlockVector::block_size;
    const int limit = offset + BlockVector::block_size;
    for(int i = offset; i < limit; ++i) {
        vector._data[i] = cos(vector._data[i]);
    }
}

void b_cos(BlockVector & vector) {
    int offset = vector.size / BlockVector::block_size;
    for(int i = offset; i < vector.size; ++i) {
        vector._data[i] = cos(vector._data[i]);
    }
}

void b_tan(BlockVector & vector, int block) {
    const int offset = block * BlockVector::block_size;
    const int limit = offset + BlockVector::block_size;
    for(int i = offset; i < limit; ++i) {
        vector._data[i] = cos(vector._data[i]);
    }
}

void b_tan(BlockVector & vector) {
    int offset = vector.size / BlockVector::block_size;
    for(int i = offset; i < vector.size; ++i) {
        vector._data[i] = cos(vector._data[i]);
    }
}

void b_exp(BlockVector & vector, int block) {
    const int offset = block * BlockVector::block_size;
    const int limit = offset + BlockVector::block_size;
    for(int i = offset; i < limit; ++i) {
        vector._data[i] = exp(vector._data[i]);
    }
}

void b_exp(BlockVector & vector) {
    int offset = vector.size / BlockVector::block_size;
    for(int i = offset; i < vector.size; ++i) {
        vector._data[i] = exp(vector._data[i]);
    }
}

void b_log(BlockVector & vector, int block) {
    const int offset = block * BlockVector::block_size;
    const int limit = offset + BlockVector::block_size;
    for(int i = offset; i < limit; ++i) {
        vector._data[i] = log(vector._data[i]);
    }
}

void b_log(BlockVector & vector) {
    int offset = vector.size / BlockVector::block_size;
    for(int i = offset; i < vector.size; ++i) {
        vector._data[i] = log(vector._data[i]);
    }
}

void b_log10(BlockVector & vector, int block) {
    const int offset = block * BlockVector::block_size;
    const int limit = offset + BlockVector::block_size;
    for(int i = offset; i < limit; ++i) {
        vector._data[i] = log10(vector._data[i]);
    }
}

void b_log10(BlockVector & vector) {
    int offset = vector.size / BlockVector::block_size;
    for(int i = offset; i < vector.size; ++i) {
        vector._data[i] = log10(vector._data[i]);
    }
}

void b_sqrt(BlockVector & vector, int block) {
    const int offset = block * BlockVector::block_size;
    const int limit = offset + BlockVector::block_size;
    for(int i = offset; i < limit; ++i) {
        vector._data[i] = sqrt(vector._data[i]);
    }
}

void b_sqrt(BlockVector & vector) {
    int offset = vector.size / BlockVector::block_size;
    for(int i = offset; i < vector.size; ++i) {
        vector._data[i] = sqrt(vector._data[i]);
    }
}

void b_abs(BlockVector & vector, int block) {
    const int offset = block * BlockVector::block_size;
    const int limit = offset + BlockVector::block_size;
    for(int i = offset; i < limit; ++i) {
        vector._data[i] = abs(vector._data[i]);
    }
}

void b_abs(BlockVector & vector) {
    int offset = vector.size / BlockVector::block_size;
    for(int i = offset; i < vector.size; ++i) {
        vector._data[i] = abs(vector._data[i]);
    }
}


void b_add(BlockVector & vector, int block, double alpha) {
    const int offset = block * BlockVector::block_size;
    const int limit = offset + BlockVector::block_size;
    for(int i = offset; i < limit; ++i) {
        vector._data[i] += alpha;
    }
}

void b_add(BlockVector & vector, double alpha) {
    int offset = vector.size / BlockVector::block_size;
    for(int i = offset; i < vector.size; ++i) {
        vector._data[i] += alpha;
    }
}


void b_sub(BlockVector & vector, int block, double alpha) {
    const int offset = block * BlockVector::block_size;
    const int limit = offset + BlockVector::block_size;
    for(int i = offset; i < limit; ++i) {
        vector._data[i] -= alpha;
    }
}

void b_sub(BlockVector & vector, double alpha) {
    int offset = vector.size / BlockVector::block_size;
    for(int i = offset; i < vector.size; ++i) {
        vector._data[i] -= alpha;
    }
}

void b_mul(BlockVector & vector, int block, double alpha) {
    const int offset = block * BlockVector::block_size;
    const int limit = offset + BlockVector::block_size;
    for(int i = offset; i < limit; ++i) {
        vector._data[i] *= alpha;
    }
}

void b_mul(BlockVector & vector, double alpha) {
    int offset = vector.size / BlockVector::block_size;
    for(int i = offset; i < vector.size; ++i) {
        vector._data[i] *= alpha;
    }
}

void b_div(BlockVector & vector, int block, double alpha) {
    const int offset = block * BlockVector::block_size;
    const int limit = offset + BlockVector::block_size;
    for(int i = offset; i < limit; ++i) {
        vector._data[i] /= alpha;
    }
}

void b_div(BlockVector & vector, double alpha) {
    int offset = vector.size / BlockVector::block_size;
    for(int i = offset; i < vector.size; ++i) {
        vector._data[i] /= alpha;
    }
}

void b_pow(BlockVector & vector, int block, double alpha) {
    const int offset = block * BlockVector::block_size;
    const int limit = offset + BlockVector::block_size;
    for(int i = offset; i < limit; ++i) {
        vector._data[i] = pow(vector._data[i],alpha);
    }
}

void b_pow(BlockVector & vector, double alpha) {
    int offset = vector.size / BlockVector::block_size;
    for(int i = offset; i < vector.size; ++i) {
        vector._data[i] = pow(vector._data[i], alpha);
    }
}


void b_add(BlockVector & result, int block, BlockVector & vector, double alpha) {
    const int offset = block * BlockVector::block_size;
    const int limit = offset + BlockVector::block_size;
    for(int i = offset; i < limit; ++i) {
        result._data[i] = vector._data[i] + alpha;
    }
}

void b_add(BlockVector & result,BlockVector & vector, double alpha) {
    int offset = vector.size / BlockVector::block_size;
    for(int i = offset; i < vector.size; ++i) {
        result._data[i] = vector._data[i]+ alpha;
    }
}


void b_sub(BlockVector & result, int block,BlockVector & vector, double alpha) {
    const int offset = block * BlockVector::block_size;
    const int limit = offset + BlockVector::block_size;
    for(int i = offset; i < limit; ++i) {
        result._data[i] =vector._data[i]- alpha;
    }
}

void b_sub(BlockVector & result,BlockVector & vector, double alpha) {
    int offset = vector.size / BlockVector::block_size;
    for(int i = offset; i < vector.size; ++i) {
        result._data[i] = vector._data[i] - alpha;
    }
}

void b_mul(BlockVector & result, int block,BlockVector & vector, double alpha) {
    const int offset = block * BlockVector::block_size;
    const int limit = offset + BlockVector::block_size;
    for(int i = offset; i < limit; ++i) {
        result._data[i] = vector._data[i]* alpha;
    }
}

void b_mul(BlockVector & result,BlockVector & vector, double alpha) {
    int offset = vector.size / BlockVector::block_size;
    for(int i = offset; i < vector.size; ++i) {
        result._data[i] = vector._data[i] * alpha;
    }
}

void b_div(BlockVector & result, int block,BlockVector & vector, double alpha) {
    const int offset = block * BlockVector::block_size;
    const int limit = offset + BlockVector::block_size;
    for(int i = offset; i < limit; ++i) {
        result._data[i] = vector._data[i]/ alpha;
    }
}

void b_div(BlockVector & result, BlockVector & vector, double alpha) {
    int offset = vector.size / BlockVector::block_size;
    for(int i = offset; i < vector.size; ++i) {
        result._data[i] = vector._data[i] / alpha;
    }
}

void b_pow(BlockVector & result, int block,BlockVector & vector, double alpha) {
    const int offset = block * BlockVector::block_size;
    const int limit = offset + BlockVector::block_size;
    for(int i = offset; i < limit; ++i) {
        result._data[i] = pow(vector._data[i],alpha);
    }
}

void b_pow(BlockVector & result,BlockVector & vector, double alpha) {
    int offset = vector.size / BlockVector::block_size;
    for(int i = offset; i < vector.size; ++i) {
        result._data[i] = pow(vector._data[i], alpha);
    }
}


void b_add( BlockVector & vector_a, int block, BlockVector & vector_b) {
    const int offset = block * BlockVector::block_size;
    const int limit = offset + BlockVector::block_size;
    for(int i = offset; i < limit; ++i) {
        vector_a._data[i] = vector_a._data[i] + vector_b._data[i];
    }
}

void b_add(BlockVector & vector_a, BlockVector & vector_b) {
    int offset = vector_a.size / BlockVector::block_size;
    for(int i = offset; i < vector_a.size; ++i) {
        vector_a._data[i] = vector_a._data[i] + vector_b._data[i];
    }
}


void b_sub(BlockVector & vector_a,int block, BlockVector & vector_b) {
    const int offset = block * BlockVector::block_size;
    const int limit = offset + BlockVector::block_size;
    for(int i = offset; i < limit; ++i) {
        vector_a._data[i] = vector_a._data[i] - vector_b._data[i];
    }
}

void b_sub(BlockVector & vector_a, BlockVector & vector_b) {
    int offset = vector_a.size / BlockVector::block_size;
    for(int i = offset; i < vector_a.size; ++i) {
        vector_a._data[i] = vector_a._data[i] - vector_b._data[i];
    }
}

void b_mul(BlockVector & vector_a,int block, BlockVector & vector_b) {
    const int offset = block * BlockVector::block_size;
    const int limit = offset + BlockVector::block_size;
    for(int i = offset; i < limit; ++i) {
        vector_a._data[i] = vector_a._data[i] * vector_b._data[i];
    }
}

void b_mul(BlockVector & vector_a, BlockVector & vector_b) {
    int offset = vector_a.size / BlockVector::block_size;
    for(int i = offset; i < vector_a.size; ++i) {
        vector_a._data[i] = vector_a._data[i] * vector_b._data[i];
    }
}

void b_div( BlockVector & vector_a,int block, BlockVector & vector_b) {
    const int offset = block * BlockVector::block_size;
    const int limit = offset + BlockVector::block_size;
    for(int i = offset; i < limit; ++i) {
        vector_a._data[i] = vector_a._data[i] / vector_b._data[i];
    }
}

void b_div(BlockVector & vector_a, BlockVector & vector_b) {
    int offset = vector_a.size / BlockVector::block_size;
    for(int i = offset; i < vector_a.size; ++i) {
        vector_a._data[i] = vector_a._data[i] / vector_b._data[i];
    }
}

void b_pow( BlockVector & vector_a,int block, BlockVector & vector_b) {
    const int offset = block * BlockVector::block_size;
    const int limit = offset + BlockVector::block_size;
    for(int i = offset; i < limit; ++i) {
        vector_a._data[i] = pow(vector_a._data[i], vector_b._data[i]);
    }
}

void b_pow(BlockVector & vector_a, BlockVector & vector_b) {
    int offset = vector_a.size / BlockVector::block_size;
    for(int i = offset; i < vector_a.size; ++i) {
        vector_a._data[i] = pow(vector_a._data[i], vector_b._data[i]);
    }
}




void b_add(BlockVector & result, int block, BlockVector & vector_a, BlockVector & vector_b) {
    const int offset = block * BlockVector::block_size;
    const int limit = offset + BlockVector::block_size;
    for(int i = offset; i < limit; ++i) {
        result._data[i] = vector_a._data[i] + vector_b._data[i];
    }
}

void b_add(BlockVector & result,BlockVector & vector_a, BlockVector & vector_b) {
    int offset = result.size / BlockVector::block_size;
    for(int i = offset; i < result.size; ++i) {
        result._data[i] = vector_a._data[i] + vector_b._data[i];
    }
}


void b_sub(BlockVector & result, int block,BlockVector & vector_a, BlockVector & vector_b) {
    const int offset = block * BlockVector::block_size;
    const int limit = offset + BlockVector::block_size;
    for(int i = offset; i < limit; ++i) {
        result._data[i] =vector_a._data[i] - vector_b._data[i];
    }
}

void b_sub(BlockVector & result,BlockVector & vector_a, BlockVector & vector_b) {
    int offset = result.size / BlockVector::block_size;
    for(int i = offset; i < result.size; ++i) {
        result._data[i] = vector_a._data[i] - vector_b._data[i];
    }
}

void b_mul(BlockVector & result, int block,BlockVector & vector_a, BlockVector & vector_b) {
    const int offset = block * BlockVector::block_size;
    const int limit = offset + BlockVector::block_size;
    for(int i = offset; i < limit; ++i) {
        result._data[i] = vector_a._data[i] * vector_b._data[i];
    }
}

void b_mul(BlockVector & result,BlockVector & vector_a, BlockVector & vector_b) {
    int offset = result.size / BlockVector::block_size;
    for(int i = offset; i < result.size; ++i) {
        result._data[i] = vector_a._data[i] * vector_b._data[i];
    }
}

void b_div(BlockVector & result, int block,BlockVector & vector_a, BlockVector & vector_b) {
    const int offset = block * BlockVector::block_size;
    const int limit = offset + BlockVector::block_size;
    for(int i = offset; i < limit; ++i) {
        result._data[i] = vector_a._data[i] / vector_b._data[i];
    }
}

void b_div(BlockVector & result, BlockVector & vector_a, BlockVector & vector_b) {
    int offset = result.size / BlockVector::block_size;
    for(int i = offset; i < result.size; ++i) {
        result._data[i] = vector_a._data[i] / vector_b._data[i];
    }
}

void b_pow(BlockVector & result, int block,BlockVector & vector_a, BlockVector & vector_b) {
    const int offset = block * BlockVector::block_size;
    const int limit = offset + BlockVector::block_size;
    for(int i = offset; i < limit; ++i) {
        result._data[i] = pow(vector_a._data[i] , vector_b._data[i]);
    }
}

void b_pow(BlockVector & result, BlockVector & vector_a, BlockVector & vector_b) {
    int offset = result.size / BlockVector::block_size;
    for(int i = offset; i < result.size; ++i) {
        result._data[i] = pow(vector_a._data[i], vector_b._data[i]);
    }
}


void b_1_norm(double & result, int block, BlockVector & vector_a) {
    const int offset = block * BlockVector::block_size;
    const int limit = offset + BlockVector::block_size;
    double sum = 0.0;
    for(int i = offset; i < limit; ++i) {
        sum += std::abs(vector_a._data[i]);
    }
    result = sum;
}
void b_1_norm(double & result, BlockVector & vector_a) {
    int offset = vector_a.size / BlockVector::block_size;
    double sum = 0.0;
    for(int i = offset; i < vector_a.size; ++i) {
        sum += std::abs(vector_a._data[i]);
    }
    result = sum;
}
void b_2_norm(double & result, int block, BlockVector & vector_a) {
    const int offset = block * BlockVector::block_size;
    const int limit = offset + BlockVector::block_size;
    double sum = 0.0;
    for(int i = offset; i < limit; ++i) {
        sum += vector_a._data[i]*vector_a._data[i];
    }
    result = sum;
}
void b_2_norm(double & result, BlockVector & vector_a) {
    int offset = vector_a.size / BlockVector::block_size;
    double sum = 0.0;
    for(int i = offset; i < vector_a.size; ++i) {
        sum += vector_a._data[i]*vector_a._data[i];
    }
    result = sum;
}

void b_p_norm(double & result, int block, BlockVector & vector_a, double p) {
    const int offset = block * BlockVector::block_size;
    const int limit = offset + BlockVector::block_size;
    double sum = 0.0;
    for(int i = offset; i < limit; ++i) {
        sum += std::pow(vector_a._data[i],p);
    }
    result = sum;
}
void b_p_norm(double & result, BlockVector & vector_a, double p) {
    int offset = vector_a.size / BlockVector::block_size;
    double sum = 0.0;
    for(int i = offset; i < vector_a.size; ++i) {
        sum += std::pow(vector_a._data[i],p);
    }
    result = sum;
}
void b_max_norm(double & result, int block, BlockVector & vector_a) {
    const int offset = block * BlockVector::block_size;
    const int limit = offset + BlockVector::block_size;
    double max = 0.0;
    for(int i = offset; i < limit; ++i) {
        double tmax = std::abs(vector_a._data[i]);
        if(tmax > max) {
            max = tmax;
        }
    }
    result = max;
}
void b_max_norm(double & result, BlockVector & vector_a) {
    int offset = vector_a.size / BlockVector::block_size;
    double max = 0.0;
    for(int i = offset; i < vector_a.size; ++i) {
        double tmax = std::abs(vector_a._data[i]);
        if(tmax > max) {
            max = tmax;
        }
    }
    result = max;
}
void max_norm(double & result, BlockVector & vector_a) {
    size_t max_size = 1000; // todo

    BlockVector local_result = BlockVector(vector_a.num_block);
    for(int i = 0; i < vector_a.num_block; ++i) {
        b_max_norm(local_result._data[i], i, vector_a);
    }

    b_max_norm(local_result._data[vector_a.num_block-1], vector_a);
    if (vector_a.num_block < max_size) {
        double max = 0.0;
        for(int i = 0; i < vector_a.num_block; ++i) {
            if (local_result._data[i] > max) {
                max = local_result._data[i];
            }
            result = max;
        }
    } else {
        max_norm(result, local_result);
    }
}

/* todo
void b_dyad(BlockMatrix & result, int block, BlockVector & vector_a, BlockVector & vector_b) {
    // todo
}
void b_dyad(BlockMatrix & result, BlockVector & vector_a, BlockVector & vector_b) {
    // todo
}
*/