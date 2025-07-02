#include <iostream>
#include <omp.h>

#include "mpi_omp_cuda/create_laplacian.hpp"
#include "mpi_omp_cuda/mpi_problem.hpp"
#include "mpi_omp_cuda/host/host.hpp"


int main(int argc, char** argv) {
    int provided;
    MPI_Init_thread(&argc, &argv, MPI_THREAD_FUNNELED, &provided);

    int comm_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &comm_rank);
    int mpi_size;
    MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);

    cudaSetDevice(comm_rank % 4);

    int device;
    cudaGetDevice(&device);
    std::cout << "MPI rank " << comm_rank << " using GPU " << device << std::endl;

    std::cout << "mpi_size=" << mpi_size << std::endl;
    std::cout << "mpi_rank=" << comm_rank << std::endl;

    int numDevices = 0;
    cudaGetDeviceCount(&numDevices);
    for (int i = 0; i < numDevices; ++i) {
        if (i == comm_rank) continue;

        int canAccessPeer = 0;
        cudaDeviceCanAccessPeer(&canAccessPeer, comm_rank, i);
        if (canAccessPeer) {
            cudaDeviceEnablePeerAccess(i, 0);
        }
    }

    /*
    int row_size = 105;
    int size = 10;
    for (int i = 0; i < size; i++){
        std::cout << "Rank = " << i << std::endl;
        auto matrix = create_laplacian_2d(32, i, size);
        std::cout <<  matrix._index_offset << ", " << matrix._num_local_rows << std::endl;
        std::cout << "data=";
        for (int k = 0; k < matrix._values.size(); k++) {
            std::cout << matrix._values[k] << ", ";
        }
        std::cout << std::endl;
    }*/
    omp_set_num_threads(1);
    // distr_problem(1,1);

    //distr_problem(1024,256);
    //problem_dist_seq(16,4); // startup problem

    //problem_dist_seq(256,64);

    //problem_dist_seq(256*16,64*16);
    mpi_aware_problem(256*16,64*16);
    MPI_Finalize();
    return 0;

    if (provided < MPI_THREAD_FUNNELED) {
        std::cerr << "[MPI] Error: MPI implementation does not provide required threading support!" << std::endl;
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    std::cout << "[MPI] world size: " << world_size << std::endl;


    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    std::cout << "[MPI] current rank: " << world_rank << std::endl;


#pragma omp parallel
    {
#pragma omp master
        {
            int thread_count = 0;
            thread_count = omp_get_num_threads();
            std::cout << "[OpenMP] Number of threads: " << thread_count << std::endl << std::endl;
        }
    }

    // benchmark_heat_equation();

}

