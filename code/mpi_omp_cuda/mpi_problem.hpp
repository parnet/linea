#pragma once
#include <cuda_runtime_api.h>
#include <iostream>
#include <util/timer.hpp>
#include <cmath>
#include <cstring>
#include <iomanip>
#include <vector>

#include "host/host.hpp"
#include "kernel/bridge.hpp"
#include "util/converter.hpp"

#if !defined(MPIX_CUDA_AWARE_SUPPORT) || !MPIX_CUDA_AWARE_SUPPORT
#error "The used MPI Implementation does not have CUDA-aware support or CUDA-aware
support can't be determined. Define SKIP_CUDA_AWARENESS_CHECK to skip this check."
#endif

inline void vec_print(const char * name, Vector & vec, int Nx,int  Ny) {
    std::cout << name << std::endl;
    for ( int i = 0; i < Nx; i++ ) {
        for ( int j = 0; j < Ny; j++ ) {
            std::cout << vec._values[i*Nx+j] << ",";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}



inline void mpi_aware_problem(int Nx, int Ny /*uppdelning utan överlappning*/) {
    // const int max_iteration = 10'000;
    const int max_iteration = 100;

    int comm_size;
    int comm_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &comm_rank);

    int proc_Nx = Nx;
    int proc_Ny = Ny + 1;  // or Nx / comm_size + overlapp
    if (!(comm_rank == 0 || comm_rank == comm_size - 1)) {
        proc_Ny += 1;
    }
    int proc_gridsize = proc_Nx*proc_Ny;

    Timer walltime;
    walltime.start();

    double alpha = 1.0;



    CRS_Matrix laplace_matrix = create_laplacian_2d(proc_Nx, proc_Ny);

    //std::cout << "rank=" << comm_rank << "\tlaplace\t"<< laplace_matrix._num_rows << "," << laplace_matrix._num_cols << std::endl;
    Timer transfer_timer;
    if (comm_rank == 0) {
        transfer_timer = Timer();
        transfer_timer.stop();
    }
    auto cu_laplace_matrix = X_CRS_Matrix(laplace_matrix);

    cudaStreamSynchronize(cu_laplace_matrix.stream);
    if (comm_rank == 0) {
        transfer_timer.stop();
        std::cout << "transfer_time= " << transfer_timer.get() << " [s]" << std::endl;
    }

    Vector laplace_diag = Vector(laplace_matrix._num_rows);
    extract_diagonal(laplace_diag, laplace_matrix, 0.66);
    auto cu_laplace_diag = X_Vector(laplace_diag);

    auto x0 = Vector(proc_gridsize);
    create_zero(x0);
    auto cu_x0 = X_Vector(x0);
    cudaStreamSynchronize(cu_x0.stream);

    //    vec_print("x0",x0,  Nx, Ny);

    auto rhs = Vector(proc_gridsize);
    if (comm_rank == 0) {
        laplace_rhs_function(rhs,Nx,proc_Ny,alpha, comm_rank*Ny-1);
        for ( int i = 0; i < Nx; i++ ) { // fix boundary
            rhs._values[          i] = 0.0; // first row
        }
    } else if (comm_rank == comm_size - 1) {
        laplace_rhs_function(rhs,Nx,proc_Ny,alpha, comm_rank*Ny-1);
        for ( int i = 0; i < Nx; i++ ) { // fix boundary
            rhs._values[(Ny)*Nx+i] = 0.0; // last row
        }
    } else {
        laplace_rhs_function(rhs,Nx,proc_Ny,alpha, comm_rank*Ny-1);
    }

    for ( int i = 0; i < proc_Ny; i++ ) {
        rhs._values[i*Nx+     0] = 0.0; // first column
        rhs._values[i*Nx+(Nx-1)] = 0.0; // last column
    }

    /*std::cout << "rhs_1=" << std::endl;
    for (int i = 0; i < rhs_3._values.size(); i++ ) {
        std::cout << rhs_3._values[i] << std::endl;
    }*/

    auto cu_rhs = X_Vector(rhs);
    cudaStreamSynchronize(cu_rhs.stream);
    auto defect = Vector(proc_Nx*proc_Ny);
    X_Vector cu_x_current(cu_x0._num_rows);
    X_Vector cu_defect(cu_x0._num_rows);

    cudaStreamSynchronize(cu_x_current.stream);




    cudaStream_t stream;
    cudaStreamCreate(&stream);


    auto normer = std::vector<double>(max_iteration, 0.0);

    Timer timer;
    timer.start();

    /*double * buffer_01; //= (double*)malloc(sizeof(double) * Nx);
    cudaMalloc((void**)&buffer_01, sizeof(double) * Nx);

    double * buffer_10; //= (double*)malloc(sizeof(double) * Nx);
    cudaMalloc((void**)&buffer_10, sizeof(double) * Nx);
    double * buffer_12; //= (double*)malloc(sizeof(double) * Nx);
    cudaMalloc((void**)&buffer_12, sizeof(double) * Nx);

    double * buffer_21; //= (double*)malloc(sizeof(double) * Nx);
    cudaMalloc((void**)&buffer_21, sizeof(double) * Nx);
    double * buffer_23; //= (double*)malloc(sizeof(double) * Nx);
    cudaMalloc((void**)&buffer_23, sizeof(double) * Nx);

    double * buffer_32; //= (double*)malloc(sizeof(double) * Nx);
    cudaMalloc((void**)&buffer_32, sizeof(double) * Nx);*/
    double global_norm;
    for (int k = 0 ; k < max_iteration; k++){
        MPI_Request requests[6];
        int req_count = 0;



        x_crs_residual(cu_defect, cu_laplace_matrix, cu_x0, cu_rhs, stream);
        x_set_row_zero(cu_defect, proc_Nx, 0, stream);
        //std::cout << "zero_row rank=" << comm_rank << " \t target_row = " << proc_Ny - 1  << std::endl;
        x_set_row_zero(cu_defect, proc_Nx, proc_Ny - 1, stream);
        cudaStreamSynchronize(stream);

        double local_norm = x_parallel_norm(cu_defect, stream);



        MPI_Iallreduce(&local_norm, &global_norm, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD, &requests[req_count++]);

        x_jacobi_step(cu_x_current, cu_x0, cu_laplace_diag, cu_defect, stream);
        cudaStreamSynchronize(stream);

        //std::cout <<"rank="<<comm_rank<< "\t"<< "datastruct-size = " << proc_Nx* proc_Ny << std::endl;
        //std::cout <<"rank="<<comm_rank<< "\t"<<"proc_Nx = " << proc_Nx << std::endl;

        if (comm_rank != 0) { // rank =1, rank=2, ... vill har resultat i första rad
            int remote_Ny = 0;
            MPI_Irecv(&cu_x0._values[proc_Nx*(proc_Ny - 1)], proc_Nx, MPI_DOUBLE, comm_rank - 1, 0, MPI_COMM_WORLD, &requests[req_count++]);
            //std::cout << "rec rank=" << comm_rank <<"\t proc=" << comm_rank - 1 << " \t tag=" << 0 << std::endl;
            //std::cout <<"rank="<<comm_rank<< "\t"<< 0 << "    <===    " << proc_Nx*(proc_Ny - 1)-1 << std::endl;
            MPI_Isend(&cu_x0._values[proc_Nx*(proc_Ny - 2)], proc_Nx, MPI_DOUBLE, comm_rank - 1, 1, MPI_COMM_WORLD, &requests[req_count++]);
            //std::cout <<"rank="<<comm_rank<< "\t"<< proc_Nx*(proc_Ny - 2)-1 << "    <===    " << 0 << std::endl;
            //std::cout << "send rank=" << comm_rank <<"\t proc=" << comm_rank - 1 << " \t tag=" << 1 << std::endl;
        }

        if (comm_rank != comm_size - 1) {  // rank =0, rank=1, ... vill har resultat i sista rad
            int remtoe_first_entry = 0;
            // cudaIpcOpenMemHandle((void**)&remote_x0_e, mem_handle_e_rec, cudaIpcMemLazyEnablePeerAccess);
            MPI_Irecv(&cu_x0._values[0], proc_Nx, MPI_DOUBLE, comm_rank + 1, 1, MPI_COMM_WORLD, &requests[req_count++]);
            //std::cout << "rec rank=" << comm_rank <<"\t proc=" << comm_rank + 1 << " \t tag=" << 0 << std::endl;
            //std::cout << proc_Nx*(proc_Ny-1) <<  "    <===    " << 0 << std::endl;
            //cudaMemcpyAsync(&cu_x0._values[proc_Nx*(proc_Ny-1)], &remote_x0_e[0], proc_Nx * sizeof(double), cudaMemcpyDeviceToDevice, stream);
            MPI_Isend(&cu_x0._values[proc_Nx], proc_Nx, MPI_DOUBLE, comm_rank + 1, 0, MPI_COMM_WORLD, &requests[req_count++]);
            //std::cout << "send rank=" << comm_rank <<"\t proc=" << comm_rank + 1 << " \t tag=" << 1 << std::endl;
        }


        cudaStreamSynchronize(stream);
        MPI_Waitall(req_count, requests, MPI_STATUSES_IGNORE);

        cu_x0.swap(cu_x_current);
        global_norm = sqrt(global_norm);
        //std::cout << "local_norm = " << std::setprecision(15)<< local_norm << std::endl;
        //std::cout << "global_norm = " << std::setprecision(15)<< global_norm << std::endl;
    }
    cudaDeviceSynchronize();
    if (comm_rank == 0) {
        timer.stop();
        walltime.stop();

        std::cout << std::endl;
        std::cout << "global_norm = " << global_norm << std::endl;
        std::cout << "N x N=" << proc_Nx << " x " << proc_Nx <<" []"<< std::endl;
        std::cout << "gridsize=" << proc_Nx*proc_Nx <<" []"<< std::endl;
        std::cout << "time=" << timer.get() <<" [s]"<< std::endl;
        std::cout << "walltime=" << walltime.get() <<" [s]"<< std::endl;
        std::cout << "time_per_iter=" << walltime.get() / max_iteration <<" [s]"<< std::endl;
        std::cout << "A=" << convert(laplace_matrix.memory_size(),Mega) <<" ["<<symbol(Mega)<<"]"<< std::endl;
        std::cout << "D=" << convert(laplace_diag.memory_size(),Mega) <<" ["<<symbol(Mega)<<"]"<< std::endl;
        std::cout << "x0=" << convert(x0.memory_size(),Mega) <<" ["<<symbol(Mega)<<"]"<< std::endl;
        std::cout << "rhs=" << convert(rhs.memory_size(),Mega) <<" ["<<symbol(Mega)<<"]"<< std::endl;
        std::cout << normer[max_iteration-1] << std::endl;
    }
}
