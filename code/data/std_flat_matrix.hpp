#ifndef DATA_STD_FLAT_MATRIX_HPP
#define DATA_STD_FLAT_MATRIX_HPP
#include <vector>

class StdFlatMatrix {
public:
    explicit StdFlatMatrix(size_t num_rows, size_t num_cols) : _num_rows(num_rows), _num_cols(num_cols) {
        _num_elements = _num_cols*num_rows;
        data.resize(_num_elements);
    }

    StdFlatMatrix(const StdFlatMatrix &other) :_num_elements(other._num_elements){
        _num_elements = other._num_elements;
        _num_rows = other._num_rows;
        _num_cols = other._num_cols;
        data.resize(_num_elements);
        for (size_t i = 0; i < _num_elements; ++i) {
            data[i] = other.data[i];
        }
    }

    ~StdFlatMatrix() = default;

    double operator()(size_t row_index, size_t col_index) const {
        return data[row_index * this->_num_rows+col_index];
    }

    double & operator()(size_t row_index, size_t col_index) {
        return data[row_index * this->_num_rows+col_index];
    }

    size_t rows() {
        return _num_rows;
    }

    size_t cols() {
        return _num_cols;
    }
private:
    size_t _num_elements;
    size_t _num_rows;
    size_t _num_cols;
    std::vector<double> data;
};
#endif //STD_MATRIX_HPP
