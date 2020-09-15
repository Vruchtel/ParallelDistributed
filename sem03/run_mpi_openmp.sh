#! /bin/bash
#
#SBATCH --ntasks=2
#SBATCH --cpus-per-task=2
#SBATCH --partition=RT
#SBATCH --job-name=example
#SBATCH --comment="Run mpi from config"
#SBATCH --output=out.txt
#SBATCH --error=err.txt
mpiexec a.out
