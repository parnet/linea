#!/bin/bash
#SBATCH --job-name=test_linear_domain_decomp
#SBATCH --nodes=1
#SBATCH --ntasks=4               # antal MPI-processer
#SBATCH --gres=gpu:4             # 4 GPUs på noden
#SBATCH --time=00:10:00          # 5 minuter
#SBATCH --cpus-per-task=1        # 1 CPU per MPI-task, pga OpenMP-thread=1
#SBATCH --output=job_%j.log
#SBATCH --error=test_%j.log
#SBATCH --partition=dc-gpu

# Sätt OpenMP-thread till 1 (för säkerhets skull)
export OMP_NUM_THREADS=1
export CUDA_VISIBLE_DEVICES=0,1,2,3

# Ladda moduler
module load CMake/3.30.3
module load NVHPC/25.5-CUDA-12
module load OpenMPI

cd $SCRATCH
mkdir -p test_run
cd test_run

# Starta appen med mpirun eller srun
srun --export=ALL,OMPI_MCA_mpi_cuda_support=1 --cpu-bind=cores --cpu-bind=map_cpu:0,32,64,96 ~/pro/linea/bin/Linea
mpirun --mca mpi_cuda_support 1 --bind-to core=cores --cpu-bind=map_cpu:0,32,64,96 ~/pro/linea/bin/Linea
