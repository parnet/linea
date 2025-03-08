#ifndef DATA_NAIVE_VECTOR_HPP
#define DATA_NAIVE_VECTOR_HPP

class NaiveVector {
public:

    explicit NaiveVector(size_t num_elements) : _num_elements(num_elements) {
        data = new double[num_elements];

    }

    NaiveVector(const NaiveVector &other) :_num_elements(other._num_elements){
        data = new double[_num_elements];
        for (size_t i = 0; i < _num_elements; ++i) {
            data[i] = other.data[i];
        }
    }

    ~NaiveVector() {
        delete[] data;
    }

    double operator[](size_t index) const {
        return data[index];
    }

    double & operator[](size_t index) {
        return data[index];
    }

    size_t num_elements() {
        return _num_elements;
    }

private:
    size_t _num_elements{};
    double * data;
};



#endif
