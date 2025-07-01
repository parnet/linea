#ifndef DISTRIBUTION_HPP
#define DISTRIBUTION_HPP


inline void distribution_range(int & lower, int & upper, int number, int rank, int size) {

    int basis = number / size;
    int rem = number % size;
    int local_num;

    if (rank < rem) {
        local_num = basis + 1;
        lower = local_num * rank;
        upper = local_num * (rank + 1) - 1 ;
    } else {
        local_num = basis;
        lower = local_num * rank + rem;
        upper = local_num * (rank + 1) + rem -1;
    }
}

inline void distribution_slice(int & lower, int & num_elements, int number, int rank, int size) {
    int basis = number / size;
    int rem = number % size;

    if (rank < rem) {
        num_elements = basis + 1;
        lower = num_elements * rank;
        // to = this->_num_local_rows * (comm_rank+1) - 1 ;
    } else {
        num_elements = basis;
        lower = num_elements * rank + rem;
        // to = this->_num_local_rows * (comm_rank+1) + rem -1;
    }
}
#endif //DISTRIBUTION_HPP
