#include "cuda/data/vector.hpp"


inline void crs_residual(CUDA_Vector defect, CUDA_Vector omega_diag_inv, CUDA_Vector x0, CUDA_Vector rhs, cudaStream_t stream = nullptr) {
    std::cout << "todo - implement" << std::endl;
}

inline double parallel_norm(CUDA_Vector vector, cudaStream_t stream = nullptr ) {
    std::cout << "todo - implement" << std::endl;
    return 0.0;
}


inline void jacobi_step(CUDA_Vector cu_x_current_0, CUDA_Vector cu_x0_0, CUDA_Vector cu_laplace_diag_0, CUDA_Vector cu_defect_0, cudaStream_t stream_0 = nullptr) {
    std::cout << "todo - implement" << std::endl;
}
